#ifndef PRODUCERCONSUMER_H
#define PRODUCERCONSUMER_H

#include"systemc.h"
#include"fifo.h"
#include<stdio.h>


template <class T, class O>
class productor : public sc_module
{
public:
sc_port<write_if_T< T >>  entrada;

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
	SC_THREAD(main);
}

 void main(){
	int n;
	T dato;
	O original;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&original);	// así se puede ver el dato original y su representación binaria
	if(!fichero)	return;

	do{
		n = fread(&original, LEN, 1, fichero);		
		dato = *tmp;
		entrada->write( dato );
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


template <class T, class O>
class consumidor : public sc_module
{
public:
sc_port<read_if_T< T >>  resultado;

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
	SC_THREAD(main);
}


 void main(){
	int n;
	T res;
	O original, calculado;
	unsigned long long *tmp;

	tmp = (unsigned long long*) (&calculado);	// así se puede ver el dato original y su representación binaria
	if(!fichero)	return;

	do{
		n = fread(&original, LEN, 1, fichero);
		if(n){
				resultado->read( res );
				*tmp = res;
				if(original != calculado)
					n=n;		// poner punto de ruptura aqui para detectar desacuerdos
		}
		wait(SC_ZERO_TIME);
	}while(n);	

	fclose( fichero );
	cout << name() << " ha terminado de recibir resultados" << endl;

	sc_stop();

	// comentar la linea anterior (sc_stop) si se prefiere mantener la ejecución
	while(true)
		wait(SC_ZERO_TIME); 
 }

private:
	FILE *fichero;
	int LEN; 
};





#endif