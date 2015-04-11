/*
 * Declaración de fpMult y fpAdd
 *
 * Universidade Da Coruña. 2015
 *
 */


#ifndef FP_PLANO_H
#define FP_PLANO_H

#include "systemc.h"
#include "fifo.h"



SC_MODULE (fpMult) {	
public:
sc_port<read_if_T<sc_uint<64>>>  op1, op2;
sc_port<write_if_T<sc_uint<64>>>  mult;

  void calculaMult();
  
	sc_uint<36> AF, BF, AE, BE, CE, AD, BD, CD;
	sc_uint<17> CF;
	sc_uint<19> xyF, xyE, xyD;
	sc_uint<19> Czt, Bzt, Azt;
	bool regSig1, regSig2; 
	sc_uint<11> regExp1, regExp2; 
	sc_uint<4> xyzt;
	sc_uint<22> finalReg;
	sc_uint<17> middleReg;
	sc_uint<16> ae_bd_f_Reg;

	sc_uint<64> res;

  SC_CTOR(fpMult) {
    cout<<"fpMult: "<<name()<<endl;
    SC_THREAD(calculaMult);

  } 

}; 


SC_MODULE (fpAdd) {	
public:
sc_port<read_if_T<sc_uint<64>>>  op1, op2;
sc_port<write_if_T<sc_uint<64>>>  suma;

  void calculaAdd();

	sc_uint<52> regNormal, regMovido;
	bool regPos1, regPos2;
	bool ceroA1, ceroB1, ceroA2, ceroB2;
	bool inf1, inf2;
	sc_uint<7> desp;
	bool regSig1, regSig2; 
	sc_uint<64> regSuma; 
	sc_uint<11> preExpo1, preExpo2;
	sc_uint<64> res;

  SC_CTOR(fpAdd) {
    cout<<"fpAdd: "<<name()<<endl;
    SC_THREAD(calculaAdd);

  } 

}; 




#endif
