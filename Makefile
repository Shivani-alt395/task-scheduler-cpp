CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

.PHONY: all app test clean

all: app test

# Build the interactive CLI
app: src/main.cpp include/MinHeap.h include/Task.h
	$(CXX) $(CXXFLAGS) src/main.cpp -o scheduler

# Build and run the unit test suite
test: tests/test_heap.cpp include/MinHeap.h include/Task.h
	$(CXX) $(CXXFLAGS) tests/test_heap.cpp -o run_tests
	./run_tests

clean:
	rm -f scheduler run_tests
