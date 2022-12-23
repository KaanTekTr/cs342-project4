all: diskprint cachetest

diskprint: diskprint.c
	gcc -o diskprint diskprint.c ext2_structures.h

cachetest: cache_test.c
	gcc -o cache-test cache_test.c

clean:
	rm -fr *~ diskprint