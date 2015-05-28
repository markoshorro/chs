#ifndef CALCULOSDE_H
#define CALCULOSDE_H


#include "systemc.h"
#include "FPsegmentado.h"
#include "fifo.h"


SC_MODULE (calculosDE) {
public:
sc_in<bool>	clk, reset;
sc_in< sc_uint<64> >  X[10];
sc_in< bool >  valX[10];
sc_out< sc_uint<64> >  resultado;
sc_out< bool >  valRes;


sc_signal<sc_uint<64>> Qcd, Qhi, Qjk, Qf2a, Qf2b, Qf2, Qf5a, Qf5b, Qf5, Qf1, Qf3, Qf4, Qs1, Qs2, Qs3, Qs4; 
sc_signal<sc_uint<64>> Qx1b, Qx0b, Qx5b, Qx2, Qx3, Qs3d, Qs1d, Qf3d, Qf4d;

sc_signal<bool> Vcd, Vhi, Vjk, Vf2a, Vf2b, Vf2, Vf5a, Vf5b, Vf5, Vf1, Vf3, Vf4, Vs1, Vs2, Vs3, Vs4; 
sc_signal<bool> Vx1d, Vx0d, Vx5d, Vf3d, Vf4d, Vs1d, Vs3d; 

fpMult *multF5a, *multF2a, *multF5b, *multF2b;
fpAdd *addCd, *addHi, *addS1, *addS2, *addS3, *addS4, *addRes;
fpSquare *sqF1, *sqF3;
fpMinimo *minimo; 
fpDesplaza1	*sh1;
fpDesplaza2 *sh2;
fpDesplaza4_neg *sh4;


// COMPLETAR A CONTINUACIÓN EL TAMAÑO QUE DEBE TENER CADA RETARDO

smallDelay_T<sc_uint<64>, 3> *retardoX1, *retardoX5, *retardoS3;
smallDelay_T<sc_uint<64>, 6> *retardoX0;
smallDelay_T<sc_uint<64>, 1> *retardoS1, *retardoF3;
smallDelay_T<sc_uint<64>, 5> *retardoF4;

  void arreglarDatos();

  SC_CTOR(calculosDE) {
	int i;
    cout<<"calculosDE: "<<name()<<endl;
	SC_METHOD(arreglarDatos);
		sensitive << X[2] << X[3]; //COMPLETAR LISTA DE SENSIBILIDAD

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

		

// COMPLETAR A CONTINUACIÓN EL TAMAÑO QUE DEBE TENER CADA RETARDO

		retardoX1 = new smallDelay_T<sc_uint<64>,  3>("retardoX1");
		retardoX5 = new smallDelay_T<sc_uint<64>,  3>("retardoX5");
		retardoS3 = new smallDelay_T<sc_uint<64>,  3>("retardoS3");
		retardoX0 = new smallDelay_T<sc_uint<64>,  6>("retardoX0");
		retardoS1 = new smallDelay_T<sc_uint<64>,  1>("retardoS1");
		retardoF3 = new smallDelay_T<sc_uint<64>,  1>("retardoF3");
		retardoF4 = new smallDelay_T<sc_uint<64>,  5>("retardoF4");


		multF5a->clk(clk);			multF2a->clk(clk);			multF5b->clk(clk);			multF2b->clk(clk);
		multF5a->reset(reset);		multF2a->reset(reset);		multF5b->reset(reset);		multF2b->reset(reset);
			
		addCd->clk(clk);			addHi->clk(clk);			addS1->clk(clk);			addS2->clk(clk);
		addCd->reset(reset);		addHi->reset(reset);		addS1->reset(reset);		addS2->reset(reset);
		addS3->clk(clk);			addS4->clk(clk);			addRes->clk(clk);	
		addS3->reset(reset);		addS4->reset(reset);		addRes->reset(reset);	
			
		sqF1->clk(clk);				sqF3->clk(clk);		
		sqF1->reset(reset);			sqF3->reset(reset);		
		
		minimo ->clk(clk);			sh1->clk(clk);				sh2->clk(clk);				sh4->clk(clk);
		minimo ->reset(reset);		sh1->reset(reset);			sh2->reset(reset);			sh4->reset(reset);
			
		retardoX1->clk(clk);		retardoX5->clk(clk);		retardoS3->clk(clk);		retardoX0->clk(clk);
		retardoX1->reset(reset);	retardoX5->reset(reset);	retardoS3->reset(reset);	retardoX0->reset(reset);
		retardoS1->clk(clk);		retardoF3->clk(clk);		retardoF4->clk(clk);	
		retardoS1->reset(reset);	retardoF3->reset(reset);	retardoF4->reset(reset);	

		retardoX1->datoIn(X[1]);		retardoX5->datoIn(X[5]);		retardoS3->datoIn(Qs3);		retardoX0->datoIn(X[0]);
		retardoX1->validIn(valX[1]);	retardoX5->validIn(valX[5]);	retardoS3->validIn(Vs3);	retardoX0->validIn(valX[0]);
		retardoX1->datoOut(Qx1b);		retardoX5->datoOut(Qx5b);		retardoS3->datoOut(Qs3d);	retardoX0->datoOut(Qx0b);
		retardoX1->validOut(Vx1d);		retardoX5->validOut(Vx5d);		retardoS3->validOut(Vs3d);	retardoX0->validOut(Vx0d);

		retardoS1->datoIn(Qs1);		retardoF3->datoIn(Qf3);		retardoF4->datoIn(Qf4);	
		retardoS1->validIn(Vs1);	retardoF3->validIn(Vf3);	retardoF4->validIn(Vf4);
		retardoS1->datoOut(Qs1d);	retardoF3->datoOut(Qf3d);	retardoF4->datoOut(Qf4d);	
		retardoS1->validOut(Vs1d);	retardoF3->validOut(Vf3d);	retardoF4->validOut(Vf4d);

		multF5a->op1( X[0] );		multF5a->op2( X[5] );		multF5a->mult( Qf5a );	
		multF5a->val1( valX[0] ); 	multF5a->val2( valX[5] );	multF5a->validOut( Vf5a );

		multF2a->op1( Qx3 );		multF2a->op2( X[4] );		multF2a->mult( Qf2a );	
		multF2a->val1( valX[3] ); 	multF2a->val2( valX[4] );	multF2a->validOut( Vf2a ); 

		multF5b->op1( Qf5a );		multF5b->op2( Qx1b );		multF5b->mult( Qf5b );	
		multF5b->val1( Vf5a ); 		multF5b->val2( Vx1d );		multF5b->validOut( Vf5b ); 

		multF2b->op1( Qf2a );		multF2b->op2( Qx5b );		multF2b->mult( Qf2b );	
		multF2b->val1( Vf2a ); 		multF2b->val2( Vx5d );		multF2b->validOut( Vf2b ); 

		addCd->op1( X[1] );			addCd->op2( Qx2 );			addCd->suma( Qcd );		
		addCd->val1( valX[1] ); 	addCd->val2( valX[2] ); 	addCd->validOut( Vcd ); 
		
		addHi->op1( X[6] );			addHi->op2( X[7] );			addHi->suma( Qhi );		
		addHi->val1( valX[6] ); 	addHi->val2( valX[7] ); 	addHi->validOut( Vhi ); 

		addS1->op1( Qf1 );			addS1->op2( Qx0b );			addS1->suma( Qs1 );			
		addS1->val1( Vf1 ); 		addS1->val2( Vx0d );		addS1->validOut( Vs1 ); 

		addS2->op1( Qf2 );			addS2->op2( Qf3d );			addS2->suma( Qs2 );		
		addS2->val1( Vf2 ); 		addS2->val2( Vf3d );		addS2->validOut( Vs2 ); 

		addS3->op1( Qf5 );			addS3->op2( Qf4d );			addS3->suma( Qs3);		
		addS3->val1( Vf5 ); 		addS3->val2( Vf4d );		addS3->validOut( Vs3 ); 

		addS4->op1( Qs1d );			addS4->op2( Qs2 );			addS4->suma( Qs4 );		
		addS4->val1( Vs1d ); 		addS4->val2( Vs2 ); 		addS4->validOut( Vs4 ); 

		addRes->op1( Qs3d );		addRes->op2( Qs4 );			addRes->suma( resultado );		
		addRes->val1( Vs3d ); 		addRes->val2( Vs4 ); 		addRes->validOut( valRes ); 

		sqF1->op( Qcd );	sqF1->validIn( Vcd );	sqF1->square( Qf1 );	sqF1->validOut( Vf1 );
		sqF3->op( Qhi );	sqF3->validIn( Vhi );	sqF3->square( Qf3 );	sqF3->validOut( Vf3 );
		
		minimo->op1( X[8] );		minimo->op2( X[9] );		minimo->minimo( Qjk );
		minimo->val1( valX[8] );	minimo->val2( valX[9] );	minimo->validOut( Vjk );

		///////////////////////////////////////////////////////////
		// TERMINAR DE CONECTAR LOS 3 DESPLAZAMIENTOS
		///////////////////////////////////////////////////////////

		sh1->op( Qjk );		sh1->validIn( Vjk );	sh1->desplaz( Qf4 );	sh1->validOut( Vf4 );
		sh2->op( Qf2b );	sh2->validIn( Vf2b );	sh2->desplaz( Qf2 );	sh2->validOut( Vf2 );
		sh4->op( Qf5b );	sh4->validIn( Vf5b );	sh4->desplaz( Qf5 );	sh4->validOut( Vf5 );

  } 
}; 

#endif