#ifndef MEMORIA_H
#define MEMORIA_H

#include "systemc.h"
#include "FPplano.h"
#include "fifo.h"


SC_MODULE (bloquesMemoria) {
public:
sc_port<write_if_T<sc_uint<64>>>  dataA[11], dataB[10];
sc_port<write_if_T< bool >> listo; 
sc_port<read_if_T<sc_uint<64>>>  nuevo[11];
sc_port<read_if_T<sc_uint<64>>>  iniVal, iniRes;
sc_port<read_if_T<sc_uint<8>>>  addrA, addrB;

  void lecturas();
  void escrituras();
  void checkeoResultado();

  FILE *resultadosFile;

  SC_CTOR(bloquesMemoria) {
	int i, j;
	double tmp;
	long long unsigned *pt;

	resultadosFile = fopen("resultados.dat", "rb");
	if(!resultadosFile)
		cerr << "Error abriendo resultados.dat" << endl;

	pt = (long long unsigned*) (&tmp);

	for(i=0; i<256; ++i){
		for(j=0; j<11; ++j){
			mem[i][j][0] = 0; //(sc_uint<64>) *pt;
			mem[i][j][1] = 0;	// por poner...
		}
		banco[i][0] = banco[i][1] = false;	// incialmente, todas las soluciones están en el primer banco
		queLista = 0;
	}

	contadorH = contadorR = 0;

    cout<<"bloquesMemoria: "<<name()<<endl;
    SC_THREAD(lecturas);
	SC_THREAD(escrituras);
	SC_THREAD(checkeoResultado);
  }

private:
  bool banco[256][2], queLista;
  //sc_uint<8> 
   unsigned char contadorH, contadorR;
  sc_uint<64> mem[256][11][2];
}; 

#endif
