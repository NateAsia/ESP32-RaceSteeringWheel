import unittest
import tkinter as tk
from tkinter import ttk
from ui_components import UIComponents

class TestUIComponents(unittest.TestCase):
    def setUp(self):
        self.root = tk.Tk()
        self.ui_components = UIComponents(self.root, lambda: None)

    def tearDown(self):
        self.root.destroy()

    def test_setup_frames(self):
        self.ui_components.setup_frames()
        self.assertIsInstance(self.ui_components.connected_frame, ttk.Frame)
        self.assertIsInstance(self.ui_components.top_outer_frame, ttk.Frame)
        self.assertIsInstance(self.ui_components.left_frame, ttk.Frame)
        self.assertIsInstance(self.ui_components.right_frame, ttk.Frame)
        self.assertIsInstance(self.ui_components.bottom_frame, ttk.Frame)
        self.assertIsInstance(self.ui_components.center_frame, ttk.Frame)

    def test_custom_button(self):
        self.ui_components.custom_button("Test", "red", False, 0, 0, self.root)
        self.assertEqual(len(self.ui_components.buttons), 1)
        self.assertEqual(len(self.ui_components.leds), 1)
        self.assertEqual(self.ui_components.buttons[0].cget("text"), "Test")

    def test_update_serial_monitor_label(self):
        self.ui_components.setup_frames()
        self.ui_components.setup_labels()
        test_data = "Test data"
        self.ui_components.update_serial_monitor_label(test_data)
        self.assertEqual(
            self.ui_components.serial_monitor_label.cget("text"), test_data
        )

    def test_setup_labels(self):
        self.ui_components.setup_frames()  # Ensure frames are set up
        self.ui_components.setup_labels()

        # Check if the correct number of labels were created
        self.assertEqual(
            len(self.ui_components.labels), len(self.ui_components.textLabels)
        )

        # Check if the labels have the correct text
        for i, label in enumerate(self.ui_components.labels):
            self.assertEqual(label.cget("text"), self.ui_components.textLabels[i])

        # Check if the labels are in the bottom frame
        for label in self.ui_components.labels:
            self.assertEqual(label.master, self.ui_components.bottom_frame)

        # Check if the labels are correctly positioned
        for i, label in enumerate(self.ui_components.labels):
            grid_info = label.grid_info()
            self.assertEqual(grid_info["row"], 1 + i)
            self.assertEqual(grid_info["column"], 1)
            self.assertEqual(grid_info["columnspan"], 2)
            self.assertEqual(grid_info["sticky"], "s")
            if i == 0:
                self.assertEqual(grid_info["pady"], (10, 0))
            else:
                self.assertEqual(grid_info["pady"], 0)

        # Check if the serial monitor label was created
        self.assertIsNotNone(self.ui_components.serial_monitor_label)
        self.assertEqual(
            self.ui_components.serial_monitor_label.cget("text"),
            "PLEASE CONNECT TO STEERING WHEEL",
        )
        self.assertEqual(self.ui_components.serial_monitor_label.master, self.root)

        # Check if the serial monitor label is correctly positioned
        pack_info = self.ui_components.serial_monitor_label.pack_info()
        self.assertEqual(pack_info["pady"], (0, 15))
        self.assertEqual(pack_info["side"], "bottom")


if __name__ == "__main__":
    unittest.main()
