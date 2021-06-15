all: sersw

sersw: sersw.c

run: sersw
	#python3 -u ./sersw.py
	#./sersw
	./sersw /dev/ttyUSB0 = +1357 -15

dbg:
	$(foreach remote,$(shell git remote),$(shell git push $(remote)))
	true

