/*
 * Main controlador.
 *
 * Universidade Da Coruña. 2015
 *
 * Marcos Horro
 * Christian Ponte
 */

// módulos y librerías externas
#include "systemc.h"
#include "fifo.h"


SC_MODULE (controller) {
  SC_CTOR (controller) {
	fifo_T<sc_uint<64>> *Qfa[11], *Qfb[10];
	fifo_T<sc_uint<64>> *Qr[10], *Qs[10], *Qo[10];
 
	for(int i=0; i<10; ++i){
		Qfa[i] = new fifo_T<sc_uint<64>>(nombreId"Qfa", i), 1);
		Qfb[i] = new fifo_T<sc_uint<64>>(nombreId"Qfb", i), 1);
		Qr[i] = new fifo_T<sc_uint<64>>(nombreId"Qr", i), 1);
		Qs[i] = new fifo_T<sc_uint<64>>(nombreId"Qs", i), 1);
		Qo[i] = new fifo_T<sc_uint<64>>(nombreId"Qo", i), 1);
	}
	Qfa[10] = new fifo_T<sc_uint<64>>(nombreId"Qfa", 10), 1);
  }

};