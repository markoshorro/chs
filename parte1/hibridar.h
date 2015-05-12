// módulos y librerías externas
#include "systemc.h"
#include "FPplano.h"
#include "fifo.h"

extern char* nombreIdx(char *cad, int idx);

SC_MODULE (hibridar) {
public:
	sc_port<read_if_T<sc_uint<64>>> r[10], s[10], o[10];
	sc_port<write_if_T<sc_uint<64>>> hibridas[10];
	
	SC_CTOR (hibridar) {
		cout<<"Hibridar: "<<name()<<endl;

		for(int i=0; i<10; i++){
			Qcte[i] = new constSrc_T<sc_uint<64>>(nombreIdx("Qcte_",i), 0x3feccccccccccccd);
			suma[i] = new fifo_T<sc_uint<64>>(nombreIdx("suma_",i), 1);
			mult[i] = new fifo_T<sc_uint<64>>(nombreIdx("mult_",i), 1);
			sumador[i] = new fpAdd(nombreIdx("sumador_",i));
			sumador[i+10] = new fpAdd(nombreIdx("sumador_",i+10));
			multiplicador[i] = new fpMult(nombreIdx("multiplicador_",i));
		}

		for(int i=0; i<10; i++){
			sumador[i]->op1(r[i]);
			sumador[i]->op2(s[i]);
			sumador[i]->suma(*suma[i]);

			multiplicador[i]->op1(*suma[i]);
			multiplicador[i]->op2(*Qcte[i]);
			multiplicador[i]->mult(*mult[i]);

			sumador[i+10]->op1(*mult[i]);
			sumador[i+10]->op2(o[i]);
			sumador[i+10]->suma(hibridas[i]);
		}
		//SC_THREAD();
	}
	
private:

	fifo_T<sc_uint<64>> *suma[10], *mult[10];
	constSrc_T<sc_uint<64>> *Qcte[10];

	fpAdd *sumador[20];
	fpMult *multiplicador[10];
};