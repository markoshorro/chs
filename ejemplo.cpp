/*
 * 
 */

// 
#include "systemc.h"

for(i=0; i<256; ++i){
    addrA->write( (sc_uint<8>) i );        addrB->write( (sc_uint<8>) 0 );
    wait(SC_ZERO_TIME);
    for(j=0; j<11; ++j){
        fa[j]->read( tmp );
        OUTRwrite( tmp );
        if(j<10){
            fb[j]->read( tmp );    // para vaciar esta cola...
        }
        wait(SC_ZERO_TIME);
    }
}

fifo_T<sc_uint<64>> *Qfa[11], *Qfb[10];
fifo_T<sc_uint<64>> *Qr[10], *Qs[10], *Qo[10];
 
for(i=0; i<10; ++i){
    Qfa[i] = new fifo_T<sc_uint<64>>(nombreId"Qfa", i), 1);
    Qfb[i] = new fifo_T<sc_uint<64>>(nombreId"Qfb", i), 1);
    Qr[i] = new fifo_T<sc_uint<64>>(nombreId"Qr", i), 1);
    Qs[i] = new fifo_T<sc_uint<64>>(nombreId"Qs", i), 1);
    Qo[i] = new fifo_T<sc_uint<64>>(nombreId"Qo", i), 1);
}
Qfa[10] = new fifo_T<sc_uint<64>>(nombreId"Qfa", 10), 1);

SC_MODULE (controller) {
  SC_CTOR (controller) {
    
  }
  void say_hello() {
    //Print "Hello World" to the console.
    cout << "Hello World.\n";
  }
};

// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {
  controller hello("HELLO");
  // Print the hello world
  hello.say_hello();
  return(0);
}