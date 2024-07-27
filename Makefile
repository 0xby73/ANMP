CXX = g++
CXXFLAGS = -Wall -Wextra -I. -Wno-unused-variable -lncurses

all: server client

server:
	$(CXX) $(CXXFLAGS) ./servercode/*.cpp -o server

client:
	$(CXX) $(CXXFLAGS) ./clientcode/*.cpp -o client

clean:
	rm -f server client

.PHONY: all clean
