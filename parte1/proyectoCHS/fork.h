#include "systemc.h"
#include "fifo.h"

SC_MODULE (fork) {
public:
	sc_port<read_if_T<sc_uint<64>>> entrada[10];
	sc_port<write_if_T<sc_uint<64>>> salida1[10];
	sc_port<write_if_T<sc_uint<64>>> salida2[10];

void replicar();
	SC_CTOR(fork) {
		cout<<"fork: "<<name()<<endl;
		SC_THREAD(replicar);
	}

};