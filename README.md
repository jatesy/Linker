Linker

====================Copyright (C) 2014 Daran Zhang. All rights reserved.================================

The linker takes individually compiled modules and creates a single executable by resolving external symbol references (e.g. variables and functions) and module relative addressing by assigning global addresses after placing the modules’ object code at global addresses.

HHOW TO COMPILE:

Simply compile this cpp file on linux using g++.

Command to compile 'linker.cc' is: 

g++ linker.cc -o linker

Then the compiler will make a executable file titled 'linker'.

HOW TO RUN:

After compiling, just use the command below to run the program:

./linker [input filename]

Then the program will start to run. The linker will start to work and the output will display on the screen.

===================Copyright (C) 2014 Daran Zhang. All rights reserved.==================================
