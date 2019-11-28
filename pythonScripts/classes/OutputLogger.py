import json
import os
from datetime import datetime
import numpy as np

class OutputLogger(object):
    def __init__(self, run_num, t_in, t_out, t_exec, virtual_mem, physical_mem, diff_chi, mse, square_mean):
        self.log = dict(
            forward_model = str,
            inversion_method = str,
            time_in=t_in.strftime("%d-%m-%Y %H:%M:%S.%f"),
            time_out=t_out.strftime("%d-%m-%Y %H:%M:%S.%f"),
            wall_time_execution=((t_out - t_in).seconds + (t_out - t_in).microseconds / 10**6),
            cpu_time_execution = t_exec,
            virtual_memory_usage_kB = virtual_mem,
            physical_memory_usage_kB = physical_mem,
            mean_squared_error=(np.square(diff_chi)).mean(),
            average_relative_error=np.sqrt(mse) / np.sqrt(square_mean) * 100,
            input_resolution=list,
            output_resolution=list,
            RAM=str,
            CPU=str,
            description="",
            image=str,
            inputfile=dict,
        )
        self.complete_output_log(run_num)
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

    def get_description(self, run_num):
        if os.path.isfile("../parallelized-fwi/results/description" + str(run_num) + ".txt"):
            with open("../parallelized-fwi/results/description" + str(run_num) + ".txt", 'r') as in_file:
                self.log["inversion_method"] = in_file.readline()[:-1]
                self.log["forward_model"] = in_file.readline()[:-1]
                path = in_file.readline()[:-1]
                self.log["description"] = in_file.readline()
            os.remove("../parallelized-fwi/results/description" + str(run_num) + ".txt")
        return path

    def get_input(self, path):
        with open("../FWIInstall/" + path[:] + "/input/" + self.log["inversion_method"][:1].upper() + 
        self.log["inversion_method"][1:] + "Input.json") as json_file:
            self.log["inputfile"] = json.load(json_file)

    def complete_output_log(self, run_num):
        generic_input = self.open_generic_input_json()
        self.log["image"] = generic_input["fileName"]
        self.determine_resolutions(generic_input)
        self.determine_hardware_specs()
        path = self.get_description(run_num)
        self.get_input(path)

    def save_output_log(self):
        log = self.log.copy()
        if not os.path.exists("../parallelized-fwi/results"):
            os.mkdir("../parallelized-fwi/results")
        with open(
            "../parallelized-fwi/results/log_"
            + datetime.now().strftime("%d_%m_%Y_%H_%M_%S") + ".json", "w") as out_file:
            json.dump(log, out_file, ensure_ascii=False, indent=4)
