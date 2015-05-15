#ifndef FIFO_H
#define FIFO_H

#include"systemc.h"
#include<stdio.h>



template <class T, int mm>
class smallFIFO : public sc_module
{
public:
sc_in<bool> clk, reset, leerIn, escrIn;
sc_out< T >  datoIn;
sc_out< T >  datoOut;
sc_out< bool > noEmpty, full;

	void gestionCola(){

		leer = leerIn.read();	escr = escrIn.read();	dato = datoIn.read();
		newNumElem = numElem;
		dataOut.write( elementos [0] );
		noEmpty.write(numElem != 0);
		full.write(numElem == max);
		if(leer){
			if(numElem==0) { cerr << "Cola vacía: " << name() << endl; exit(-1); }
			--newEnumElem;
			for(i=0; i<(numElem); ++i) newElementos[i] = elementos[i+1];
		}else
			for(i=0; i<(numElem); ++i) newElementos[i] = elementos[i];
		if (escr){
			if(newNumElem==max ) { cerr << "Cola llena: " << name() << endl; exit(-1); }
			newElementos[newEnumElem++ ] = dato;
		}
	}

	void registros(){
		if(reset)		numElem= 0;
		else{
			for(i=0; i<(numElem); ++i){ elementos[i] = newElementos[i];}
		}
		fireRegs.write( !fireRegs.read() );
	}

	SC_CTOR(smallFIFO) {
		max = mm;
		numElem = 0;
		fireRegs.write( false);
		//elementos = new T[mm];
		//newElementos = new T[mm];
		SC_METHOD(gestionCola);
		sensitive << read << write << dataIn << fireRegs;
		SC_METHOD(registros);
		sensitive << clk.pos();
	}
private:
	int max;
	bool leer, escr;
	int numElem, newNumElem;
	T dato, elementos[mm], newElementos[mm];
	sc_signal<bool> fireRegs; // me simplifica la codificación
};



template <class T, int mm>
class smallDelay_T : public sc_module
{
public:
sc_in<bool> clk, reset, validIn;
sc_in< T >  datoIn;
sc_out< T >  datoOut;
sc_out< bool > validOut;

	void registros(){
		int i;
		if(reset){
			for(i=0; i<mm; ++i){
				val[i] = false;
				elementos[i] = 0;
			}
		}else{
			for(i=(mm-1); i>0; --i){ 
				val[i] = val[i-1];
				elementos[i] = elementos[i-1];				
			}
			val[0] = validIn.read();
			//if(val[0])				cout << name() << " OK @ " << sc_time_stamp() << endl;
			elementos[0] = datoIn.read();
		}			
		fireRegs.write( !fireRegs.read() );
	}

	void salida(){
		validOut.write(val[mm-1]);
		//if(val[mm-1])			cout << name() << " GO @ " << sc_time_stamp() << endl;
		datoOut.write(elementos[mm-1]);
	}

	SC_CTOR(smallDelay_T) {
		SC_METHOD(registros);
		sensitive << clk.pos();
		SC_METHOD(salida);
		sensitive << fireRegs;
		fireRegs.write(true);
	}
private:
	T elementos[mm];
	bool val[mm];
	sc_signal<bool> fireRegs;
};




#endif