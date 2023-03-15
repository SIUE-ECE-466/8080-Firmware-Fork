/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

extern uint16_t* MODE;
extern uint16_t* FAULT;
extern float* Current;
extern float* Voltage;
extern float* I0;
extern float* V0;
extern uint16_t* OpModeReq;
extern uint16_t* Behavior;
extern float* I0REF;
extern float* QVSOC;
extern float* V0REF;
extern float* IMAG;
extern float* IL;
extern float* VC;
extern float* REAL;
extern float* VX;
extern float* KP;
extern float* KI;
extern float* KD;
extern float* QRSOC;
extern float* REALTIME;
extern float* KPV;
extern float* KIV;
extern float* KDV;
extern float* VCOFREQ;
extern float* VCOMAG;
extern float* VZEROCAL;
extern float* VGAINCAL;
extern float* IZEROCAL;
extern float* IGAINCAL;
extern float* TRIGLEVEL;
extern float* TIMEBASE;
extern uint16_t* BITSTOGO;
extern uint16_t* FANSPEED;
extern uint16_t* TRSELA;
extern uint16_t* TRSELB;
extern uint16_t* TRSELEN;
extern uint16_t* PWMFLAG;

#endif /* REGISTERS_H */