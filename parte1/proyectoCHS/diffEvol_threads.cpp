/*
 *
 * MAIN Differential Evolution
 * Universidade da Coruña. 2015.
 * Itinerario Ingeniería de Computadores
 *
 */


#include "systemc.h"

#include "fifo.h"
#include "FPplano.h"
#include "memoria.h"
#include "hibridar.h"
#include "checkear_hibridos.h"
#include "controller.h"
#include "fork.h"
#include "calculosDE.h"
#include "producerConsumer.h"

char* nombreIdx(char *cad, int idx)
{
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

class top : public sc_module
{
	public:
		fifo_T<sc_uint<64>> *qINTRO, *qOUTRO;
		fifo_T<sc_uint<64>> *Qfa[11], *Qfb[10];
		fifo_T<sc_uint<64>> *Qr[10], *Qs[10], *Qo[10];
		fifo_T<sc_uint<64>> *QiniVal, *QiniRes;
		fifo_T<sc_uint<8>> *Qrand1, *Qrand2, *Qrand3;
		fifo_T<sc_uint<8>> *QaddrA, *QaddrB;
		fifo_T<sc_uint<10>> *Qhibridar;
		fifo_T<bool> *Qlisto;
		fifo_T<sc_uint<64>> *Qnuevo[11], *QparaCalcular[10];
		fifo_T<sc_uint<64>> *Qhibridas[10], *Qcorregidas[10];

		productor<sc_uint<64>, double> *datosEntrada;
		productor<sc_uint<8>, unsigned char> *random[3];
		productor<sc_uint<10>, unsigned short> *randomHibridar;
		consumidor<sc_uint<64>, double> *resultadosSalida;

		controller *instControl;
		bloquesMemoria *instMemoria;

		//Atención a las siguientes líneas: 

		hibridar *instHibridar;
		checkear_hibridos *instCheckearHibridos;
		fork *instFork;
		calculosDE * instCalculosDE;

		fpMult *multiplicador;
		fpAdd *sumador;

		SC_CTOR(top) // the module constructor
		{
			int i;
			char *ficherosRandom[3] = {"random8bits_1.dat", "random8bits_2.dat", "random8bits_3.dat"};

			datosEntrada = new productor<sc_uint<64>, double>("datosEntrada", "entradaDE.dat");
			for(i=0; i<3; ++i)	
				random[i] = new productor<sc_uint<8>, unsigned char>(nombreIdx("random", i), ficherosRandom[i]);
			randomHibridar = new productor<sc_uint<10>, unsigned short>("randomHibridar", "hibridar.dat" );

			resultadosSalida = new consumidor<sc_uint<64>, double> ("resultadosSalida", "salidaDE.dat");

			qINTRO = new fifo_T<sc_uint<64>>("qINTRO", 1);
			qOUTRO = new fifo_T<sc_uint<64>>("qOUTRO", 1);
			QiniVal = new fifo_T<sc_uint<64>>("QiniVal", 1);
			QiniRes = new fifo_T<sc_uint<64>>("QiniRes", 1);
			Qrand1 = new fifo_T<sc_uint<8>>("Qrand1", 1);
			Qrand2 = new fifo_T<sc_uint<8>>("Qrand2", 1);
			Qrand3 = new fifo_T<sc_uint<8>>("Qrand3", 1);
			QaddrA = new fifo_T<sc_uint<8>>("QaddrA", 1);
			QaddrB = new fifo_T<sc_uint<8>>("QaddrB", 1);
			Qhibridar = new fifo_T<sc_uint<10>>("Qhibridar", 1);
			Qlisto = new fifo_T<bool>("Qlisto", 1);
			for(i=0; i<10; ++i){
				Qfa[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qfa", i), 1);
				Qfb[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qfb", i), 1);
				Qr[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qr", i), 1);
				Qs[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qs", i), 1);
				Qo[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qo", i), 1);
				Qnuevo[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qnuevo", i), 1);
				Qhibridas[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qhibridas", i), 1);
				Qcorregidas[i] = new fifo_T<sc_uint<64>>(nombreIdx("Qcorregidas", i), 1);
				QparaCalcular[i] = new fifo_T<sc_uint<64>>(nombreIdx("QparaCalcular", i), 1);
			}
			Qfa[10] = new fifo_T<sc_uint<64>>(nombreIdx("Qfa", 10), 1);
			Qnuevo[10] = new fifo_T<sc_uint<64>>(nombreIdx("Qnuevo", 10), 1);

			instControl = new controller("instControl");
			instMemoria = new bloquesMemoria("instMemoria");
			instHibridar = new hibridar("hibridar");
			instCheckearHibridos = new checkear_hibridos("instCheckearHibridos");
			instCalculosDE = new calculosDE("instCalculosDE");

			instControl->INTRO( *qINTRO );		instControl->OUTRO( *qOUTRO );
			instControl->iniVal( *QiniVal );	instControl->iniRes( *QiniRes );
			instControl->rand1( *Qrand1 );		instControl->rand2( *Qrand2 );
			instControl->rand3( *Qrand3 );
			instControl->addrA( *QaddrA );		instControl->addrB( *QaddrB );
			instControl->listo( *Qlisto );		instControl->hibridar( *Qhibridar );

			for(i=0; i<10; ++i){
				instControl->fa[i]( *Qfa[i] );	instControl->fb[i]( *Qfb[i] );
				instControl->r[i]( *Qr[i] );	instControl->s[i]( *Qs[i] );
				instControl->o[i]( *Qo[i] );
			}
			instControl->fa[10]( *Qfa[10] );


			instMemoria->iniVal( *QiniVal );	instMemoria->iniRes( *QiniRes );
			instMemoria->addrA( *QaddrA );		instMemoria->addrB( *QaddrB );
			instMemoria->listo( *Qlisto );

			for(i=0; i<10; ++i){
				instMemoria->dataA[i]( *Qfa[i] );	instMemoria->dataB[i]( *Qfb[i] );
				instMemoria->nuevo[i]( *Qnuevo[i] );
			}
			instMemoria->dataA[10]( *Qfa[10] );
			instMemoria->nuevo[10]( *Qnuevo[10] );


			for(i=0; i<10; ++i){
				instHibridar->r[i]( *Qr[i] );	instHibridar->s[i]( *Qs[i] );
				instHibridar->o[i]( *Qo[i] );	instHibridar->hibridas[i]( *Qhibridas[i] );
			}

			for(i=0; i<10; ++i){
				instCheckearHibridos->variables[i]( *Qhibridas[i] );
				instCheckearHibridos->corregidas[i]( *Qcorregidas[i] );
			}

			instFork = new fork("fork");
			for(i=0; i<10; ++i){
				instFork->entrada[i]( *Qcorregidas[i] );
				instFork->salida1[i]( *Qnuevo[i] );
				instFork->salida2[i]( *QparaCalcular[i] );
			}

			///////////////////////////////////////////////////////
			//EL MÓDULO DE CÁLCULOS NO ESTÁ CONECTADO, HACERLO AQUÍ
			///////////////////////////////////////////////////////
			for(i=0; i<10; ++i){
				instCalculosDE->X[i]( *QparaCalcular[i] );
			}
			instCalculosDE->resultado( *Qnuevo[10] );

			//Atención: si queremos inspeccionar todo lo que entra en una cola, podemos usar los métodos setInspect y resetInspect
			//Qnuevo[10]->setInspect();

			datosEntrada->entrada(*qINTRO);

			random[0]->entrada( *Qrand1 );		random[1]->entrada( *Qrand2 );	random[2]->entrada( *Qrand3 );
			randomHibridar->entrada( *Qhibridar );
			resultadosSalida->resultado( *qOUTRO );
		} // CTOR
};


int sc_main(int nargs, char* vargs[]){

	top principalDE("top");
	sc_start();

	return 0;

}
