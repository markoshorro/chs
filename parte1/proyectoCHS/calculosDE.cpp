#include"calculosDE.h"


void calculosDE::arreglarDatos(){

	sc_uint<64> tmp;

	while(true){

		X[0]->read( tmp );		Qx0a->write( tmp );		Qx0b->write( tmp );
		X[1]->read( tmp );		Qx1a->write( tmp );		Qx1b->write( tmp );
		X[5]->read( tmp );		Qx5a->write( tmp );		Qx5b->write( tmp );

		X[2]->read( tmp );		tmp.bit(63) = !tmp.bit(63);		Qx2->write( tmp );	// restar
		X[3]->read( tmp );		tmp.bit(63) = false;			Qx3->write( tmp );	// abso
		
		wait(SC_ZERO_TIME);
	}

}