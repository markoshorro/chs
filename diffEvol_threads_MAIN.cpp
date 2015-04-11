#include "systemc.h"

#include "fifo.h"
#include "FPplano.h"
#include "controller.h"
#include "producerConsumer.h"
#include "producerConsumerMultiple.h"



char* nombreIdx(char *cad, int idx){
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

class top : public sc_module
{
public:
fifo_T<sc_uint<64>> // DECLARA AQUÍ LAS COLAS;

// TANTOS PRODUCTORES Y CONSUMIDORES COMO SEAN NECESARIOS
productor<  ??  >, ?? > *NOMBRE;
consumidor<  ??  >, ?? > *NOMBRE;

productorMultiple<  ??  >, ??, ?? > *NOMBRE;
consumidorMultiple<  ??  >, ??, ?? > *NOMBRE;


controller *instTU_MODULO;



SC_CTOR(top) // the module constructor
{

/* INSTANCIAR AQUÍ:

	COLAS
	PRODUCTORES
	CONSUMIDORES

	TU_MODULO
*/


// CONECTAR AQUÍ LOS PRODUCTORES Y CONSUMIDORES CON TU_MODULO UTILIZANDO LAS COLAS


}
};


int sc_main(int nargs, char* vargs[])
{
	top principalDE("top");
	sc_start();

	return 0;
}
