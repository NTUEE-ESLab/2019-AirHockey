# STM32L475 - Bluetooth version
## Prerequisites
* Download Mbed Studio: https://os.mbed.com/studio/
* Clone ble-HearRate example: https://os.mbed.com/teams/mbed-os-examples/code/mbed-os-example-ble-HeartRate/
* Add sensor library to project (link: http://developer.mbed.org/teams/ST/code/BSP_B-L475E-IOT01/#bfe8272ced90)

## How to Install
* Copy(replace) the files in `mbed-os` directory into `mbed-os-example-ble-HeartRate/mbed-os/features/FEATURE_BLE/`
* Move `main.cpp` and `MyService.h` into project
* Compile and write into two boards (remember to give two boards different PLAYER values (0 or 1)!)
* Execute `main.py` in `2019-AirHockey/RPi/`  
```
$ sudo python3 main.py
```
* Enjoy the game!

