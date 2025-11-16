## About
Logical function dokumentation of my md_Bluetti_ESP32_Bridge
## Overwiew
I use a Raspberry Pi 5 as Host for Home Assistant to control a tinyhouse without grid connection. The RasPi does not connect well with my Bluetti AC300 via Bluetooth.
I decided to use ESP32 device as interface and use MQTT for Communication with HA. Bluetooth communication works very well between ESP and Bluetti.

I forked the repository of MarioLukas and worked a lot to understand how it works. This forced me to explane a little bit.
