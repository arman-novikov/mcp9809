#!/usr/bin/python
import smbus # pip3 install smbus
from time import sleep

DEFAULT_ADDR = 0x18
TEMP_REG = 0x05
MCP9808_REG_CONFIG = 0x01
MCP9808_SHUTDOWN_CONFIG = 0x0100
MCP9808_REG_MANUF_ID = 0x06
MCP9808_REG_DEVICE_ID = 0x07

# 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)

class MCP9808:
	"""i2c precise thermometer driver"""
	def __init__(self, bus, addr=DEFAULT_ADDR):
		self._bus = smbus.SMBus(bus)
		self._addr = addr
		#self._bus.write_word_data(self._addr, MCP9808_REG_CONFIG, 0x00)
		sleep(0.3)
	def get_T_c(self):
                #self._bus.write_byte_data(self._addr, TEMP_REG, 0)
                data = self._bus.read_word_data(self._addr, TEMP_REG)
                print(data)
                data = ((data << 8) & 0xFF00) + (data >> 8)  # bigendian to little              
                return self._convert_TEMP_REG_to_C(data)

	def _convert_TEMP_REG_to_C(self, data):
		# data = [mask: 000] [sign: 1] [data: 1111 1111 1111]
		if data == 0xFFFF: # invalid read
                    print("EINVAL")
                    return -22 # todo: use throw

		res = data & 0x0FFF # zeroing not data bits
		sign = data & 0x1000
		res /= 16.0 # scale
		if sign: # if below zero
			res -= 256	# 256 => according to its datasheet
		return res


if __name__ == "__main__":
	thermo = MCP9808(1)
	while(1):
            print( thermo.get_T_c() )
            sleep(1.0)