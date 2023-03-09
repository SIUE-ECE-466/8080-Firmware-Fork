/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 *
 * This project handles the charge and discharge of power supply
 * Firmware code for the power supply
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "Registers.h"
#include "Register_map.h"

extern "C" {
#include "../PowerSupply_modbus/libmodbus/modbus.h"
}

#define SOMAXCONN       0x7fffffff
#define DEFAULT_BUFLEN 256
#define My_slave_id 2

int registermap(uint16_t* registers, modbus_mapping_t* registerMap, uint8_t* recvbuf);
extern int Estimator();
int communication();


using namespace std;

mutex mtx;

modbus_t* mb;
int ListenSocket = -1;
int ClientSocket = -1;
int iResult = 0;
int iSendResult = 0;
int Status = -1;

uint8_t recvbuf[DEFAULT_BUFLEN] = { 0 };
int recvbuflen = DEFAULT_BUFLEN;

modbus_mapping_t registerMap;

int main()
{
	cout << "ECE 466" << endl;
	cout << "8080 Firmware Group" << endl;

	/* Creating Modbus TCP Context*/
	mb = modbus_new_tcp("192.168.4.12", 502);
	if (mb == NULL)
	{
		fprintf(stderr, "unable to allocate libmodbus context\n");
		return -1;
	}
	
	/*set slave ID*/
	Status = modbus_set_slave(mb, My_slave_id);
	if (Status == -1) printf("Invalid slave id");

	/* Listen for Modbus TCP master/client connection request*/
	ListenSocket = modbus_tcp_listen(mb, SOMAXCONN);

	if (ListenSocket == -1) {
		fprintf(stderr, "Conection failed: %s\n", modbus_strerror(errno));
		modbus_free(mb);
		closesocket(ListenSocket);
		return -1;
	}

	/* Accept the Modbus TCP request*/
	ClientSocket = modbus_tcp_accept(mb, &ListenSocket);

	if (ClientSocket == -1) {
		fprintf(stderr, "accept failed with error: %d\n", WSAGetLastError());
		modbus_free(mb);
		closesocket(ListenSocket);
		return 1;
	}
	cout << "Connected" << endl;

	/* Close the Socket after accepting connection*/
	closesocket(ListenSocket);

	/*communicatioln thread*/
	thread t1(communication);
	/*Estimator thread*/
	thread t2(Estimator);

	t1.join();
	t2.join();
	

	return 0;

}
int communication()
{
	// Receive until the peer shuts down the connection
	do {

		iResult = modbus_receive(mb, recvbuf);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			for (int i = 0; i < iResult; i++) {
				printf("msg[%d] : %X\n", i, recvbuf[i]);
			}

			/*Register the map address*/
			registermap(Register_Base, &registerMap, recvbuf);

			// reply modbus response
			iSendResult = modbus_reply(mb, recvbuf, iResult, &registerMap);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	this_thread::sleep_for(chrono::microseconds(10000));
}
int registermap(uint16_t* registers, modbus_mapping_t* registerMap, uint8_t *recvbuf)
{
	int addr = (recvbuf[8] << 8) + recvbuf[9];
	registerMap->start_registers = addr;
	registerMap->nb_registers = 125;
	registerMap->tab_registers = registers;

	return 0;
}