SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc,%.o,$(SRCS))
ELF:=mybash
CC:=g++
CFLAGS:=-g -Wall 
$(ELF):$(OBJS)
	g++ -o $@ $^  
clean:
	rm -rf $(OBJS) $(ELF)
