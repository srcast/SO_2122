all: server client

server: bin/sdstored

client: bin/sdstore

bin/sdstored: obj/sdstored.o obj/estruturas.o
	gcc -g obj/estruturas.o obj/sdstored.o -o bin/sdstored

obj/sdstored.o: src/sdstored.c 
	gcc -Wall -w -g -c src/sdstored.c -o obj/sdstored.o

bin/sdstore: obj/sdstore.o
	gcc -g obj/sdstore.o -o bin/sdstore

obj/sdstore.o: src/sdstore.c
	gcc -Wall -w -g -c src/sdstore.c -o obj/sdstore.o

obj/estruturas.o: src/estruturas.c src/estruturas.h
	gcc -Wall -w -g -c src/estruturas.c -o obj/estruturas.o

run:
	bin/stored etc/transf.config bin/sdstore-transformations

teste:
	proc-file 3 samples/sample1.txt outputs/saida1.txt bcompress nop gcompress nop
	proc-file 2 samples/sample2.txt outputs/saida2.txt gcompress nop bcompress

clean:
	rm obj/* tmp/* bin/sdstore bin/sdstored