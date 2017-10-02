#/afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/bi/afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/sh

/afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/pq add /afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/worksim.py 0.1 30
sleep 15
for i in $(seq 9); do
    /afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/pq add /afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/worksim.py 0.$i 5
done
sleep 5
for i in $(seq 9); do
    /afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/pq add /afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/worksim.py 0.$i 1
done

watch /afs/nd.edu/user28/mdalonz1/Operating_Systems/cse-30341-fa17-project02/pq status
