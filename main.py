import serial
import serial.tools.list_ports
import pyautogui
from decimal import Decimal
from re import fullmatch
from time import sleep

ports = list(serial.tools.list_ports.comports())
port = None
for port in ports:
    if port.description[: port.description.rfind(" ")].strip() == "USB-SERIAL CH340":
        port = port.device
        break

if port is None:
    print("Устройство не подключено!")
    raise SystemExit(0)


def to_coordinate(resp):
    x = Decimal(WIDTH) / 2 + Decimal(WIDTH) / 180 * Decimal(resp)
    return int(x), 400


WIDTH, HEIGHT = pyautogui.size()

ser = serial.Serial(port, 115200)
print("start")
while True:
    response = ser.readline()
    decoded_response = response.decode("utf-8").strip()
    if fullmatch(r"-?\d+\.\d+", decoded_response):
        print(repr(decoded_response))
        x, y = to_coordinate(decoded_response)
        # print(x)
        pyautogui.moveTo(x, y)
    sleep(0.1)
    ser.reset_input_buffer()
