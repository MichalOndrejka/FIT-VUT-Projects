all:
	g++ -std=gnu++11 -pedantic -O2 -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS main.cpp -o ims -lsimlib -lm

run: ims
	./ims 30

clean:
	rm -f ims