CXX = g++
CXXFLAGS = -Wall -Wextra -I. -Wno-unused-variable

all: server client

server:
	$(CXX) $(CXXFLAGS) ./servercode/*.cpp -o server -lncurses

client:
	$(CXX) $(CXXFLAGS) ./clientcode/*.cpp -o client -lncurses

clean:
	rm -f server client

.PHONY: all clean
