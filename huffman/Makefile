all:
	gcc -Wall huffman.c -o a -lm

check:
	valgrind -s --leak-check=full --show-leak-kinds=all ./a -c ./test/huffman.docx ./test/out
	rm ./test/out

test:
	./a -c ./test/huffman.c ./test/out
	./a -x ./test/out ./test/restored.c
	diff -s ./test/huffman.c ./test/restored.c

	./a -c ./test/huffman.docx ./test/out
	./a -x ./test/out ./test/restored.docx
	diff -s ./test/huffman.docx ./test/restored.docx

	rm ./test/out ./test/restored.docx ./test/restored.c

.PHONY: test