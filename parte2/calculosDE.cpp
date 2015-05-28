#include"calculosDE.h"


void calculosDE::arreglarDatos(){

		sc_uint<64> tmp2, tmp3;

		/////////////////////////////////////////////////////////////////
		// ESCRIBIR AQUÍ EL CÓDIGO QUE CAMBIA LOS SIGNOS DE X[2] y X[3]
		/////////////////////////////////////////////////////////////////

		tmp2 = X[2].read();
		tmp3 = X[3].read();

		tmp2.bit(63) = ~tmp2.bit(63);
		tmp3.bit(63) = 0;

		Qx2.write( tmp2 );
		Qx3.write( tmp3 );

}