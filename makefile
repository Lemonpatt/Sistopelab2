all: Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o lab2 broker worker

Crear_archivos.o: Crear_archivos.c Crear_archivos.h
			g++ -g -c Crear_archivos.c

Bmp.o: Bmp.c Bmp.h
			g++ -g -c Bmp.c

Filtros.o: Filtros.c Filtros.h
			g++ -g -c Filtros.c

fbroker.o: fbroker.c fbroker.h
			g++ -g -c fbroker.c

fworker.o: fworker.c fworker.h
			g++ -g -c fworker.c

lab2: Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o lab2.c
	g++ -g Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o lab2.c -o lab2

broker: Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o broker.c
	g++ -g Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o broker.c -o broker

worker: Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o worker.c
	g++ -g Crear_archivos.o Bmp.o Filtros.o fbroker.o fworker.o worker.c -o worker

clean:
	rm -f *.o Crear_archivos Bmp Filtros fbroker fworker lab2 broker worker

