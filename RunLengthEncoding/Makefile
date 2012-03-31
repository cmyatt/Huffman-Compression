CC=gcc
CFLAGS=-I. -lm

OBJ = ../bits.o ../codes.o ../buffer.o ../defs.o ../file.o ../node.o ../profiler.o ../huffman.o run_length.o run_length_test.o


../bits.o : ../bits.c ../bits.h
	$(CC) -c -o $@ $< $(CFLAGS)

../codes.o : ../codes.c ../defs.h ../codes.h
	$(CC) -c -o $@ $< $(CFLAGS)

../buffer.o : ../buffer.c ../buffer.h ../codes.h
	$(CC) -c -o $@ $< $(CFLAGS)

../defs.o : ../defs.c ../defs.h ../bits.h
	$(CC) -c -o $@ $< $(CFLAGS)

../file.o : ../file.c ../file.h ../defs.h
	$(CC) -c -o $@ $< $(CFLAGS)

../node.o : ../node.c ../defs.h ../node.h
	$(CC) -c -o $@ $< $(CFLAGS)

../profiler.o : ../profiler.c ../profiler.h ../defs.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
../huffman.o : ../huffman.c ../defs.h ../node.h ../file.h ../codes.h ../buffer.h ../profiler.h ../huffman.h
	$(CC) -c -o $@ $< $(CFLAGS)

run_length.o : run_length.c run_length.h ../bits.h ../buffer.h ../file.h ../defs.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
run_length_test.o : run_length_test.c ../defs.h ../file.h ../buffer.h run_length.h
	$(CC) -c -o $@ $< $(CFLAGS)


run_length_test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
