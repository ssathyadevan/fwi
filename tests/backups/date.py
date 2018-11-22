from datetime import datetime

def find(substr, infile, outfile):
    lines = filter(lambda x: substr in x, open (infile))
    #toprint = lines.replace(substr,'')
    open(outfile, 'w').writelines(lines)
datetime_object = datetime.strptime('Mon Nov 19 17:12:17 2018', '%c')#a %b %d %Y 
datetime_object2 = datetime.now()
print str((datetime_object2 - datetime_object).seconds)

find("Starting at","default.out","newtest.out")
find("Finished at","default.out","newtest.out")
