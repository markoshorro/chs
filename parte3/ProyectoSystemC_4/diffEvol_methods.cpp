#include "systemc.h"

#include "FPsegmentado.h"
#include "producerConsumerMultiple.h"
#include "moverDatos.h"
#include "moverDatos.h"


char* nombreIdx(char *cad, int idx){
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

class top : public sc_module
{
public:
sc_in<bool> clk, reset;
	
sc_signal<sc_uint<64>> sigR[10], sigS[10], sigO[10], sigHib[10], sigCheck[10];
sc_signal<bool> validR, validS, validO, valHib[10], valCheck[10], leido;

productorMultiple<sc_uint<64>, double, 10> *datoR, *datoS, *datoO;
consumidorMultiple<sc_uint<64>, double, 10> *resultadosCorregidas;

hibridar *instHib;
checkearHibridos *instCheck;

SC_CTOR(top) // the module constructor
{
	int i;

	datoR = new productorMultiple<sc_uint<64>, double, 10>("datoR", "bench100/benchHib_fileR.dat");
	datoS = new productorMultiple<sc_uint<64>, double, 10>("datoS", "bench100/benchHib_fileS.dat");
	datoO = new productorMultiple<sc_uint<64>, double, 10>("datoO", "bench100/benchHib_fileO.dat");

	resultadosCorregidas = new consumidorMultiple<sc_uint<64>, double, 10> ("resultadosCorregidas", "bench100/benchCheck_fileCorregidas.dat");

	instHib = new hibridar("hibridar");
	instCheck = new checkearHibridos("check");
	

	datoR->clk(clk);	datoR->reset(reset);	datoR->leido(leido);	datoR->valid(validR);	for(i=0; i<10; ++i)			datoR->entrada[i]( sigR[i] );	
	datoS->clk(clk);	datoS->reset(reset);	datoS->leido(leido);	datoS->valid(validS);	for(i=0; i<10; ++i)			datoS->entrada[i]( sigS[i] );
	datoO->clk(clk);	datoO->reset(reset);	datoO->leido(leido);	datoO->valid(validO);	for(i=0; i<10; ++i)			datoO->entrada[i]( sigO[i] );


	resultadosCorregidas->clk(clk);
	resultadosCorregidas->reset(reset);
	resultadosCorregidas->valid(valCheck[0]);
	for(i=0; i<10; ++i)	
		resultadosCorregidas->resultado[i]( sigCheck[i] );


	leido.write( true );	// solo válido para probar módulos sueltos

	instHib->clk(clk);
	instHib->reset(reset);
	instHib->validRSO( validR ); // sirve uno cualquiera: R, S u O
	for(i=0; i<10; ++i){
		//////////////////////////////////////////
		// REALIZAR AQUÍ LAS CONEXIONES QUE FALTAN
		//////////////////////////////////////////

	}

	instCheck->clk(clk);
	instCheck->reset(reset);
	for(i=0; i<10; ++i){
		//////////////////////////////////////////
		// REALIZAR AQUÍ LAS CONEXIONES QUE FALTAN
		//////////////////////////////////////////

	}



}

};


int sc_main(int nargs, char* vargs[]){

	sc_clock clk ("clk", 10, SC_NS);	// ciclo de 10 ns
	sc_signal <bool> res;

	top principalDE("top");
	principalDE.clk(clk);
	principalDE.reset(res);

	res.write(true);         sc_start(20, SC_NS);
	res.write(false);        sc_start(2000000, SC_NS);

	cout << endl << endl << "Programa finalizado" << endl << endl;
	return 0;

}
