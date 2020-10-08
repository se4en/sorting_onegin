CC=g++
CCFLAGS=-Wall -o

sorting:
	$(CC) sorting.cpp $(CCFLAGS) sorting

run: sorting
	./sorting

clean:
	rm -rf sorting