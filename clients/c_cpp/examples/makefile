SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:%.cpp=%.o)

.PHONY: build demo test clean

all: build

build: libsal.so

demo: saldemo libsal.so

libsal.so: ${OBJS}
	g++ -shared -Wl,-soname,libsal.so -o libsal.so ${OBJS} -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON -lPocoUtil

saldemo: libsal.so demo.cpp
	g++ -o saldemo demo.cpp -std=c++11 -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON -lPocoUtil -lsal -Iinclude -L.

%.o: %.cpp
	g++ -fPIC -std=c++11 -o $@ -c $< -Iinclude

test: libsal.so
	$(MAKE) -C tests test

clean:
	rm -f *.o src/*.o libsal.so saldemo
	$(MAKE) -C tests clean
