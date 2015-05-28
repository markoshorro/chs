#include "systemc.h"

#include "FPsegmentado.h"
#include "producerConsumerMultiple.h"
#include "producerConsumer.h"
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
	
sc_signal<sc_uint<64>> sigX[10], sigRes;
sc_signal<bool> leido, validX, valRes;

productorMultiple<sc_uint<64>, double, 10> *datosX;
consumidor<sc_uint<64>, double> *resultados;

calculosDE *instCalculosDE;

SC_CTOR(top) // the module constructor
{
	int i;

	datosX = new productorMultiple<sc_uint<64>, double, 10>("datoS", "benchCheck_fileCorregidas.dat");
	resultados = new consumidor<sc_uint<64>, double>("resultados", "benchCalcular_fileResultado.dat");

	instCalculosDE = new calculosDE("calculosDE");
	
	datosX->clk(clk);	
	datosX->reset(reset);	
	datosX->leido(leido);	
	datosX->valid(validX);	
	for(i=0; i<10; ++i)			
		datosX->entrada[i]( sigX[i] );


	instCalculosDE->clk(clk);
	instCalculosDE->reset(reset);
	instCalculosDE->resultado( sigRes );
	instCalculosDE->valRes( valRes );
	for(i=0; i<10; ++i){
		//////////////////////////////////////////
		// REALIZAR AQUÍ LAS CONEXIONES QUE FALTAN
		//////////////////////////////////////////

		instCalculosDE->X[i]( sigX[i] );
		instCalculosDE->valX[i]( validX );
	}

	resultados->clk(clk);
	resultados->reset(reset);
	resultados->valid(valRes);
	resultados->resultado(sigRes);



	leido.write( true );	// solo válido para probar módulos sueltos

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
