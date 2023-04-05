CC = gcc
program:
	$(CC) -o program src/main.c src/compress.c src/dataReader.c src/frequency.c src/splitData.c src/stripCompressFile.c src/tree.c src/sumController.c src/decompressor.c -lm
