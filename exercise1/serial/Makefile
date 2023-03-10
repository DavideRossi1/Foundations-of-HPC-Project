CC=gcc

INCDIR=include
OBJDIR=obj

RW=$(OBJDIR)/read-write_pgm.o 
INIT=$(OBJDIR)/init.o 
RUN=$(OBJDIR)/run.o 
OBJECTS=main.o $(RW) $(INIT) $(RUN)

main.x: $(OBJECTS)
#	mkdir -p output
	mkdir -p snap
	$(CC) $(OBJECTS) -o $@
main.o: main.c
	$(CC) -c main.c
obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $^ -o $@
clean:
	rm -rf *.o *.x
	rm -rf obj/*.o
	rm -rf snap/snapshot*
