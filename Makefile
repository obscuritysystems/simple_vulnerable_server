vulnerable_server: vulnerable_server.c
	gcc -fno-stack-protector -o vulnerable_server vulnerable_server.c
clean:
	rm vulnerable_server
