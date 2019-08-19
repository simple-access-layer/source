.PHONY: all clean

all:
	g++ -fPIC -std=c++11 -c sal.cpp
	g++ -shared -Wl,-soname,libsal.so -o libsal.so sal.o
	g++ -o saldemo demo.cpp -std=c++11 -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON -lsal -L.

#	g++ -std=c++11 -c *.cpp
#	g++ -o saldemo *.o -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON

clean:
	rm -f *.o libsal.so saldemo
