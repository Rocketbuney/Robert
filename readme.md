
 
# Introduction
This document serves as the design document for the Robert Project. Robert will be a small Raspberry Pi based robot.
# Overview of the Design
This project is built off of The Raspberry Pi Zero W, along with a couple of modules handling motor control and battery management. the project will run on C and will not require any Dependencies to build, aiding the ability to develop on a separate machine. They aren't any memory restraints as the Pi will only be running this code.
# Scope
This project includes the following key features to it's function:
- Ability to interface efficiently with GPIO pins
- Accurate control of motors
- Reading of sensory values
- Development of an autonomous and human control modes
- A user interface in the browser as a means to comunicate with Robert
# System Design
Robert aims to use a Data-Orientated approach utilizing C's speed and procedural nature. The code-base will be modularised into each respective major component as listed above.
