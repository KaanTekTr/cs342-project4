all: diskprint

pagesim: diskprint.c
	gcc -o diskprint diskprint.c ext2_structures.h

clean:
	rm -fr *~ diskprint