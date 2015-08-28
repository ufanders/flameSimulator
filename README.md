# flameSimulator

This is a poor attempt at porting a flame pattern generator for an 8x8 RGB LED matrix from Arduino to the PIC16F1 platform. This code was run using the PCB within the matriLedWatch project.

It runs super fucking slow but it does run. If you find a way to eliminate floating-point usage in the code it should run well. There are lots of HSL-RGB and RGB-HSL conversion algos out there but they all use floats, argh!
