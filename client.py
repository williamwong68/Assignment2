import paho.mqtt.client as mqtt
import serial
import string

HIGH = 100
MEDIUM = 28
LOW = 3


# AWS EC2 IP
EC2 = "3.27.48.19"
#Bluetooth connect port number
COM = "/dev/rfcomm1"

# Connect to MQTT
def on_connect(client, userdata, flags, rc):
	print("Connected to MQTT")
	print("Connected with result code " + str(rc))
	# Create Topics
	client.subscribe("Air/Humi")
	client.subscribe("Air/Temp")
	client.subscribe("Air/TVOC")
	client.subscribe("Air/ECO2")
	client.subscribe("Air/H2")
	client.subscribe("Air/Ethanol")
	client.subscribe("Air/PM1.0")
	client.subscribe("Air/PM2.5")
	client.subscribe("Air/PM10")

# Sending message
def on_message(client, userdata, msg):
	print(msg.topic + " " + str(msg.payload))
	
	if msg.topic == "Air/TVOC":
		if float(msg.payload) >= HIGH:
			ser = serial.Serial(COM, 9600)
			ser.write(str.encode("GRNLED_ON\n"))
			print("GRNLED_ON")
		else:
			ser = serial.Serial(COM, 9600)
			ser.write(str.encode("GRNLED_OFF\n"))
			print("GRNLED_OFF")
	if msg.topic == "Air/Temp":
		if float(msg.payload) >= MEDIUM:
			ser = serial.Serial(COM, 9600)
			ser.write(str.encode("REDLED_ON\n"))
			print("REDLED_ON")
		else:
			ser = serial.Serial(COM, 9600)
			ser.write(str.encode("REDLED_OFF\n"))
			print("REDLED_OFF")
	if msg.topic == "Air/PM2.5":
		if str(msg.payload) >= str(LOW):
			ser = serial.Serial(COM, 9600)
			ser.write(str.encode("YELLED_ON\n"))
			print("YELLED_ON")
		else:
			ser = serial.Serial(COM, 9600)
			ser.write(str.encode("YELLED_OFF\n"))
			print("YELLED_OFF")

        
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message



client.connect(EC2, 1883, 60)
client.loop_forever()

