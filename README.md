# MoonTracking

This program calculates the distance to the Moon for a given time period using the JPL SPICE Toolkit.

## Installation

1. Download and install Visual Studio from the [official website](https://visualstudio.microsoft.com/downloads/).
2. Download the latest version of the SPICE Toolkit from the [NAIF website](https://naif.jpl.nasa.gov/naif/toolkit_C.html).
3. Extract the contents of the downloaded SPICE Toolkit ZIP file.
4. Create a new Visual Studio project or open an existing project that uses the SPICE Toolkit.
5. In Visual Studio, right-click on the project in the Solution Explorer and select Properties.
6. In the C/C++ section, add the SPICE Toolkit include directory to the Additional Include Directories field.
7. In the Linker section, add the SPICE Toolkit library directory to the Additional Library Directories field.
8. In the Linker section, add the SPICE Toolkit library files to the Additional Dependencies field.
9. Click Apply and then click OK to save the changes.

## Downloading BSP Files

BSP files are required to calculate the position of celestial bodies. They can be downloaded from the NASA website on the Planetary Constants Kernel (PCK), Lunar Constants Kernel (LCK), and Leapseconds Kernel (LSK) page.

To download the BSP files:

1. Go to the [Planetary Constants Kernel (PCK), Lunar Constants Kernel (LCK), and Leapseconds Kernel (LSK) page](https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/).
2. Download the BSP files you wish to use and save them in a folder of your choice.
3. Make sure to use the correct path to the file in your SPICE Toolkit function calls.

