#! /bin/bash
# Simple bash script to transform CAN message received from the screen.

if [ $# -ne 2 ]; then
	echo -e "Wrong args : \"$0 can0 16\""
	echo -e "        or : \"$0 can1 16\""
	echo "Arg 1 is the CAN interface."
	echo "Arg 2 is the node ID of the screen in decimal. (factory is 16)"
	echo "End of $0."
	exit 1
fi
if [ "$1" != "can0" ] && [ "$1" != "can1" ] && [ "$1" != "vcan0" ] && [ "$1" != "vcan1" ]; then
	echo -e "Wrong args : \"$0 can0 \"test over can0!!\"\" \n\tor : \"$0 can1 \"test over can1\"??\""
	echo "CAN interface should be can0 or can1 or vcan0 or vcan1"
	echo "End of $0."
	exit 1
fi

#Check if can-utils is installed
canInstall=1
if [ -z `command -v candump` ]; then
	canInstall=0
	echo "candump not installed : mode DEBUG enabled."
	echo "More info on https://github.com/linux-can/can-utils to install candump."
fi

#When running=0 : the soft stop
running=1
#Catch stop
function ctrl_c() {
	echo "Stop $0."
	running=0
}
# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

#node id to send.
printf -v canId "%03hx" $((0x180+$2))
echo "Start candump can1,$canId:7ff -n 1"
#echo -e "Up\t Right\t Down\t Left\t Enter\t Cancel\t"
echo -e "Up\t Down\t Cancel\t"
while [[ $running -eq 1 ]]; do
	resp=`candump can1,$canId:7ff -n 1`
	resp0=0x`echo $resp | awk '{print $4}'`
	resp1=0x`echo $resp | awk '{print $5}'`

	valueUp=$(($resp0==0x81))+2*$(($resp0==0x01))+4*$(($resp1&1))
	valueDown=$(($resp0==0x84))+2*$(($resp0==0x04))+4*$(($resp1&4))
	valueCancel=$(($resp0==0xA0))+2*$(($resp0==0x20))+4*$(($resp1&32))
	if [[ $valueUp -eq 1 ]]; then
		echo -en "_/¯\t"
	elif [[ $valueUp -eq 2 ]]; then
		echo -en "¯\\_ \t"
	elif [[ $valueUp -ge 4 ]]; then
		echo -en "¯¯¯\t"
	else
		echo -en "___\t"
	fi
	if [[ $valueDown -eq 1 ]]; then
		echo -en "_/¯\t"
	elif [[ $valueDown -eq 2 ]]; then
		echo -en "¯\\_ \t"
	elif [[ $valueDown -ge 16 ]]; then
		echo -en "¯¯¯\t"
	else
		echo -en "___\t"
	fi
	if [[ $valueCancel -eq 1 ]]; then
		echo -en "_/¯\t"
	elif [[ $valueCancel -eq 2 ]]; then
		echo -en "¯\\_ \t"
	elif [[ $valueCancel -ge 128 ]]; then
		echo -en "¯¯¯\t"
	else
		echo -en "___\t"
	fi
	echo -en "\r"
done
exit 0
