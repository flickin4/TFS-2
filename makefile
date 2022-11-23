a.out : main.o drive.o ui.o
	gcc main.o drive.o ui.o

main.o : main.c drive.h
	gcc -c main.c

drive.o : drive.c ui.h
	gcc -c drive.c

ui.o : ui.c ui.h
	gcc -c ui.c

clean:
	rm *.o a.out