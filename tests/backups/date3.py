from datetime import datetime

def find(substr, infile):
    lines = filter(lambda x: substr in x, open (infile))
    line = lines[0]
    manip = line.replace(substr,'').replace("\n",'')
    return manip

finishing_time_string = find("Finished at ","default.out")
starting_time_string  = find("Starting at ","default.out")
datetime_start_cpp = datetime.strptime(starting_time_string,'%c')
datetime_finish_cpp  = datetime.strptime(finishing_time_string,'%c')
seconds_of_time_fwi_took = (datetime_finish_cpp - datetime_start_cpp).seconds

print str((datetime_finish_cpp - datetime_start_cpp).seconds)
