# PFI UADE
## Design and Simulation of a Wireless Sensor Network based on long-range technology.


***
## About
This code is part of the final engineering project to obtain the degree of telecommunications engineer at UADE.
The authors of the work and the following code are:

 - Damian del Campo
 - Jonathan Salini

***
## Table of Contents
1. [Objective](#Objective)
2. [Arduino Node algorithm](#Arduino-Node-algorithm) 
3. [Simple Server Dashboard](#Simple-Server-Dashboard)
4. [Installation](#Installation)

<a name="Objective"></a>
## Objective
Create a simple lora mesh network based on the open source RadioHead library, with arduino uno and esp32 modules equipped with LoRa tranceivers. Publishes the information collected by the nodes on a web dashboard via MQTT events.

<a name="Arduino-Node-algorithm"></a>
## Arduino Node algorithm
This program should set up a LoRa mesh network. Depending on its node ID, it will wait for incoming LoRa messages and reply an ACK message to the sender.
If the node ID is #1, it also relays the message via MQTT events to a public cloud broker. All other nodes will send hello messages to the other nodes on the network waiting for the ACK.
If any node is out of coverage, the entire network is in charge of rerouting the routes so that the messages continue to reach their destination.

It was designed to work with two modules:
 - Arduino UNO + Lora Shield transceiver
 - Heltec ESP32 LoRa Wifi v2

<p align="center">
     <img src="https://github.com/chewydc/PFI_UADE/blob/556ff3eec2be77469075e509a350a82d2f04a344/img/Layout.JPG">
</p>

<a name="Simple-Server-Dashboard"></a>
## Simple Server Dashboard
Simple Server for a Dashboard. It's shows MQTT incoming messages.

<p align="center">
     <img src="https://github.com/chewydc/PFI_UADE/blob/556ff3eec2be77469075e509a350a82d2f04a344/img/Dashboard.JPG">
</p>

<a name="Installation"></a>
## Installation
 
```
$ git clone https://github.com/chewydc/PFI_UADE.git
$ cd Dashboard
$ npm install
$ npm start
```
