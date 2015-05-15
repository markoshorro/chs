#ifndef FPSEGMENTADO_H
#define FPSEGMENTADO_H

#include "systemc.h"
#include "fifo.h"

SC_MODULE (fpMinimo) {
public:
 	sc_in < bool > clk, reset, val1, val2;

	sc_in < sc_uint <64>> op1, op2;
	sc_out < sc_uint <64>> minimo;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	sc_uint<64> sc1, sc2;
	bool valid;

  void calculaMinimo(){
	  bool expMenor, expIgual, mantMenor, preMenor, esMenor;

	  expMenor = sc1(62, 52) < sc2(62, 52);
	  expIgual = sc1(62, 52) == sc2(62, 52);
	  mantMenor = sc1(51, 0) < sc2(51, 0);
	  preMenor = expMenor || (mantMenor && expIgual);
	  esMenor = (sc1.bit(63) != sc2.bit(63)) ? sc1.bit(63) : (sc1.bit(63) ^ preMenor);
	  minimo.write( esMenor ? sc1 : sc2);
	  validOut.write(valid);
  }

	void registros(){
		bool vv;
		if(reset.read()){
			valid = false;
			sc1 = sc2 = 0;
		}else{
			valid = val1.read();
			vv = val2.read(); 
			if(vv != valid )
				cout << name() << "Valid no coincide " << valid << vv << endl;

			sc1 = op1.read();	sc2 = op2.read();	  
		}
		fireRegs = !fireRegs;
  }


  SC_CTOR(fpMinimo) {
    cout<<"fpMinimo: "<<name()<<endl;
	fireRegs = true;
	SC_METHOD(calculaMinimo);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();		
  } 

}; 



SC_MODULE (fpMaximo) {
public:
 	sc_in < bool > clk, reset, val1, val2;

	sc_in < sc_uint <64>> op1, op2;
	sc_out < sc_uint <64>> maximo;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	sc_uint<64> sc1, sc2;
	bool valid;

  void calculaMaximo(){
	  bool expMenor, expIgual, mantMenor, preMenor, esMenor;

	  expMenor = sc1(62, 52) < sc2(62, 52);
	  expIgual = sc1(62, 52) == sc2(62, 52);
	  mantMenor = sc1(51, 0) < sc2(51, 0);
	  preMenor = expMenor || (mantMenor && expIgual);
	  esMenor = (sc1.bit(63) != sc2.bit(63)) ? sc1.bit(63) : (sc1.bit(63) ^ preMenor);
	  maximo.write( esMenor ? sc2 : sc1);
	  validOut.write(valid);
  }

	void registros(){
		bool vv;
		if(reset.read()){
			valid = false;
			sc1 = sc2 = 0;
		}else{
			valid = val1.read();
			vv = val2.read(); 
			if(vv != valid )
				cout << name() << "Valid no coincide " << valid << vv << endl;
		
			sc1 = op1.read();	sc2 = op2.read();	  
			if(vv)
				vv=vv;
		}
		fireRegs = !fireRegs;
  }


  SC_CTOR(fpMaximo) {
    cout<<"fpMaximo: "<<name()<<endl;
	fireRegs = true;
	SC_METHOD(calculaMaximo);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();		
  } 

}; 





SC_MODULE (fpMult) {		// segmentado en 3 etapas
public:
 	sc_in < bool > clk, reset, val1, val2;

	sc_in < sc_uint <64>> op1, op2;
	sc_out < sc_uint <64>> mult;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	sc_uint<64> sc1, sc2;
	bool valid;
	bool PRINT;

	void calculaMult();
	void registros();

	sc_uint<36> AF, BF, AE, BE, CE, AD, BD, CD;
	sc_uint<17> CF;
	sc_uint<19> xyF, xyE, xyD;
	sc_uint<19> Czt, Bzt, Azt;
	bool regSig1, regSig2, regSig3; 
	bool valid1, valid2, valid3;
	sc_uint<11> regExp1, regExp2, regExp3; 
	sc_uint<4> xyzt;
	sc_uint<22> finalReg;
	sc_uint<17> middleReg;
	sc_uint<16> ae_bd_f_Reg;
	sc_uint<64> res;

	sc_uint<36> r_AF, r_BF, r_AE, r_BE, r_CE, r_AD, r_BD, r_CD;
	sc_uint<17> r_CF;
	sc_uint<19> r_xyF, r_xyE, r_xyD;
	sc_uint<19> r_Czt, r_Bzt, r_Azt;
	sc_uint<4> r_xyzt;
	sc_uint<22> r_finalReg;
	sc_uint<17> r_middleReg;
	sc_uint<16> r_ae_bd_f_Reg;
	sc_uint<64> r_res;


  SC_CTOR(fpMult) {
    cout<<"fpMult: "<<name()<<endl;

	fireRegs = true;
	SC_METHOD(calculaMult);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();
	PRINT = false;
  } 

}; 


SC_MODULE (fpAdd) {		// segmentado en 3 etapas
public:
 	sc_in < bool > clk, reset, val1, val2;

	sc_in < sc_uint <64>> op1, op2;
	sc_out < sc_uint <64>> suma;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	sc_uint<64> sc1, sc2;
	bool valid;
	bool PRINT;

	void calculaAdd();
	void registros();

	sc_uint<52> regNormal, regMovido;
	bool valid1, valid2, valid3;
	bool regPos1, regPos2;
	bool r_ceroA1, r_ceroB1;	
	bool r_inf1, r_inf2;
	sc_uint<7> des;
	bool regSig1, regSig2; 
	sc_uint<64> regSuma; 
	sc_uint<11> r_preExpo1, r_preExpo2;

	sc_uint<52> Normal, Movido;
	bool Pos1;
	bool ceroA1, ceroB1, ceroA2, ceroB2;	
	bool inf1;
	sc_uint<7> desp;
	bool Sig1; 
	sc_uint<64> Suma; 
	sc_uint<11> preExpo1;


  SC_CTOR(fpAdd) {
    cout<<"fpAdd: "<<name()<<endl;

	fireRegs = true;
	SC_METHOD(calculaAdd);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();
	PRINT = false;
  } 

}; 


SC_MODULE (fpSquare) {		// segmentado en 3 etapas
public:
 	sc_in < bool > clk, reset, validIn;
	sc_in < sc_uint <64>> op;
	sc_out < sc_uint <64>> square;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	sc_uint<64> sc1;
	bool PRINT;

	void calculaSquare();
	void registros();

	sc_uint<4> r_xy2;
	sc_uint<36> r_A2, r_B2, r_AB, r_BC, r_AC;
	sc_uint<19> r_xA, r_xB, r_xC;
	sc_uint<17> r_C2;

	sc_uint<4> xy2;
	sc_uint<36> A2, B2, AB, BC, AC;
	sc_uint<19> xA, xB, xC;
	sc_uint<17> C2;

	sc_uint<36> ab_f;
	sc_uint<16> ab_f_Reg;
	sc_uint<22> final, finalReg;
	sc_uint<36> middle, middleReg;

	bool valid1, valid2, valid3;
	sc_uint<11> preExp1, regExp1, regExp2; 


  SC_CTOR(fpSquare) {
    cout<<"fpSquare: "<<name()<<endl;

	fireRegs = true;
	SC_METHOD(calculaSquare);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();
	PRINT = false;
  } 

}; 



SC_MODULE (fpDesplaza1) {
public:
 	sc_in < bool > clk, reset, validIn;
	sc_in < sc_uint <64>> op;
	sc_out < sc_uint <64>> desplaz;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	bool valid;
	sc_uint<64> sc1;

	void calculaDesplazamiento1(){
		sc_uint<64> scR;
		sc_int<13> exp;

		exp = sc1(62,52);	exp -= 1;
		if(exp<=0)
			desplaz.write( 0 );
		else{
			scR = sc1;		scR(62,52) = exp(10, 0);
			desplaz.write( scR );		
		}
		validOut.write(valid);
	}

	void registros(){
		if(reset.read()){
			sc1 = 0;			valid = false;		
		}else{
			sc1 = op.read();	valid = validIn.read();
		}
		fireRegs = !fireRegs;
	}

  SC_CTOR(fpDesplaza1) {
    cout<<"fpDesplaza1: "<<name()<<endl;
	fireRegs = true;
	SC_METHOD(calculaDesplazamiento1);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();		
  } 
}; 




SC_MODULE (fpDesplaza2) {
public:
 	sc_in < bool > clk, reset, validIn;
	sc_in < sc_uint <64>> op;
	sc_out < sc_uint <64>> desplaz;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	bool valid;
	sc_uint<64> sc1;

	void calculaDesplazamiento2(){
		sc_uint<64> scR;
		sc_int<13> exp;

		exp = sc1(62,52);	exp -= 2;
		if(exp<=0)
			desplaz.write( 0 );
		else{
			scR = sc1;		scR(62,52) = exp(10, 0);
			desplaz.write( scR );		
		}
		validOut.write(valid);
	}

	void registros(){
		if(reset.read()){
			sc1 = 0;			valid = false;		
		}else{
			sc1 = op.read();	valid = validIn.read();
		}
		fireRegs = !fireRegs;
	}

  SC_CTOR(fpDesplaza2) {
    cout<<"fpDesplaza2: "<<name()<<endl;
	fireRegs = true;
	SC_METHOD(calculaDesplazamiento2);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();		
  } 
}; 



SC_MODULE (fpDesplaza4_neg) {
public:
 	sc_in < bool > clk, reset, validIn;
	sc_in < sc_uint <64>> op;
	sc_out < sc_uint <64>> desplaz;
	sc_out <bool> validOut;

	sc_signal<bool> fireRegs;
	bool valid;
	sc_uint<64> sc1;

	void calculaDesplazamiento4(){
		sc_uint<64> scR;
		sc_int<13> exp;

		exp = sc1(62,52);	exp -= 4;
		if(exp<=0)
			desplaz.write( 0 );
		else{
			scR = sc1;		
			scR.bit(63) = !sc1.bit(63);
			scR(62,52) = exp(10, 0);
			desplaz.write( scR );		
		}
		validOut.write(valid);
	}

	void registros(){
		if(reset.read()){
			sc1 = 0;			valid = false;		
		}else{
			sc1 = op.read();	valid = validIn.read();
		}
		fireRegs = !fireRegs;
	}

  SC_CTOR(fpDesplaza4_neg) {
    cout<<"fpDesplaza4_neg: "<<name()<<endl;
	fireRegs = true;
	SC_METHOD(calculaDesplazamiento4);
		sensitive << fireRegs;
	SC_METHOD(registros);
		sensitive << clk.pos();		
  } 
}; 




#endif