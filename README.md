# SBM (Simple BitMap) <WIP

## Table of Content
[Overview](#overview)<br>
[Objective](#objective)<br>
[Setup](#setup)<br>
[Building](#building)<br>
[Executing](#installing)<br>
[Resources](#resources)<br>

## Overview

This is my attempt at creating a portable C library that can be used to link with other pico_sdk based projects in order for me to have functionality to write values to an SH1106 OLED via SPI.<br>

## Objective

- [ ] Create portable library containing basic SH1106 OLED functionality.<br>

## Setup

Here's how I setup the GPIO Ports on the Pico2<br>

On SH1106 Connection:
GND -> GND(pin 28)<br>
VCC -> 3V3(OUT, pin 36)<br>
CLK -> GP18(pin24)<br>
MOSI -> GP19(pin25)<br>
RES -> GP20(pin26)<br>
DC -> GP16(pin21)<br>
CS -> GP17(pin22)<br>

Picture:
![SH1106 connected to RP2350](images/SH1106_with_RP2350.jpg)

## Building 

## Installing

## Resources

[SH1106 Adafruit Product Spec](https://cdn-shop.adafruit.com/product-files/5228/5223-ds.pdf)<br>
[Adafruit SH1106 Examples](https://github.com/adafruit/Adafruit_SH110x/blob/master/)<br>
[Micro Center OLED](https://www.microcenter.com/product/643965/inland-iic-spi-13-128x64-oled-v20-graphic-display-module-for-arduino-uno-r3)<br>
[Sino Wealth SH1106 PDF](https://www.pololu.com/file/0J1813/SH1106.pdf)<br>
