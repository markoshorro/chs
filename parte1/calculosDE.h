#ifndef CALCULOSDE_H
#define CALCULOSDE_H


#include "systemc.h"
#include "FPplano.h"
#include "fifo.h"


SC_MODULE (calculosDE) {
public:
sc_port<read_if_T<sc_uint<64>>>  X[10];
sc_port<write_if_T<sc_uint<64>>> resultado;	


fifo_T<sc_uint<64>> *Qcd, *Qhi, *Qjk, *Qf2a, *Qf2b, *Qf2, *Qf5a, *Qf5b, *Qf5, *Qf1, *Qf3, *Qf4, *Qs1, *Qs2, *Qs3, *Qs4; 
fifo_T<sc_uint<64>> *Qx1a, *Qx1b, *Qx0a, *Qx0b, *Qx5a, *Qx5b, *Qx2, *Qx3;
fpMult *multF5a, *multF2a, *multF5b, *multF2b;
fpAdd *addCd, *addHi, *addS1, *addS2, *addS3, *addS4, *addRes;
fpSquare *sqF1, *sqF3;
fpMinimo *minimo; 
fpDesplaza1	*sh1;
fpDesplaza2 *sh2;
fpDesplaza4_neg *sh4;

	void arreglarDatos();

  SC_CTOR(calculosDE) {
	int i;
    cout<<"calculosDE: "<<name()<<endl;

		multF5a = new fpMult("multF5a");		multF2a = new fpMult("multF2a");
		multF5b = new fpMult("multF5b");		multF2b = new fpMult("multF2b");

		addCd = new fpAdd("addCd");				addHi = new fpAdd("addHi");
		addS1 = new fpAdd("addS1");				addS2 = new fpAdd("addS2");
		addS3 = new fpAdd("addS3");				addS4 = new fpAdd("addS4");
		addRes = new fpAdd("addRes");

		sqF1 = new fpSquare("sqF1");			sqF3 = new fpSquare("sqF3");
		minimo = new fpMinimo("minimo");
		sh1 = new fpDesplaza1("sh1");
		sh2 = new fpDesplaza2("sh2");
		sh4 = new fpDesplaza4_neg("sh4");

		Qx0a = new fifo_T<sc_uint<64>>("Qx0a", 1); 
		Qx0b = new fifo_T<sc_uint<64>>("Qx0b", 20); // 8?
		Qx1a = new fifo_T<sc_uint<64>>("Qx1a", 1);
		Qx1b = new fifo_T<sc_uint<64>>("Qx1b", 10); // 4?
		Qx5a = new fifo_T<sc_uint<64>>("Qx5a", 1);
		Qx5b = new fifo_T<sc_uint<64>>("Qx5b", 10); // 4?
		Qx2 = new fifo_T<sc_uint<64>>("Qx2", 1); 
		Qx3 = new fifo_T<sc_uint<64>>("Qx3", 1); 

		Qcd = new fifo_T<sc_uint<64>>("Qcd", 1);
		Qhi = new fifo_T<sc_uint<64>>("Qhi", 1);
		Qjk = new fifo_T<sc_uint<64>>("Qjk", 1);
		Qf2a = new fifo_T<sc_uint<64>>("Qf2a", 1);
		Qf2b = new fifo_T<sc_uint<64>>("Qf2b", 1);
		Qf2 = new fifo_T<sc_uint<64>>("Qf2", 1);
		Qf5a = new fifo_T<sc_uint<64>>("Qf5a", 1);
		Qf5b = new fifo_T<sc_uint<64>>("Qf5b", 1);
		Qf5 = new fifo_T<sc_uint<64>>("Qf5", 1);
		Qf1 = new fifo_T<sc_uint<64>>("Qf1", 1);
		Qf3 = new fifo_T<sc_uint<64>>("Qf3", 4);	// 2?
		Qf4 = new fifo_T<sc_uint<64>>("Qf4", 20);	// 6?
		Qs1 = new fifo_T<sc_uint<64>>("Qs1", 4);	// 2?
		Qs2 = new fifo_T<sc_uint<64>>("Qs2", 1);
		Qs3 = new fifo_T<sc_uint<64>>("Qs3", 10);	// 4?
		Qs4 = new fifo_T<sc_uint<64>>("Qs4", 1);
		
		multF5a->op1( *Qx0a );	multF5a->op2( *Qx5a );	multF5a->mult( *Qf5a );
		multF2a->op1( *Qx3 );	multF2a->op2( X[4] );	multF2a->mult( *Qf2a );
		multF5b->op1( *Qf5a );	multF5b->op2( *Qx1b );	multF5b->mult( *Qf5b );
		multF2b->op1( *Qf2a );	multF2b->op2( *Qx5b );	multF2b->mult( *Qf2b );

		addCd->op1( *Qx1a );	addCd->op2( *Qx2 );		addCd->suma( *Qcd );
		addHi->op1( X[6] );		addHi->op2( X[7] );		addHi->suma( *Qhi );
		addS1->op1( *Qf1 );		addS1->op2( *Qx0b );	addS1->suma( *Qs1 );
		addS2->op1( *Qf2 );		addS2->op2( *Qf3 );		addS2->suma( *Qs2 );
		addS3->op1( *Qf5 );		addS3->op2( *Qf4);		addS3->suma( *Qs3);
		addS4->op1( *Qs1 );		addS4->op2( *Qs2 );		addS4->suma( *Qs4 );
		addRes->op1( *Qs3 );	addRes->op2( *Qs4 );	addRes->suma( resultado );

		sqF1->op( *Qcd );	sqF1->square( *Qf1 );
		sqF3->op( *Qhi );	sqF3->square( *Qf3 );
		
		minimo->op1( X[8] );	minimo->op2( X[9] );	minimo->minimo( *Qjk );
		sh1->op( *Qjk );		sh1->desplaz( *Qf4 );
		sh2->op( *Qf2b );		sh2->desplaz( *Qf2 );
		sh4->op( *Qf5b );		sh4->desplaz( *Qf5 );


		SC_THREAD(arreglarDatos);
  } 
}; 

#endif