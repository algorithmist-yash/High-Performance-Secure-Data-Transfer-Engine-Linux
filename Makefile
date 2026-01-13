# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -O2 -Iinclude -pthread

# Linker flags (OpenSSL)
LDFLAGS = -lssl -lcrypto

# Directories
SRC = src
BIN = bin

# Targets
all: server client

server:
	$(CXX) $(CXXFLAGS) $(SRC)/server.cpp $(SRC)/crypto.cpp src/metrics.cpp -o $(BIN)/server $(LDFLAGS)

client:
	$(CXX) $(CXXFLAGS) $(SRC)/client.cpp $(SRC)/crypto.cpp src/metrics.cpp -o $(BIN)/client $(LDFLAGS)

clean:
	rm -f $(BIN)/server $(BIN)/client

all: server client

stress:
	$(CXX) $(CXXFLAGS) src/stress_client.cpp src/crypto.cpp src/metrics.cpp -o bin/stress_client $(LDFLAGS)

