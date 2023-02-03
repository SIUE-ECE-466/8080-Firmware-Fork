/*
 * This project handles the charge and discharge of power supply
 * Firmware code for the power supply 
 */

#include <iostream>
extern "C" {
#include <modbus.h>
}

#define SOMAXCONN       0x7fffffff
#define DEFAULT_BUFLEN 8

using namespace std;

int main()
{
	cout << "ECE 466" << endl;
	cout << "8080 Firmware Group" << endl;

	modbus_t* mb;
	int ListenSocket = -1;
	int ClientSocket = -1;
	int iResult = 0;
	int iSendResult = 0;

	uint8_t recvbuf[DEFAULT_BUFLEN] = { 0 };
	int recvbuflen = DEFAULT_BUFLEN;
	
	/* Creating Modbus TCP Context*/
	mb = modbus_new_tcp("192.168.1.145", 502);
	if (mb == NULL)
	{
		fprintf(stderr, "unable to allocate libmodbus context\n");
		return -1;
	}

	/* Listen for Modbus TCP master/client connection request*/
	ListenSocket = modbus_tcp_listen(mb, SOMAXCONN);

	if (ListenSocket == -1){
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

	/*if (modbus_set_debug(mb, 1) != 0) {
		printf(" Debug is not set\n");
	}*/

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

}