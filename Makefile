CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

program: $(OBJ)
	$(CC) -o program src/main.c src/compress.c src/dataReader.c src/frequency.c src/splitData.c src/stripCompressFile.c src/tree.c src/sumController.c src/decompressor.c -lm


clean:
	rm program src/*.o

test1_file1 = test/pan_tadeusz.txt
test1_file2 = test/pan_tadeusz_compressed.txt
test1_file3 = test/pan_tadeusz_decompressed.txt

test2_file1 = test/test2.jpeg
test2_file2 = test/test2_compressed.jpeg
test2_file3 = test/test2_decompressed.jpeg

test_formula:
	echo "Testing:"
	echo "infile: $(file1)"
	echo "Compressed file: $(file2)"
	echo "Decompressed file: $(file3)"
	@$(MAKE) -s compare file1=$(file1) file2=$(file3)

test1: program	
	@./program $(test1_file1) $(test1_file2)
	@./program $(test1_file2) $(test1_file3)	
	@$(MAKE) -s test_formula file1=$(test1_file1) file2=$(test1_file2) file3=$(test1_file3)
	
test2: program
	@./program $(test2_file1) $(test2_file2)
	@./program $(test2_file2) $(test2_file3)	
	@$(MAKE) -s test_formula file1=$(test2_file1) file2=$(test2_file2) file3=$(test2_file3)

test3: program
	@./program $(test1_file1) $(test1_file2) --12
	@./program $(test1_file2) $(test1_file3)	
	@$(MAKE) -s test_formula file1=$(test1_file1) file2=$(test1_file2) file3=$(test1_file3)
	
test4: program
	@./program $(test1_file1) $(test1_file2) --16
	@./program $(test1_file2) $(test1_file3)	
	@$(MAKE) -s test_formula file1=$(test1_file1) file2=$(test1_file2) file3=$(test1_file3)

test5: program
	@./program $(test2_file1) $(test2_file2) --12
	@./program $(test2_file2) $(test2_file3)	
	@$(MAKE) -s test_formula file1=$(test2_file1) file2=$(test2_file2) file3=$(test2_file3)

test6: program
	@./program $(test2_file1) $(test2_file2) --16
	@./program $(test2_file2) $(test2_file3)	
	@$(MAKE) -s test_formula file1=$(test2_file1) file2=$(test2_file2) file3=$(test2_file3)



compare:
	echo "Checking: $(file1) $(file2)"
	@if cmp -s $(file1) $(file2); then \
		echo "files are the same"; \
	else \
		echo "files are different"; \
	fi
