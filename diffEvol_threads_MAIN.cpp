/*
 * Main module (top)
 *
 * Universidade Da Coruña. 2015
 *
 * Marcos Horro
 * Christian Ponte
 */

#include "systemc.h"

#include "fifo.h"
#include "FPplano.h"
#include "controller.h"
#include "producerConsumer.h"
#include "producerConsumerMultiple.h"

char* nombreIdx(char *cad, int idx)
{
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

class top : public sc_module
{
	public:
		// colas
		fifo_T<sc_uint<64>> *Qfa[11], *Qfb[10];
		fifo_T<sc_uint<64>> *Qr[10], *Qs[10], *Qo[10];

		// TANTOS PRODUCTORES Y CONSUMIDORES COMO SEAN NECESARIOS
		//productor<  ??  >, ?? > *NOMBRE;
		//consumidor<  ??  >, ?? > *NOMBRE;

		//productorMultiple<  ??  >, ??, ?? > *NOMBRE;
		//consumidorMultiple<  ??  >, ??, ?? > *NOMBRE;


		controller *instController;

		SC_CTOR(top) // the module constructor
		{
			// instancias
			instController = new controller("microcontroller");
			for(int i=0; i<10; ++i){
				Qfa[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qfa", i), 1);
				Qfb[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qfb", i), 1);
				Qr[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qr", i), 1);
				Qs[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qs", i), 1);
				Qo[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qo", i), 1);
			}
			Qfa[10] = new fifo_T<sc_uint<64>>(nombreIdx("Qfa", 10), 1);
			/* INSTANCIAR AQUÍ:
				COLAS
				PRODUCTORES
				CONSUMIDORES
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
