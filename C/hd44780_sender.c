/*
 * hd44780_sender.c
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
	fprintf(stderr, "Version 0.1 (2018/03/23) made by Trilys.\n");
	fprintf(stderr, "\nUsage: %s canX ID \"message to send!\"", prg);
	fprintf(stderr, "\nWill run through canX, sending \"message to send!\" to ID.");
	fprintf(stderr, "\nExemple :");
	fprintf(stderr, "\n  %s can0 16 \"Test over CAN0\" will print \"Test over CAN0\" to the screen 0x10 (=16) on CAN0\n", prg);
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
	
	if (argc!=4 || strstr(argv[1], "can") == NULL || (strcmp(argv[1], "can")!=48 && strcmp(argv[1], "can")!=49)) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	unsigned char can_num_interface = strcmp(argv[1], "can")-48;
	int arg_can_id=(int)strtol(argv[2], NULL, 10);
	
	printf("\ncan=%d, id=x%02hx",can_num_interface, arg_can_id);
	printf("\nMessage=%s", argv[3]);
	unsigned char len=strlen(argv[3]);
	if (len>32) {
		printf("\nMessage >32 char (%d).",len);
	}
	char data1[9], data2[9], data3[9], data4[9];
	//Padd all with space:
	unsigned char i;
	for (i = 0; i < 8; i += 1) {
		data1[i]=0x20;
		data2[i]=0x20;
		data3[i]=0x20;
		data4[i]=0x20;
	}
	data1[8] = '\0';
	data2[8] = '\0';
	data3[8] = '\0';
	data4[8] = '\0';
	
	if (len>=8) {
		memcpy(data1, &argv[3][0], 8);
	} else {
		memcpy(data1, &argv[3][0], len);
	}
	if (len>=16) {
		memcpy(data2, &argv[3][8], 8);
	} else if(len>8) {
		memcpy(data2, &argv[3][8], len-8);
	}
	if (len>=24) {
		memcpy(data3, &argv[3][16], 8);
	} else if(len>16) {
		memcpy(data3, &argv[3][16], len-16);
	}
	if (len>=32) {
		memcpy(data4, &argv[3][24], 8);
	} else if(len>24) {
		memcpy(data4, &argv[3][24], len-24);
	}
	
	CAN_init(can_num_interface);
	
	CAN_send(can_num_interface, 0x100+arg_can_id, data1, 8);
	CAN_send(can_num_interface, 0x200+arg_can_id, data2, 8);
	CAN_send(can_num_interface, 0x300+arg_can_id, data3, 8);
	CAN_send(can_num_interface, 0x400+arg_can_id, data4, 8);
	
	CAN_deinit(can_num_interface);
	printf("\n\nEnd of %s\n\n",argv[0]);
}
