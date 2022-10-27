CXX = g++ -g
CXXFLAGS = -Wall

Driver.out: CTree.o Driver.o
	$(CXX) $(CXXFLAGS) CTree.o Driver.o -o Driver.out

CTree.o: CTree.cpp CTree.h
	$(CXX) $(CXXFLAGS) -c CTree.cpp

Driver.o: Driver.cpp
	$(CXX) $(CXXFLAGS) -c Driver.cpp


clean:
	rm *~
	rm *.o*

run:
	./Driver.out

make valgrind:
	valgrind --track-origins=yes --leak-check=full ./Driver.out
