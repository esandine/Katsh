strtest: exec.c
	gcc cmds.c -c
	gcc utils.c -c
	gcc -g cmds.o utils.o exec.c
run:
	./a.out
clean:
	rm a.out
