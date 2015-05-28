#ifndef MOVERDATOS_H
#define MOVERDATOS_H

#include "systemc.h"
#include "FPsegmentado.h"

extern char* nombreIdx(char *cad, int idx);

SC_MODULE (checkearHibridos) {
public:
sc_in<bool> clk, reset;
sc_in<sc_uint<64>>  variables[10];
sc_in<bool> validIn[10];
sc_out<sc_uint<64>> corregidas[10];
sc_out<bool> validOut[10];

fpMinimo *min[10];
fpMaximo *max[10];

sc_signal<sc_uint<64>> sigMin[10], sigMax[10], sigCteMin[10], sigCteMax[10];
sc_signal<bool> valMin[10];


  SC_CTOR(checkearHibridos) {
	int i;
	double dMax[10] = { 2, 1, 2, 0, 2, 1, 2, 1, 2, 1 };			// VALORES MÁXIMOS PERMITIDOS
	double dMin[10] = { 0, -1, -2, -1, 0, 0, -2, -1, -1, 0 };	// VALORES MÍNIMOS PERMITIDOS
	unsigned long long *lMax, *lMin;
	sc_uint<64> sc_Max[10], sc_Min[10];
    cout<<"checkearHibridos: "<<name()<<endl;

	lMax = (unsigned long long*) dMax;
	lMin = (unsigned long long*) dMin;


	for(i=0; i<10; ++i){	sc_Max[i] = lMax[i];		sc_Min[i] = lMin[i];	cout << hex << sc_Max[i] << ";" << sc_Min[i] << endl; }

	for(i=0; i<10; ++i){
		min[i] = new fpMinimo(nombreIdx("minCheck", i));
		max[i] = new fpMaximo(nombreIdx("maxCheck", i));
		sigCteMin[i].write( sc_Min[i] );
		sigCteMax[i].write( sc_Max[i] );
	}

	for(i=0; i<10; ++i){
		min[i]->clk( clk );					min[i]->reset( reset );
		max[i]->clk( clk );					max[i]->reset( reset );
		min[i]->op1(sigCteMax[i]);			min[i]->op2(variables[i]);
		min[i]->val1(validIn[i]);			min[i]->val2(validIn[i]);
		min[i]->minimo(sigMin[i]);			min[i]->validOut(valMin[i]);
		max[i]->op1(sigCteMin[i]);			max[i]->op2(sigMin[i]);
		max[i]->val1(valMin[i]);			max[i]->val2(valMin[i]);
		max[i]->maximo(corregidas[i]);		max[i]->validOut(validOut[i]);
	}
  } 
}; 

SC_MODULE (hibridar) {
public:
sc_in<bool> clk, reset;
sc_in<sc_uint<64>>  r[10], s[10], o[10];
sc_in<bool> validRSO;
sc_out<sc_uint<64>> hibridas[10];
sc_out<bool> validH[10];

fpAdd *resta[10], *suma[10];	
fpMult *mult[10];
smallDelay_T<sc_uint<64>, 6> *retardos[10];	// LOS RETARDOS SON NECESARIOS PARA COMPENSAR QUE EL RESULTADO DE LA MULTIPLICACIÓN SALE VARIOS CICLOS MÁS TARDE DE HABER ENTRADO LOS VALORES ORIGINALES

// CUANTOS CICLOS DEBEN TENER LOS RETARDOS ANTERIORES PARA COMPENSAR LOS TIEMPOS DE LA SUMA Y LA MULTIPLICACIÓN ??

sc_signal<sc_uint<64>> sigResta[10], sigMult[10], delayedSumando[10], sig09;
sc_signal<bool> validResta[10], validMult[10], validDelay[10];


  SC_CTOR(hibridar) {
	int i;
	unsigned long long *pt;
	sc_uint<64> constante;
	double doble09 = 0.9;
    cout<<"hibridar: "<<name()<<endl;
    
	pt = (unsigned long long *) &doble09;
	constante = (sc_uint<64>) *pt;

	for(i=0; i<10; ++i){
		resta[i] = new fpAdd(nombreIdx("HibSub", i));
		suma[i] = new fpAdd(nombreIdx("HibAdd", i));
		mult[i] = new fpMult(nombreIdx("HibMult", i));
		retardos[i] = new smallDelay_T<sc_uint<64>, 6>(nombreIdx("delayHibridar", i));
	}

	sig09.write(constante);

	for(i=0; i<10; ++i) {
		resta[i]->clk( clk );		resta[i]->reset( reset );
		suma[i]->clk( clk );		suma[i]->reset( reset );
		mult[i]->clk( clk );		mult[i]->reset( reset );
		retardos[i]->clk( clk );	retardos[i]->reset( reset );
		retardos[i]->datoIn( o[i] );
		retardos[i]->datoOut( delayedSumando[i] );
		retardos[i]->validIn( validRSO );
		retardos[i]->validOut( validDelay[i] );

		resta[i]->op1( r[i] );
		resta[i]->val1( validRSO );
		resta[i]->op2( s[i] );
		resta[i]->val2( validRSO );
		resta[i]->suma( sigResta[i] );
		resta[i]->validOut( validResta[i] );

		mult[i]->op1( sigResta[i] );
		mult[i]->op2( sig09 );
		mult[i]->val1( validResta[i] );
		mult[i]->val2( validResta[i] );
		mult[i]->mult( sigMult[i] );
		mult[i]->validOut( validMult[i] );

		suma[i]->op1( sigMult[i] );
		suma[i]->op2( delayedSumando[i] );
		suma[i]->val1( validMult[i] );
		suma[i]->val2( validDelay[i] );
		suma[i]->suma( hibridas[i] );
		suma[i]->validOut( validH[i] );

		//////////////////////////////////////////
		// REALIZAR AQUÍ LAS CONEXIONES QUE FALTAN
		//////////////////////////////////////////

	}


  }

}; 



#endif