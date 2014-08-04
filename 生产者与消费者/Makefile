.PHONY:clean
CC=g++
CFLAGS=-Wall -g
MAIN=main.exe
OBJS=test_main.o ConsumeThread.o ProduceThread.o Queue.o Buffer.o Condition.o
$(MAIN):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread
%.o:%.cpp
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o *.exe
