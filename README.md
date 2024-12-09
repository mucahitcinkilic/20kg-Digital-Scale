# Digital Weighing Scale with STM32F401RE and HX711

This project implements a digital weighing scale system using the STM32F401RE microcontroller and the HX711 ADC module. It is designed to measure weights up to 20kg accurately and efficiently using a load cell sensor.

## Features

- **Weight Measurement**: Capable of measuring up to 20kg with high accuracy.
- **STM32F401RE**: Utilizes the STM32F401RE microcontroller for fast and precise data processing.
- **HX711 ADC Module**: Reads analog data from the load cell and converts it to digital.
- **Calibration**: Includes routines to calibrate the system for accurate measurements.
- **Filtering**: Optional filtering techniques to reduce noise and improve measurement stability.

## Hardware Requirements

- **STM32F401RE Microcontroller** (or compatible STM32F4 series board)
- **HX711 ADC Module** (for interfacing with the load cell)
- **Load Cell Sensor** (for measuring weight)
- **Jumper wires** for connections
- **Power supply** for STM32 and HX711

## Software Requirements

- **STM32CubeMX** for peripheral configuration (GPIO, ADC, etc.)
- **STM32 HAL Library** for development
- **Keil uVision or STM32CubeIDE** for programming and debugging
- **Serial Monitor** for displaying the measured weight (optional)

## Circuit Diagram

Below is a simple wiring diagram for connecting the HX711 to the STM32F401RE:

- **HX711 VCC** → 3.3V on STM32
- **HX711 GND** → GND on STM32
- **HX711 DT** → GPIO pin (e.g., PA0)
- **HX711 SCK** → GPIO pin (e.g., PA1)

Ensure to use the proper connections and pins based on your hardware setup.

## Installation and Setup

1. **Clone the repository**:

   ```bash
   git clone https://github.com/yourusername/STM32-HX711-Weight-Scale.git

Youtube: https://www.youtube.com/shorts/bFfYANUlhHY