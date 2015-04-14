/*
 * Declaración de controller
 *
 * Universidade Da Coruña. 2015
 *
 * Marcos Horro
 * Christian Ponte
 */

// módulos y librerías externas
#include "systemc.h"
#include "fifo.h"

SC_MODULE (controller) {
public:
	// declaración de puertos: read y write
	sc_port<read_if_T<sc_uint<64>>> INTRO;
	sc_port<read_if_T<sc_uint<8>>> rand1, rand2, rand3;
	sc_port<read_if_T<sc_uint<16>>> hibridar;
	sc_port<read_if_T<bool>> listo;
	sc_port<read_if_T<sc_uint<64>>> fa[11], fb[10];

	sc_port<write_if_T<sc_uint<8>>> addrA, addrB;
	sc_port<write_if_T<sc_uint<64>>> iniVal, iniRes;
	sc_port<write_if_T<sc_uint<64>>> r[10], s[10], o[10];
	sc_port<write_if_T<sc_uint<64>>> OUTRO;

	// declaración de métodos
	void process();

	SC_CTOR (controller) {
		cout<<"Controller: "<<name()<<endl;
		SC_THREAD(process);
	}
	
private:
	// atributos privados
	sc_uint<64> G; // número de generaciones
	sc_uint<64> x[10];
	sc_uint<64> tmp;

	// métodos privados
	void memToOut();
	void init();
};