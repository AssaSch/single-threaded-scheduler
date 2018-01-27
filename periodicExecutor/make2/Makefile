# *** MACROS
CC=gcc
CFLAGS=-g -pedantic -Wall -Wconversion -fPIC

INC_PATH=../../include/
HEAP_PATH=../genBinaryHeap/
VEC_PATH=../genericVector/
LOG_PATH=../logger/
LIB_PATH=../../lib/
CFLAGS += -I$(INC_PATH)

# *** TARGETS
SRCS=$(wildcard executor_container.c periodicExecutor.c task.c timeTools.c)
OBJS=$(SRCS:.c=.o)
HEAP_SRC= $(wildcard $(HEAP_PATH)genBinaryHeap.c)
HEAP_OBJS=$(HEAP_SRC:.c=.o)
VEC_SRC= $(wildcard $(VEC_PATH)generic_vector.c)
VEC_OBJS=$(VEC_SRC:.c=.o)
LOG_SRC= $(wildcard $(LOG_PATH)logger.c)
LOG_OBJS=$(LOG_SRC:.c=.o)

TARGET=testSuite.out

$(TARGET): $(OBJS) $(HEAP_OBJS) $(VEC_OBJS) $(LOG_OBJS)
	$(CC) $(CFLAGS) executor_container.c periodicExecutor.c task.c timeTools.c $(HEAP_PATH)genBinaryHeap.c $(VEC_PATH)generic_vector.c $(LOG_PATH)logger.c -c
	$(CC) -shared -Wl,-soname,libexecutor.so.1 -o libexecutor.so.1.0   *.o
	sudo ln -sf libexecutor.so.1.0 libexecutor.so.1
	sudo ln -sf libexecutor.so.1.0 libexecutor.so
	sudo mv libexecutor.so.1.0 $(LIB_PATH)
	sudo mv libexecutor.so.1 $(LIB_PATH)
	sudo mv libexecutor.so $(LIB_PATH)
	$(CC) $(CFLAGS) -L$(LIB_PATH) executor_test.c -lexecutor -o $(TARGET)
	export LD_LIBRARY_PATH=../../lib:$LD_LIBRARY_PATH


all: $(TARGET)

# *** COMMANDS
clean:
	-rm $(TARGET)
	-rm *.o

run:
	./$(TARGET)
	
##################################


