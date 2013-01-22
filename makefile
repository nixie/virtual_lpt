CFLAGS=-std=c99 -Wall -pedantic -W -ggdb3
# uncomment this to disable debugging
#CFLAGS=-std=c99 -Wall -pedantic -W -ggdb3 -DNDEBUG

PROG=vp
SRC=main.[ch] iface.[ch]

all: $(PROG)

main.o: main.c makefile
	gcc -c $(CFLAGS) main.c

iface.o: iface.c iface.h makefile
	gcc -c $(CFLAGS) iface.c

$(PROG): iface.o main.o makefile
	gcc -o $(PROG) -lSDL iface.o main.o

clean:
	rm -vf $(PROG) ./*.o $(PROG).tar.gz

tgz: $(SRC) makefile
	echo making source tarball
	tar -cvzf $(PROG).tar.gz $(SRC) makefile

edit: 
	vim -p $(SRC)
