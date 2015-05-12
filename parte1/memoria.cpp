#include "memoria.h"

void bloquesMemoria::lecturas(){

	int i, a, b;
	sc_uint<8> dirA, dirB;

	while(true){
		addrA->read(dirA);		addrB->read(dirB);
		a = dirA.to_int();		b = dirB.to_int();

		for(i=0; i<10; ++i){
			dataA[i]->write( mem[a][i][ banco[a][queLista] ] );
			dataB[i]->write( mem[b][i][ banco[b][queLista] ] );
		}
		
		dataA[10]->write( mem[a][10][ banco[a][queLista] ] ); // resultado, sólo se usa al terminar
		// esta lectura habrá que hacerla por el puerto B del block RAM?

		wait(SC_ZERO_TIME);
	}
}


void bloquesMemoria::escrituras(){

	int i, j;
	sc_uint<64> tmp;

	for(i=0; i<256; ++i){	// introduce los datos originales de entrada
		for(j=0; j<10; ++j){
			iniVal->read( tmp );
			mem[i][j][ 0 ] = tmp;
			wait(SC_ZERO_TIME);
		}
	}

	while(true){
		for(i=0; i<10; ++i){
			nuevo[i]->read( tmp );
			if((contadorH == 0xd2) && (i==3) )
				i=i;
			mem[contadorH][i][ !banco[contadorH][queLista] ] = tmp;	// OJO						
		}
		contadorH++;
		wait(SC_ZERO_TIME);
	}
}


void bloquesMemoria::checkeoResultado(){

	int i;
	sc_uint<64> tmp, ref, Scal, Sref;
	unsigned long long leido;
	bool esMenor;

	for(i=0; i<256; ++i){	// introduce los resultados precalculados para los datos originales
		iniRes->read( tmp );
		mem[i][10][ 0 ] = tmp;
	}

	while(true){
		ref = mem[contadorR][10][ banco[contadorR][queLista] ]; // el resultado con el que toca comparar
		nuevo[10]->read( tmp );	// lee el resultado
		
		fread(&leido, 8, 1, resultadosFile);
		if(tmp.to_uint64() != leido)
			leido = leido;

		if(contadorR == 0xd2)
				i=i;

		Scal = tmp;		Scal.bit(63) = ref.bit(63);
		Sref = ref;		Sref.bit(63) = tmp.bit(63);

		esMenor = Scal < Sref;
		esMenor ^= ( (Scal.bit(63) == true) && (Sref.bit(63) == true) );

		if( esMenor ){	// el híbrido mejora al anterior
			mem[contadorR][10][ !banco[contadorR][queLista] ] = tmp;
			banco[contadorR][!queLista] = !banco[contadorR][queLista];
		}else{
			banco[contadorR][!queLista] = banco[contadorR][queLista];
		}
		if(contadorR == 255){	 
			listo->write( true );
			queLista = !queLista;
		}

		contadorR++;
		wait(SC_ZERO_TIME);
	}
}

