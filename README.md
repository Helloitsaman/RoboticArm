# ESP32 Bluetooth Controlled Robotic Arm

## Overview

This project implements a Bluetooth-controlled robotic arm using an ESP32 and the ESP32Servo library. The robotic arm consists of six servo motors, including a dual-servo gripper. The arm is controlled through a Bluetooth serial controller application, allowing smooth real-time movement while buttons are held.

The program uses non-blocking timing with `millis()` for responsive servo control and includes an automatic stop mechanism when no command is received.

---

## Features

* Bluetooth serial communication using ESP32 Classic Bluetooth
* Real-time servo movement while buttons are held
* Six servo motor control
* Dual-servo gripper
* Home position command
* Non-blocking control using `millis()`
* Automatic stop when Bluetooth commands are no longer received
* Servo position limiting to prevent over-travel

---

## Hardware Requirements

* ESP32 Development Board
* 6 Servo Motors (MG995/MG996R or equivalent)
* External regulated 5V power supply for servos
* Connecting wires

**Note:** Do not power multiple servos directly from the ESP32. Use a dedicated external power supply with a common ground connected to the ESP32.

---

## Servo Connections

| Servo         | GPIO Pin |
| ------------- | -------: |
| Base Rotation |       14 |
| Base Joint    |       25 |
| Shoulder      |       26 |
| Elbow         |       27 |
| Left Claw     |       32 |
| Right Claw    |       33 |

---

## Bluetooth Configuration

Bluetooth Device Name:

```
Robotic_Arm
```

Pair your phone with the ESP32 and connect using a Bluetooth serial controller application.

---

## Command Mapping

| Command | Function          |
| ------- | ----------------- |
| Q       | Rotate Base Left  |
| E       | Rotate Base Right |
| L       | Move Base Left    |
| R       | Move Base Right   |
| F       | Shoulder Up       |
| B       | Shoulder Down     |
| A       | Elbow Up          |
| C       | Elbow Down        |
| S       | Open Claw         |
| X       | Close Claw        |
| Z       | Stop Movement     |
| H       | Home Position     |

---

## Servo Limits

| Servo         |     Range |
| ------------- | --------: |
| Base Rotation | 0° – 180° |
| Base Joint    | 0° – 180° |
| Shoulder      | 0° – 180° |
| Elbow         | 0° – 180° |
| Gripper       |  0° – 90° |

---

## Home Position

When the `H` command is received, the robotic arm returns to the following position:

| Servo         | Position |
| ------------- | -------: |
| Base Rotation |      90° |
| Base Joint    |      90° |
| Shoulder      |      90° |
| Elbow         |      90° |
| Left Claw     |      45° |
| Right Claw    |     135° |

---

## Program Behavior

* The controller continuously sends commands while a button is held.
* Servo positions are updated every 20 ms.
* If no command is received for 250 ms, movement automatically stops.
* Servo positions remain at their last location until another movement command is received.
* The program uses `millis()` instead of `delay()` to ensure smooth and responsive operation.

---

## Required Libraries

Install the following libraries using the Arduino IDE Library Manager:

* ESP32Servo
* BluetoothSerial

---

## Project Structure

```text
Bluetooth_Robotic_Arm/
│
├── Bluetooth_Robotic_Arm.ino
├── README.md
├── images/
│   ├── robotic_arm.jpg
│   └── wiring_diagram.png
└── videos/
    └── demo.mp4
```

---

## Compilation

1. Install the required libraries.
2. Select the correct ESP32 board in the Arduino IDE.
3. Connect the ESP32.
4. Upload the sketch.
5. Pair the ESP32 Bluetooth device named `Robotic_Arm`.
6. Open the Bluetooth controller application and begin controlling the robotic arm.

---

## License

This project is provided for educational and personal use. Modify and distribute it as required.
