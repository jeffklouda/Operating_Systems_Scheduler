CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LXX = g++
LXXFLAGS = -L.

all: pq

logging.o: logging.cpp pq.h
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

scheduler.o: scheduler.cpp pq.h
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

pq.o: pq.cpp pq.h
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

ipc_communication.o: ipc_communication.cpp pq.h
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

pq: pq.o ipc_communication.o scheduler.o logging.o
	@echo "Linking $@..."
	@$(LXX) $(LXXFLAGS) -o $@ $^

clean:
	rm -f pq 
	rm -f *.o
