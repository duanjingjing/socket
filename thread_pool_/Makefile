.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=test
OBJS=test_pool.o Thread.o Condition.o ThreadPool.o
LINK=-lpthread
STAR=-std=c++0x
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN) $(LINK)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $<  -o $@ $(STAR)
clean:
	rm -f *.o $(BIN)
