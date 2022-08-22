# suspended-sediment-sensor
:ocean: S.S. Rhône: 3D-printed, open-source suspended sediment sensor for river network monitoring (prototype 3.0)

## Introduction
This repo is dedicated to the open-source turbidity sensor. More info can be found here:
- https://rivertechlabs.org/
- https://hyd.ifu.ethz.ch/research/riparian/susp-sed-sense.html

This repo contains several items to build the current prototype of the open-source turbidity sensor including:
- The code needed to upload to the ESP32
- CAD files
- Hookup schematics

## Code
Before you begin, you need the ESP-IDF installed on your computer. Please visit the [Espressif website](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) for more information on installation.

The current code takes 100 frequency measurements and enters deep-sleep for 1min, this can of course be changed. Also, the code isn't beautiful and I apologize but it will get better with time.

## Hookup schematics
<img src="Images/schematic.png" width="700">

## Contact
For questions, please email me at droujko@ifu.baug.ethz.ch or @rivertechjess on Twitter :bird:

-----------------------------------------
## Prototype 1.0
This repo also contains the CAD files for prototype 1.0 of the turbidity sensor (you can use this to 3D print the device in a school project)
