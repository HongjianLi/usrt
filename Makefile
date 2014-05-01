CC=clang++ -std=c++11 -O2

bin/usrt: obj/atom.o obj/ligand.o obj/main.o
	${CC} -o $@ $^

obj/%.o: src/%.cpp
	${CC} -o $@ $< -c

clean:
	rm -f bin/usrt obj/*
