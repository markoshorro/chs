#ifndef PRODUCERCONSUMERMULTIPLE_H
#define PRODUCERCONSUMERMULTIPLE_H

#include"systemc.h"
#include"fifo.h"
#include<stdio.h>

template <class T, class O, unsigned short N>
class productorMultiple : public sc_module
{
public:
sc_in<bool> clk, reset, leido;
sc_out< T >  entrada[N];
sc_out< bool > valid;

SC_HAS_PROCESS(productorMultiple);

productorMultiple( sc_module_name name_, char *fileName) : sc_module(name_)
{
	cout<<"productorMultiple: "<<name() << "  " << fileName << endl;
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
	int n, i;
	T dato;
	O original;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&original);	// así puedo ver el dato original y su representación binaria

	if(reset.read()){
		if(!fichero)	return;
		fseek(fichero, 0, SEEK_SET);
		for(i=0; i<N; ++i){
			n = fread(&original, LEN, 1, fichero);
			dato = *tmp;
			entrada[i].write( dato );
		}
		valid.write( n != 0 );
		cont = 0;
	}else{
		if(leido.read()){
			++cont;
			for(i=0; i<N; ++i){
				n = fread(&original, LEN, 1, fichero);
				dato = *tmp;
				entrada[i].write( dato );
			}
			valid.write( n != 0 );
		}
	}
 }

private:
	FILE *fichero;
	int LEN; 
	int cont;
};



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////




template <class T, class O, unsigned short N>
class consumidorMultiple : public sc_module
{
public:
sc_in<bool> clk, reset, valid;
sc_in< T >  resultado[N];

SC_HAS_PROCESS(consumidorMultiple);

consumidorMultiple( sc_module_name name_, char *fileName) : sc_module(name_)
{
	cout<<"consumidorMultiple: "<<name() << "  " << fileName << endl;
	fichero = fopen(fileName, "rb");
	LEN = sizeof( O );
	if( !fichero ){
		cerr << "No se puede abrir el fichero de resultados: " << fileName << endl;
		return;
	}
	SC_METHOD(main);
	sensitive << clk.pos();
	cont = 0;
}


 void main(){
	int n, i;
	T res;
	O original, calculado;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&calculado);	// así puedo ver el dato original y su representación binaria

	if(reset){
		if(!fichero)	return;
		fseek(fichero, 0, SEEK_SET);
		cont = 0;
	}else{
		n=1;	// dummy
		if(valid.read()){
			for(i=0; i<N; ++i){
				res = resultado[i].read();
				tmp = (unsigned long long*) (&calculado);
				*tmp = res;
				n = fread(&original, LEN, 1, fichero);
				if(original != calculado)
					cerr << name() << " diferentes " << original << " y " << calculado << " Serie e indice: " << cont << " , " << i << endl;
			}
			++cont;
		}
	}

 }

private:
	FILE *fichero;
	int LEN; 
	int cont;
};




#endif