/*
 * Práctica de Codiseño Hardware Software
 *
 * Universidade Da Coruña. 2015
 *
 */

#include "systemc.h"

#include "FPsegmentado.h"
#include "producerConsumerMultiple.h"
#include "producerConsumer.h"
#include "moverDatos.h"
#include "calculosDE.h"


char* nombreIdx(char *cad, int idx){
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

class top : public sc_module
{
public:
sc_in<bool> clk, reset;

// Hibridar
sc_signal<sc_uint<64>> sigR[10], sigS[10], sigO[10], sigHib[10], sigCheck[10];
sc_signal<bool> validR, validS, validO, valHib[10], valCheck[10], leido;

productorMultiple<sc_uint<64>, double, 10> *datoR, *datoS, *datoO;
consumidorMultiple<sc_uint<64>, double, 10> *resultadosCorregidas;

hibridar *instHib;
checkearHibridos *instCheck;

// Calculos DE
sc_signal<sc_uint<64>> sigRes;
sc_signal<bool> valRes;
consumidor<sc_uint<64>, double> *resultados;
calculosDE *instCalculosDE;

SC_CTOR(top) // the module constructor
{
	int i;

	// Produce los datos al módulo de hibridación
	datoR = new productorMultiple<sc_uint<64>, double, 10>("datoR", "benchHib_fileR.dat");
	datoS = new productorMultiple<sc_uint<64>, double, 10>("datoS", "benchHib_fileS.dat");
	datoO = new productorMultiple<sc_uint<64>, double, 10>("datoO", "benchHib_fileO.dat");

	// Consumidor de los datos de CalculaDE
	resultados = new consumidor<sc_uint<64>, double>("resultados", "benchCalcular_fileResultado.dat");

	// Instancias de los módulos
	instHib = new hibridar("hibridar");
	instCheck = new checkearHibridos("check");	
	instCalculosDE = new calculosDE("calculosDE");

	datoR->clk(clk);	datoR->reset(reset);	datoR->leido(leido);	datoR->valid(validR);	for(i=0; i<10; ++i)			datoR->entrada[i]( sigR[i] );	
	datoS->clk(clk);	datoS->reset(reset);	datoS->leido(leido);	datoS->valid(validS);	for(i=0; i<10; ++i)			datoS->entrada[i]( sigS[i] );
	datoO->clk(clk);	datoO->reset(reset);	datoO->leido(leido);	datoO->valid(validO);	for(i=0; i<10; ++i)			datoO->entrada[i]( sigO[i] );

	leido.write(true); // necesario para que el productor avance

	instHib->clk(clk);
	instHib->reset(reset);
	instHib->validRSO( validR ); // sirve uno cualquiera: R, S u O
	for(i=0; i<10; ++i){
		// IN
		instHib->r[i]( sigR[i] );
		instHib->s[i]( sigS[i] );
		instHib->o[i]( sigO[i] );
		// OUT
		instHib->hibridas[i]( sigHib[i] );
		instHib->validH[i]( valHib[i] );
	}

	instCheck->clk(clk);
	instCheck->reset(reset);
	for(i=0; i<10; ++i){
		// IN
		instCheck->variables[i]( sigHib[i] );
		instCheck->validIn[i]( valHib[i] );
		// OUT
		instCheck->corregidas[i]( sigCheck[i] );
		instCheck->validOut[i]( valCheck[i] );
	}

	// Se conecta el módulo de CalculosDE con el de checkeo y con el consumer
	instCalculosDE->clk( clk );
	instCalculosDE->reset( reset );
	instCalculosDE->resultado( sigRes );
	instCalculosDE->valRes( valRes );
	for(i=0; i<10; ++i) {
		instCalculosDE->X[i]( sigCheck[i] );
		instCalculosDE->valX[i]( valCheck[i] );
	}
	
	// Conexiones del consumidor
	resultados->clk(clk);
	resultados->reset(reset);
	resultados->valid(valRes);
	resultados->resultado(sigRes);
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
