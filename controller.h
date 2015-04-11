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
	// declaración de puertos
	sc_port<read_if_T<sc_uint<64>>> INTRO;
	sc_port<read_if_T<sc_uint<8>>> rand1;
	sc_port<read_if_T<sc_uint<8>>> rand2;
	sc_port<read_if_T<sc_uint<8>>> rand3;
	sc_port<read_if_T<sc_uint<10>>> hibridar;
	sc_port<read_if_T<sc_uint<64>>> fa[11];
	sc_port<read_if_T<sc_uint<64>>> fb[10];

	sc_port<write_if_T<sc_uint<8>>> addrA;
	sc_port<write_if_T<sc_uint<8>>> addrB;
	sc_port<write_if_T<sc_uint<64>>> OUTRO;

	// declaración de métodos
	void memToOut();

	SC_CTOR (controller) {
		cout<<"Controller: "<<name()<<endl;
	}
	
private:
};