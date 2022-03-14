

#if !defined(EHLang_COCO_PARSER_H__)
#define EHLang_COCO_PARSER_H__

#include "SymbolTable.h"
#include "CodeGenerator.h"
#include "wchar.h"


#include "Core/Types/Vector.h"
#include "Scanner.h"


namespace EHLang {


class Logic;

class Errors {
public:
	int count;			// number of errors detected

	Errors();
	
	~Errors() {
        for (int i=0 ;i < errors.size(); i++) {
            coco_string_delete(errors[i]->msg);
            delete errors[i];
        }
        errors.clear();
    }
	
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	
    class ErrorItem {

    public:
	
		enum Kind {
			ERROR,
			WARNING
		};

        ErrorItem(Kind ekind, int line_n, int col_n, wchar_t *s)
            : kind(ekind), line(line_n), col(col_n), msg(s) {

        }

        int kind;
        int line;
        int col;
        wchar_t *msg;
    };

    Vector<ErrorItem*> errors;
	

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_ident=1,
		_number=2
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

int // types
	  undef, integer, boolean;

	int // object kinds
	  var, proc;
	
	Logic* logic;
	SymbolTable   *tab;
	CodeGenerator *gen;

	void Err(wchar_t* msg) {
		errors->Error(la->line, la->col, msg);
	}

	void InitDeclarations() { // it must exist
		undef = 0; integer = 1; boolean = 2; // types
		var = 0; proc = 1; // object kinds

	}


  
/*--------------------------------------------------------------------------*/


	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void Expr(int &type);
	void SimExpr(int &type);
	void Term(int &type);
	void Factor(int &type);
	void Ident(wchar_t* &name);
	void ProcDecl();
	void VarDecl();
	void Stat();
	void EHLang();
	void Type(int &type);

	void Parse();

}; // end Parser

} // namespace


#endif

