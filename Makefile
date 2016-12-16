CC=clang++
CFLAGS=-stdlib=libc++ -std=c++14 -O3

fto1:
	$(CC) $(CFLAGS) fto1.cpp -o fto1
