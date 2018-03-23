#! /bin/bash
# Simple bash script to transform ascii message to CAN for the screen.

if [ $# -ne 3 ]; then
	echo -e "Wrong args : \"$0 can0 16 \"test over can0!!\"\""
	echo -e "        or : \"$0 can1 16 \"test over can1??\"\""
	echo "Arg 1 is the CAN interface."
	echo "Arg 2 is the node ID of the screen in decimal. (factory is 16)"
	echo "Arg 3 can't be greater than 32 chars (which is the maximum capactity of the screen."
	echo "End of $0."
	exit 1
fi
if [ "$1" != "can0" ] && [ "$1" != "can1" ] && [ "$1" != "vcan0" ] && [ "$1" != "vcan1" ]; then
	echo -e "Wrong args : \"$0 can0 \"test over can0!!\"\" \n\tor : \"$0 can1 \"test over can1\"??\""
	echo "CAN interface should be can0 or can1 or vcan0 or vcan1"
	echo "End of $0."
	exit 1
fi

#Copy data received in arg 3 to pad it with space.
dataToSend=$3
if [ ${#3} -gt 32 ]; then
	echo "The data to send is too big (${#3}), it can't be bigger than 32!"
else
	#We padd all missing char with [space].
	pad=$(printf '%0.1s' " "{1..32})
	dataToSend=$3$pad
fi

#Check if can-utils is installed
canInstall=1
if [ -z `command -v cansend` ]; then
	canInstall=0
	echo "cansend not installed : mode DEBUG enabled."
	echo "More info on https://github.com/linux-can/can-utils to install cansend."
fi

#0 padding node id to send.
printf -v canId "%02hx" $2

#Brute change ASCII arg 3 into HEX.
printf -v data1 "%02x%02x%02x%02x%02x%02x%02x%02x" "'${dataToSend:0:1}" "'${dataToSend:1:1}" "'${dataToSend:2:1}" "'${dataToSend:3:1}" "'${dataToSend:4:1}" "'${dataToSend:5:1}" "'${dataToSend:6:1}" "'${dataToSend:7:1}"
printf -v data2 "%02x%02x%02x%02x%02x%02x%02x%02x" "'${dataToSend:8:1}" "'${dataToSend:9:1}" "'${dataToSend:10:1}" "'${dataToSend:11:1}" "'${dataToSend:12:1}" "'${dataToSend:13:1}" "'${dataToSend:14:1}" "'${dataToSend:15:1}"
printf -v data3 "%02x%02x%02x%02x%02x%02x%02x%02x" "'${dataToSend:16:1}" "'${dataToSend:17:1}" "'${dataToSend:18:1}" "'${dataToSend:19:1}" "'${dataToSend:20:1}" "'${dataToSend:21:1}" "'${dataToSend:22:1}" "'${dataToSend:23:1}"
printf -v data4 "%02x%02x%02x%02x%02x%02x%02x%02x" "'${dataToSend:24:1}" "'${dataToSend:25:1}" "'${dataToSend:26:1}" "'${dataToSend:27:1}" "'${dataToSend:28:1}" "'${dataToSend:29:1}" "'${dataToSend:30:1}" "'${dataToSend:31:1}"

#Send the data to the correct address or print if cansend not installed.
if [ $canInstall -eq 0 ]; then
	echo "cansend $1 1${canId}#$data1"
	echo "cansend $1 2${canId}#$data2"
	echo "cansend $1 3${canId}#$data3"
	echo "cansend $1 4${canId}#$data4"
else
	cansend $1 1${canId}#$data1
	cansend $1 2${canId}#$data2
	cansend $1 3${canId}#$data3
	cansend $1 4${canId}#$data4
fi

exit 0
