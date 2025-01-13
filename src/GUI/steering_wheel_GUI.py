import os
import tkinter as tk

from serial_handler import SerialHandler
from ui_components import UIComponents


class SteeringWheelGUI:
    def __init__(self, master):
        self.master = master
        self.master.geometry("600x800")
        self.master.title("Steering Wheel GUI")

        self.serial_handler = SerialHandler(self.update_ui_status)
        self.ui_components = UIComponents(self.master, self.serial_handler.setup_serial)

        self.setup_ui()

    def setup_ui(self):
        self.setup_menu()
        self.ui_components.setup_frames()
        self.ui_components.setup_buttons()
        self.ui_components.setup_progress_bars()
        self.ui_components.setup_labels()

    def setup_menu(self):
        menu_bar = tk.Menu(self.master)
        menu_bar.add_cascade(label="Select Port", menu=self.create_port_menu())
        self.master.config(menu=menu_bar)

    def create_port_menu(self):
        menu = tk.Menu(self.master, tearoff=0)
        ports = [p for p in os.listdir("/dev") if p.startswith("cu")]
        for port in ports:
            menu.add_command(label=port, command=lambda p=port: self.set_port(p))
        return menu

    def set_port(self, port):
        self.serial_handler.set_port("/dev/" + port)

    def update_ui_status(self, data):
        self.ui_components.update_serial_monitor_label(data)
        sections = data.split(":")
        for i, item in enumerate(sections):
            if item.startswith("BTNS"):
                self.ui_components.update_button_states(sections[i + 1])
            if item.startswith("POTS"):
                self.ui_components.update_gauge_status(sections[i + 1])
            if item.startswith("MODES"):
                self.ui_components.update_mode_status(sections[i + 1])
