all:
	gcc -o sersw sersw.c
	./sersw

run:
	python3 -u ./sersw.py

dbg:

