CC = gcc
CXX = g++
C11 = -std=c++11
OPT = -Wall -O3 -Wno-unused-function -Wno-unused-variable

cauchy: src/cauchy.cpp
	$(CXX) $(C11) $< -o $@

.PHONY: clean
clean:
	rm -fr cauchy generador

cleanData:
	rm -fr line.txt c1.txt c2.txt
