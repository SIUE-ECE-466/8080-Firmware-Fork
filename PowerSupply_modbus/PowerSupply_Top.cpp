/*
 * This project handles the charge and discharge of power supply
 * Firmware code for the power supply
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
extern "C" {
#include <modbus.h>
}

#define SOMAXCONN       0x7fffffff
#define DEFAULT_BUFLEN 8

int communication();
int printValue();

using namespace std;

mutex mtx;
uint8_t value[10] = { '0', '2', '4', '2', '0', ' - 2','- 4', '- 2', '0', '\n' };

modbus_t* mb;
int ListenSocket = -1;
int ClientSocket = -1;
int iResult = 0;
int iSendResult = 0;

uint8_t recvbuf[DEFAULT_BUFLEN] = { 0 };
int recvbuflen = DEFAULT_BUFLEN;

int main()
{
	cout << "ECE 466" << endl;
	cout << "8080 Firmware Group" << endl;

	/* Creating Modbus TCP Context*/
	mb = modbus_new_tcp("192.168.1.144", 502);
	if (mb == NULL)
	{
		fprintf(stderr, "unable to allocate libmodbus context\n");
		return -1;
	}

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



	thread t1(communication);
	thread t2(printValue);

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

			// Echo the buffer back to the sender
			iSendResult = send_msg(mb, &recvbuf[0], iResult);
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

int printValue() {
	for (;;) {
		for (int i = 0; i < 10; i++) {
			iSendResult = send_msg(mb, (uint8_t*)&value[i], sizeof(uint8_t));
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
			this_thread::sleep_for(chrono::microseconds(10000));
		}
	}
}