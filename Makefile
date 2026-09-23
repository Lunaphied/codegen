CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

codegen: codegen.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f codegen

.PHONY: clean
