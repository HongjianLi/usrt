CC=clang++

all: bin/usrtencode bin/usrencode bin/usrscore

bin/usrtencode: obj/atom.o obj/ligand.o obj/moment.o obj/usrtencode.o
	${CC} -o $@ $^

bin/usrencode: obj/atom.o obj/ligand.o obj/moment.o obj/usrencode.o
	${CC} -o $@ $^

bin/usrscore: obj/usrscore.o
	${CC} -o $@ $^

obj/%.o: src/%.cpp
	${CC} -o $@ $< -c -std=c++11 -O2

clean:
	rm -f bin/usrtencode bin/usrencode bin/usrscore obj/*
