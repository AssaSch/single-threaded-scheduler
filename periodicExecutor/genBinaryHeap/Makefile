
SRC = genBinaryHeap_test.c genBinaryHeap.c $(VEC_PATH)generic_vector.c $(INC_PATH)genericHeap.h $(VEC_PATH)generic_vector.h $(INC_PATH)mu_test.h

INC_PATH = ./

VEC_PATH = ../genericVector/

FLAGS = -g -ansi -pedantic -Wall -Wconversion 

a.out: $(SRC)
		
	gcc $(FLAGS) -I$(INC_PATH) -I$(VEC_PATH) genBinaryHeap_test.c genBinaryHeap.c $(VEC_PATH)generic_vector.c -lm

run: 
	./a.out
		
clean:
	rm a.out
