/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 */

#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

#include <stdint.h>

/* Constants declaration */
#define Registers_Len 1024

 /* Global Variables declaration */
uint8_t Register_Base[Registers_Len] = { 0 };

/* Registers */
#define CPU1TOCMMSGRAM1_BASE Register_Base
uint16_t* MODE = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 0);
uint16_t* FAULT = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 2);
float* Current = (float*)(CPU1TOCMMSGRAM1_BASE + 4);
float* Voltage = (float*)(CPU1TOCMMSGRAM1_BASE + 8);
float* I0 = (float*)(CPU1TOCMMSGRAM1_BASE + 12);
float* V0 = (float*)(CPU1TOCMMSGRAM1_BASE + 16);
uint16_t* OpModeReq = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 20);
uint16_t* Behavior = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 22);
float* I0REF = (float*)(CPU1TOCMMSGRAM1_BASE + 24);
float* QVSOC = (float*)(CPU1TOCMMSGRAM1_BASE + 28);
float* V0REF = (float*)(CPU1TOCMMSGRAM1_BASE + 32);
float* IMAG = (float*)(CPU1TOCMMSGRAM1_BASE + 36);
float* IL = (float*)(CPU1TOCMMSGRAM1_BASE + 40);
float* VC = (float*)(CPU1TOCMMSGRAM1_BASE + 44);
float* REAL = (float*)(CPU1TOCMMSGRAM1_BASE + 48);
float* VX = (float*)(CPU1TOCMMSGRAM1_BASE + 52);
float* KP = (float*)(CPU1TOCMMSGRAM1_BASE + 56);
float* KI = (float*)(CPU1TOCMMSGRAM1_BASE + 60);
float* KD = (float*)(CPU1TOCMMSGRAM1_BASE + 64);
float* QRSOC = (float*)(CPU1TOCMMSGRAM1_BASE + 68);
float* REALTIME = (float*)(CPU1TOCMMSGRAM1_BASE + 72);
float* KPV = (float*)(CPU1TOCMMSGRAM1_BASE + 76);
float* KIV = (float*)(CPU1TOCMMSGRAM1_BASE + 80);
float* KDV = (float*)(CPU1TOCMMSGRAM1_BASE + 84);
float* VCOFREQ = (float*)(CPU1TOCMMSGRAM1_BASE + 88);
float* VCOMAG = (float*)(CPU1TOCMMSGRAM1_BASE + 92);
float* VZEROCAL = (float*)(CPU1TOCMMSGRAM1_BASE + 96);
float* VGAINCAL = (float*)(CPU1TOCMMSGRAM1_BASE + 100);
float* IZEROCAL = (float*)(CPU1TOCMMSGRAM1_BASE + 104);
float* IGAINCAL = (float*)(CPU1TOCMMSGRAM1_BASE + 108);
float* TRIGLEVEL = (float*)(CPU1TOCMMSGRAM1_BASE + 112);
float* TIMEBASE = (float*)(CPU1TOCMMSGRAM1_BASE + 116);
uint16_t* BITSTOGO = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 120);
uint16_t* FANSPEED = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 122);
uint16_t* TRSELA = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 124);
uint16_t* TRSELB = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 126);
uint16_t* TRSELEN = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 128);
uint16_t* PWMFLAG = (uint16_t*)(CPU1TOCMMSGRAM1_BASE + 130);


#endif /* REGISTER_MAP_H */