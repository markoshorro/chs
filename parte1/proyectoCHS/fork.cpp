#include "fork.h"

void fork::replicar(){

	sc_uint<64> tmp;
	int i;

	while(true){
		for(i=0; i<10; ++i){
			entrada[i]->read( tmp );
			salida1[i]->write( tmp );
			salida2[i]->write( tmp );
		}
		wait(SC_ZERO_TIME);
	}

}