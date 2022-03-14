#ifndef __PASCALPLEX_H__
#define __PASCALPLEX_H__

#include "Core/Types/Base.h"
#include "Core/Types/Str.h"
#include "Core/Utils/Singleton.h"
#include "Core/Types/Vector.h"


class PascalPSource;

#define STRGLGTH    16
#define SETHIGH     47
#define ORDMAXCHAR  63
#define ORDMINCHAR   0
#define MAXINT          32767

class PascalPGlobals : public Singleton<PascalPGlobals> {

public:
    PascalPGlobals();

    typedef enum {
        LETTER, NUMBER, SPECIAL, ILLEGAL, CHSTRQUO, CHCOLON, CHPERIOD, CHLT, CHGT,
        CHLPAREN, CHSPACE
    } CharType;

    CharType chartype[256];     //chartp

    typedef enum {
        IDENT, INTCONST, REALCONST, STRINGCONST, NOTSY, MULOP, ADDOP, RELOP,
        LPARENT, RPARENT, LBRACK, RBRACK, COMMA, SEMICOLON, PERIOD, ARROW, COLON,
        BECOMES, LABELSY, CONSTSY, TYPESY, VARSY, FUNCSY, PROGSY, PROCSY, SETSY,
        PACKEDSY, ARRAYSY, RECORDSY, FILESY, FORWARDSY, BEGINSY, IFSY, CASESY,
        REPEATSY, WHILESY, FORSY, WITHSY, GOTOSY, ENDSY, ELSESY, UNTILSY, OFSY,
        DOSY, TOSY, DOWNTOSY, THENSY, OTHERSY
    } Symbol;

    typedef enum {
        MUL, RDIV, ANDOP, IDIV, IMOD, PLUS, MINUS, OROP, LTOP, LEOP, GEOP, GTOP,
        NEOP, EQOP, INOP, NOOP
    } Operator_;

    /*constants*/
/***********/
    typedef long setty[SETHIGH / 32 + 2];

    typedef enum {
        reel, pset, strg
    } cstclass;

    typedef struct constant {
        cstclass cclass;
        union {
            char rval[STRGLGTH];
            setty pval;
            struct {
                char slgth;
                char sval[STRGLGTH];
            } U2;
        } UU;
    } constant;

    typedef struct valu {
        /*intval never set nor tested*/
        bool intval;
        union {
            long ival;
            constant* valp;
        } UU;
    } valu;


    Vector<String> rw;
    Symbol rsy[35];
    Operator_ rop[35];

    long digmax;
};


class PascalPLex {

public:

    /*
    void doError(const char* msg) {
        error_line = line; error_pos = pos; error_msg = msg; error = true;
    }

    void doError(const char* msg, int line, int pos) {
        error_line = line; error_pos = pos; error_msg = msg; error = true;
    }

    bool error;
    int error_line;
    int error_pos;
    QString error_msg;
    */

    PascalPLex();

    void error(long ferrnr);
    void insymbol();
    void skip(long* fsys);

    String id;
    PascalPGlobals::Symbol sym;   /*last symbol*/
    PascalPGlobals::Operator_ op; /*classification of last symbol*/
    long lgth;                    /*length of last string constant*/


    bool errors;
    PascalPGlobals::valu val;   /*value of last constant*/

    bool list, prcode, prtables;
    /*output options for
                                        -- source program listing
                                        -- printing symbolic code
                                        -- displaying ident and struct tables
                                        --> procedure option*/
    bool debug;

protected:

    PascalPSource* src;
    long mxint10;

    void skipwhitespace();
    void options();
};



class PascalPSource {

public:
    PascalPSource() : line(0), pos(-1) {}

    enum {
        EOS = 0,
        EOL = 13
    };

    virtual char next() = 0;

    virtual char forward() = 0;

    char ch;
    int line;
    int pos;
};

class PascalPStringSource : public PascalPSource {
public:
    PascalPStringSource(const String& _src) : PascalPSource(), src(_src), src_pos(-1) {
        src_size = src.size();
        next();
    }

    char next() override;
    char forward() override;

private:
    const String& src;
    int src_pos;
    int src_size;
};



#endif /* __PASCALPLEX_H__ */
