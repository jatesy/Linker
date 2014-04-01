Linker
==============================Copyright (C) 2014 Daran Zhang. All rights reserved.========================================

The linker takes individually compiled modules and creates a single executable by resolving external symbol references (e.g. variables and functions) and module relative addressing by assigning global addresses after placing the modules’ object code at global addresses.

HOW TO COMPILE:

Simply compile this cpp file on linux using g++.

Command to compile 'Daran_Lab1_code.cpp' is: 

g++ Daran_Lab2_Code.cpp -o scheduler

Then the compiler will make a executable file titled 'scheduler'.

HOW TO RUN:

After compiling, just use the command below to run the program:

./scheduler -s[FLS|R<num>] inputfile randfile (e.g. "./scheduler -sR10 ").

Then the program will start to run. The linker will start to work and the output will display on the screen.

You can also use runit.sh and diffit.sh to run inputs and compare outputs in batch.

==============================Copyright (C) 2014 Daran Zhang. All rights reserved.========================================
