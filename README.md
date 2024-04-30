# Air Quality Monitoring Data Logger
This project aims to develop an air quality monitoring data logger using an SCD41 sensor to measure air quality, temperature, and humidity at regular intervals. The data is stored locally on the device and can be accessed via PC. The system is designed to be power-efficient with sleep modes and includes an OLED display for real-time measurement display, a buzzer for alerting abnormal levels, and an SD card module for data saving.
![WhatsApp Image 2024-04-18 at 12 08 58_578e7148](https://github.com/azmanbakhtiar/Data-logger-SCD41-STM32-OLED-project/assets/145319590/4782e6c4-7760-4d38-b61b-12bb739fb456)
![Web_Photo_Editor](https://github.com/azmanbakhtiar/Data-logger-SCD41-STM32-OLED-project/assets/145319590/f8c3d4ae-bd44-4284-840a-e1db101cc810)

## Specifications

1. **Sensor**: SCD41 sensor for air quality, temperature, and humidity measurement every 15 minutes.
2. **Development Environment**: Code written in offline mbed studio.
3. **Microcontroller**: STM32F446RE board.
4. **Display**: OLED for displaying the latest readings/measurements.
5. **Timeline**: 2-3 weeks starting from April 15th, 2024.
6. **Power Efficiency**: Utilizes sleep modes for power efficiency.
7. **PCB Design**: PCB design is not included in this project.
8. **Battery Charging**: Utilizes TP4056 module for battery charging and protection.
9. **Data Storage**: The device stores data locally on an SD card, accessible via PC in a txt or CSV file format.
10. **Alert System**: Includes a buzzer for alerting abnormal levels of temperature, humidity, or air quality.

## Components

1. STM32F446RE
2. SCD41
3. OLED display
4. TP4056 module with protection
5. Small LiPo or Li-ion battery
6. SD card module
7. Jumper wires
8. Breadboard
9. Tools such as a soldering iron, digital multimeter

*Note: Additional components may be required as the project progresses.*

## Setup and Usage

1. Connect all components according to the circuit diagram.
2. Install necessary software tools such as offline mbed studio.
3. Upload the provided code to the STM32F446RE board.
4. Insert an SD card into the SD card module.
5. Power on the device and monitor the OLED display for real-time measurements.
6. Connect the device to a PC via USB to access stored data in txt or CSV format from the SD card.
7. Monitor the device for abnormal levels of temperature, humidity, or air quality indicated by the buzzer alert.

## Future Improvements

1. Implement PCB design for a more compact and professional-looking device.
2. Explore additional features such as wireless data transmission.
3. Enhance user interface for easier data access and visualization.
4. Optimize power consumption further to prolong battery life.
5. Incorporate additional sensors for more comprehensive environmental monitoring.

## Contributors
Asfandyar Ali


## License

This project is licensed under the [License Name] License - see the [LICENSE.md](LICENSE.md) file for details.

---

Feel free to modify this README to suit your project's specific needs and add any additional details or instructions as necessary.
