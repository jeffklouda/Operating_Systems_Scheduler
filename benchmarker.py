import os
import sys
import subprocess

commandBase = os.path.abspath("./workloads/");
commandBase += "/io-mixed-duration-";
command = {} 
command[0] = commandBase + "ascending.sh";
command[1] = commandBase + "convoy.sh";
command[2] = commandBase + "descending.sh";
command[3] = commandBase + "long.sh";
command[4] = commandBase + "medium.sh";
command[5] = commandBase + "mixed-burst.sh";
command[6] = commandBase + "mixed.sh";
command[7] = commandBase + "mixed-staggered.sh";
command[8] = commandBase + "mixed-stampede.sh";
command[9] = commandBase + "short.sh";
command[10] = commandBase + "tester.sh";

if (len(sys.argv) < 2):
    print "Please enter number of command as an argument"
    print "Commands: "
    for i, co in enumerate(command):
        print i,
        print ": ",
        print command[i]
    exit(1)

commandNo = int(sys.argv[1])
os.chdir("/tmp/")
print sys.argv
print "Executing ",
print command[commandNo]
os.system(command[commandNo]);
exit(0)
