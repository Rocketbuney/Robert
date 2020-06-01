# Robert
### Robert is a simple Raspberry Pi based robot. Using the wiringPi framework and utilizing the following hardware:
- Raspberry Pi Zero W
- JuiceBox Zero Battery Management Board
- 3.7v 1600mAh Lithium Polymer Battery
- Adafruit TB6612 Motor Driver Board
- HC-SR04 UltraSonic Distance Sensor (x3)
- 5v to 3.3v Logic Level Converter Board

### Pn Map
| Pin Number | Function |   | Function | Pin Number |
|:----------:|:--------:|:-:|:--------:|:----------:|
|    3.3v    |   Power  |   |   Power  |     5v     |
|      8     |   AIN2   |   |   Power  |     5v     |
|      9     |   AIN1   |   |  Ground  |     GND    |
|      7     |   STBY   |   |   BIN1   |     15     |
|     GND    |  Ground  |   |  Unused  |     16     |
|      0     |   Trig1  |   |   PWMA   |      1     |
|      2     |   Trig3  |   |  Ground  |     GND    |
|      3     |   Trig2  |   |   Echo1  |      4     |
|    3.3v    |   Power  |   |   Echo3  |      5     |
|     12     |   BIN2   |   |  Ground  |     GND    |
|     13     |  Unused  |   |   Echo2  |      6     |
|     14     |  Unused  |   |  Unused  |     10     |
|     GND    |  Ground  |   |  Unused  |     11     |
|     30     |  Unused  |   |  Unused  |     31     |
|     21     |  Unused  |   |  Ground  |     GND    |
|     22     |  Unused  |   |  Unused  |     26     |
|     23     |  Unused  |   |  Ground  |     GND    |
|     24     |   PWMB   |   |  Unused  |     27     |
|     24     |  Unused  |   |  Unused  |     28     |
|     GND    |  Ground  |   |  Unused  |     29     |

## To Do:
 - [x] Move Project to modern C++
 - [ ] Add Driver Code for TB6612 Motor Driver
 - [ ] Add Driver Code for HC-SR04 UltraSonic Distance Sensors
 - [ ] Implement User Control of Robert
 - [ ] Implement Autonomous Controll
