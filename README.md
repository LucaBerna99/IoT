# IoT Project

The project outlines the development of a home automation system utilizing Internet of Things (IoT) technology. 

Here’s a brief overview of its key components and functionalities:

## Project Overview
### Objective
To create a domotic system that autonomously manages IoT smart devices, allowing user interaction via a Telegram Bot. 
The system controls roller shutters and lights based on environmental measurements such as temperature, humidity, and brightness.

## Hardware Components
### Actuators: 
Utilizes Shelly 2.5 devices to control roller shutters and lights, enabling communication through MQTT and COAP protocols.
### Sensors: 
Employs an Arduino Nano IoT 33 board with various sensors, including:
- DHT11 for humidity and temperature
- Analog photocell for brightness detection
- Passive Infrared (PIR) sensor for human presence detection.
### Control Unit: 
A Raspberry Pi serves as the MQTT broker and runs Node-RED, which manages the control logic and user interactions.

## Software Functionality
### Node-RED: 
Acts as the central control unit, processing sensor data and sending commands to actuators based on predefined conditions to optimize energy consumption and automate tasks.
### Telegram Bot: 
Provides remote control capabilities, allowing users to interact with the system, check sensor statuses, and issue commands.
### Automation Logic
The system automatically adjusts roller shutters and lights to maximize natural light and minimize energy waste. It also integrates with smart air conditioners to optimize cooling based on temperature and humidity readings.
### Alexa Compatibility: The system can be controlled via Amazon Alexa, allowing voice commands for device management.


This project exemplifies the integration of various IoT technologies to create a cohesive smart home environment that enhances user convenience and energy efficiency.
