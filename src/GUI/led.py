import tkinter as tk

class LED(tk.Canvas):
    def __init__(self, master, size=20, color="red", off_color="black", **kwargs):
        super().__init__(master, width=size, height=size, **kwargs)
        self.size = size
        self.color = color
        self.off_color = off_color
        self.create_oval(size/4, size/4, size, size, fill="black", outline="black")
        self.state = False
        self.update_state()

    def update_state(self):
        fill_color = self.color if self.state else self.off_color
        self.itemconfig(1, fill=fill_color)

    def set_state(self, state):
        self.state = state
        self.update_state()