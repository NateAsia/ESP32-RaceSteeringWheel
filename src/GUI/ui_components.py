import tkinter as tk
from tkinter import ttk

from led import LED


class UIComponents:
    def __init__(self, master, setup_serial_callback):
        self.master = master
        self.setup_serial_callback = setup_serial_callback
        self.buttons = []
        self.leds = []
        self.bars = []
        self.mode_labels = []
        self.labels = []
        self.textLabels = [
            "RT2Y MODE ACTIVE: ",
            "DUAL HOLD ACTIVE: ",
            "L MODE: ",
            "R MODE: ",
            "f-1: ",
            "f-2: ",
        ]
        self.serial_monitor_label = None
        self.connected_led = None

    def setup_frames(self):
        self.connected_frame = ttk.Frame(self.master)
        self.top_outer_frame = ttk.Frame(self.master)
        self.left_frame = ttk.Frame(self.top_outer_frame)
        self.right_frame = ttk.Frame(self.top_outer_frame)
        self.bottom_frame = ttk.Frame(self.master)
        self.center_frame = ttk.Frame(self.top_outer_frame)

        self.connected_frame.pack(pady=(10, 0))
        self.top_outer_frame.pack(side=tk.TOP, pady=(50, 10))
        self.left_frame.grid(row=0, column=0, sticky=tk.S)
        self.center_frame.grid(row=0, column=1, padx=50, sticky=tk.S)
        self.right_frame.grid(row=0, column=2, sticky=tk.S)
        self.bottom_frame.pack(pady=(15, 0))

    def setup_buttons(self):
        self.custom_button("L1 (1)", "blue", flip=True, frame=self.left_frame)
        self.custom_button("L2 (2)", "red", flip=True, frame=self.left_frame)
        self.custom_button("L3 (3)", "yellow", flip=True, frame=self.left_frame)

        self.custom_button("R2 (4)", "red", row=0, col=0, frame=self.right_frame)
        self.custom_button("R3 (5)", "yellow", row=1, col=0, frame=self.right_frame)

        self.custom_button(
            "REAR L (6)", "lightblue", row=0, flip=True, frame=self.bottom_frame
        )
        self.custom_button(
            "REAR R (7)", "lightblue", row=0, col=2, frame=self.bottom_frame
        )

        connect_button = tk.Button(
            self.connected_frame, text="Connect", command=self.setup_serial_callback
        )
        connect_button.grid(row=0, column=1)
        self.connected_led = LED(self.connected_frame, color="green", off_color="red")
        self.connected_led.grid(row=0, column=0)

    def setup_progress_bars(self):
        for i in range(3):
            pb = tk.ttk.Progressbar(
                self.center_frame,
                orient="vertical",
                maximum=3000,
                value=1500,
                mode="determinate",
                length=270,
            )
            pb.grid(row=0, column=i, padx=10 if i != 1 else 5)
            self.bars.append(pb)

    def setup_labels(self):
        for i, text in enumerate(self.textLabels):
            label = tk.Label(self.bottom_frame, text=text)
            label.grid(
                row=1 + i,
                column=1,
                columnspan=2,
                sticky="s",
                pady=(10, 0) if i == 0 else 0,
            )
            self.labels.append(label)

        self.serial_monitor_label = tk.Label(
            self.master, text="PLEASE CONNECT TO STEERING WHEEL"
        )
        self.serial_monitor_label.pack(pady=(0, 15), side=tk.BOTTOM)

    def custom_button(
        self, name="BTN", color="green", flip=False, row=None, col=None, frame=None
    ):
        if row is None:
            row = len(self.leds)
        if col is None:
            col = 0
        if not frame:
            frame = self.master
        led = LED(frame, size=100, color=color)
        led.grid(row=row, column=(col + 1) if flip else col, padx=0, pady=0)
        button = tk.Button(frame, text=name, state=tk.DISABLED)
        button.grid(row=row, column=col if flip else col + 1, padx=0, pady=0)
        self.buttons.append(button)
        self.leds.append(led)

    def update_serial_monitor_label(self, data):
        self.serial_monitor_label.config(text=data)

    def update_button_states(self, data: list[bool]):
        # Implement button state update logic here
        # TODO
        pass

    def update_gauge_status(self, data: list[int]):
        # Implement gauge status update logic here
        # TODO
        pass

    def update_mode_status(self, data: list[int]):
        # Implement mode status update logic here
        # TODO
        pass
