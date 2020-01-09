# 2019-AirHockey
* An air-hockey game implemented on STM32L475 and Raspberry Pi.
* Two versions are available: **BLE** & **Wifi**
* The **Wifi** version has better performance

## Setup & Installation

### Raspberry Pi
* Prerequisites: `bluepy` (for ble) and `pygame` (ble and wifi)
```sh
$ pip3 install bluepy
$ pip3 install pygame
```

### STM32L475
#### Prerequisites
* Download Mbed Studio: https://os.mbed.com/studio/
* Add sensor library to project (link: http://developer.mbed.org/teams/ST/code/BSP_B-L475E-IOT01/#bfe8272ced90)

#### BLE version
* Clone ble-HearRate example: https://os.mbed.com/teams/mbed-os-examples/code/mbed-os-example-ble-HeartRate/
* Copy(replace) the files in `mbed-os` directory into `mbed-os-example-ble-HeartRate/mbed-os/features/FEATURE_BLE/`
* Move `main.cpp` and `MyService.h` into project (same directory as `mbed-os/`)
* Compile and write into two boards. Remember to assign two boards different `PLAYER` values (0 or 1)! (in `main.cpp:27`)

* See [`STM32L475VG/ble/README.md`](./STM32L475VG/ble/README.md) for more detailed information

### Wifi version
* Move `main.cpp` into project (same directory as `mbed-os/`)
* Substitute ssid and password in code below and copy it to `mbed_app.json` under project
```
{
    "config": {
        "wifi-ssid": {
            "help": "WiFi SSID",
            "value": "\"WIFI-SSID\""
        },
        "wifi-password": {
            "help": "WiFi Password",
            "value": "\"WIFI-PASSWORD\""
        }
    },
    "target_overrides": {
        "*": {
            "platform.stdio-convert-newlines": true
        }
    }
}
```
* Change `IP_ADDR` in `main.cpp:26-27` to the IP address of RPi
* Compile and write into two boards. Remember to give two boards different `PLAYER` values (0 or 1)! (in `main.cpp:30`)

* See [`STM32L475VG/wifi/README.md`](./STM32L475VG/wifi/README.md)  

## How to Run
### Raspberry Pi
#### BLE version
```sh
cd RPi/src/
sudo python3 main.py
```

#### Wifi version
```sh
cd RPi/src/
sudo python3 main-socket.py
```

### STM32L475
Nothing has to be done! If the striker does not react or behaves strangely, press the blue button to reconnect and recalibrate.  

After the commands are executed, you may enter the starting menu. Enter `1`, `3`, or `5` to start the game (for 1, 3, or 5 minutes respectively).  
Press `Esc` to exit the game.  

## Screenshots and Demo
* Starting menu
* In game
* One player wins
* Demo video: https://drive.google.com/file/d/1DMVhMzvl215tsdMatFl2ZeyUQp4JkgY9/view?usp=sharing

## References
* ARM-MBED documents (sensors):  
https://os.mbed.com/teams/ST/code/DISCO_L475VG_IOT01-Sensors-BSP/file/986c1f5db128/main.cpp/
* airhockey: https://github.com/ross85/airhockey
* UDP Socket
https://os.mbed.com/docs/mbed-os/v5.9/reference/udpsocket.html
https://www.itread01.com/p/432886.html

For more information, see [report](./report.md) for detailed implementation.
