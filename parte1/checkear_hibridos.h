// módulos y librerías externas
#include "systemc.h"
#include "FPplano.h"
#include "fifo.h"

extern char* nombreIdx(char *cad, int idx);

SC_MODULE (checkear_hibridos) {
public:
	sc_port<read_if_T<sc_uint<64>>> variables[10];
	sc_port<write_if_T<sc_uint<64>>> corregidas[10];
	
	SC_CTOR (checkear_hibridos) {
		cout<<"Check Hibridos: "<<name()<<endl;

		sc_uint<64> vectorMin[] = {0x0000000000000000, 0xbff0000000000000, 0xc000000000000000, 0xbff0000000000000, 0x0000000000000000, 0x0000000000000000, 0xc000000000000000, 0xbff0000000000000, 0xbff0000000000000, 0x0000000000000000};
		sc_uint<64> vectorMax[] = {0x4000000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x0000000000000000, 0x4000000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x3ff0000000000000, 0x4000000000000000, 0x3ff0000000000000};


		for(int i=0; i<10; i++){
			minimos[i] = new fifo_T<sc_uint<64>>(nombreIdx("minimos_",i), 1);
			minimo[i] = new fpMinimo(nombreIdx("minimo_",i));
			maximo[i] = new fpMaximo(nombreIdx("maximo_",i+10));
			CteMin[i] = new constSrc_T<sc_uint<64>>(nombreIdx("CteMin_",i), vectorMin[i]);
			CteMax[i] = new constSrc_T<sc_uint<64>>(nombreIdx("CteMax_",i), vectorMax[i]);
		}

		for(int i=0; i<10; i++){			
			minimo[i]->op1(*CteMax[i]);
			minimo[i]->op2(variables[i]);
			minimo[i]->minimo(*minimos[i]);

			maximo[i]->op1(*CteMin[i]);
			maximo[i]->op2(*minimos[i]);
			maximo[i]->maximo(corregidas[i]);
		
		}
		//SC_THREAD();
	}
	
private:

	fifo_T<sc_uint<64>> *minimos[10];
	constSrc_T<sc_uint<64>> *CteMin[10], *CteMax[10];

	fpMinimo *minimo[10];
	fpMaximo *maximo[10];
};