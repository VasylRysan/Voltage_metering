from device.voltmeter import ArduinoVoltmeter

if __name__ == '__main__':

    active_port = "COM8"
    baudrate = 115200

    voltmeter = ArduinoVoltmeter(active_port, baudrate)
    voltmeter.start_voltmeter(2)
