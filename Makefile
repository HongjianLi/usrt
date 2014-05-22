CC=clang++

all: bin/encode bin/score

bin/encode: obj/atom.o obj/ligand.o obj/moment.o obj/encode.o
	${CC} -o $@ $^

bin/score: obj/score.o
	${CC} -o $@ $^

obj/%.o: src/%.cpp
	${CC} -o $@ $< -c -std=c++11 -O2

clean:
	rm -f bin/encode bin/score obj/*
