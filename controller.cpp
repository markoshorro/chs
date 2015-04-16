/*
 * Implementación funciones controller
 *
 * Universidade Da Coruña. 2015
 *
 * Marcos Horro
 * Christian Ponte
 */

/*
 * NOTA: se han añadido periódicamente, en todos los bucles,
 *       la función wait(SC_ZERO_TIME) para intentar mejorar
 *       el funcionamiento de SystemC
 */

#include "controller.h"

/*
 * Funciona a modo de main, dado que delega la inicialización en
 * un método init, realiza las G generaciones y finaliza enviando
 * los datos por salida.
 */
void controller::process()
{
	sc_uint<8> i=0, j=0, k=0;
	bool cont; 
	sc_uint<8> addr1, addr2, addr3;
	sc_uint<10> mask;
	sc_uint<64> tmp;

	init(); // inicialización de datos

	for(i=0; i<G; i++) {
		for(j=0; j<256; j++) {

			// Pidiendo valores de las soluciones rand1 y rand2
			rand1->read( addr1 );		   rand2->read( addr2 );
			addrA->write( addr1 );        addrB->write( addr2 );
			wait(SC_ZERO_TIME);
			for(k=0; k<10; k++) { 
				fa[k]->read(A[k]);
				fb[k]->read(B[k]);
				wait(SC_ZERO_TIME);
			}
			fa[10]->read(A[10]); // vaciar undécimo resultado de dataA

			// Pidiendo valores de la solución original e indicada por rand3
			rand3->read( addr3 );
			addrA->write( (sc_uint<8>) j );        addrB->write( addr3 );
			wait(SC_ZERO_TIME);
			for(k=0; k<10; k++) {
				fa[k]->read(O[k]);
				fb[k]->read(C[k]); // vaciar undécimo resultado de dataA
				wait(SC_ZERO_TIME);
			}
			fa[10]->read(O[10]); // vaciar undécimo resultado de dataA

			// Leyendo máscara de hibridación
			hibridar->read( mask );
			for(k=0; k<10; k++) {
				if (mask.bit(k)) {
					tmp = B[k];
					tmp.bit(63) = !tmp.bit(63);
					r[k]->write(A[k]);
					s[k]->write(tmp);
					o[k]->write(C[k]);
				} else {
					tmp = A[k];
					tmp.bit(63) = !tmp.bit(63);
					r[k]->write(A[k]);
					s[k]->write(tmp);
					o[k]->write(O[k]);					
				}
				wait(SC_ZERO_TIME);
			}
		} 
		// 256 individuos
		cont = false;
		while(!cont) { listo->read(cont); }
		wait(SC_ZERO_TIME);
	}
	// G generaciones

	memToOut(); // sacamos por salida OUTRO tras G generaciones
}

/*
 * Esta función realiza el proceso de iniciar el conjunto
 */
void controller::init()
{
	int i,j;
	sc_uint<64> tmp;

	wait(SC_ZERO_TIME);
	INTRO->read(G); // leemos número de generaciones
	for(i=0;i<256;i++) {	
		for(j=0;j<10;j++) {
			INTRO->read(tmp); // leemos valores iniciales
			iniVal->write(tmp); // los mandamos a memoria
			wait(SC_ZERO_TIME);
		}
		INTRO->read(tmp);
		iniRes->write(tmp);
		wait(SC_ZERO_TIME);
	}
}


/*
 * Esta función realiza el proceso de enviar a la salida
 */
void controller::memToOut()
{
	int i,j;
	sc_uint<64> tmp;

	for(i=0; i<256; i++){
		addrA->write( (sc_uint<8>) i );        addrB->write( (sc_uint<8>) 0 );
		wait(SC_ZERO_TIME);
		for(j=0; j<11; j++){
			fa[j]->read( tmp );
			OUTRO->write( tmp );
			if(j<10){
				fb[j]->read( tmp );    // para vaciar esta cola...
			}
			wait(SC_ZERO_TIME);
		}
	}
}