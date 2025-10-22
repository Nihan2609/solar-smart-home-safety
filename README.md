# Solar Smart Home Safety System  
A smart home safety & monitoring system leveraging solar power, designed to keep your home safe and energy-efficient.

## Table of Contents  
- [Project Overview](#project‐overview)  
- [Features](#features)  
- [System Architecture](#system‐architecture)  
- [Hardware & Components](#hardware‐components)  
- [Firmware & Software](#firmware‐software)  
- [Getting Started](#getting‐started)  
  - [Prerequisites](#prerequisites)  
  - [Setup](#setup)  

## Project Overview  
This project is aimed at implementing a smart home safety system powered by solar energy. The goal is to monitor key home parameters (such as intrusions, fire, gas leaks, power status) and alert the homeowner while minimizing grid electricity usage by utilizing a solar-powered setup.

## Features  
- Solar-powered energy supply for home safety electronics  
- Real-time monitoring of home environmental & security sensors  
- Alerts/notifications for safety events (e.g., intrusion, fire, gas leak)  
- Energy usage monitoring to optimize solar battery and grid switch-over  
- Modular firmware to adapt to various sensors & home layouts  
- Minimal wiring and flexible deployment for retro-fitting existing homes  

## System Architecture  
The system is composed of:  
- A solar PV panel + charge controller + battery system that powers the monitoring hardware.  
- Sensor modules (e.g., motion sensor, smoke detector, gas sensor) interfaced to a microcontroller.  
- A central smart home controller (firmware) that reads sensors, controls actuators (e.g., alarms, lights), manages energy flow, and communicates events to the homeowner (via WiFi, mobile app or web).  
- (Optional) A dashboard or mobile app for remote monitoring and configuration.

## Hardware Components  
Here are the main hardware components used in this system:  
- Solar panel (e.g., 12 V/100 W) + mounting structure  
- Solar charge controller + deep-cycle battery  
- Microcontroller board (e.g., Arduino, ESP32)  
- Sensors: motion (PIR), smoke/flammable gas (MQ-2/MQ-7), door/window magnetic sensor, power monitoring module  
- Actuators: buzzer/alarm, LED indicator, relay for switching loads  
- Communication module: WiFi (ESP), optional GSM or LoRa  
- Wiring, power distribution, enclosures  

> You may list the exact component models you used (e.g., “MQ-2 Gas Sensor Module”, “HC-SR501 PIR Motion Sensor”, etc.).

## Firmware & Software  
- Firmware directory: `firmware/smart_home/` contains microcontroller code (written in C++).  
- Components directory: `components/` contains sensor/actuator interface code modules.  
- Documentation: `Safe_Home.pdf` describes the design, circuit diagrams, and implementation details. :contentReference[oaicite:0]{index=0}  
- The microcontroller code reads sensor inputs, decides if a safety event occurred, controls actuators, logs events, and sends notifications.  
- (If applicable) A backend or cloud interface handles storing logs and presenting them via dashboard or mobile app.

## Getting Started  
### Prerequisites  
- The microcontroller development environment (e.g., Arduino IDE or PlatformIO)  
- Libraries for sensors, WiFi/communication module  
- Hardware components (see above) assembled and wired  
- A mounted solar + battery system capable of powering the electronics  

### Setup  
1. Clone the repository:  
   ```bash  
   git clone https://github.com/Nihan2609/solar-smart-home-safety.git  
