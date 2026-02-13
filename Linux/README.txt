bb_scd41
--------

Getting Started
---------------
To use this library on Linux, first run the makefile in this directory to
create a static link library. Ensure you have gcc installed and simply open
a terminal window in this directory and type 'make'. The static library and
header file will be copied to your /usr/local directory for easy access.

Pre-requisites
--------------
This code uses the SMBUS API and requires the libi2c-dev package to be installed before use:
sudo apt install libi2c-dev

Examples
--------
In the examples/Linux folder are makefile projects which show how to use
the library from C++. The API is the same on Arduino, Linux and esp-idf,


