install:
	gcc -pthread main.c -o main.exe -lgmp
	gcc make_heavy.c -o make_heavy.exe

clear:
	rm ./*.exe &
	rm ./*.out &
	rm ./*.temp
rm: install
	./main.exe
