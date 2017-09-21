CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LXX = g++
LXXFLAGS = -L.

all: pq

pq.o: pq.cpp pq.h
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

pq: pq.o
	@echo "Linking $@..."
	@$(LXX) $(LXXFLAGS) -o $@ $^

clean:
	rm -f pq 
	rm -f *.o
