from serial import Serial
from serial import SerialException


class ArduinoVoltmeter:
    def __init__(
            self,
            port_name: str,
            baud: int,
    ) -> None:
        self.port_name = port_name
        self.baud = baud
        self.cycles = 1
        self.voltmeter = None

    def start_voltmeter(self, cycles: int):
        try:
            self.voltmeter = Serial(self.port_name, self.baud, timeout=1)
        except SerialException:
            print("Please connect arduino to the USB port")
        if self.voltmeter:
            self.connect()
            self.set_nplc(cycles)
            self.measure()

    def connect(self) -> None:
        print("Waiting for response from Arduino...")
        while True:
            if self.voltmeter.in_waiting > 0:
                response = self.voltmeter.readline().decode().strip()
                print(f"{response}.")
                break
            else:
                continue

    def set_nplc(self, cycles: int) -> None:
        print("Trying to set NPLC...")
        command = f"c{cycles}"
        self.voltmeter.write(command.encode())
        while True:
            if self.voltmeter.in_waiting > 0:
                response = self.voltmeter.readline().decode().strip()
                if response == "OK":
                    self.cycles = cycles
                    print(f"NPLC is set to {self.cycles}.")
                    print(
                        "Measuring duration is "
                        f"{1000 / 50 * self.cycles} ms."
                    )
                    break
                else:
                    print(f"Arduino {response}.")
                    print("NPLC is set to 1 (default). ")
                    print(
                        "Measuring duration is "
                        f"{1000 / 50 * self.cycles} ms."
                    )
                    break

    def measure(self) -> None:
        print("Starting measurements...")
        try:
            while True:
                self.voltmeter.write(b'm')
                if self.voltmeter.in_waiting > 0:
                    voltage = self.voltmeter.readline().decode().strip()
                    print(f"Voltage: {voltage} V, NPLC = {self.cycles}.")
        except KeyboardInterrupt:
            print("Measurement stopped by user.")
