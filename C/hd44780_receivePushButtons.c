/*
 * hd44780_receivePushButtons.c
 * This file is part of HD44780_CANBus
 *
 * Copyright (C) 2018 - Trilys
 *
 * HD44780_CANBus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * HD44780_CANBus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HD44780_CANBus. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
//Catch ^C
#include <signal.h>
#include <time.h>

//Trilys lib:
//Access to CAN interfaces
#include "can_min.h"
#include "config.h"

//Sleep during loop to slow CPU rate
#include <unistd.h>

//Work on message
#include <string.h>

static volatile int running = 1;

char interface_can_name[6];

void print_usage(char *prg)
{
	fprintf(stderr, "Version 0.1 (2018/03/27) made by Trilys.\n");
	fprintf(stderr, "\nUsage: %s canX ID", prg);
	fprintf(stderr, "\nWill run through canX, receiving all clicked buttons from ID.");
	fprintf(stderr, "\nExemple :");
	fprintf(stderr, "\n  \"%s can0 16\" will show :", prg);
	fprintf(stderr, "\nUp	Right	Down	Left	Enter	Cancel", prg);
	fprintf(stderr, "\n_	_…", prg);
	fprintf(stderr, "\n  (use CTRL-C to terminate %s)\n\n", prg);
	fprintf(stderr, "\n");
}

void local_sigterm(int signo)
{
	if (running == 0) {
		printf("\nViolent kill...\n");
		exit(0);
	}
	printf("\nKill cell_bench...\n");
	running = 0;
}


int main(int argc, char *argv[])
{
	//Catch ctrl_c to stop the prog
	signal(SIGTERM, local_sigterm);
	signal(SIGHUP, local_sigterm);
	signal(SIGINT, local_sigterm);
	
	if (argc!=3 || strstr(argv[1], "can") == NULL || (strcmp(argv[1], "can")!=48 && strcmp(argv[1], "can")!=49)) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	unsigned char can_num_interface = strcmp(argv[1], "can")-48;
	int arg_can_id=(int)strtol(argv[2], NULL, 10);
	
	printf("\ncan=%d, id=x%02hx",can_num_interface, arg_can_id);
	
	unsigned long can_id_filter;
	long idReceived;
	char message_can[8];
	char sizeOfCAN;
	unsigned char i;
	
	can_id_filter=0x180+arg_can_id;
	
	CAN_init(can_num_interface);
	
	unsigned char valueUp, valueRight, valueDown, valueLeft, valueEnter, valueCancel;
	setbuf(stdout, NULL);	//Remove printf buffer to print data directly
	printf("\nUp\t Right\t Down\t Left\t Enter\t Cancel\t\n");
	while (running) {
		idReceived=0;
		for (i = 0; i < 8; i += 1) {
			message_can[i] = 0;
		}
		sizeOfCAN=0;
/*		CAN_receive_lite_multifilters(can_num_interface, 1, can_id_filter, MAX_TIME_CANRC_WAITER, &idReceived, message_can, &sizeOfCAN);*/
		CAN_receive_lite(can_num_interface, can_id_filter, MAX_TIME_CANRC_WAITER, message_can, &sizeOfCAN);
		if (sizeOfCAN==2) {
			valueUp		=(message_can[0]==0x81)	+2*(message_can[0]==0x01)	+4*(message_can[1]&1);
			valueRight	=(message_can[0]==0x82)	+2*(message_can[0]==0x02)	+4*(message_can[1]&2);
			valueDown	=(message_can[0]==0x84)	+2*(message_can[0]==0x04)	+4*(message_can[1]&4);
			valueLeft	=(message_can[0]==0x88)	+2*(message_can[0]==0x08)	+4*(message_can[1]&8);
			valueEnter	=(message_can[0]==0x90)	+2*(message_can[0]==0x10)	+4*(message_can[1]&16);
			valueCancel	=(message_can[0]==0xA0)	+2*(message_can[0]==0x20)	+4*(message_can[1]&32);
			if (valueUp==1) {
				printf("_/¯\t");
			} else if (valueUp==2) {
				printf("¯\\_ \t");
			} else if (valueUp>=4) {
				printf("¯¯¯\t");
			} else {
				printf("___\t");
			}
			if (valueRight==1) {
				printf("_/¯\t");
			} else if (valueRight==2) {
				printf("¯\\_ \t");
			} else if (valueRight>=8) {
				printf("¯¯¯\t");
			} else {
				printf("___\t");
			}
			if (valueDown==1) {
				printf("_/¯\t");
			} else if (valueDown==2) {
				printf("¯\\_ \t");
			} else if (valueDown>=16) {
				printf("¯¯¯\t");
			} else {
				printf("___\t");
			}
			if (valueLeft==1) {
				printf("_/¯\t");
			} else if (valueLeft==2) {
				printf("¯\\_ \t");
			} else if (valueLeft>=32) {
				printf("¯¯¯\t");
			} else {
				printf("___\t");
			}
			if (valueEnter==1) {
				printf("_/¯\t");
			} else if (valueEnter==2) {
				printf("¯\\_ \t");
			} else if (valueEnter>=64) {
				printf("¯¯¯\t");
			} else {
				printf("___\t");
			}
			if (valueCancel==1) {
				printf("_/¯\t");
			} else if (valueCancel==2) {
				printf("¯\\_ \t");
			} else if (valueCancel>=128) {
				printf("¯¯¯\t");
			} else {
				printf("___\t");
			}
			printf("\r",message_can[0], message_can[1]);
			
		} else {
			running=0;
		}
	}
	
	
	CAN_deinit(can_num_interface);
	printf("\n\nEnd of %s\n\n",argv[0]);
}






