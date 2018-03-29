#HD44780_CANBus
===============

Repo containing Bash and C files for the Trilys CAN-Bus Screen HD44780

##Dependencies:
---------------
###For all :
* A BeagleBone,
* Trilys_CAN_creator to create CAN0 or CAN1 (https://github.com/Trilys/canbus_on_beaglebone).  

###For bash
* can-utils installed and ready to use: https://github.com/linux-can/can-utils  

##Execution:
------------
###Using bash  
Sending message to the screen using SendData.sh in Bash folder needs 3 args :

1. Arg 1 is the CAN interface.
2. Arg 2 is the node ID of the screen in decimal. (factory is 0x10=16)
3. Arg 3 can't be greater than 32 chars (which is the maximum capactity of the screen).

Exemple, send data to the node ID 0x10 (=16) :

    ./SendData.sh can0 16 "test over can0!!
    ./SendData.sh can1 16 "test over can0!!

Receiving message from the screen using ReceivePushButtons.sh in Bash folder needs 2 args :

1. Arg 1 is the CAN interface.
2. Arg 2 is the node ID of the screen in decimal. (factory is 0x10=16)

Exemple, receive data from the node ID 0x10 (=16) :

    ./ReceivePushButtons.sh can0 16
    ./ReceivePushButtons.sh can1 16

will give you an interactive information of the satus of the buttons (in the next exemple, the Up button is pressed for more than 500ms, the cancel button has been released) :

    Up	 Down	 Cancel	
    ¯	 _	 \


