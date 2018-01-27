
SRC = generic_vector_test.c generic_vector.c $(INC_PATH)vector.h $(INC_PATH)mu_test.h

#INC_PATH = ../../include/
INC_PATH = ./

FLAGS = -g -ansi -pedantic -Wall -Wconversion 

a.out: $(SRC)
		
	gcc $(FLAGS) -I$(INC_PATH) generic_vector_test.c generic_vector.c -lm

run: 
	./a.out
		
clean:
	rm a.out
