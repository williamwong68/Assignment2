import serial
import string
import paho.mqtt.publish as publish

# AWS EC2 ip
EC2 = "3.27.48.19"
COM = "/dev/rfcomm0"


# Connect to bluetooth
ser = serial.Serial(COM, 9600)
ser.write(str.encode('start\r\n')) # Send write to Arduino

while True:
	if ser.in_waiting > 0:
		rawserial = ser.readline()
		cookedserial = rawserial.decode('utf-8').strip('\r\n')
		humi = cookedserial[11:16]
		temp = cookedserial[32:36]
		TVOC = cookedserial[80:83]
		eCO2 = cookedserial[100:104]
		H2 = cookedserial[111:116]
		Ethanol = cookedserial[122:127]
		PM1 = cookedserial[142:144]
		PM25 = cookedserial[160:162]
		PM10 = cookedserial[165:167]

		print("Humi: " + humi)
		print("Temp: " + temp)
		print("TVOC: " + TVOC)
		#print("eCO2: " + eCO2)
		#print("H2: " + H2)
		#print("Ethanol: " + Ethanol)
		#print("PM1.0: " + PM1)
		print("PM2.5: " + PM25)
		#print("PM10: " + PM10)

		
		publish.single("Air/Humi", humi, hostname = EC2)
		publish.single("Air/Temp", temp, hostname = EC2)
		publish.single("Air/TVOC", TVOC, hostname = EC2)
		#publish.single("Air/ECO2", eCO2, hostname = EC2)
		#publish.single("Air/H2", H2, hostname = EC2)
		#publish.single("Air/Ethanol", Ethanol, hostname = EC2)
		#publish.single("Air/PM1.0", PM1, hostname = EC2)
		publish.single("Air/PM2.5", PM25, hostname = EC2)
		#publish.single("Air/PM10", PM10, hostname = EC2)
