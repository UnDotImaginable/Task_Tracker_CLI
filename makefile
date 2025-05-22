output: main.o functions.o
	clang++ main.o functions.o -o output

main.o: main.cpp functions.h
	clang++ -c main.cpp

functions.o: functions.cpp functions.h
	clang++ -c functions.cpp

clean:
	rm *.o output