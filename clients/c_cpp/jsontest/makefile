.PHONY: all clean

all:
	g++ -Wall -fPIC -std=c++11 -c *.cpp
	g++ -o jsontest *.o -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON

clean:
	rm -f *.o jsontest

