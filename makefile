all: diskprint

pagesim: diskprint.c
	gcc -o diskprint diskprint.c diskprint.h 

clean:
	rm -fr *~ diskprint