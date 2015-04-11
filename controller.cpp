/*
 * Implementación funciones controller
 *
 * Universidade Da Coruña. 2015
 *
 * Marcos Horro
 * Christian Ponte
 */

// módulos y librerías externas
#include "controller.h"

void controller::memToOut()
{
	for(int i=0; i<256; ++i){
		addrA->write( (sc_uint<8>) i );        addrB->write( (sc_uint<8>) 0 );
		wait(SC_ZERO_TIME);
		for(int j=0; j<11; ++j){
			fa[j]->read( tmp );
			OUTRO->write( tmp );
			if(j<10){
				fb[j]->read( tmp );    // para vaciar esta cola...
			}
			wait(SC_ZERO_TIME);
		}
	}
}