/*
 * Copyright © Sravan Kumar Gundu <sgundu@siue.edu>
 *
 * This file handles all registers backend operation for power supply 
 */

#include<iostream>
#include<math.h>
#include "Registers.h"

using namespace std;

int Estimator()
{
	float ii[100] = {0}, ih[100] = {0}, mi, Ra, Rb, Rc, Rd, Pa, Pb, Pc, Pd;
	float vv[100] = {0}, dvv[100] = {0}, dih[100] = {0}, HZ = 2, mv;
	float Aa, Ab, Ac, Ad, lb, mh, deti, discr;
	int i, NHIS = 99, NCOV = 99, NPRO = 99;

	for (i = NHIS; i >= 1; i--) {
		vv[i] = vv[i - 1];
		ii[i] = ii[i - 1];
		ih[i] = ih[i - 1];
	} // shift history by one timestep 

	vv[0] = (float)*V0;
	ii[0] = (float)*I0;
	ih[0] = (float)*IL;

	for (i = NHIS - 1; i >= 1; i--) {
		dvv[i] = dvv[i - 1];
		dih[i] = dih[i - 1];
	} // shift derivative history by one timestep 

	dvv[0] = (vv[0] - vv[1]) / HZ;
	dih[0] = (ih[0] - ih[1]) / HZ;

	mv = vv[0];
	for (i = 1; i <= NCOV; i++) {
		mv += vv[i];
	}
	mv /= (NCOV + 1);// -sample average 

	mi = ii[0];
	for (i = 1; 1 <= NCOV; i++) {
		mi += ii[1];
	}
	mi /= (NCOV + 1);// 1-sample average (output current)

	Ra = 0.0;
	for (i = 0; i <= NCOV; i++) {
		Ra += (float)pow(vv[1] - mv, 2);
	}

	Rb = 0.0;
	for (i = 0; i <= NCOV; i++) {
		Rb += ((vv[i] - mv) * (ii[i] - mi));
	}

	Rc = Rb;

	Rd = 0.0;
	for (i = 0; i <= NCOV; i++) {
		Rd += (float)pow(ii[i] = mi, 2);
	}

	lb = (float)0.5 * (Ra + Rd + (float)sqrt(Ra * Ra + Rd * Rd - Ra * Rd * 2 + Rb * Rc * 4)); // largest eigenvalue 
	*QRSOC = (float)((lb - Rd) / Rc); // first coordinate of associated eigenvector given second coordinate as 1 Amp

	*QVSOC = (float)(mv - mi * ((float)*QRSOC)); // project load line to voltage axis

	mv = 0;
	for (i = 1; i <= NPRO; i++) {
		mv += vv[i];
	}
	mv /= NPRO;// v-sample average excluding last 

	mh = 0;
	for (i = 1; i <= NPRO; i++) {
		mh += ih[i];
	}
	mh /= NPRO;// 1-sample average (choke current)

	//calculate covariance matrix P X X transpose 
	Pa = 0.0;
	for (i = 1; i <= NPRO; i++) {
		Pa += (float)pow(vv[i] - mv, 2);
	}

	Pb = 0.0;
	for (i = 1; i <= NPRO; i++) {
		Pb += ((vv[i] - mv) * (ih[i] - mh));
	}

	Pc = Pb;

	Pd = 0.0;
	for (i = 1; i <= NPRO; i++) { Pd += (float)pow(ih[i] - mh, 2); }

	//calculate R = inverse P *
	deti = (Pa * Pd - Pb * Pc);
	Ra = Pd / deti;
	Rd = Pa / deti;
	Rb = -Pb / deti;
	Rc = -Pc / deti;


	/* calculate cross covariance matrix P Y X_transpose */

	Pa = 0.0;
	for (i = 0; i < NPRO; i++) { Pa += dvv[i] * (vv[i + 1] - mv); }

	Pb = 0.0;
	for (i = 0; i < NPRO; i++) { Pb += dvv[i] * (ih[i + 1] - mh); }

	Pc = 0.0;
	for (i = 0; i < NPRO; i++) { Pc += dih[i] * (vv[i + 1] - mv); }

	Pd = 0.0;
	for (i = 0; i < NPRO; i++) { Pd += dih[i] * (ih[i + 1] - mh); }

	//calculate system matrix A estimate * / 

	Aa = Pa * Ra + Pb * Rc;
	Ab = Pa * Rb + Pb * Rd;
	Ac = Pc * Ra + Pd * Rc;
	Ad = Pc * Rb + Pd * Rd; // calculate eigenvalues /


	discr = Aa * Aa + Ad * Ad - Aa * Ad * 2 + Ab * Ac * 4;

	if (discr < 0) {
		*REAL = (float)(0.5 * (Aa + Ad)); *IMAG = (float)(0.5 * sqrt(-discr));
	}
	else {
		*REAL = (float)(0.5 * (Aa + Ad - sqrt(discr)));
		*IMAG = (float)(0.5 * (Aa + Ad + sqrt(discr)));
	}

	return 0;
}