import serial
from tkinter import messagebox

class SerialHandler:
    def __init__(self, update_callback, port="", baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.serial_connection = None
        self.update_callback = update_callback

    def set_port(self, port):
        self.port = port
        self.setup_serial()

    def setup_serial(self):
        if self.port:
            try:
                self.serial_connection = serial.Serial(self.port, self.baudrate)
                self.read_serial()
            except serial.SerialException as e:
                messagebox.showerror("Serial Error", f"Error: {e}")

    def read_serial(self):
        if self.serial_connection:
            try:
                data = self.serial_connection.readline().decode().strip()
                if data:
                    self.update_callback(data)
            except serial.SerialException as e:
                messagebox.showerror("Serial Error", f"Error: {e}")
                self.serial_connection = None
        
        if self.serial_connection:
            self.serial_connection.after(1, self.read_serial)
