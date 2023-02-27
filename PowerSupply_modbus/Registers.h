/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

extern uint16_t* MODE;
extern uint16_t* FAULT;
extern float* REALTIME;
extern float* V0REF;
extern float* TODAY;
extern float* VX;
extern float* V0;
extern float* VSOC;
extern float* IMAG;
extern float* REAL;
extern float* I0;
extern float* IL;
extern float* VC;
extern float* KP;
extern float* KI;
extern float* KD;
extern float* I0REF;
extern float* RSOC;
extern float* QRSOC;
extern float* QVSOC;
extern float* KA;
extern float* KB;

#endif /* REGISTERS_H */