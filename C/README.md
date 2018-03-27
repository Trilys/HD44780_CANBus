# C for HD44780_CANBus
----------------------

Get and send data to the Trilys CAN Screen

##Compilation
-------------

    gcc can_min.c lib_can.c hd44780_sender.c -o hd44780_sender

You can add debug with -D DEBUG option and use virtual CAN with -D VCAN option in the gcc command.

Or

    make

To install :

    sudo make install

To clean : 

    make clean


##Utilization
-------------

Send "This is a test over 2 lines!" to the screen ID 0x10 over CAN1 :

    ./hd44780_sender can1 16 "This is a test over 2 lines!"

Receive buttons transition and print them into the terminal (ID=0x10 and CAN1) :

    ./hd44780_receivePushButtons can1 16

Receive buttons and print them into the screen (ID=0x10 and CAN1) :

    ./hd44780_demo can1 16
