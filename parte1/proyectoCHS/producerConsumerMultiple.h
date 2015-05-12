#ifndef PRODUCERCONSUMERMULTIPLE_H
#define PRODUCERCONSUMERMULTIPLE_H

#include"systemc.h"
#include"fifo.h"
#include<stdio.h>

template <class T, class O, unsigned short N>
class productorMultiple : public sc_module
{
public:
sc_port<write_if_T< T >>  entrada[N];

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
	SC_THREAD(main);
}


 void main(){
	int n, i;
	T dato;
	O original;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&original);	// así puedo ver el dato original y su representación binaria
	if(!fichero)	return;

	do{
		for(i=0; i<N; ++i){
			n = fread(&original, LEN, 1, fichero);
			dato = *tmp;
			entrada[i]->write( dato );
		}
		wait(SC_ZERO_TIME);
	}while(n);

	fclose( fichero );
	while(true)
		wait(SC_ZERO_TIME); 
 }

private:
	FILE *fichero;
	int LEN; 
};



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////




template <class T, class O, unsigned short N>
class consumidorMultiple : public sc_module
{
public:
sc_port<read_if_T< T >>  resultado[N];

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
	SC_THREAD(main);
}


 void main(){
	int n, i;
	T res;
	O original, calculado;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&calculado);	// así puedo ver el dato original y su representación binaria
	if(!fichero)	return;

	do{
		for(i=0; i<N; ++i){
			n = fread(&original, LEN, 1, fichero);
			if(n){
					resultado[i]->read( res );
					*tmp = res;
					if(original != calculado)
						n=n;
			}
		}
		wait(SC_ZERO_TIME);
	}while(n);

	fclose( fichero );
	//sc_stop();

	// borrar la línea anterior si se desea que la simulación continue
	while(true)
		wait(SC_ZERO_TIME); 
 }

private:
	FILE *fichero;
	int LEN; 
};




#endif