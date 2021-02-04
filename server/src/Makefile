SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc,%.o,$(SRCS))
ELF:=thread_pool
CC:=g++
CFLAGS:=-g -Wall 
$(ELF):$(OBJS)
	g++ -o $@ $^ -pthread 
clean:
	rm -rf $(OBJS) $(ELF)
