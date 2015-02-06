raycast:
	gcc -lm raycast.c -o $@

clean:
	rm --force raycast
