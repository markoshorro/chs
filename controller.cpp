/*
 * Implementación funciones controller
 *
 * Universidade Da Coruña. 2015
 *
 * Marcos Horro
 * Christian Ponte
 */

#include "controller.h"

/*
 * Esta función realiza el proceso de generar las G poblaciones
 */
void controller::process()
{
	int i,j;
	
	init(); // primera iteración

	for(i=0;i<G;i++) {
		for(j=0;j<256;i++) {
		}
	}
}

/*
 * Esta función realiza el proceso de iniciar el conjunto
 */
void controller::init()
{
	INTRO->read(G); // leemos número de generaciones
	for(int i=0;i<10;i++) {
		INTRO->read(x[i]); // leemos valores iniciales
		wait(SC_ZERO_TIME);
	}
}


/*
 * Esta función realiza el proceso de enviar a la salida
 */
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