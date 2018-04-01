CC = gcc
CXX = g++
C11 = -std=c++11
OPT = -Wall -O3 -Wno-unused-function -Wno-unused-variable

cauchy: src/cauchy.cpp
	$(CXX) $(C11) $< -o $@

cauchy3D: src/cauchy3D.cpp
	$(CXX) $(C11) $< -o $@

bfgs: src/bfgs.cpp
	$(CXX) $(C11) $< -o $@

bfgs3D: src/bfgs3D.cpp
	$(CXX) $(C11) $< -o $@

generator: src/generator.cpp
	$(CXX) $(C11) $< -o $@

generator3D: src/generator3D.cpp
	$(CXX) $(C11) $< -o $@

.PHONY: clean
clean:
	rm -fr cauchy cauchy3D bfgs bfgs3D generador generator generator3D 

cleanData:
	rm -fr *.txt
