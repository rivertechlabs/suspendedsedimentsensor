# :ocean:suspended-sediment-sensor
This sensor is prototype 3.0. Previous prototype (2.0) is found in this [repo](https://github.com/rivertechlabs/turbiditysensor).

**Improvements on this version:**
- :stuck_out_tongue_winking_eye: Entirely 3D printed design, so no need to beg a technician for help
- :racehorse: Longer battery life
- :dancers: Custom printed circuit board (PCB) as an [Arduino MRK WAN 1310](https://store-usa.arduino.cc/products/arduino-mkr-wan-1310) shield for an easier and cleaner assembly with a more durable design 

-----------------------------------------
A 3D-printed, open-source suspended sediment sensor for river network monitoring.
## Introduction
This repo is dedicated to the open-source turbidity sensor. More info can be found here:
- https://rivertechlabs.org/
- https://hyd.ifu.ethz.ch/research/riparian/susp-sed-sense.html

This repo contains several items to build the current prototype of the open-source turbidity sensor including:
- The code needed to upload to the microcontroller
- CAD files
- 3D printing files
- PCB files
- Hookup schematics, examples, and photos

## Code
Before you begin, you need the ESP-IDF installed on your computer. Please visit the [Espressif website](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) for more information on installation.

The current code takes 100 frequency measurements and enters deep-sleep for 1min, this can of course be changed. Also, the code isn't beautiful and I apologize but it will get better with time.

## CAD files

## 3D printing files
Here you will find the printer files (.gcode) and the PrusaSlicer files (.3mf). The CAD files were exported as .stp and imported into the PrusaSlicer program (free). See the .3mf files to understand the settings we used in PrusaSlicer for optimal printing. The main settings were:
- Avoid crossing perimeters [enabled]: so that there was less material to clean after printing
- 5 perimeters, 5 bottom solid layers, and AT LEAST 0.15mm layer height for all components (on the thread and cap to ensure they were waterproof, see this [article](https://blog.prusa3d.com/watertight-3d-printing-pt1-vases-cups-and-other-open-models_48949/))
- The lens inserts ("zapfen") have an even smaller layer height
- The "teeth" have variable layer height (ranging from 0.07-0.15mm to ensure that the zapfen fit into the teeth)

The .3mf files were exported as .gcode files and then printed on a Prusa I3 Mk3 and/or Mk3S Filament Printer.

## PCB

## Hookup schematics
<img src="Images/schematic.png" width="700">

## Contact
For questions, please email me at droujko@ifu.baug.ethz.ch or @rivertechjess on Twitter :bird:
