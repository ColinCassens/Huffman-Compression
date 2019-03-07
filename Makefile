#GCC = gcc -std=c99 -Wall -Wshadow -Wvla -pedantic -g *.c -o
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
VAL = valgrind --tool=memcheck --log-file=valgrind.log --leak-check=full --show-reachable=yes --verbose  --track-origins=yes
TESTFLAGS = -DTEST_MAIN
SRCS = main.c #UPDATE WITH MORE FILES
OBJS = $(SRCS:%.c=%.o)

PA1: $(OBJS)
	$(GCC) $(TESTFLAGS) $(OBJS) -o pa1

.c.o:
	$(GCC) $(TESTFLAGS) -c $*.c

VAL: pa1
	$(VAL) ./pa1 pa1_examples/original/gophers gophers.count gophers.tree gophers.code gophers.hbt

test1: pa1
	./pa1 pa1_examples/original/gophers gophers.count gophers.tree gophers.code gophers.hbt 
	diff gophers.count pa1_examples/count/gophers.count
	diff gophers.tree pa1_examples/tree/gophers.tree
	diff gophers.code pa1_examples/code/gophers.code
	#diff gophers.hbt pa1_examples/compressed/gophers.hbt

test2: pa1
	./pa1 pa1_examples/original/woods woods.count woods.tree woods.code woods.hbt 
	diff woods.count pa1_examples/count/woods.count
	diff woods.tree pa1_examples/tree/woods.tree
	diff woods.code pa1_examples/code/woods.code

test3: pa1
	./pa1 pa1_examples/original/stone stone.count stone.tree stone.code stone.hbt 
	diff stone.count pa1_examples/count/stone.count
	diff stone.tree pa1_examples/tree/stone.tree
	diff stone.code pa1_examples/code/stone.code

#Not working
test4: pa1
	./pa1 pa1_examples/original/lorum lorum.count lorum.tree lorum.code lorum.hbt 	# Almost there
	diff lorum.count pa1_examples/count/lorum.count
	diff lorum.tree pa1_examples/tree/lorum.tree		# Not working
	
#Not working
test5: pa1
	./pa1 pa1_examples/original/binary1 binary1.count binary1.tree binary1.code binary1.hbt # Core Dump Again
	diff binary1.count pa1_examples/count/binary1.count
	diff binary1.tree pa1_examples/tree/binary1.tree 	# Core Dump

test6: pa1
	./pa1 pa1_examples/original/empty empty.count empty.tree empty.code empty.hbt 
	diff empty.count pa1_examples/count/empty.count
	diff empty.tree pa1_examples/tree/empty.tree 
	diff empty.code pa1_examples/code/empty.code

zip:
	zip pa1.zip main.c Makefile main.h

clean: 
	rm -f *.count *.tree *.code *.hbt main.o valgrind.log
	clear

depend:
	makedepend $(SRCS)# DO NOT DELETE