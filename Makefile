CC=g++
CCFLAGS=-Wall -Wextra -o

sorting:
	$(CC) sorting.cpp $(CCFLAGS) sorting

run: sorting
	./sorting

clean:
	rm -rf sorting