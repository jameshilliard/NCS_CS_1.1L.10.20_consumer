
The Broadcom Speed Service application is available in all FAP-based Broadcom DSL platforms, and can be compiled as a standalone application for standard Linux PCs (Desktop).

Speed Service Desktop
=====================

In order to build Speed Service to run on a Linux PC, do the following on the target Linux PC:

gcc -Wall -D_REENTRANT -D_POSIX_TIMERS speedsvc.c -o speedsvc -lrt

