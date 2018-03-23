//Definition for CAN:
#ifdef VCAN
	#define CAN_INTERFACE0 "vcan0"
	#define CAN_INTERFACE1 "vcan1"
#else
	#define CAN_INTERFACE0 "can0"
	#define CAN_INTERFACE1 "can1"
#endif

#define MAX_CHAR_CAN 8

//can_min.c
#define MAX_TIME_CANRC_CONNECT 100
#define ID \x00\x00
#define PSW \xDF\x4B

#define MAX_TIME_CANRC_WAITER 5000
