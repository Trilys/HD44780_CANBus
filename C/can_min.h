/*
 * can_min.h
 * This file is part of can_to_modbus
 *
 * Copyright (C) 2016 - Trilys
 *
 * can_to_modbus is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * can_to_modbus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with can_to_modbus. If not, see <http://www.gnu.org/licenses/>.
 */


//Send the first sizeOfData bytes of data_to_send to id.
//Return 0 if ok
//ex : CAN_send(0x600,CAN, 8); or CAN_send(0x601,"\x4b\x20\x29\x01\xe8\x03\x00\x00", 8);
char CAN_send(char num_can_interface, long id, char *data_to_send, char sizeOfData);

// Use CAN_send_offi only if can_send doesn't work with your message
//CAN_send_offi("601#2B0050030000EFFA");
//CAN_send_offi("12345678#2B0050030000EFFA");
//Return 0 if ok
char CAN_send_offi(unsigned char num_can_interface, char *data_to_send);

//Receive CAN from canIDtoFilter marked as decimal.
char CAN_receive(unsigned char num_can_interface, long canIDtoFilter, long delayToOffInUs, char canReceived[], char *sizeOfCAN);

//Launch CAN_init to launch CAN_receive_lite or CAN_connect_lite, then CAN_deinit
char CAN_init(unsigned char num_can_interface);
char CAN_receive_lite(unsigned char num_can_interface, long canIDtoFilter, long delayToOffInUs, char canReceived[], char *sizeOfCAN);
//size addressToFilter = 3 bytes : address×2+sub-address
char CAN_receive_filtered_lite(unsigned char num_can_interface, long canIDtoFilter, char addressToFilter[],long delayToOffInUs, char canReceived[], char *sizeOfCAN);
char CAN_receive_lite_multifilters(unsigned char num_can_interface, unsigned char nbIDtoFilter, long canIDtoFilter[], long delayToOffInUs, long *canId, char canReceived[], char *sizeOfCAN);
//Close sockets
char CAN_deinit(unsigned char num_can_interface);

