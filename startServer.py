import os
import sys

# launch server
if len(sys.argv) < 3:
    print "Please enter policy as first argument and number \
of CPUS as second"
    exit(1)

basepath = os.path.abspath("./")
command = basepath + "/pq -f pq.mdalonz1"
command += " -p "
command += sys.argv[1]
command += " -n "
command += sys.argv[2]
print "Executing: ",
print command
os.chdir("/tmp/")
os.system("rm pq.mdalonz1")
os.system(command)
