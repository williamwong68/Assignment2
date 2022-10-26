# Assignment2
This system can detect Air Quality, Temperature and Humidity
# Sensors
It collects all the sensors data and transfer them to tier 2 which is Raspberry Pi via Bluetooth
# client
It's subscriber for MQTT and doing the downstream which compare data and send command back to Teensy control LED light on/off via Raspberry
# downstream
Received command from MQTT subscriber and Raspberry pi to control LED light on/off
# publish
Receive and show the collected data from sensoring tier via Bluetooth
