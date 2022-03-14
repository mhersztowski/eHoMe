

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"
#include "Logic.h"

namespace EHLang {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::Expr(int &type) {
		int type1; Logic::Op op = Logic::UNKNOWN; 
		SimExpr(type);
		if (la->kind == 3 /* "==" */ || la->kind == 4 /* "<" */ || la->kind == 5 /* ">" */) {
			if (la->kind == 3 /* "==" */) {
				Get();
				op = Logic::EQU; 
			} else if (la->kind == 4 /* "<" */) {
				Get();
				op = Logic::LSS; 
			} else {
				Get();
				op = Logic::GTR; 
			}
			SimExpr(type1);
			logic->doSimExpr(type, type1, op); 
		}
}

void Parser::SimExpr(int &type) {
		int type1; Logic::Op op = Logic::UNKNOWN; 
		Term(type);
		while (la->kind == 6 /* "+" */ || la->kind == 7 /* "-" */) {
			if (la->kind == 6 /* "+" */) {
				Get();
				op = Logic::PLUS; 
			} else {
				Get();
				op = Logic::MINUS; 
			}
			Term(type1);
			if (type != integer || type1 != integer) 
			     Err(L"integer type expected");
			   gen->Emit(op); 
		}
}

void Parser::Term(int &type) {
		int type1; Logic::Op op = Logic::UNKNOWN; 
		Factor(type);
		while (la->kind == 8 /* "*" */ || la->kind == 9 /* "/" */) {
			if (la->kind == 8 /* "*" */) {
				Get();
				op = Logic::TIMES; 
			} else {
				Get();
				op = Logic::SLASH; 
			}
			Factor(type1);
			if (type != integer || type1 != integer) 
			 Err(L"integer type expected");
			gen->Emit(op);
			
		}
}

void Parser::Factor(int &type) {
		int n; Obj *obj; wchar_t* name; 
		type = undef; 
		if (la->kind == _ident) {
			Ident(name);
			obj = tab->Find(name); type = obj->type;
			if (obj->kind == var) {
			if (obj->level == 0) gen->Emit(CodeGenerator::LOADG, obj->adr);
			else gen->Emit(CodeGenerator::LOAD, obj->adr);
			} else Err(L"variable expected"); 
		} else if (la->kind == _number) {
			Get();
			swscanf(t->val, L"%d", &n);	//n = Convert.ToInt32(t->val); 
			gen->Emit(CodeGenerator::CONST, n); type = integer; 
		} else if (la->kind == 7 /* "-" */) {
			Get();
			Factor(type);
			if (type != integer) {
			 Err(L"integer type expected"); type = integer;
			}
			gen->Emit(CodeGenerator::NEG); 
		} else if (la->kind == 10 /* "true" */) {
			Get();
			gen->Emit(CodeGenerator::CONST, 1); type = boolean; 
		} else if (la->kind == 11 /* "false" */) {
			Get();
			gen->Emit(CodeGenerator::CONST, 0); type = boolean; 
		} else SynErr(29);
}

void Parser::Ident(wchar_t* &name) {
		Expect(_ident);
		name = coco_string_create(t->val); 
}

void Parser::ProcDecl() {
		wchar_t* name; Obj *obj; int adr; 
		Expect(12 /* "void" */);
		Ident(name);
		obj = tab->NewObj(name, proc, undef); obj->adr = gen->pc;
		if (coco_string_equal(name, L"Main")) gen->progStart = gen->pc; 
		tab->OpenScope(); 
		Expect(13 /* "(" */);
		Expect(14 /* ")" */);
		Expect(15 /* "{" */);
		gen->Emit(CodeGenerator::ENTER, 0); adr = gen->pc - 2; 
		while (StartOf(1)) {
			if (la->kind == 25 /* "int" */ || la->kind == 26 /* "bool" */) {
				VarDecl();
			} else {
				Stat();
			}
		}
		Expect(16 /* "}" */);
		gen->Emit(CodeGenerator::LEAVE); gen->Emit(CodeGenerator::RET);
		gen->Patch(adr, tab->topScope->nextAdr);
		tab->CloseScope(); 
}

void Parser::VarDecl() {
		wchar_t* name; int type; 
		Type(type);
		Ident(name);
		tab->NewObj(name, var, type); 
		while (la->kind == 27 /* "," */) {
			Get();
			Ident(name);
			tab->NewObj(name, var, type); 
		}
		Expect(18 /* ";" */);
}

void Parser::Stat() {
		int type; wchar_t* name; Obj *obj;
		int adr, adr2, loopstart; 
		switch (la->kind) {
		case _ident: {
			Ident(name);
			obj = tab->Find(name); 
			if (la->kind == 17 /* "=" */) {
				Get();
				if (obj->kind != var) Err(L"cannot assign to procedure"); 
				Expr(type);
				Expect(18 /* ";" */);
				if (type != obj->type) Err(L"incompatible types");
				if (obj->level == 0) gen->Emit(CodeGenerator::STOG, obj->adr);
				else gen->Emit(CodeGenerator::STO, obj->adr); 
			} else if (la->kind == 13 /* "(" */) {
				Get();
				Expect(14 /* ")" */);
				Expect(18 /* ";" */);
				if (obj->kind != proc) Err(L"object is not a procedure");
				gen->Emit(CodeGenerator::CALL, obj->adr); 
			} else SynErr(30);
			break;
		}
		case 19 /* "if" */: {
			Get();
			Expect(13 /* "(" */);
			Expr(type);
			Expect(14 /* ")" */);
			if (type != boolean) Err(L"boolean type expected");
			gen->Emit(CodeGenerator::FJMP, 0); adr = gen->pc - 2; 
			Stat();
			if (la->kind == 20 /* "else" */) {
				Get();
				gen->Emit(CodeGenerator::JMP, 0); adr2 = gen->pc - 2;
				gen->Patch(adr, gen->pc);
				adr = adr2; 
				Stat();
			}
			gen->Patch(adr, gen->pc); 
			break;
		}
		case 21 /* "while" */: {
			Get();
			loopstart = gen->pc; 
			Expect(13 /* "(" */);
			Expr(type);
			Expect(14 /* ")" */);
			if (type != boolean) Err(L"boolean type expected");
			gen->Emit(CodeGenerator::FJMP, 0); adr = gen->pc - 2; 
			Stat();
			gen->Emit(CodeGenerator::JMP, loopstart); gen->Patch(adr, gen->pc); 
			break;
		}
		case 22 /* "read" */: {
			Get();
			Ident(name);
			Expect(18 /* ";" */);
			obj = tab->Find(name);
			if (obj->type != integer) Err(L"integer type expected");
			gen->Emit(CodeGenerator::READ);
			if (obj->level == 0) gen->Emit(CodeGenerator::STOG, obj->adr);
			else gen->Emit(CodeGenerator::STO, obj->adr); 
			break;
		}
		case 23 /* "write" */: {
			Get();
			Expr(type);
			Expect(18 /* ";" */);
			if (type != integer) Err(L"integer type expected");
			gen->Emit(CodeGenerator::WRITE); 
			break;
		}
		case 15 /* "{" */: {
			Get();
			while (StartOf(1)) {
				if (StartOf(2)) {
					Stat();
				} else {
					VarDecl();
				}
			}
			Expect(16 /* "}" */);
			break;
		}
		default: SynErr(31); break;
		}
}

void Parser::EHLang() {
		wchar_t* name;
		InitDeclarations(); 
		Expect(24 /* "program" */);
		Ident(name);
		tab->OpenScope(); 
		Expect(15 /* "{" */);
		while (la->kind == 25 /* "int" */ || la->kind == 26 /* "bool" */) {
			VarDecl();
		}
		while (la->kind == 12 /* "void" */) {
			ProcDecl();
		}
		Expect(16 /* "}" */);
		tab->CloseScope(); 
}

void Parser::Type(int &type) {
		type = undef; 
		if (la->kind == 25 /* "int" */) {
			Get();
			type = integer; 
		} else if (la->kind == 26 /* "bool" */) {
			Get();
			type = boolean; 
		} else SynErr(32);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	EHLang();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 28;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[3][30] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,T, x,x,x,T, x,T,T,T, x,T,T,x, x,x},
		{x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,T, x,x,x,T, x,T,T,T, x,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ident expected"); break;
			case 2: s = coco_string_create(L"number expected"); break;
			case 3: s = coco_string_create(L"\"==\" expected"); break;
			case 4: s = coco_string_create(L"\"<\" expected"); break;
			case 5: s = coco_string_create(L"\">\" expected"); break;
			case 6: s = coco_string_create(L"\"+\" expected"); break;
			case 7: s = coco_string_create(L"\"-\" expected"); break;
			case 8: s = coco_string_create(L"\"*\" expected"); break;
			case 9: s = coco_string_create(L"\"/\" expected"); break;
			case 10: s = coco_string_create(L"\"true\" expected"); break;
			case 11: s = coco_string_create(L"\"false\" expected"); break;
			case 12: s = coco_string_create(L"\"void\" expected"); break;
			case 13: s = coco_string_create(L"\"(\" expected"); break;
			case 14: s = coco_string_create(L"\")\" expected"); break;
			case 15: s = coco_string_create(L"\"{\" expected"); break;
			case 16: s = coco_string_create(L"\"}\" expected"); break;
			case 17: s = coco_string_create(L"\"=\" expected"); break;
			case 18: s = coco_string_create(L"\";\" expected"); break;
			case 19: s = coco_string_create(L"\"if\" expected"); break;
			case 20: s = coco_string_create(L"\"else\" expected"); break;
			case 21: s = coco_string_create(L"\"while\" expected"); break;
			case 22: s = coco_string_create(L"\"read\" expected"); break;
			case 23: s = coco_string_create(L"\"write\" expected"); break;
			case 24: s = coco_string_create(L"\"program\" expected"); break;
			case 25: s = coco_string_create(L"\"int\" expected"); break;
			case 26: s = coco_string_create(L"\"bool\" expected"); break;
			case 27: s = coco_string_create(L"\",\" expected"); break;
			case 28: s = coco_string_create(L"??? expected"); break;
			case 29: s = coco_string_create(L"invalid Factor"); break;
			case 30: s = coco_string_create(L"invalid Stat"); break;
			case 31: s = coco_string_create(L"invalid Stat"); break;
			case 32: s = coco_string_create(L"invalid Type"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	
	ErrorItem* ei = new ErrorItem(ErrorItem::ERROR, line, col, s);
	errors.append(ei);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	ErrorItem* ei = new ErrorItem(ErrorItem::ERROR, line, col, coco_string_create(s));
	errors.append(ei);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	ErrorItem* ei = new ErrorItem(ErrorItem::WARNING, line, col, coco_string_create(s));
	errors.append(ei);
}


} // namespace

