CC=mpicc -O3 -fopenmp -march=native -g -Wall -Wextra

INCDIR=include
OBJDIR=obj

RW=$(OBJDIR)/read_write_pgm.o 
INIT=$(OBJDIR)/w_init.o 
IT_STATIC=$(OBJDIR)/grow_static.o
IT_ORDERED=$(OBJDIR)/grow_ordered.o
OBJECTS= obj/main.o $(RW) $(INIT) $(IT_ORDERED) $(IT_STATIC)

main.x: $(OBJECTS)
	mkdir -p snap
	$(CC) $(OBJECTS) -o $@
obj/main.o: main.c
	$(CC) -c main.c -o $@
obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $^ -o $@
clean:
	rm -rf main.x
	rm -rf obj/*.o
	rm -rf snap/snapshot*
