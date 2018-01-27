# *** MACROS
CC=gcc
CFLAGS=-g -pedantic -Wall -Wconversion

INC_PATH=./modules/
HEAP_PATH=./genBinaryHeap/
VEC_PATH=./genericVector/
LOG_PATH=./logger/
MODULES_PATH=./modules/
CFLAGS += -I$(INC_PATH) -I$(HEAP_PATH) -I$(LOG_PATH) -I$(VEC_PATH)

# *** TARGETS
SRCS=$(wildcard $(MODULES_PATH)*.c)
OBJS=$(SRCS:.c=.o)
HEAP_SRC= $(wildcard $(HEAP_PATH)genBinaryHeap.c)
HEAP_OBJS=$(HEAP_SRC:.c=.o)
VEC_SRC= $(wildcard $(VEC_PATH)generic_vector.c)
VEC_OBJS=$(VEC_SRC:.c=.o)
LOG_SRC= $(wildcard $(LOG_PATH)logger.c)
LOG_OBJS=$(LOG_SRC:.c=.o)

TARGET=testSuite.out

$(TARGET): $(OBJS) $(HEAP_OBJS) $(VEC_OBJS) $(LOG_OBJS)
	$(CC) $(OBJS) $(HEAP_OBJS) $(VEC_OBJS) $(LOG_OBJS) -o $(TARGET)

all: $(TARGET)

include depends

depends:
	$(CC) -MM -I$(INC_PATH) -I$(HEAP_PATH) -I$(VEC_PATH) -I$(LOG_PATH) $(SRCS) $(HEAP_SRC) $(VEC_SRC) $(LOG_SRC) > depends

# *** COMMANDS
clean:
	-rm $(TARGET)
	-rm *.o
	-rm depends

run:
	./$(TARGET)
	
##################################

