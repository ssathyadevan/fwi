import json
import os
from datetime import datetime
import numpy as np

class OutputLogger(object):
    def __init__(self, t_in, t_out, diff_chi, mse, square_mean ):
        self.log = dict(
            time_in=t_in.strftime("%d-%m-%Y %H:%M:%S"),
            time_out=t_out.strftime("%d-%m-%Y %H:%M:%S"),
            time_execution=(t_out - t_in).seconds,
            mean_squared_error=(np.square(diff_chi)).mean(),
            average_relative_error=np.sqrt(mse) / np.sqrt(square_mean) * 100,
            input_resolution=list,
            output_resolution=list,
            RAM=str,
            CPU=str,
            description="",
            image=str,
        )
        self.complete_output_log()
        self.save_output_log()

    @staticmethod
    def open_generic_input_json():
        with open(
            "../parallelized-fwi/inputFiles/default/input/GenericInput.json"
        ) as json_file:
            return json.load(json_file)

    def determine_resolutions(self, generic_input):
        self.log["input_resolution"] = [
            generic_input["ngrid_original"]["x"],
            generic_input["ngrid_original"]["z"],
        ]
        self.log["output_resolution"] = [
            generic_input["ngrid"]["x"],
            generic_input["ngrid"]["z"],
        ]

    def determine_hardware_specs(self):
        os.system("lshw -class cpu -short > cpu_specs")
        cpu = list(open("cpu_specs"))
        os.system("lshw -class memory -short > memory_specs")
        memory = list(open("memory_specs"))
        #print(memory[-1][memory[-1].rfind("  "):memory[-1].rfind("\n")])
        self.log["RAM"] = memory[-1][memory[-1].rfind("  "):memory[-1].rfind("\n")]
        self.log["CPU"] = cpu[-1][cpu[-1].rfind("  "):cpu[-1].rfind("\n")]
        os.remove("cpu_specs")
        os.remove("memory_specs")

    def get_description(self):
        if os.path.isfile("../parallelized-fwi/results/description.txt"):
            with open("../parallelized-fwi/results/description.txt", 'r') as in_file:
                self.log["description"] = in_file.read()
            os.remove("../parallelized-fwi/results/description.txt")

    def complete_output_log(self):
        generic_input = self.open_generic_input_json()
        self.log["image"] = generic_input["fileName"]
        self.determine_resolutions(generic_input)
        self.determine_hardware_specs()
        self.get_description()

    def save_output_log(self):
        log = self.log.copy()
        if not os.path.exists("../parallelized-fwi/results"):
            os.mkdir("../parallelized-fwi/results")
        with open(
            "../parallelized-fwi/results/log_"
            + datetime.now().strftime("%d_%m_%Y_%H_%M_%S") + ".json", "w") as out_file:
            json.dump(log, out_file, ensure_ascii=False, indent=4)
