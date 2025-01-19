import unittest
from unittest.mock import Mock, patch
import serial

# Import SerialHandler from the correct path
from serial_handler import SerialHandler


class TestSerialHandler(unittest.TestCase):
    def setUp(self):
        self.mock_callback = Mock()
        self.serial_handler = SerialHandler(self.mock_callback)

    @patch("serial.Serial")  # Changed from 'serial.Serial'
    def test_setup_serial_success(self, mock_serial):
        self.serial_handler.port = "/dev/ttyUSB0"
        self.serial_handler.setup_serial()
        mock_serial.assert_called_once_with("/dev/ttyUSB0", 115200)

    @patch(
        "serial.Serial",
        side_effect=serial.SerialException("Test error"),
    ) 
    @patch("tkinter.messagebox.showerror")
    def test_setup_serial_failure(self, mock_showerror, mock_serial):
        self.serial_handler.port = "/dev/ttyUSB0"
        self.serial_handler.setup_serial()
        mock_showerror.assert_called_once_with("Serial Error", "Error: Test error")

    @patch("serial.Serial")  # Changed
    def test_read_serial_success(self, mock_serial):
        mock_serial_instance = mock_serial.return_value
        mock_serial_instance.readline.return_value = b"Test data\n"

        self.serial_handler.port = "/dev/ttyUSB0"
        self.serial_handler.setup_serial()

        self.mock_callback.assert_called_once_with("Test data")

    @patch("serial.Serial")  # Changed
    @patch("tkinter.messagebox.showerror")
    def test_read_serial_failure(self, mock_showerror, mock_serial):
        mock_serial_instance = mock_serial.return_value
        mock_serial_instance.readline.side_effect = serial.SerialException("Read error")

        self.serial_handler.port = "/dev/ttyUSB0"
        self.serial_handler.setup_serial()
        self.serial_handler.read_serial()

        mock_showerror.assert_called_once_with("Serial Error", "Error: Read error")
        self.assertIsNone(self.serial_handler.serial_connection)

    def test_set_port(self):
        with patch.object(SerialHandler, "setup_serial") as mock_setup_serial:
            self.serial_handler.set_port("/dev/ttyUSB1")
            self.assertEqual(self.serial_handler.port, "/dev/ttyUSB1")
            mock_setup_serial.assert_called_once()


if __name__ == "__main__":
    unittest.main()
