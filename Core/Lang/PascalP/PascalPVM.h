#ifndef __PASCALPVM_H__
#define __PASCALPVM_H__

#include "Core/Types/Base.h"

#define CODEMAX 8650
#define MAXSTK  13650   /* size of variable store */
#define OVERM   18000
#define MAXSTR  18001

class PascalPVM {

public:
	typedef char bit4;
	typedef char bit6;
	typedef short bit20;
	typedef short address;
	typedef long settype[3];

	typedef struct _REC_code {
		/* the program */
		unsigned op1 : 7, p1 : 4;
		signed int q1 : 16;
		unsigned op2 : 7, p2 : 4;
		short q2;
	} _REC_code;

	typedef union _REC_store {
		long vi;
		double vr;
		bool vb;
		settype vs;
		char vc;
		address va;
		/*address in store*/
		long vm;
	} _REC_store;

	_REC_code code[CODEMAX + 1];
	short pc;   /*program address register*/
	bit6 op;
	bit4 p;
	bit20 q;

	_REC_store store[OVERM + 1];
	address mp, sp, np, ep;

	bool interpreting;

	void run();

protected:

	void errori(char* string);
	address base(long ld);
	void compare();

	void callsp();

};




#endif /* __PASCALPVM_H__ */
