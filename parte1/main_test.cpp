#include "systemc.h"

#include "fifo.h"
#include "FPplano.h"
#include "fpAbs.h"
#include "producerConsumer.h"


class top : public sc_module
{
public:

fifo_T<sc_uint<64>> *Qsum1, *Qsum2, *Qmult, *Qsuma, *Qproducto, *Qresultado; // AÑADIR AQUÍ LAS COLAS QUE FALTAN

productor<sc_uint<64>, double> *productor1, *productor2, *productor3;
consumidor<sc_uint<64>, double> *resultadosSalida;

// Añadir aquí la declaración de los módulos sumador, multiplicador y absoluto
fpAbs *absoluto;
fpAdd *sumador;
fpMult *multiplicador;

SC_CTOR(top) // the module constructor
{
int i;

productor1 = new productor<sc_uint<64>, double>("productor_mult1", "sumando1.dat");
productor2 = new productor<sc_uint<64>, double>("productor_mult2", "sumando2.dat");
productor3 = new productor<sc_uint<64>, double>("productor_mult3", "multiplicador.dat");

resultadosSalida = new consumidor<sc_uint<64>, double> ("resultadosSalida", "resultadoAddMultAbs.dat");


// Instanciación de las colas
Qsum1 = new fifo_T<sc_uint<64>>("Qsum1", 1);
Qsum2 = new fifo_T<sc_uint<64>>("Qsum2", 1);
Qmult = new fifo_T<sc_uint<64>>("Qmult", 1);
Qsuma = new fifo_T<sc_uint<64>>("Qsuma", 1);
Qproducto = new fifo_T<sc_uint<64>>("Qproducto", 1);
Qresultado = new fifo_T<sc_uint<64>>("Qresultado", 1);


// Instanciar aquí los módulos sumador, multiplicador y absoluto
absoluto = new fpAbs("absoluto");
sumador = new fpAdd("sumador");
multiplicador = new fpMult("multiplicador");


productor1->entrada( *Qsum1 );
productor2->entrada( *Qsum2 );
productor3->entrada( *Qmult );

resultadosSalida->resultado( *Qresultado );

sumador->op1( *Qsum1 );
sumador->op2( *Qsum2 );
sumador->suma( *Qsuma );

// realizar aquí las conexiones que faltan
multiplicador->op1(*Qsuma);
multiplicador->op2(*Qmult);
multiplicador->mult(*Qproducto);

absoluto->op(*Qproducto);
absoluto->abs(*Qresultado);

}
};


int sc_main(int nargs, char* vargs[]){

	top principalDE("top");
	sc_start();

	return 0;

}
