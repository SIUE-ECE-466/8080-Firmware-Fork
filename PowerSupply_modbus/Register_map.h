/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 */

#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

#include <stdint.h>

/* Constants declaration */
#define Registers_Len 42
#define Total_Registers 22

 /* Global Variables declaration */
uint16_t Register_Base[Registers_Len] = { 0 };

typedef struct {
	uint32_t Register_addr;
	uint16_t size;
	uint16_t* ptr;
} RegsterMap;

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

/* Register Mapping datastructure*/

RegsterMap Registers[Total_Registers] = {
	{400001, 1, (uint16_t*)(Register_Base + 0)},
	{400002, 1, (uint16_t*)(Register_Base + 2)},
	{400003, 2, (uint16_t*)(Register_Base + 4)},
	{400005, 2, (uint16_t*)(Register_Base + 8)},
	{400007, 2, (uint16_t*)(Register_Base + 12)},
	{400009, 2, (uint16_t*)(Register_Base + 16)},
	{400011, 2, (uint16_t*)(Register_Base + 20)},
	{400013, 2, (uint16_t*)(Register_Base + 24)},
	{400015, 2, (uint16_t*)(Register_Base + 28)},
	{400017, 2, (uint16_t*)(Register_Base + 32)},
	{400019, 2, (uint16_t*)(Register_Base + 36)},
	{400021, 2, (uint16_t*)(Register_Base + 40)},
	{400023, 2, (uint16_t*)(Register_Base + 44)},
	{400025, 2, (uint16_t*)(Register_Base + 48)},
	{400027, 2, (uint16_t*)(Register_Base + 52)},
	{400029, 2, (uint16_t*)(Register_Base + 56)},
	{400031, 2, (uint16_t*)(Register_Base + 60)},
	{400033, 2, (uint16_t*)(Register_Base + 64)},
	{400035, 2, (uint16_t*)(Register_Base + 68)},
	{400037, 2, (uint16_t*)(Register_Base + 72)},
	{400039, 2, (uint16_t*)(Register_Base + 76)},
	{400041, 2, (uint16_t*)(Register_Base + 80)},
};

#endif /* REGISTER_MAP_H */