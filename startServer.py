import os

# launch server
basepath = os.path.abspath("./")
command = basepath + "/pq -f mdalonz1.pq"
os.chdir("/tmp/")
os.system("rm pq.mdalonz1")
os.system(command)
