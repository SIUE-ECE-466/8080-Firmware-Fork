/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 */

#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

#include <stdint.h>

/* Constants declaration */
#define Registers_Len 125

 /* Global Variables declaration */
uint16_t Register_Base[Registers_Len] = { 0 };

/* Registers */
uint16_t* MODE   = (uint16_t*)(Register_Base + 0);
uint16_t* FAULT  = (uint16_t*)(Register_Base + 2);
float* REALTIME  = (float*)(Register_Base + 4);
float* V0REF     = (float*)(Register_Base + 8);
float* TODAY     = (float*)(Register_Base + 12);
float* VX        = (float*)(Register_Base + 16);
float* V0        = (float*)(Register_Base + 20);
float* VSOC      = (float*)(Register_Base + 24);
float* IMAG      = (float*)(Register_Base + 28);
float* REAL      = (float*)(Register_Base + 32);
float* I0        = (float*)(Register_Base + 36);
float* IL        = (float*)(Register_Base + 40);
float* VC        = (float*)(Register_Base + 44);
float* KP        = (float*)(Register_Base + 48);
float* KI        = (float*)(Register_Base + 52);
float* KD        = (float*)(Register_Base + 56);
float* I0REF     = (float*)(Register_Base + 60);
float* RSOC      = (float*)(Register_Base + 64);
float* QRSOC     = (float*)(Register_Base + 68);
float* QVSOC     = (float*)(Register_Base + 72);
float* KA        = (float*)(Register_Base + 76);
float* KB        = (float*)(Register_Base + 80);


#endif /* REGISTER_MAP_H */