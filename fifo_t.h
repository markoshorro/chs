
public:

SC_HAS_PROCESS(ManyConstSrc_T);

ManyConstSrc_T( sc_module_name name_, T *constantes_, int num_) :  sc_module(name_)
{
	constantes = new T[num_];
	for(int i; i<num_; ++i)	constantes[i] = constantes_[i];
	num = num_;		turno = 0;
}

~ManyConstSrc_T(){	delete constantes; }

void read(T& c) {
	c = constantes[turno++];
	if(turno == num)	turno = 0;
	read_event.notify();
} 	
int num_available() {return 1;}

private:
	T *constantes;
	int num, turno;
	sc_event read_event;
};



#endif