# Compiler and flags
CXX := g++
CXXFLAGS := -O2 -g -std=c++17
DEPFLAGS := -lssl -lcrypto

# Default target
all: layer1 layer2_3.enc

detection.o: detection.cpp
	$(CXX) $(CXXFLAGS) -c detection.cpp -o detection.o

dhexchange.o: dhexchange.cpp
	$(CXX) $(CXXFLAGS) -c dhexchange.cpp -o dhexchange.o

layer1: layer1.cpp crypto.h
	$(CXX) $(CXXFLAGS) layer1.cpp -o layer1 $(DEPFLAGS)

layer2_3.enc: layer2_3.cpp crypto.h dhexchange.o
	$(CXX) $(CXXFLAGS) layer2_3.cpp dhexchange.o -o layer2_3 $(DEPFLAGS)
	openssl enc -aes-256-cbc -in layer2_3 -out layer2_3.enc -pass pass:abcde

# Clean up
clean:
	rm -rf *.0 *.enc *.dec layer1 layer2_3

# Phony targets
.PHONY: all clean

