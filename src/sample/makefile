all: main inner

inner: inner.cpp
	g++ -o inner inner.cpp

main: main.cpp
	g++ -o main main.cpp -lssl -lcrypto
