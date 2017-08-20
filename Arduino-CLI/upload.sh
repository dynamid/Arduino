#!/bin/bash
#===================== CONFIGURATION =====================
SKETCH=Blink/Blink.ino
ARDUINO_PREFIX=/Applications/Arduino.app/Contents
ARDUINO_BUILD=$ARDUINO_PREFIX/Java/arduino-builder
AVR=$ARDUINO_PREFIX/Java/hardware/tools/avr
HW=$ARDUINO_PREFIX/Java/hardware
PACKAGES=./Packages
TOOLS=$ARDUINO_PREFIX/Java/tools-builder
ARDUINO_LIBS=/Applications/Arduino.app/Contents/Java/libraries
LIBS=./libraries

# Must be an absolute path
BUILDFOLDER=/Users/nstouls/Documents/INSA/Arduino/Arduino-CLI/tmp/
if [ ! -d $BUILDFOLDER ]
then
    mkdir $BUILDFOLDER
fi

#May change depending on target :
ARCH=arduino:avr:yun
#-fqbn=arduino:avr:yun
VIDPID=0X2341_0X8041
# -vid-pid=0X2341_0X8041 : specific to each board, according to boards.txt:
#     /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/boards.txt



#===================== STEP 1: Dump préférences ... inutile ? =====================
# /Applications/Arduino.app/Contents/Java/arduino-builder -dump-prefs -logger=machine -hardware /Applications/Arduino.app/Contents/Java/hardware -hardware /Users/nstouls/Library/Arduino15/packages -tools /Applications/Arduino.app/Contents/Java/tools-builder -tools /Applications/Arduino.app/Contents/Java/hardware/tools/avr -tools /Users/nstouls/Library/Arduino15/packages -built-in-libraries /Applications/Arduino.app/Contents/Java/libraries -libraries /Users/nstouls/Documents/Arduino/libraries -fqbn=arduino:avr:yun -vid-pid=0X2341_0X8041 -ide-version=10801 -build-path /var/folders/y6/q5yxn54125z8z556xhbn85940000gn/T/arduino_build_44200 -warnings=none -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avrdude.path=/Applications/Arduino.app/Contents/Java/hardware/tools/avr -prefs=runtime.tools.avr-gcc.path=/Applications/Arduino.app/Contents/Java/hardware/tools/avr -prefs=runtime.tools.arduinoOTA.path=/Applications/Arduino.app/Contents/Java/hardware/tools/avr -verbose ./Blink/Blink.ino
#
# $ARDUINO_BUILD \
#     -dump-prefs \
#     -logger=machine \
#     -hardware $HW \
#     -hardware $PACKAGES \
#     -tools $TOOLS \
#     -tools $AVR \
#     -tools $PACKAGES \
#     -built-in-libraries $ARDUINO_LIBS \
#     -libraries $LIBS \
#     -fqbn=$ARCH \
#     -vid-pid=$VIDPID \
#     -build-path $BUILDFOLDER \
#     -warnings=none \
#     -prefs=build.warn_data_percentage=75 \
#     -prefs=runtime.tools.avrdude.path=$AVR \
#     -prefs=runtime.tools.avr-gcc.path=$AVR \
#     -prefs=runtime.tools.arduinoOTA.path=$AVR \
#     -verbose \
#     $SKETCH
# #    -ide-version=10801 \



#===================== STEP 2: Compile =====================

#/Applications/Arduino.app/Contents/Java/arduino-builder -compile -logger=machine -hardware /Applications/Arduino.app/Contents/Java/hardware -hardware /Users/nstouls/Library/Arduino15/packages -tools /Applications/Arduino.app/Contents/Java/tools-builder -tools /Applications/Arduino.app/Contents/Java/hardware/tools/avr -tools /Users/nstouls/Library/Arduino15/packages -built-in-libraries /Applications/Arduino.app/Contents/Java/libraries -libraries /Users/nstouls/Documents/Arduino/libraries -fqbn=arduino:avr:yun -vid-pid=0X2341_0X8041 -ide-version=10801 -build-path /var/folders/y6/q5yxn54125z8z556xhbn85940000gn/T/arduino_build_44200 -warnings=none -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avrdude.path=/Applications/Arduino.app/Contents/Java/hardware/tools/avr -prefs=runtime.tools.avr-gcc.path=/Applications/Arduino.app/Contents/Java/hardware/tools/avr -prefs=runtime.tools.arduinoOTA.path=/Applications/Arduino.app/Contents/Java/hardware/tools/avr -verbose /Users/nstouls/CloudINSA/Documents/CITI/DynaMid/Arduino/Arduino-CLI/Blink/Blink/Blink.ino

$ARDUINO_BUILD \
    -compile \
    -logger=machine  \
    -hardware $HW \
    -hardware $PACKAGES \
    -tools $TOOLS \
    -tools $AVR \
    -tools $PACKAGES \
    -built-in-libraries $ARDUINO_LIBS \
    -libraries $LIBS \
    -fqbn=$ARCH \
    -vid-pid=$VIDPID \
    -ide-version=10801 \
    -build-path $BUILDFOLDER \
    -warnings=none \
    -prefs=build.warn_data_percentage=75 \
    -prefs=runtime.tools.avrdude.path=$AVR \
    -prefs=runtime.tools.avr-gcc.path=$AVR \
    -prefs=runtime.tools.arduinoOTA.path=$AVR \
    -verbose \
    $SKETCH




#    Redémarrage forcé en cours en 1200bps ouvert/fermé sur le port /dev/cu.usbmodem1411



#    /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -v -patmega32u4 -cavr109 -P/dev/cu.usbmodem1411 -b57600 -D -Uflash:w:/var/folders/y6/q5yxn54125z8z556xhbn85940000gn/T/arduino_build_44200/Blink.ino.hex:i

$AVR/bin/avrdude \
    -C$AVR/etc/avrdude.conf \
    -v \
    -patmega32u4 \
    -cavr109 \
    -P/dev/cu.usbmodem1411 \
    -b57600 \
    -D \
    -Uflash:w:$BUILDFOLDER/Blink.ino.hex:i
