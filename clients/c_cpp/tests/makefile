SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)

.PHONY: build

all: build

build: ${OBJS}
	g++ -o runtests ${OBJS} -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON -lsal -L.. -Wl,-rpath ..

%.o: %.cpp
	g++ -std=c++11 -o $@ -c $< -I../include

test: build
	./runtests --reporter compact --success

clean:
	rm -f *.o runtests
