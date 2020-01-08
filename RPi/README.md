# Raspberry Pi Setup
* The code is based on the [airhockey repository](https://github.com/ross85/airhockey) by [ross85](https://github.com/ross85) and modified to accept data from STM32.

## Prequisites
* BLE version: `bluepy`, `pygame`
```
$ pip3 install bluepy
$ pip3 install pygame
```
* Wifi version: `pygame`
```
$ pip3 install pygame
```

## How to Run
In `src/` directory,
   * To run BLE version
   ```
   $ sudo python3 main.py
   ```
   * To run Wifi version
   ```
   $ sudo python3 main-socket.py
   ```
