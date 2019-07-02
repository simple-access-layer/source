.PHONY: all clean

all:
	g++ -Wall -fPIC -std=c++11 -c *.cpp
	g++ -shared -Wl,-soname,libsal.so.1 -o libsal.so.1.0 *.o

clean:
	rm -f *.o libsal.so.1.0

