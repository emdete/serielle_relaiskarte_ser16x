all: sersw

sersw: sersw.c

run:
	python3 -u ./sersw.py

dbg: sersw
	./sersw
	./sersw /dev/ttyUSB0 = +1357 -15

