import serial
import time

term=serial.Serial("/dev/ttyAMA0",9600)

term.write(0x32)
term.write(0x30)
term.write(0x66)
term.write(0)
term.write(0xff)
term.write(0xff)