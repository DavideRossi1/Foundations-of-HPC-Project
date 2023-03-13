CC=mpicc -O3 -fopenmp -march=native -g -Wall -Wextra

INCDIR=include
OBJDIR=obj

RW=$(OBJDIR)/read_write_pgm2.o 
INIT=$(OBJDIR)/w_init2.o 
IT_STATIC=$(OBJDIR)/grow_static5.o 
#IT_WAVE=$(OBJDIR)/iterate_wave.o
IT_ORDERED=$(OBJDIR)/grow_ordered.o
OBJECTS=main.o $(RW) $(INIT) $(IT_ORDERED) $(IT_STATIC)

main.x: $(OBJECTS)
	mkdir -p output
	mkdir -p snap
	$(CC) $(OBJECTS) -o $@
main.o: main.c
	$(CC) -c main.c
obj/%.o: %.c
	mkdir -p obj
	$(CC) -c $^ -o $@
clean:
	rm -rf *.o *.x
	rm -rf obj/*.o
	rm -rf snap/*.pgm
