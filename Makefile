CC=clang++ -std=c++11 -O2

all: bin/usrt bin/usr

bin/usrt: obj/atom.o obj/ligand.o obj/moment.o obj/usrt.o
	${CC} -o $@ $^

bin/usr: obj/atom.o obj/ligand.o obj/moment.o obj/usr.o
	${CC} -o $@ $^

obj/%.o: src/%.cpp
	${CC} -o $@ $< -c

clean:
	rm -f bin/usrt bin/usr obj/*
