import unittest
from unittest.mock import Mock, patch
import tkinter as tk
from steering_wheel_gui import SteeringWheelGUI
from ui_components import UIComponents
from serial_handler import SerialHandler

class TestSteeringWheelGUI(unittest.TestCase):

    def setUp(self):
        self.root = tk.Tk()
        self.gui = SteeringWheelGUI(self.root)

    def tearDown(self):
        self.root.destroy()

    def test_init(self):
        self.assertIsInstance(self.gui.serial_handler, SerialHandler)
        self.assertIsInstance(self.gui.ui_components, UIComponents)

    def test_setup_menu(self):
        menu_bar = self.root.nametowidget(self.root.winfo_children()[0])
        self.assertIsInstance(menu_bar, tk.Menu)
        self.assertEqual(menu_bar.index('end'), 0)  # Only one menu item
        self.assertEqual(menu_bar.entrycget(0, 'label'), 'Select Port')

    @patch('os.listdir')
    def test_create_port_menu(self, mock_listdir):
        mock_listdir.return_value = ['cu.usbmodem1', 'cu.usbmodem2']
        port_menu = self.gui.create_port_menu()
        self.assertEqual(port_menu.index('end'), 1)  # Two menu items
        self.assertEqual(port_menu.entrycget(0, 'label'), 'cu.usbmodem1')
        self.assertEqual(port_menu.entrycget(1, 'label'), 'cu.usbmodem2')

    @patch.object(SerialHandler, 'set_port')
    def test_set_port(self, mock_set_port):
        self.gui.set_port('cu.usbmodem1')
        mock_set_port.assert_called_once_with('/dev/cu.usbmodem1')

    def test_update_ui_status(self):
        test_data = "BTNS:1-0-1:POTS:1500-2000:MODES:1-0-2-3"
        with patch.object(UIComponents, 'update_serial_monitor_label') as mock_update_label, \
             patch.object(UIComponents, 'update_button_states') as mock_update_buttons, \
             patch.object(UIComponents, 'update_gauge_status') as mock_update_gauge, \
             patch.object(UIComponents, 'update_mode_status') as mock_update_mode:
            
            self.gui.update_ui_status(test_data)
            
            mock_update_label.assert_called_once_with(test_data)
            mock_update_buttons.assert_called_once_with('1-0-1')
            mock_update_gauge.assert_called_once_with('1500-2000')
            mock_update_mode.assert_called_once_with('1-0-2-3')

if __name__ == '__main__':
    unittest.main()
