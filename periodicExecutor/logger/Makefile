# *** MACROS
CC=gcc
CFLAGS=-g -ansi -pedantic -Wall -Wconversion

INC_PATH=./
CFLAGS += -I$(INC_PATH)

# *** TARGETS
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

TARGET=testSuite.out

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)  -pthread

all: $(TARGET)

include depends

depends:
	$(CC) -MM -I$(INC_PATH) $(SRCS) > depends

# *** COMMANDS
clean:
	-rm $(TARGET)
	-rm *.o
	-rm depends

run:
	./$(TARGET)
##################################

