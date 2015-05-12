#ifndef PRODUCERCONSUMER_H
#define PRODUCERCONSUMER_H

#include"systemc.h"
#include"fifo.h"
#include<stdio.h>


template <class T, class O>
class productor : public sc_module
{
public:
sc_in<bool> clk, reset, leido;
sc_out< T >  entrada;
sc_out< bool > valid;

SC_HAS_PROCESS(productor);

productor( sc_module_name name_, char *fileName) : sc_module(name_)
{
	cout<<"productor: "<<name() << "  " << fileName << endl;
	fichero = fopen(fileName, "rb");
	LEN = sizeof( O );
	if( !fichero ){
		cerr << "No se puede abrir el fichero de entrada: " << fileName << endl;
		return;
	}
	SC_METHOD(main);
	sensitive << clk.pos();
}

 void main(){
	int n;
	T dato;
	O original;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&original);	// así puedo ver el dato original y su representación binaria

	if(reset.read()){
		if(!fichero)	return;
		fseek(fichero, 0, SEEK_SET);
		n = fread(&original, LEN, 1, fichero);
		dato = *tmp;
		entrada.write( dato );
		valid.write( n != 0 );
	}else{
		n=1;	// dummy
		if(leido.read()){
			n = fread(&original, LEN, 1, fichero);
			dato = *tmp;
			entrada.write( dato );
			valid.write( n != 0 );
		}
	}

	if(n == 0)
		n = n;	// el fichero ha terminado
 }

private:
	FILE *fichero;
	int LEN; 
};


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



template <class T, class O>
class consumidor : public sc_module
{
public:
sc_in<bool> clk, reset, valid;
sc_in< T >  resultado;

SC_HAS_PROCESS(consumidor);

consumidor( sc_module_name name_, char *fileName) : sc_module(name_)
{
	cout<<"consumidor: "<<name() << "  " << fileName << endl;
	fichero = fopen(fileName, "rb");
	LEN = sizeof( O );
	if( !fichero ){
		cerr << "No se puede abrir el fichero de resultados: " << fileName << endl;
		return;
	}
	SC_METHOD(main);
	sensitive << clk.pos();
}


 void main(){
	int n;
	T res;
	O original, calculado;
	unsigned long long *tmp;

	if(reset){
		if(!fichero)	return;
		fseek(fichero, 0, SEEK_SET);
		contOK = contBAD = 0;
		lastWasValid = false;
	}else{
		n=1;	// dummy
		if(valid.read()){
			res = resultado.read();
			tmp = (unsigned long long*) (&calculado);
			*tmp = res;
			n = fread(&original, LEN, 1, fichero);
			if(original != calculado)
				++contBAD;
			else
				++contOK;
		}else{
			if(lastWasValid){
				cout << name() << " " << contOK << " datos correctos recibidos. " << contBAD << " incorrectos @ " << sc_time_stamp() << endl;
				contOK = contBAD = 0;
			}
		}
		lastWasValid = valid.read();
	}
 }

private:
	FILE *fichero;
	int LEN; 
	int contOK, contBAD;
	bool lastWasValid;
};


#endif