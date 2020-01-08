# 2019-AirHockey
## Motivation
### Life is tiring and frustrating (especially for EE students). We need some entertainments!
### A project that releases our pressure
### Meets the requirements
* Implement wifi socket programming
* Use accelerometer and gyroscope in STM32
* Assisted with RPi
### Interesting and competitive game!

## Method
### STM32
#### Sensor (Accelerometer)
Since acceleration is biased by approximately a constant value, we calibrate at the beginning each time to get the offset,  subtracting the offset to get more precise values. STM32 should send the direction to RPi each time step, we had tried sending acceleration, velocity, and displacement, and found that sending velocity is the best. We calculate the velocity by integrate the acceleration, which is sampled every 1 - 2 ms. We also add a function such that if the user pick up the STM32 (Z-axis changing), STM32 will stop.
#### 


### RPi


## Result


## Reference
### ARM-MBED documents (sensors)
https://os.mbed.com/teams/ST/code/DISCO_L475VG_IOT01-Sensors-BSP/file/986c1f5db128/main.cpp/
### airhockey
https://github.com/ross85/airhockey
### UDP Socket
https://os.mbed.com/docs/mbed-os/v5.9/reference/udpsocket.html  
https://www.itread01.com/p/432886.html
