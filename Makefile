all:
	gcc speck.c -o speck -g
	gcc speck-ks.c -o speck-ks -g
clean:
	rm speck speck-ks

