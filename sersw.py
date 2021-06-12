#!/usr/bin/env python3
from time import sleep
from serial import Serial

class RelaisKarte(Serial):
	def __init__(self, **kwargs):
		super(RelaisKarte, self).__init__(**kwargs)
		self.rtscts = False
		self.xonxoff = False
		self.timeout = .5
		self.writeTimeout = .5
		self.read(size=4, ) # read garbage from previous runs
		self.read(size=4, ) # read garbage from previous runs
		self.read(size=4, ) # read garbage from previous runs

	def send_command(self, a, b, c):
		buf = bytes([a, b, c, a ^ b ^ c, ])
		self.write(buf)
		print('send', buf)
		buf = self.read(size=4, )
		print('received', buf)
		return buf

	def set_on(self, bits, port=0x01):
		return self.send_command(
			0x01, # Command SET
			port, # Port
			bits, # Value
			)

	def get_on(self, port=0x01):
		return self.send_command(
			0x02, # Command GET
			port, # Port
			0, # Value
			)

	def all_on(self, port=0x01):
		return self.send_command(
			0x03, # Command ALL_ON
			port, # Port
			0, # Value
			)

	def all_off(self, port=0x01):
		return self.send_command(
			0x04, # Command ALL_OFF
			port, # Port
			0, # Value
			)

def main(port='/dev/ttyUSB0', baudrate=19200, *args):
	r = RelaisKarte(port=port, baudrate=int(baudrate), )
	r.all_off()
	sleep(.4)
	r.all_on()
	sleep(.4)
	r.set_on(0xf0)
	sleep(.4)
	r.set_on(0x0f)
	sleep(.4)
	for n in range(8):
		r.set_on(1<<n)
		r.get_on()
		sleep(.4)
	r.all_off()

if __name__ == '__main__':
	from sys import argv
	exit(main(*argv[1:]))
# vim:tw=0:nowrap
