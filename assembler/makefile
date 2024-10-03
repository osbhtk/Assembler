assembler:  assembler.o get_input.o symbol.o get_word.o put_word.o valid_word.o second_pass.o build_files.o put_extern.o update_obj.o
	gcc -g -ansi -pedantic -Wall assembler.o get_input.o symbol.o get_word.o put_word.o valid_word.o second_pass.o build_files.o put_extern.o update_obj.o -o assembler
assembler.o:  assembler.c assembler.h
	gcc -c -ansi -pedantic -Wall assembler.c -o assembler.o
get_input.o:  get_input.c assembler.h
	gcc -c -ansi -pedantic -Wall get_input.c -o get_input.o
symbol.o:  symbol.c assembler.h
	gcc -c -ansi -pedantic -Wall symbol.c -o symbol.o
get_word.o:  get_word.c assembler.h
	gcc -c -ansi -pedantic -Wall get_word.c -o get_word.o
put_word.o:  put_word.c assembler.h
	gcc -c -ansi -pedantic -Wall put_word.c -o put_word.o
valid_word.o:  valid_word.c assembler.h
	gcc -c -ansi -pedantic -Wall valid_word.c -o valid_word.o
second_pass.o:  second_pass.c assembler.h
	gcc -c -ansi -pedantic -Wall second_pass.c -o second_pass.o
build_files.o:  build_files.c assembler.h
	gcc -c -ansi -pedantic -Wall build_files.c -o build_files.o
put_extern.o:  put_extern.c assembler.h
	gcc -c -ansi -pedantic -Wall put_extern.c -o put_extern.o
update_obj.o:  update_obj.c assembler.h
	gcc -c -ansi -pedantic -Wall update_obj.c -o update_obj.o

