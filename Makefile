# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.

# Source files
SRC = sim/sim.cpp common/id.cpp

# Output executable
TARGET = sim.exe

# Default rule
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)