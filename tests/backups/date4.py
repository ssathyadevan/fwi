from datetime import datetime

def find(substr,whichin):
    from datetime import datetime
    lines           = filter(lambda x: substr in x, open(whichin+".out"))
    line            = lines[0]
    manip           = line.replace(substr,'').replace("\n",'')
    start_or_finish = (datetime.strptime(manip,'%c'))
    return start_or_finish 

datetime_bench_start  = find("Starting at ", "hiquality")
datetime_bench_finish = find("Finished at ","hiquality")
bench_total_seconds   = (datetime_bench_finish - datetime_bench_start).seconds 
datetime_new_start    = find("Starting at ","default")
datetime_new_finish   = find("Finished at ","default")
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds 
print bench_total_seconds
print new_total_seconds
print float(bench_total_seconds)/float(new_total_seconds)
