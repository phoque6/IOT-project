# IOT-project
A smart door system for the deaf and hard of hearing which in extension can also be used for the public.

# Steps to setup the project
1. Make the connections between various components as shown in the circuit diagrams in the circuit-diagram folder.

2. Click on the following link to setup the camera with the arduino https://create.arduino.cc/projecthub/techmirtz/visual-capturing-with-ov7670-on-arduino-069ebb. (The 'code' folder in our source-code folder can be used instead of what you get after going through the instructions from the above link)

3. Upload the arduino program SimpleRead.ino present in the SimpleRead folder to the arduino connected to the camera.

4. Upload the arduino program ultrasonic-serial.ino present in the ultrasonic-serial.ino folder to the other arduino.

5. Once the arduino program ultrasonic-serial.ino is running in the arduino, connect the arduino to the laptop and run the python program bolt-arduino-RxTx.py which clicks and sends the image of the visitor.

6. The owner must join the telegram channel 'Smart Door Alerts' (https://t.me/alerts_boltIOT19) to receive the alert/notification about the visitor.

7. The owner must also have a bolt-iot account to send the message to the door and control the door.
