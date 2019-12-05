CC = gcc
AS = as
LD = ld

OBJECTS = password.o base.o secuencia.o

CFLAGS = -Wall -g  -c
ASFLAGS = -g 
LDFLAGS = -g 

main.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o main.elf -lwiringPi -lncurses

punto3_menu.o: password.c
	$(CC) $(CFLAGS) password.c -lncurses -o password.o

base.o: base.c
	$(CC) $(CFLAGS) base.c -o base.o
	
secuencia.o: secuencia.c
	$(CC) $(CFLAGS) secuencia.c -o secuencia.o

clean:
	rm *.o *.elf 
