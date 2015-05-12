#ifndef FP_PLANO_H
#define FP_PLANO_H

#include "systemc.h"
#include "fifo.h"

SC_MODULE (fpMinimo) {
public:
sc_port<read_if_T<sc_uint<64>>>  op1, op2;
sc_port<write_if_T<sc_uint<64>>>  minimo;

  void calculaMinimo(){
	  bool expMenor, expIgual, mantMenor, preMenor, esMenor;
	  sc_uint<64> sc1, sc2;

	  while(true){
		  op1->read(sc1);	op2->read(sc2);
		  expMenor = sc1(62, 52) < sc2(62, 52);
		  expIgual = sc1(62, 52) == sc2(62, 52);
		  mantMenor = sc1(51, 0) < sc2(51, 0);
		  preMenor = expMenor || (mantMenor && expIgual);
		  esMenor = (sc1.bit(63) != sc2.bit(63)) ? sc1.bit(63) : (sc1.bit(63) ^ preMenor);
		  minimo->write( esMenor ? sc1 : sc2);
		  wait(SC_ZERO_TIME);
	  }
  }
  SC_CTOR(fpMinimo) {
    cout<<"fpMinimo: "<<name()<<endl;
    SC_THREAD(calculaMinimo);

  } 
}; 


SC_MODULE (fpMenor) {
public:
sc_port<read_if_T<sc_uint<64>>>  op1, op2;
sc_port<write_if_T<bool>>  menor;	// false: op1 es menor	true: op2 es menor

  void calculaMinimo(){
	  bool expMenor, expIgual, mantMenor, preMenor, esMenor;
	  sc_uint<64> sc1, sc2;

	  while(true){
		  op1->read(sc1);	op2->read(sc2);
		  expMenor = sc1(62, 52) < sc2(62, 52);
		  expIgual = sc1(62, 52) == sc2(62, 52);
		  mantMenor = sc1(51, 0) < sc2(51, 0);
		  preMenor = expMenor || (mantMenor && expIgual);
		  esMenor = (sc1.bit(63) != sc2.bit(63)) ? sc1.bit(63) : (sc1.bit(63) ^ preMenor);
		  menor->write( !esMenor );
		  wait(SC_ZERO_TIME);
	  }
  }
  SC_CTOR(fpMenor) {
    cout<<"fpMenor: "<<name()<<endl;
    SC_THREAD(calculaMinimo);

  } 
}; 




SC_MODULE (fpMaximo) {
public:
sc_port<read_if_T<sc_uint<64>>>  op1, op2;
sc_port<write_if_T<sc_uint<64>>>  maximo;

  void calculaMaximo(){
	  bool expMenor, expIgual, mantMenor, preMenor, esMenor;
	  sc_uint<64> sc1, sc2;

	  while(true){
		  op1->read(sc1);	op2->read(sc2);
		  expMenor = sc1(62, 52) < sc2(62, 52);
		  expIgual = sc1(62, 52) == sc2(62, 52);
		  mantMenor = sc1(51, 0) < sc2(51, 0);
		  preMenor = expMenor || (mantMenor && expIgual);
		  esMenor = (sc1.bit(63) != sc2.bit(63)) ? sc1.bit(63) : (sc1.bit(63) ^ preMenor);
		  maximo->write( esMenor ? sc2 : sc1);
		  wait(SC_ZERO_TIME);
	  }
  }
  SC_CTOR(fpMaximo) {
    cout<<"fpMaximo: "<<name()<<endl;
    SC_THREAD(calculaMaximo);

  } 
}; 

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


SC_MODULE (fpSquare) {
public:
sc_port<read_if_T<sc_uint<64>>>  op;
sc_port<write_if_T<sc_uint<64>>>  square;

  void calculaSquare();


	sc_uint<4> xy2;
	sc_uint<36> A2, B2, AB, BC, AC;
	sc_uint<19> xA, xB, xC;
	sc_uint<17> C2;

	sc_uint<16> ab_f_Reg;
	sc_uint<22> finalReg;
	sc_uint<36> middleReg;
	sc_uint<11> regExp1, regExp2; 

	sc_uint<64> res;


  SC_CTOR(fpSquare) {
    cout<<"fpSquare: "<<name()<<endl;
    SC_THREAD(calculaSquare);

  } 

}; 



SC_MODULE (fpDesplaza1) {
public:
sc_port<read_if_T<sc_uint<64>>>  op;
sc_port<write_if_T<sc_uint<64>>>  desplaz;

void calculaDesplazamiento1(){
	sc_uint<64> sc1, scR;
	sc_int<13> exp;
	while(true){
		op->read(sc1);
		exp = sc1(62,52);
		exp -= 1;
		if(exp<=0)
			desplaz->write( 0 );
		else{
			scR = sc1;
			scR(62,52) = exp(10, 0);

			desplaz->write( scR );
		}
		wait(SC_ZERO_TIME);
	}
}
  SC_CTOR(fpDesplaza1) {
    cout<<"fpDesplaza1: "<<name()<<endl;
    SC_THREAD(calculaDesplazamiento1);
  } 
}; 



SC_MODULE (fpDesplaza2) {
public:
sc_port<read_if_T<sc_uint<64>>>  op;
sc_port<write_if_T<sc_uint<64>>>  desplaz;

void calculaDesplazamiento2(){
	sc_uint<64> sc1, scR;
	sc_int<13> exp;
	while(true){
		op->read(sc1);
		exp = sc1(62,52);
		exp -= 2;
		if(exp<=0)
			desplaz->write( 0 );
		else{
			scR = sc1;
			scR(62,52) = exp(10, 0);

			desplaz->write( scR );
		}
		wait(SC_ZERO_TIME);
	}
}
  SC_CTOR(fpDesplaza2) {
    cout<<"fpDesplaza2: "<<name()<<endl;
    SC_THREAD(calculaDesplazamiento2);
  } 
}; 


SC_MODULE (fpDesplaza4_neg) {
public:
sc_port<read_if_T<sc_uint<64>>>  op;
sc_port<write_if_T<sc_uint<64>>>  desplaz;

void calculaDesplazamiento4(){
	sc_uint<64> sc1, scR;
	sc_int<13> exp;
	while(true){
		op->read(sc1);
		exp = sc1(62,52);
		exp -= 4;
		if(exp<=0)
			desplaz->write( 0 );
		else{
			scR = sc1;
			scR.bit(63) = !sc1.bit(63);	// aquí es donde se cambia el signo
			scR(62,52) = exp(10, 0);

			desplaz->write( scR );
		}
		wait(SC_ZERO_TIME);
	}
}
  SC_CTOR(fpDesplaza4_neg) {
    cout<<"fpDesplaza4_neg: "<<name()<<endl;
    SC_THREAD(calculaDesplazamiento4);
  } 
}; 


SC_MODULE (fpGenConst) {
public:
sc_port<write_if_T<sc_uint<64>>>  constante;

void produceConstante(){

	while(true){
		constante->write( constant );
		wait(SC_ZERO_TIME);
	}
}


SC_HAS_PROCESS(fpGenConst);

  fpGenConst(sc_module_name name_, double const_=64) : sc_module(name_)
  {
	unsigned long long *pt;
	pt = (unsigned long long *) &const_;
	constant = (sc_uint<64>) *pt;

    cout<<"fpGenConst: "<<name()<<endl;
    SC_THREAD(produceConstante);
  }

private:
  sc_uint<64> constant;
}; 


#endif
