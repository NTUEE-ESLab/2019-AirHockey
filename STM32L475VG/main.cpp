/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/gap/Gap.h"
#include "ble/services/HeartRateService.h"
#include "MyService.h"
#include "pretty_printer.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"

#define PLAYER_NUM 0

const static char DEVICE_NAME[] = "Hockey";

Serial pc(USBTX, USBRX);
Semaphore sem(1);

Thread t_sensor;
Thread t_ble;

static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

class Sensors {
#define SCALE_MULTIPLIER    0.045
#define TIMESTEP            0.0005
public:
    Sensors(events::EventQueue &event_queue) : _event_queue(event_queue) {
        BSP_ACCELERO_Init();    
        BSP_GYRO_Init();
        align();
        _event_queue.call_every(1, this, &Sensors::update);
    }

    void calculate(float * pGyroDataXYZ, int16_t * pAccDataXYZ) {
        for (int i = 0; i < 3; ++i) {
            if (abs(pGyroDataXYZ[i]) > 15)
                _angle[i] += (pGyroDataXYZ[i] + _GyroAccumulate[i]) / 2 * TIMESTEP * SCALE_MULTIPLIER;
            if (abs(pAccDataXYZ[i]) > 5 && pAccDataXYZ[i] * _velocity[i] >= 0) {
                _velocity[i] += (pAccDataXYZ[i] + _AccAccumulate[i]) / 2 * TIMESTEP;
                _position[i] += (pAccDataXYZ[i] + _AccAccumulate[i]) / 2 * TIMESTEP * TIMESTEP / 2;
            } else _velocity[i] = 0;

            // if (abs(pAccDataXYZ[i] - AccAccumulate[i]) < 2)
            //     velocity[i] = 0;
            
            _position[i] += _velocity[i] * TIMESTEP;
        }

        for (int i = 0; i < 3; ++i) {
            _GyroAccumulate[i] = pGyroDataXYZ[i];
            _AccAccumulate[i] = pAccDataXYZ[i];
        }

    }

    void align() {
        pc.printf("Aligning...\n");

        while (_sample_num < 2000) {
            _sample_num++;
            BSP_GYRO_GetXYZ(_pGyroDataXYZ);
            BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
            for (int i = 0; i < 3; ++i) {
                _GyroOffset[i] += _pGyroDataXYZ[i];
                _AccOffset[i] += _pAccDataXYZ[i];
            }

            wait(TIMESTEP);
        }

        for (int i = 0; i < 3; ++i)
            pc.printf("%d ", _AccOffset[i]);
        pc.printf("\n");

        for (int i = 0; i < 3; ++i) {
            _GyroOffset[i] /= _sample_num;
            _AccOffset[i] /= _sample_num;
        }

        for (int i = 0; i < 3; ++i)
            pc.printf("%d ", _AccOffset[i]);
        pc.printf("\n");

        pc.printf("Done alignment\n");
        _sample_num = 0;
    }

    void update() {
        // pc.printf("Waiting at Sensors::update...\n");
        // sem.wait();
        // pc.printf("Enter Sensors::update\n");
        // pc.printf("Start updating...\n");
        // socket.set_blocking(1);


        // while (1){
        ++_sample_num;

        BSP_GYRO_GetXYZ(_pGyroDataXYZ);
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        for (int i = 0; i < 3; ++i) {
            _pGyroDataXYZ[i] = (_pGyroDataXYZ[i] - _GyroOffset[i]) * SCALE_MULTIPLIER;
            _pAccDataXYZ[i] = _pAccDataXYZ[i] - _AccOffset[i];
        }

        // if (0) {
        if (_sample_num % 100 == 0) {
            // pc.printf("\nGYRO_X = %.2f\n", pGyroDataXYZ[0]);
            // pc.printf("GYRO_Y = %.2f\n", pGyroDataXYZ[1]);
            // pc.printf("GYRO_Z = %.2f\n", pGyroDataXYZ[2]);

            // pc.printf("\nANGLE_X = %f\n", angle[0]);
            // pc.printf("ANGLE_Y = %f\n", angle[1]);
            pc.printf("ANGLE_Z = %f\n", _angle[2]);

            // pc.printf("\nACCELERO_X = %d\n", pAccDataXYZ[0]);
            // pc.printf("ACCELERO_Y = %d\n", pAccDataXYZ[1]);
            // pc.printf("ACCELERO_Z = %d\n", pAccDataXYZ[2]);

            // pc.printf("\nVELOCITY_X = %f\n", velocity[0]);
            // pc.printf("VELOCITY_Y = %f\n", velocity[1]);
            // pc.printf("VELOCITY_Z = %f\n", velocity[2]);

            // pc.printf("\nPOSITION_X = %f\n", position[0]);
            // pc.printf("POSITION_Y = %f\n", position[1]);
            // pc.printf("POSITION_Z = %f\n", position[2]);
        }

        if (_sample_num % 100 == 0) {
            if (abs(_velocity[0]) > 1) {
                if (_velocity[0] > 0)
                    pc.printf("%5s ", "back");
                else pc.printf("%5s ", "front");
            } else pc.printf("still ");

            if (abs(_velocity[1]) > 1) {
                if (_velocity[1] > 0)
                    pc.printf("%5s\n", "right");
                else pc.printf("%5s\n", "left");
            } else pc.printf("still\n");

            pc.printf("\n");
        }
        
        wait(TIMESTEP);
    
        calculate(_pGyroDataXYZ, _pAccDataXYZ);

        // }
        // sem.release();
        // pc.printf("Release sem at Sensors::udpate\n");
    }

    void getDirection(uint8_t& right, uint8_t& up) {
        // if (sample_num % 100 == 0) {
        // pc.printf("In Sensors::getDirection...\n");

        // pc.printf("%f\n", abs(_velocity[0]));
        // if  (abs(_velocity[0]) > 1.0)
        //     pc.printf("%f\n", _velocity[0]);
        // else pc.printf("ha1\n");

        // if  (abs(_velocity[0]) > 1)
        //     pc.printf("%f\n", _velocity[0]);
        // else pc.printf("ha2\n");
        

        if (abs(_velocity[0]) > 1) {
            if (_velocity[0] < 0) {
                pc.printf("%5s ", "back");
                up = 2;
            }
            else {
                pc.printf("%5s ", "front");
                up = 1;
            }
        } else {
            pc.printf("still ");
            up = 0;
        }

        if (abs(_velocity[1]) > 1) {
            if (_velocity[1] > 0) {
                pc.printf("%5s\n", "right");
                right = 1;
            }
            else {
                pc.printf("%5s\n", "left");
                right = 2;
            }
        } else {
            pc.printf("still\n");
            right = 0;
        }

        pc.printf("\n");
        
        // }
        up = (uint8_t)_velocity[0];
        right = (uint8_t)_velocity[1];

        pc.printf("%d %d\n", up, right);

    }

    // returns angle / 2 due to 8 bit
    void getAngle(uint8_t& angle) {
        angle = int(_angle[2]) / 2;
    }

private:
    float _angle[3] = {};
    float _velocity[3] = {};
    float _position[3] = {};

    float _GyroAccumulate[3] = {};
    float _AccAccumulate[3] = {};

    int   _sample_num = 0;
    int16_t _pAccDataXYZ[3] = {0};
    float _pGyroDataXYZ[3] = {0};

    int   _AccOffset[3] = {};
    float _GyroOffset[3] = {};

    events::EventQueue &_event_queue;
};


















class MyDemo : ble::Gap::EventHandler {
public:
    MyDemo(BLE &ble, events::EventQueue &event_queue, uint8_t player, Sensors * sensor) :
        _ble(ble),
        _event_queue(event_queue),
        _led1(LED1, 1),
        _connected(false),
        _uuid(GattService::UUID_MY_SERVICE),
        _service(ble, player),
        _sensor(sensor),
        _adv_data_builder(_adv_buffer) { }

    void start() {
        _ble.gap().setEventHandler(this);

        _ble.init(this, &MyDemo::on_init_complete);

        _event_queue.call_every(500, this, &MyDemo::blink);
        pc.printf("_event_queue setup blink\n");
        _event_queue.call_every(1, this, &MyDemo::send_sensor_value);
        pc.printf("_event_queue setup update_sensor_value\n");

        // _event_queue.dispatch_forever();
        // pc.printf("_event_queue dispatch forever\n");
    }

private:
    /** Callback triggered when the ble initialization process has finished */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params) {
        if (params->error != BLE_ERROR_NONE) {
            pc.printf("Ble initialization failed.");
            return;
        }

        print_mac_address();

        start_advertising();
        pc.printf("Advertising...\n");
    }

    void start_advertising() {
        /* Create advertising parameters and payload */

        ble::AdvertisingParameters adv_parameters(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(1000))
        );

        _adv_data_builder.setFlags();
        _adv_data_builder.setAppearance(ble::adv_data_appearance_t::HUMAN_INTERFACE_DEVICE_HID);
        _adv_data_builder.setLocalServiceList(mbed::make_Span(&_uuid, 1));
        _adv_data_builder.setName(DEVICE_NAME);

        /* Setup advertising */

        ble_error_t error = _ble.gap().setAdvertisingParameters(
            ble::LEGACY_ADVERTISING_HANDLE,
            adv_parameters
        );

        if (error) {
            pc.printf("_ble.gap().setAdvertisingParameters() failed\r\n");
            return;
        }

        error = _ble.gap().setAdvertisingPayload(
            ble::LEGACY_ADVERTISING_HANDLE,
            _adv_data_builder.getAdvertisingData()
        );

        if (error) {
            pc.printf("_ble.gap().setAdvertisingPayload() failed\r\n");
            return;
        }

        /* Start advertising */

        error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

        if (error) {
            pc.printf("_ble.gap().startAdvertising() failed\r\n");
            return;
        }
    }

    void send_sensor_value() {
        // pc.printf("%d\n", _connected);
        if (_connected) {
            // pc.printf("haha\n");
            // Do blocking calls or whatever is necessary for sensor polling.
            // In our case, we simply update the HRM measurement.
            // _hr_counter++;

            // //  100 <= HRM bps <=175
            // if (_hr_counter == 175) {
            //     _hr_counter = 100;
            // }

            uint8_t right = 0, up = 0, angle = 0;

            // pc.printf("Waiting at MyDemo::send_sensor_value...\n");
            // sem.wait();
            // pc.printf("Enter MyDemo::send_sensor_value\n");
            
            // pc.printf("Getting dierction info...\n");
            _sensor->getDirection(right, up);
            // pc.printf("_sensor->getDirection(right, up)");
            // pc.printf("Getting angle info...\n");
            _sensor->getAngle(angle);
            // pc.printf("_sensor->getAngle(angle)");
            
            // sem.release();
            // pc.printf("Release sem at MyDemo::send_sensor_value\n");

            // pc.printf("Updating info...\n");
            // if (0 != right || 0 != up
            if (1) {
                _service.updateInfo(right, up, angle);
            }
            // pc.printf("%d, %d, %d\n", right, up, angle);
        }
    }

    void blink(void) {
        _led1 = !_led1;
    }

private:
    /* Event handler */

    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&) {
        _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
        _connected = false;
    }

    virtual void onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
        if (event.getStatus() == BLE_ERROR_NONE) {
            pc.printf("Successfully connected\n");
            _connected = true;
        } else pc.printf("Failed connecting\n");
    }

private:
    BLE &_ble;
    events::EventQueue &_event_queue;
    DigitalOut _led1;

    Sensors * _sensor;
    bool _connected;

    UUID _uuid;

    MyService _service;

    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;

    uint8_t _right;
    uint8_t _up;
    uint8_t _angle;
};

/** Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

int main()
{
    pc.baud(115200);
    pc.printf("Program start\n");

    Sensors sensor(event_queue);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events);

    MyDemo demo(ble, event_queue, PLAYER_NUM, &sensor);
    demo.start();
    pc.printf("Demo started!\n");

    pc.printf("Start dispatching...\n");

    // t_sensor.start(callback(&Sensors::update));
    event_queue.dispatch_forever();

    return 0;
}

