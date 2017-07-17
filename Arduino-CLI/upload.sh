#!/bin/bash
SKETCH=Blink/Blink.ino
ARDUINO_PREFIX=/Applications/Arduino.app/Contents/
ARDUINO_BUILD=$ARDUINO_PREFIX/Java/arduino-builder
AVR=$ARDUINO_PREFIX/Java/hardware/tools/avr
HW=$ARDUINO_PREFIX/Java/hardware
PACKAGES=./Packages
TOOLS=$ARDUINO_PREFIX/Java/tools-builder
ARDUINO_LIBS=/Applications/Arduino.app/Contents/Java/libraries
LIBS=./libraries

$ARDUINO_BUILD -dump-prefs -logger=machine \
        -hardware $HW -hardware $Packages \
        -tools $TOOLS -tools $AVR -tools $Packages \
        -built-in-libraries $ARDUINO_LIBS -libraries $LIBS \
        -fqbn=arduino:avr:yun \
        -vid-pid=0X2341_0X8041 \
        -build-path . \
        -warnings=none \
        -prefs=build.warn_data_percentage=75 \
        -prefs=runtime.tools.avrdude.path=$AVR \
        -prefs=runtime.tools.avr-gcc.path=$AVR \
        -prefs=runtime.tools.arduinoOTA.path=$AVR \
        -verbose \
        $SKETCH
