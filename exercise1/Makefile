CC=gcc -g

INCDIR=include
OBJDIR=obj

RW=$(OBJDIR)/my_write_pgm.o 
INIT=$(OBJDIR)/my_init.o 
STAT=$(OBJDIR)/run_stat.o 
#IT_WAVE=$(OBJDIR)/iterate_wave.o
#IT_ORDERED=$(OBJDIR)/iterate_ordered.o
OBJECTS=mymain.o $(RW) $(INIT) $(STAT)

mymain.x: $(OBJECTS)
	mkdir -p output
	mkdir -p snap
	$(CC) $(OBJECTS) -o $@
mymain.o: mymain.c
	$(CC) -c mymain.c
obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $^ -o $@
clean:
	rm -rf *.o *.x
	rm -rf obj/*.o
	rm -rf snap/snapshot*