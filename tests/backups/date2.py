from datetime import datetime

def find(substr, infile, outfile):
    lines = filter(lambda x: substr in x, open (infile))
    line = lines[0]
    #toprint = lines.replace(substr,'')
    open(outfile, 'w').writelines(line)
    manip = line.replace(substr,'')
    open(outfile, 'w').writelines(manip)
    return manip #datetime_object3=datetime.strptime(manip, '%c')

#datetime_object3 = datetime.strptime(find("Starting at ","default.out","newtest.out"),'%c')
find("Finished at ","default.out","newtest2.out")
datetime_object = datetime.strptime('Mon Nov 19 17:12:17 2018', '%c')#a %b %d %Y 
datetime_object2 = datetime.now()
#print str((datetime_object2 - datetime_object).seconds)
print str((datetime_object3 - datetime_object).seconds)

find("Starting at ","default.out","newtest.out")
find("Finished at ","default.out","newtest2.out")
