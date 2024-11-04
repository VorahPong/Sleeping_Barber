Project #3 Sleeping Barber Problem
Developed by VorahPong, Mean
Date 11/07/2024

* Overview
  This program was coded with C++ Xcode 16 on Mac.

  The purpose of this code is to demonstrate the use of binary semaphore and threads to simulate the barber problem.

  The sleeping barber problem is a classic inter-process communication and synchronization
  problem between multiple operating system processes. The problem is analogous to that of 
  keeping a barber working when there are customers, resting when there are none, and doing so in an orderly manner.

* Features
  The two programs client and server allowed two computers to talk to each other. The server
  will automatically shutdown when client disconnected.

* Usage
To compile this project, you must have these files:
    vorahpong_mean_project_3_optional.cpp

To compile:
    Simply use a C++ compiler.


Output example:

How many barbers are working in the shop? : 1

How many customers are coming today? : 2

*************************
* Barber Shop is Open!! *
*************************


Barber number 1 is going to sleep.

Customer number 1 has entered.

Barber number 1 is waking up.

Barber number 1 is cutting hair.

Customer number 1 is getting a haircut.

Customer number 2 has entered.

Customer number 1 left with a good haircut.

Barber number 1 has finished cutting hair.

Barber number 1 is cutting hair.

Customer number 2 is getting a haircut.

Barber number 1 has finished cutting hair.

Barber number 1 is going to sleep.

Customer number 2 left with a good haircut.

No more customers for today!

*************************
* Barber Shop is Close! *
*************************

Program ended with exit code: 0
