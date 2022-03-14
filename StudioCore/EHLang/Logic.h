#ifndef LOGIC_H
#define LOGIC_H



#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "Scanner.h"
#include "Parser.h"

namespace EHLang {

	class Logic {

	public:

		enum Op {
			UNKNOWN, PLUS, MINUS, TIMES, SLASH, EQU, LSS, GTR
		};

		Scanner* scanner;
		Parser* parser;
		SymbolTable* tab;
		CodeGenerator* gen;

		void doExpr(int &type, int type1, Op op) {
			if (type != type1) 
				parser->Err(L"incompatible types");
			gen->Emit(op); 
			type = parser->boolean;
		}

		void doSimExpr(int type, int type1, Op op) {
			if (type != parser->integer || type1 != parser->integer)
				parser->Err(L"integer type expected");
			gen->Emit(op);
		}

		void doTerm(int type, int type1, Op op) {
			if (type != parser->integer || type1 != parser->integer)
				parser->Err(L"integer type expected");
			gen->Emit(op);
		}

		void doFactorIdent(int &type, wchar_t* name) {
			Obj* obj;

			obj = tab->Find(name); type = obj->type;
			if (obj->kind == parser->var) {
				if (obj->level == 0) 
					gen->Emit(CodeGenerator::LOADG, obj->adr);
				else 
					gen->Emit(CodeGenerator::LOAD, obj->adr);
			}
			else parser->Err(L"variable expected");
		}

		void doFactorNumber(int &type) {
			int n;
			swscanf(parser->t->val, L"%d", &n);	//n = Convert.ToInt32(t->val); 
			gen->Emit(CodeGenerator::CONST, n); 
			type = parser->integer;
		}

		void doFactorNeg(int &type) {
			if (type != parser->integer) {
				parser->Err(L"integer type expected"); 
				type = parser->integer;
			}
			gen->Emit(CodeGenerator::NEG);
		}

		void doFactorTrue(int& type) {
			gen->Emit(CodeGenerator::CONST, 1); 
			type = parser->boolean;
		}

		void doFactorFalse(int& type) {
			gen->Emit(CodeGenerator::CONST, 0); 
			type = parser->boolean;
		}

		void doProcDeclIdent(wchar_t* name) {
			Obj* obj;
			obj = tab->NewObj(name, parser->proc, parser->undef); 
			obj->adr = gen->pc;
			if (coco_string_equal(name, L"Main")) 
				gen->progStart = gen->pc;
			tab->OpenScope();
		}

		void doProcDeclEnter(int &adr) {
			gen->Emit(CodeGenerator::ENTER, 0); 
			adr = gen->pc - 2;
		}

		void doProcDeclLeave(int& adr) {
			gen->Emit(CodeGenerator::LEAVE); 
			gen->Emit(CodeGenerator::RET);
			gen->Patch(adr, tab->topScope->nextAdr);
			tab->CloseScope();
		
		}



	};

}


#endif