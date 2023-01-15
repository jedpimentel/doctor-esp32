import serial
ser = serial.Serial('COM3', 9600)

s = bytearray([0b11111111, 255, 255])
ser.write(s)

print('wut')