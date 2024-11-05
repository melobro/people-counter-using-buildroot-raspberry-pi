# Raspberry Pi 7-Segment Display and Motion Sensor Project 🍓💡

## Overview
This project involves interfacing a Raspberry Pi with a 4-digit 7-segment display and PIR motion sensors. The system detects motion in a room using the sensors and displays relevant information on the 7-segment display.

## Components 🛠️
- Raspberry Pi
- 4-digit 7-segment display
- 3x PIR Motion Sensors
- Nulaxy C900 Webcam
- Ultrasonic Sensor
- Breadboard
- Necessary connecting wires

## Repository Structure 📁

- Documentation
   - [Datasheets](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/documentation/Datasheets)
   - [Presentations](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/documentation/Presentations)
   - [Project-Pictures](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/documentation/Project_Pictures)
   - [2 sensors code](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/source_code/2%20sensors%20code) (2nd sensor code)
   - [Hello](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/source_code/hello) (Hello World for the RaspBerry PI)
   - [7-Segment-Display](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/source_code/7%20Segment-Display) (Functions for the Display)
   - [Sensor](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/source_code/sensor) (Functions for the PIR Sensors)
   - [Sensor](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/tree/main/source_code/D) (Proximity Sensor)

## Wiki Structure 📚

- [Planning](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Planning)
- [Concept](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Concept)
- [Design](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Design)
- [Implementation](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Implementation)
- [Test](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Test)
- [Manual](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Manual)

## Quick starting Guide 🚀

1. **Compile the Code:**
   - Make sure the IP-Adress of the RaspBerry PI is identical to the one in the Makefile. (If necessary change the IP-Adress in the Makefile)
   - Build the Code with the existing Makefile by using the make command.

2. **Run the Program:**
   - Execute the compiled program on your Raspberry Pi if there is no Shell-Script that automatically starts the programm after boot. 

3. **Observations:**
   - Observe the 7-segment display for updates when motion is detected.

### Full Manual is found [here](https://gitlab.rz.htw-berlin.de/c71_cse/2023ws_teamd/-/wikis/Manual)
