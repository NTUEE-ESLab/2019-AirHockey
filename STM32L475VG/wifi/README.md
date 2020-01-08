# STM32L475 - Wifi version
## Prerequisites
* Download Mbed Studio: https://os.mbed.com/studio/
* Add example program: `File -> New Program -> mbed-os-example-wifi -> Done`
* Add sensor library to project (link: http://developer.mbed.org/teams/ST/code/BSP_B-L475E-IOT01/#bfe8272ced90)

## How to Install
* Move `main.cpp` and into project
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
* Change `IP_ADDR` and `PORT_NUM` in `main.cpp:26-27` to those of RPi's
* Compile and write into two boards (remember to give two boards different PLAYER values (0 or 1)!)
* Execute `main-socket.py` in `2019-AirHockey/RPi/`  
```
$ sudo python3 main-socket.py
```
* Enjoy the game!

