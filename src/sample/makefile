all: main inner

inner: inner.cpp
	g++ -o inner inner.cpp

main: main.cpp crypto.o
	g++ -o main main.cpp crypto.o -lssl -lcrypto

crypto.o: crypto.cpp
	g++ -c -o crypto.o crypto.cpp -lssl -lcrpyto
