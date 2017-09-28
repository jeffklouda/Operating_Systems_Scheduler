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
commandNo = int(sys.argv[1])
# launch server
os.chdir("/tmp/")
print sys.argv
#os.system("~/CSE_2017-2018/cse_30341/cse-30341-fa17-project02/pq -f mdalonz1.pq");
print "Executing ",
print command[commandNo]
#command[i] += " > ~/logfile.txt"
os.system(command[commandNo]);
exit
#> ~/CSE_2017-2018/cse30341/cse-30341-fa17-project02/testfile.txt");
