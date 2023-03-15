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
#include "Register_map.h"

extern "C" {
#include "../PowerSupply_modbus/libmodbus/modbus.h"
}

#define SOMAXCONN       0x7fffffff
#define DEFAULT_BUFLEN 300
#define My_slave_id 2

int registermap(uint8_t* registers, modbus_mapping_t* registerMap, uint8_t* recvbuf);
extern int Estimator();

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

std::chrono::time_point<std::chrono::system_clock> TimeZero, TimeNow;
std::chrono::duration<float> TimeDelta;
int64_t TIME = 0;
float TimeToSpice;

float RVCO = 1.0;   // synphase component of VCO oscillator
float QVCO = 0.0;   // quadrature component
float RRVCO = 1.0;  // history value for zero-crossing detection
float QQVCO = 0.0;  // history value

void InitiateContRegisters() {
	*MODE = 0;          // the state of the State Machine running the power converter
	*FAULT = 0;         // bits of FAULT indicate various conditions and are set to stop the converter 
	*Voltage = 0.0;     // averaged output voltage reading
	*Current = 0.0;     // averaged output current reading
	*I0 = 0.0;          // instantaneous output current reading
	*V0 = 0.0;          // instantaneous output voltage reading
	*OpModeReq = 0;     // request to change MODE (for example, turn the Constant Current charging)
	*Behavior = 0;		// alterations to the behavior of the State Machine
	*V0REF = 0.0;       // voltage setting, for the converter in the voltage control mode}
	*VX = 1.1;          // running values of PWM command translated into voltage
	*IMAG = 0.0;        // calculated estimate of control loop dominant pole imaginary part
	*REAL = 0.0;        // calculated estimate of control loop dominant pole real part
	*IL = 0.0;          // inductor current reading
	*VC = 0.3;          // capacitor voltage reading
	*KP = 0.01;			// current control loop proportional gain
	*KI = 0.01;			// current control loop integrative gain
	*KD = 0.0;          // current control loop derivative gain
	*REALTIME = 1.7;    // time in seconds (frozen HMI will show default 1.7 sec)
	*KPV = 0.02;        // voltage control loop proportional gain
	*KIV = 0.02;        // voltage control loop integrative gain
	*KDV = 0.0;         // voltage control loop derivative gain
	*I0REF = 0.0;       // current setpoint, for the converter in the current control mode
	*V0REF = 0.0;       // calculated estimate of battery internal resistance
	*QRSOC = 0.0;       // Relay open if 1 (based on relay coil monitor)
	*QVSOC = 0.0;       // PWM disabled if 1 (need to disable first and then enable if done the first time)
	*VCOFREQ = 0.2;		// frequency of the VCO oscillator
	*VCOMAG = 1.0;		// magnitude of the VCO oscillator
	*VZEROCAL = 0.0;	// voltage sensor offset calibration
	*VGAINCAL = 1.0;	// voltage sensor gain calibration
	*IZEROCAL = 0.0;	// current sensor offset calibration
	*IGAINCAL = 60.0;	// current sensor gain calibration
	*TRIGLEVEL = 99999.0;// oscilloscope trigger level
	*BITSTOGO = 1;		// various bits to trigger the oscilloscope
	*TIMEBASE = 0.1;    // oscilloscope time base (time between samples on screen) (will work down to 0.003 sec)
	*FANSPEED = 222;    // fan initially turned full on
	*TRSELA = 0;        // oscilloscope trace A selector
	*TRSELB = 1;        // oscilloscope trace B selector
	*PWMFLAG = 0;		// PWM disabled if the flag is set
}

void powerstage() {
	TimeZero = std::chrono::system_clock::now();
	*REALTIME = 0.0;
	TimeToSpice = *REALTIME + 0.00001;
	while (1) {
		TimeNow = std::chrono::system_clock::now();
		TimeDelta = TimeNow - TimeZero;
		*REALTIME = TimeDelta.count();
		if (*REALTIME < TimeToSpice) continue;
		TimeToSpice = *REALTIME + 0.00001;

		// Variable-frequency sinusoidal ripple generator
		RVCO -= *VCOFREQ * QVCO * 0.0000628319;
		QVCO += *VCOFREQ * RVCO * 0.0000628319;
		if (QQVCO <= 0 && QVCO >= 0) RVCO = *VCOMAG; else
			if (RRVCO >= 0 && RVCO <= 0) QVCO = *VCOMAG; else
				if (QQVCO > 0 && QVCO <= 0) RVCO = -*VCOMAG; else
					if (RRVCO < 0 && RVCO >= 0) QVCO = -*VCOMAG;
		RRVCO = RVCO; QQVCO = QVCO;

		*Voltage = 10 * RVCO;
		*Current = 150 * QVCO;
	}
}

int communication()
{
	// Receive until the peer shuts down the connection
	do {

		iResult = modbus_receive(mb, recvbuf);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
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
int registermap(uint8_t* registers, modbus_mapping_t* registerMap, uint8_t* recvbuf)
{
	int addr = (recvbuf[8] << 8) + recvbuf[9];
	int count = (recvbuf[10] << 8) + recvbuf[11];
	registerMap->start_registers = addr;
	registerMap->nb_registers = count;
	registerMap->tab_registers = registers;

	return 0;
}


int main()
{
	InitiateContRegisters();

	cout << "ECE 466" << endl;
	cout << "8080 Firmware Group" << endl;

	/* Creating Modbus TCP Context*/
	mb = modbus_new_tcp("127.0.0.1", 502);
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
	thread t2(powerstage);

	t1.join();
	t2.join();
	

	return 0;

}
