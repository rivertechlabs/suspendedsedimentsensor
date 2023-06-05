# :ocean:Ötz-T: suspended sediment sensor
## News!

**This sensor was published in HardwareX! Read [Ötz-T: 3D-printed open-source turbidity sensor with Arduino shield for suspended sediment monitoring](https://linkinghub.elsevier.com/retrieve/pii/S2468067223000020).**

-----------------------------------------
This sensor is prototype 3.0. Previous prototype (2.0) is found in this [repo](https://github.com/rivertechlabs/turbiditysensor) (new repo necause I consider the two versions different enough).

**Improvements on this version:**
- :sleeping: Low-power [Arduino MRK WAN 1310](https://store-usa.arduino.cc/products/arduino-mkr-wan-1310) microcontroller
- :stuck_out_tongue_winking_eye: Entirely 3D printed design, so no need to beg a technician for help
- :racehorse: Longer battery life
- :dash: Turbidity, temperature and pressure sensor
- :recycle: Programed with Arduino IDE
- :dancers: Custom printed circuit board (PCB) as an Arduino shield for an easier and cleaner assembly with a more durable design 

-----------------------------------------
A 3D-printed, open-source suspended sediment sensor for river network monitoring.
## Introduction
This repo is dedicated to the open-source turbidity sensor. More info can be found here:
- https://rivertechlabs.org/
- https://hyd.ifu.ethz.ch/research/riparian/susp-sed-sense.html

This repo contains several items to build the current prototype of the open-source turbidity sensor including:
- The code needed to upload to the microcontroller
- 3D printing files
- PCB files
- CAD files

## Code
There are two codes in the Code folder: main.ino and set_time.ino. Please follow the instructions in set time.ino about changing the date in the program. After the program has flashed, every time the Arduino restarts (disconnect and reconnect to power) it will restart the RTC time to the original compile time of the set time.ino program. To mitigate this issue, the next program, main.ino, can be immediately flashed without disconnecting power or set time.ino can be re-flashed after commenting-out line 79 (“setPCF85263();”). Follow by flashing main.ino onto the Arduino. Please note that additional libraries may need to be downloaded and added to your library (e.g. the MS5803-05 external library [32]). The main.ino program was written to take a measurement every hour (one measurement is an average of 15 measurements) to conserve battery, but this can be changed in the code if you require higher temporal resolution to capture larger variability e.g. from flushing of sediment from a dam.

## 3D printing files
Here you will find the printer files (.gcode) and the PrusaSlicer files (.3mf). The CAD files were exported as .stp and imported into the PrusaSlicer program (free). See the .3mf files to understand the settings we used in PrusaSlicer for optimal printing. The main settings were:
- Avoid crossing perimeters [enabled]: so that there was less material to clean after printing
- 5 perimeters, 5 bottom solid layers, and AT LEAST 0.15mm layer height for all components (on the thread and cap to ensure they were waterproof, see this [article](https://blog.prusa3d.com/watertight-3d-printing-pt1-vases-cups-and-other-open-models_48949/))
- The lens inserts ("zapfen") have an even smaller layer height
- The "teeth" have variable layer height (ranging from 0.07-0.15mm to ensure that the zapfen fit into the teeth)

The .3mf files were exported as .gcode files and then printed on a Prusa I3 Mk3 and/or Mk3S Filament Printer.

## PCB
We have provided the Gerber files to directly order the PCBs. Alternatively, the .PcbDoc and .SchDoc files can be opened in Altium (the eductional license can be obtained for free) where you can export your own Gerber files. Finally, the schematic and PCB files were also provided as KiCad files so that they can be viewed and edited without Altium.

## CAD
In this folder we have the original CAD files (as STEP) so that they can be downloaded and adjusted for your project.

## Contact
For questions, please email me at droujko@ifu.baug.ethz.ch or @rivertechjess on Twitter :bird:
