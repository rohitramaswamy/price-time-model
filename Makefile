# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++14

# Compiler flags for debugging
DEBUGFLAGS = -g

# Name of the target executable
TARGET = main

# Source files to compile (include all relevant .cpp files)
SOURCES = model.cpp market.cpp bot.cpp

# Rule for creating the target executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# 'make debug' rule for compiling with debugging information
debug: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(SOURCES) -o $(TARGET)

# 'make clean' rule for removing build artifacts
clean:
	rm -f $(TARGET) *.o
