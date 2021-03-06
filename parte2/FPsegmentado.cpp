#include "FPsegmentado.h"
#include "fifo.h"

void fpAdd::registros(){
	bool vv;
	unsigned long long *tmp;
	double dob;

	tmp = (unsigned long long*) &dob;

	if(reset.read()){
		valid1 = valid2 = valid3 = false;
		sc1 = sc2 = 0;
	}else{
		valid3 = valid2;		valid2 = valid1;		valid1 = val1.read();
		
		vv = val2.read(); 
		if(vv != valid1 )
			cout << name() << " Valid no coincide " << valid1 << " <> " << vv << endl;

		regNormal = Normal;		regMovido = Movido;		
		r_ceroA1 = ceroA1;		r_ceroB1 = ceroB1;
		des = desp;				regSuma = Suma;			
		
		regSig2 = regSig1;			regSig1 = Sig1;	
		regPos2 = regPos1;			regPos1 = Pos1;
		r_inf2 = r_inf1;			r_inf1 = inf1;	
		r_preExpo2 = r_preExpo1;	r_preExpo1 = preExpo1;

		sc1 = op1.read();	sc2 = op2.read();	  

	}
	fireRegs = !fireRegs;
}




void fpAdd::calculaAdd(){

	sc_uint<12> restaExp, calcExp;
	sc_uint<13> expo;
	sc_uint<11> absExp, preExpo;

	bool equalExp, mantisaAmayor;

	sc_uint<52> normal, movido, preNormal, preMovido, mantisa;
	sc_uint<64> signedNormal, signedMovido, tmpRes;
	sc_uint<64> sh6, sh5, sh4, sh3, sh2, sh1, sh0, compSh0, sumaCorregida;
	sc_uint<64> preRes, regRes;
	sc_uint<61> norm32, norm16, norm8, norm4, norm2, norm1;
	sc_uint<6> sube;
	bool sePasa, cero_1, cero_2;

	// etapa 1

	cero_1 = ( sc1(62, 52) == 0 );		cero_2 = ( sc2(62, 52) == 0 );
	inf1 = ( sc1(62, 52) == 0x7ff ) || ( sc2(62, 52) == 0x7ff );

	restaExp = sc1(62, 52) - sc2(62, 52); 

	equalExp = (restaExp == 0);
			
	if( sc1(51, 0) < sc2(51, 0)	)	mantisaAmayor = false;
	else							mantisaAmayor = true;

	if( restaExp.bit(11) ){	absExp = (~restaExp(10, 0) ) + 1;	preExpo1 = sc2(62, 52);
	}else{					absExp = restaExp(10, 0);			preExpo1 = sc1(62, 52); }


	sePasa = absExp.bit(6) | absExp.bit(7) | absExp.bit(8) | absExp.bit(9) | absExp.bit(10);
	desp = (sePasa << 6) | absExp(5, 0);


	if( restaExp.bit(11) || (equalExp && !mantisaAmayor) ){
		Normal = sc2(51, 0);
		Movido = sc1(51, 0);
		Pos1 = sc2.bit(63);
		ceroA1 = cero_2;	ceroB1 = cero_1;
	}else{
		Normal = sc1(51, 0);
		Movido = sc2(51, 0);
		Pos1 = sc1.bit(63);
		ceroA1 = cero_1;	ceroB1 = cero_2;
	}

	Sig1 = sc1.bit(63) ^ sc2.bit(63);


	// etapa 2

	signedNormal = regNormal << 9;		signedNormal.bit(61) = ! r_ceroA1;
	signedMovido = regMovido << 9;		signedMovido.bit(61) = ! r_ceroB1;

	if( des.bit(6) )	sh6 = 0;		
	else				sh6 = signedMovido;
	if( des.bit(5) ){	sh5(63, 32) = 0;	sh5(31, 0) = sh6(63, 32);}
	else				sh5 = sh6;
	if( des.bit(4) ){	sh4(63, 48) = 0;	sh4(47, 0) = sh5(63, 16);}
	else				sh4 = sh5;
	if( des.bit(3) ){	sh3(63, 56) = 0;	sh3(55, 0) = sh4(63, 8); }
	else				sh3 = sh4;
	if( des.bit(2) ){	sh2(63, 60) = 0;	sh2(59, 0) = sh3(63, 4);}
	else				sh2 = sh3;
	if( des.bit(1) ){	sh1(63, 62) = 0;	sh1(61, 0) = sh2(63, 2);}
	else				sh1 = sh2;
	if( des.bit(0) ){	sh0.bit(63) = 0; 	sh0(62, 0) = sh1(63, 1);}
	else				sh0 = sh1;

	if( regSig1 )	compSh0 = ~sh0;
	else			compSh0 = sh0;

	Suma = signedNormal + compSh0 + regSig1; 

	// etapa 3	

	if( regSuma(61, 30) == 0){		sube.bit(5) = 1;  norm32 = regSuma(29, 1) << 32; }
	else{							sube.bit(5) = 0;  norm32 = regSuma(61, 1);	}
	if( norm32(60, 45) == 0){		sube.bit(4) = 1;  norm16 = norm32(44, 0) << 16; }									
	else{							sube.bit(4) = 0;  norm16 = norm32(60, 0); }
	if( norm16(60, 53) == 0){		sube.bit(3) = 1;   norm8 = norm16(52, 0) << 8; }
	else{							sube.bit(3) = 0;   norm8 = norm16(60, 0); }
	if( norm8(60, 57) == 0){		sube.bit(2) = 1;   norm4 = norm8(56, 0) << 4; }
	else{							sube.bit(2) = 0;   norm4 = norm8(60, 0); }
	if( norm4(60, 59) == 0){		sube.bit(1) = 1;   norm2 = norm4(58, 0) << 2; }
	else{							sube.bit(1) = 0;   norm2 = norm4(60, 0); }
	if( norm2.bit(60) == 0){		sube.bit(0) = 1;   norm1 = norm2(59, 0) << 1; }
	else{							sube.bit(0) = 0;   norm1 = norm2(60, 0); }

	if( regSuma.bit(62) ){		mantisa = regSuma(61, 10);	expo = r_preExpo2 + 1;
	}else{
		if( regSuma.bit(61) ){	mantisa = regSuma(60, 9);	expo = r_preExpo2;
		}else{					mantisa = norm1(59, 8);		expo = r_preExpo2 - sube;}
	}

	if( expo.bit(12) || (expo == 0) || ((sube == 0x3f) && (regSuma.bit(62)==0)) ) // expo <= 0
		tmpRes = 0;
	else{
		if( r_inf2 || expo.bit(11) ){
			tmpRes.bit(63) = regPos2;
			tmpRes(62, 52) = 0x7ff;
			tmpRes(51, 0) = 0;
		}else{
			tmpRes.bit(63) = regPos2;
			tmpRes(62, 52) = expo(10, 0);
			tmpRes(51, 0) = mantisa;
		}
	}
				
	suma.write( tmpRes );
	validOut.write( valid3 );

}






void fpMult::registros(){
	bool vv;
	unsigned long long *tmp;
	double dob;

	tmp = (unsigned long long*) &dob;

	if(reset.read()){
		valid1 = valid2 = valid3 = false;
		sc1 = sc2 = 0;
	}else{
		valid3 = valid2;		valid2 = valid1;		valid1 = val1.read();

		vv = val2.read(); 
		if(vv != valid1 )
			cout << name() << " Valid no coincide " << valid1 << vv << endl;

		regSig3 = regSig2;		regSig2 = regSig1;		
		regExp3 = regExp2;		regExp2 = regExp1;

		r_AF = AF;		r_BF = BF;		r_AE = AE;		r_BE = BE;		r_CE = CE;
		r_AD = AD;		r_BD = BD;		r_CD = CD;		r_CF = CF;		r_xyF = xyF;
		r_xyE = xyE;	r_xyD = xyD;	r_Czt = Czt;	r_Bzt = Bzt;	r_Azt = Azt;
		r_xyzt = xyzt;	r_res = res;
		r_finalReg = finalReg;	r_middleReg = middleReg;	r_ae_bd_f_Reg = ae_bd_f_Reg;

		sc1 = op1.read();	sc2 = op2.read();	

	}
	fireRegs = !fireRegs;
}


void fpMult::calculaMult(){

	sc_uint<11> exp1, exp2, expMult;
	sc_uint<13> calcExp;		// lo hago sin signo para convertirlo m�s f�cilmente a VHDL

	sc_uint<18> A, B, C, D, E, F;
	sc_uint<17> yF, yE, yD;
	sc_uint<17> Ct, Bt, At;
	sc_uint<34> preCF;

	sc_uint<4> pre_xyzt;

	sc_uint<35> cf_bf_ce;
	sc_uint<36> af_be_cd, trailing;
	sc_uint<21> F_stuff;
	sc_uint<36> ae_bd_f;
	sc_uint<35> e_ad_b;
	sc_uint<22> front;
	sc_uint<36> middle;
	
	sc_uint<64> scR;
	sc_uint<52> mantisa;


	// etapa 1

	A = sc1(50, 34);	B = sc1(33, 17);	C = sc1(16, 0);
	D = sc2(50, 34);	E = sc2(33, 17);	F = sc2(16, 0);

	AF = (A * F);		BF = (B * F);		preCF = C * F;		CF = ( preCF(33, 17) );
	AE = (A * E);		BE = (B * E);		CE = (C * E);
	AD = (A * D);		BD = (B * D);		CD = (C * D);

	B = (B * D); 	BD = 0;

	if( sc1.bit(51) )	{	yF = F;		yE = E;		yD = D;}
	else				{	yF = 0;		yE = 0;		yD = 0;}

	if( sc2.bit(51) )	{	Ct = C;		Bt = B;		At = A;}
	else				{	Ct = 0;		Bt = 0;		At = 0;}

	xyF = ( (F << 1) + yF );		xyE = ( (E << 1) + yE );		xyD = ( (D << 1) + yD );
	Czt = ( (C << 1) + Ct );		Bzt = ( (B << 1) + Bt );		Azt = ( (A << 1) + At );


	pre_xyzt.bit(3) = sc1.bit(51) & sc2.bit(51);	 // hacer la suma me daba problemas con la inferencia del signo de los operandos
	pre_xyzt.bit(2) = ! pre_xyzt.bit(3);
	pre_xyzt.bit(1) = sc1.bit(51) ^ sc2.bit(51);
	pre_xyzt.bit(0) = pre_xyzt.bit(3);

	xyzt = ( pre_xyzt );

	regSig1 = ( sc1.bit(63) ^ sc2.bit(63) );

	exp1 = sc1(62, 52);		exp2 = sc2(62, 52);	
	calcExp = exp1 + exp2 +  0x1C01;	// equivale a - 1023;

	if((exp1 == 0) || (exp2 == 0) )			
		regExp1 = ( 0 );
	else{
		if((exp1 == 0x7ff) || (exp2 == 0x7ff) )
			regExp1 = ( 0x7ff );
		else{
			if( calcExp.bit(12) )	// negativo, underflow
				regExp1 = ( 0 );
			else
				if( calcExp > 0x7ff )	// underflow
					regExp1 = ( 0x7ff );
				else
					regExp1 = ( calcExp(10, 0) );
		}
	}


	// etapa 2

	cf_bf_ce = r_CF + r_BF + r_CE;
	af_be_cd = r_AF + r_BE + r_CD;

	trailing = cf_bf_ce(34, 17) + af_be_cd;
	F_stuff = trailing(35, 17) + r_xyF + r_Czt;

	ae_bd_f = r_AE + r_BD + F_stuff + 1;
	e_ad_b = r_xyE + r_AD + r_Bzt;

	front = r_xyD + r_Azt + (r_xyzt << 17);

	middle = e_ad_b + ae_bd_f(34, 17);
	finalReg = front + middle(35, 17);
	middleReg = middle(16, 0);
	ae_bd_f_Reg = ae_bd_f(16, 1);


	// etapa 3

	scR.bit(63) = regSig3;
	
	if( r_finalReg.bit(20)){	// producto no normalizado
		expMult = regExp3 + 1;
		mantisa(51, 32) = r_finalReg(19, 0);
		mantisa(31, 15) = r_middleReg(16, 0);
		mantisa(14, 0) = r_ae_bd_f_Reg(15, 1);
	}else{
		expMult = regExp3;
		mantisa(51, 33) = r_finalReg(18, 0);
		mantisa(32, 16) = r_middleReg(16, 0);
		mantisa(15, 0) = r_ae_bd_f_Reg;
	}

	if(regExp3 == 0)
		scR = 0;
	else{
		if(regExp3 == 0x7ff){
			scR(62, 52) = 0x7ff;	scR(51, 0) = 0;
		}else{
			scR(62, 52) = expMult;	scR(51, 0) = mantisa;
		}
	}

	mult.write ( scR );
	validOut.write( valid3 );


}










void fpSquare::registros(){

	unsigned long long *tmp;
	double dob;

	tmp = (unsigned long long*) &dob;

	if(reset.read()){
		valid1 = valid2 = valid3 = false;
		sc1 = 0;
	}else{
		valid3 = valid2;		valid2 = valid1;		valid1 = validIn.read();

		regExp2 = regExp1;	regExp1 = preExp1;	
		r_xy2 = xy2;		r_A2 = A2;
		r_B2 = B2;			r_AB = AB;			r_BC = BC;
		r_AC = AC;			r_xA = xA;			r_xB = xB;
		r_xC = xC;			r_C2 = C2;

		ab_f_Reg = ab_f(16, 1);
		finalReg = final;	middleReg = middle;

		sc1 = op.read();

	}
	fireRegs = !fireRegs;
}





void fpSquare::calculaSquare(){

	sc_uint<11> exp1, expSq;
	sc_uint<13> calcExp;		// lo hago sin signo para convertirlo m�s f�cilmente a VHDL

	sc_uint<18> A, B, C;
	sc_uint<18> yA, yB, yC;
	sc_uint<36> preC2; 
	sc_uint<34> preCF;

	sc_uint<35> c2_bc;
	sc_uint<36> ac_b2, trailing;
	sc_uint<21> F_stuff;	
	sc_uint<35> xb_a2;
	sc_uint<22> front;

	sc_uint<64> scR;
	sc_uint<52> mantisa;


	// etapa 1
		
	A = sc1(50, 34);		B = sc1(33, 17);		C = sc1(16, 0);

	if(valid1 && PRINT)
		valid1 = valid1;

	A2 = A * A;		B2 = B * B;		preC2 = C * C;	C2 = preC2(33, 17);
	AB = A * B;		BC = B * C;		AC = A * C;

	if( sc1.bit(51) ){		yA = A;				yB = B;				yC = C;
	}else{					yA = yB = yC = 0;	}

	xA = (A << 1) + yA;		xB = (B << 1) + yB;		xC = (C << 1) + yC;
		
	xy2 = sc1.bit(51) ? 9 : 4; // 

	exp1 = sc1(62, 52);
	calcExp = (exp1 << 1) +  0x1C01;	// equivale a - 1023;

	if( exp1 == 0 )			
		preExp1 = 0;
	else{
		if( exp1 == 0x7ff )
			preExp1 = 0x7ff;
		else{
			if( calcExp.bit(12) )	// negativo, underflow
				preExp1 = 0 ;
			else
				if( calcExp > 0x7ff )	// underflow
					preExp1 = 0x7ff;
				else
					preExp1 = calcExp(10, 0);
		}
	}


	// etapa 2

	c2_bc = r_C2 + (r_BC(33, 0) << 1) ;
	ac_b2 = (r_AC(33, 0) << 1) + r_B2(33, 0);

	trailing = c2_bc(34, 17) + ac_b2;
	F_stuff = trailing(35, 17) + (r_xC << 1);

	ab_f = (r_AB << 1) + F_stuff + 1;
	xb_a2 = (r_xB << 1) + r_A2;

	front = (r_xA << 1) + (r_xy2 << 17);

	middle = xb_a2 + ab_f(34, 17);
	final = front + middle(35, 17);	

	// etapa 3

	if( finalReg.bit(20)){	// producto no normalizado
		expSq = regExp2 + 1;
		mantisa(51, 32) = finalReg(19, 0);
		mantisa(31, 15) = middleReg(16, 0);
		mantisa(14, 0) = ab_f_Reg(15, 1);
	}else{
		expSq = regExp2;
		mantisa(51, 33) = finalReg(18, 0);
		mantisa(32, 16) = middleReg(16, 0);
		mantisa(15, 0) = ab_f_Reg;
	}

	if(regExp2 == 0)
		scR = 0;
	else{
		if(regExp2 == 0x7ff){
			scR.bit(63) = 0;	scR(62, 52) = 0x7ff;	scR(51, 0) = 0;
		}else{
			scR.bit(63) = 0;	scR(62, 52) = expSq;	scR(51, 0) = mantisa;
		}
	}

	square.write( scR );
	validOut.write( valid3 );

}


