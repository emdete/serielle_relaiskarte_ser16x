all:
	gcc -O -Wall -o sersw sersw.c
	./sersw
	./sersw /dev/ttyUSB0 = +1357 -15

run:
	python3 -u ./sersw.py

dbg:

