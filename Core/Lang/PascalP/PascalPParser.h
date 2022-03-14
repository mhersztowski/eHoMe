#ifndef __PASCALPPARSER_H__
#define __PASCALPPARSER_H__

#include "Core/Types/Base.h"
#include "PascalPUtils.h"

/*
class PascalPLex;
class PascalPSymTab;
class PascalPVM;
*/

#define displimit       20
#define maxlevel        10
#define intsize         1
#define intal           1
#define realsize        1
#define realal          1
#define charsize        1
#define charal          1
#define charmax         1
#define boolsize        1
#define boolal          1
#define ptrsize         1
#define adral           1
#define setsize         1
#define setal           1
#define stackal         1
#define stackelsize     1
#define strglgth        16
#define sethigh         47
#define setlow          0
#define ordmaxchar      63
#define ordminchar      0
#define maxint          32767
#define lcaftermarkstack  5
#define fileal          charal
/* stackelsize = minimum size for 1 stackelement
               = k*stackal
   stackal     = scm(all other al-constants)
   charmax     = scm(charsize,charal)
                 scm = smallest common multiple
   lcaftermarkstack >= 4*ptrsize+max(x-size)
                     = k1*stackelsize  */
#define maxstack        1
#define parmal          stackal
#define parmsize        stackelsize
#define recal           stackal
#define filebuffer      4
#define maxaddr         maxint


typedef enum {
    ident, intconst, realconst, stringconst, notsy, mulop, addop, relop,
    lparent, rparent, lbrack, rbrack, comma, semicolon, period, arrow, colon,
    becomes, labelsy, constsy, typesy, varsy, funcsy, progsy, procsy, setsy,
    packedsy, arraysy, recordsy, filesy, forwardsy, beginsy, ifsy, casesy,
    repeatsy, whilesy, forsy, withsy, gotosy, endsy, elsesy, untilsy, ofsy,
    dosy, tosy, downtosy, thensy, othersy
} symbol;
typedef enum {
    mul, rdiv, andop, idiv, imod, plus, minus, orop, ltop, leop, geop, gtop,
    neop, eqop, inop, noop
} operator_;
typedef long setofsys[3];

typedef enum {
    letter, number, special, illegal, chstrquo, chcolon, chperiod, chlt, chgt,
    chlparen, chspace
} chtp;

/*constants*/
/***********/
typedef long setty[sethigh / 32 + 2];

typedef enum {
    reel, pset, strg
} cstclass;

typedef struct constant {
    cstclass cclass;
    union {
        char rval[strglgth];
        setty pval;
        struct {
            char slgth;
            char sval[strglgth];
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


/*names*/
/*******/

typedef enum {
    types, konst, vars, field, proc, func
} idclass;
typedef long setofids;

typedef enum {
    actual, formal
} idkind;
typedef char alpha[8];

typedef struct identifier {
    alpha name;
    struct identifier* llink, * rlink;
    struct structure* idtype;
    struct identifier* next;
    /* p2c: pcom.p, line 145:
     * Note: Field width for klass assumes enum idclass has 6 elements [105] */
    unsigned klass : 3;
    union {
        valu values;
        struct {
            unsigned vkind : 1;
            /* p2c: pcom.p, line 148:
             * Note: Field width for vkind assumes enum idkind has 2 elements [105] */
            unsigned vlev : 4, vaddr : 15;
        } U2;
        unsigned fldaddr;
        struct {
            /* p2c: pcom.p, line 150: Note:
             * Field width for pfdeckind assumes enum declkind has 2 elements [105] */
            unsigned pfdeckind : 1;
            union {
                unsigned key;
                struct {
                    unsigned pflev : 4;
                    long pfname;
                    /* p2c: pcom.p, line 153:
                     * Note: Field width for pfkind assumes enum idkind has 2 elements [105] */
                    unsigned pfkind : 1;
                    union {
                        struct {
                            unsigned forwdecl : 1, externl : 1;
                        } U0;
                    } UU;
                } U1;
            } UU;
        } U4;
    } UU;
} identifier;


typedef char disprange;

typedef enum {
    blck, crec, vrec, rec
} where;



/*data structures*/
/*****************/
typedef char levrange;

typedef short addrrange;

typedef enum {
    scalar, subrange, pointer, power, arrays, records, files, tagfld, variant
} structform;
typedef enum {
    standard, declared
} declkind;

typedef struct structure {
    unsigned marked : 1;   /*for test phase only*/
    unsigned size : 15;
    /* p2c: pcom.p, line 121: Note:
     * Field width for form assumes enum structform has 9 elements [105] */
    unsigned form : 4;
    union {
        struct {
            /* p2c: pcom.p, line 122: Note:
             * Field width for scalkind assumes enum declkind has 2 elements [105] */
            unsigned scalkind : 1;
            union {
                struct identifier* fconst;
            } UU;
        } U0;
        struct {
            struct structure* rangetype;
            valu min, max;
        } U1;
        struct structure* eltype;
        struct structure* elset;
        struct {
            struct structure* aeltype, * inxtype;
        } U4;
        struct {
            struct identifier* fstfld;
            struct structure* recvar;
        } U5;
        struct structure* filtype;
        struct {
            struct identifier* tagfieldp;
            struct structure* fstvar;
        } U7;
        struct {
            struct structure* nxtvar, * subvar;
            valu varval;
        } U8;
    } UU;
} structure;



/*expressions*/
/*************/
typedef enum {
    cst, varbl, expr
} attrkind;
typedef enum {
    drct, indrct, inxd
} vaccess;

typedef struct attr {
    structure* typtr;
    attrkind kind;
    union {
        valu cval;
        struct {
            vaccess access;
            union {
                struct {
                    levrange vlevel;
                    addrrange dplmt;
                } U0;
                addrrange idplmt;
            } UU;
        } U1;
    } UU;
} attr;

typedef struct testpointer {
    structure* elt1, * elt2;
    struct testpointer* lasttestp;
} testpointer;

/*labels*/
/********/

typedef struct labl {
    struct labl* nextlab;
    bool defined_;
    long labval, labname;
} labl;

typedef struct filerec {
    alpha filename;
    struct filerec* nextfile;
} filerec;


class PascalPParser {

public:
    /*basic symbols*/
/***************/

   

    /*-------------------------------------------------------------------------*/


    typedef struct _REC_display {
        /*=blck:   id is variable id*/
        identifier* fname;
        labl* flabel;   /*=crec:   id is field id in record with*/
      /* p2c: pcom.p, line 254:
       * Note: Field width for occur assumes enum where has 4 elements [105] */
       /* constant address*/
        unsigned occur : 2;
        union {
            struct {
                levrange clev;   /*=vrec:   id is field id in record with*/
                addrrange cdspl;   /* variable address*/
            } U1;
            addrrange vdspl;
        } UU;
    } _REC_display;

    typedef struct _REC_errlist {
        long pos;
        unsigned nmr : 9;
    } _REC_errlist;

    _TEXT prr;   /* comment this out when compiling with pcom */
/*returned by source program scanner
 insymbol:
 **********/

    symbol sy;   /*last symbol*/
    operator_ op;   /*classification of last symbol*/
    valu val;   /*value of last constant*/
    long lgth;   /*length of last string constant*/
    alpha id;   /*last identifier (possibly truncated)*/
    char kk;   /*nr of chars in last identifier*/
    char ch;   /*last character*/
    bool eol;   /*end of line flag*/


    /*counters:*/
    /***********/

    long chcnt;   /*character counter*/
    addrrange lc, ic;   /*data location and instruction counter*/
    long linecount;


    /*switches:*/
    /***********/

    bool dp;   /*declaration part*/
    bool prterr;
    /*to allow forward references in pointer type
                                      declaration by suppressing error message*/
    bool list, prcode, prtables;
    /*output options for
                                        -- source program listing
                                        -- printing symbolic code
                                        -- displaying ident and struct tables
                                        --> procedure option*/
    bool debug;


    /*pointers:*/
    /***********/
    structure* parmptr, * intptr, * realptr, * charptr, * boolptr, * nilptr,
        * textptr;
    /*pointers to entries of standard ids*/
    identifier* utypptr, * ucstptr, * uvarptr, * ufldptr, * uprcptr, * ufctptr;
    /*pointers to entries for undeclared ids*/
    identifier* fwptr;   /*head of chain of forw decl type ids*/
    filerec* fextfilep;   /*head of chain of external files*/
    testpointer* globtestp;   /*last testpointer*/


    /*bookkeeping of declaration levels:*/
    /************************************/

    levrange level;   /*current static level*/
    disprange disx;   /*level of last id searched by searchid*/
    disprange top;   /*top of display*/

    /*where:   means:*/
    _REC_display display[displimit + 1];   /* --> procedure withstatement*/


    /*error messages:*/
    /*****************/

    char errinx;   /*nr of errors in current source line*/
    _REC_errlist errlist[10];
    bool input_ok = true;



    /*expression compilation:*/
    /*************************/

    attr gattr;   /*describes the expr currently compiled*/

    /*structured constants:*/
/***********************/

    static setofsys constbegsys, simptypebegsys, typebegsys, blockbegsys,
        selectsys, facbegsys, statbegsys, typedels;
    static chtp chartp[256];
    /*nr. of res. words*/
    static alpha rw[35];
    /*nr. of res. words + 1*/
    static char frw[9];
    /*nr. of res. words*/
    static symbol rsy[35];
    static symbol ssy[256];
    /*nr. of res. words*/
    static operator_ rop[35];
    static operator_ sop[256];
    static alpha na[35];
    static char mn[61][4];
    static char sna[23][4];
    static schar cdx[61];
    static schar pdx[23];
    static long ordint[256];

    static long intlabel, mxint10, digmax;
    
    void initscalars();

    static void initPascalP();

    static void initsets();

    static void reswords();
    static void symbols();
    static void rators();
    static void instrmnemonics();
    static void procmnemonics();
    static void chartypes();
    static void initdx();
    static void inittables();
  

protected:

    typedef struct LOC_insymbol {
        bool test;
    } LOC_insymbol;

    struct LOC_printtables {
        disprange lim;
    };

    typedef char oprange;


    typedef struct caseinfo {
        struct caseinfo* next;
        long csstart, cslab;
    } caseinfo;


    /* Local variables for block: */
    struct LOC_block {
        setofsys fsys;
        identifier* fprocp;
        bool test;
    };

    struct LOC_typ {
        struct LOC_block* LINK;
        addrrange displ;
    };

    struct LOC_procdeclaration {
        struct LOC_block* LINK;
        bool forw;
    };

#define cstoccmax       65
#define cixmax          1000

    struct LOC_body {
        struct LOC_block* LINK;
        constant* cstptr[cstoccmax];
        char cstptrix;
        long topnew, topmax;
        addrrange lcmax;
    };

    struct LOC_statement {
        struct LOC_body* LINK;
        setofsys fsys;
        identifier* lcp;
    };

    struct LOC_call {
        struct LOC_statement* LINK;
        setofsys fsys;
        identifier* fcp;
        char lkey;
    };

    struct LOC_expression {
        struct LOC_statement* LINK;
    };

    struct LOC_simpleexpression {
        struct LOC_expression* LINK;
    };

    struct LOC_term {
        struct LOC_simpleexpression* LINK;
    };


    void mark_(long** p);
    void release_(long* p);
    void endofline(void);
    void error(long ferrnr);
    void nextch(struct LOC_insymbol* LINK);
    void options(struct LOC_insymbol* LINK);
    void insymbol(void);
    void enterid(identifier* fcp);
    void searchsection(identifier* fcp, identifier** fcp1);
    void searchid(setofids fidcls, identifier** fcp);
    void getbounds(structure* fsp, long* fmin, long* fmax);
    long alignquot(structure* fsp);
    void align(structure* fsp, addrrange* flc);
    void markstp(structure* fp);
    void markctp(identifier* fp);
    void marker(struct LOC_printtables* LINK);
    void followstp(structure* fp, struct LOC_printtables* LINK);
    void followctp(identifier* fp, struct LOC_printtables* LINK);
    void printtables(bool fb);
    void genlabel(long* nxtlab);
    void skip(long* fsys, struct LOC_block* LINK);
    void constant_(long* fsys, structure** fsp, valu* fvalu, struct LOC_block* LINK);
    bool equalbounds(structure* fsp1, structure* fsp2, struct LOC_block* LINK);
    bool comptypes(structure* fsp1, structure* fsp2, struct LOC_block* LINK);
    bool string(structure* fsp, struct LOC_block* LINK);
    void simpletype(long* fsys, structure** fsp, addrrange* fsize, struct LOC_typ* LINK);
    void fieldlist(long* fsys, structure** frecvar, struct LOC_typ* LINK);
    void typ(long* fsys, structure** fsp, addrrange* fsize, struct LOC_block* LINK);
    void labeldeclaration(struct LOC_block* LINK);
    void constdeclaration(struct LOC_block* LINK);
    void typedeclaration(struct LOC_block* LINK);
    void vardeclaration(struct LOC_block* LINK);
    void parameterlist(long* fsy, identifier** fpar, struct LOC_procdeclaration* LINK);
    void procdeclaration(symbol fsy, struct LOC_block* LINK);
    void mes(long i, struct LOC_body* LINK);
    void putic(struct LOC_body* LINK);
    void gen0(oprange fop, struct LOC_body* LINK);
    void gen1(oprange fop, long fp2, struct LOC_body* LINK);
    void gen2(oprange fop, long fp1, long fp2, struct LOC_body* LINK);
    void gentypindicator(structure* fsp, struct LOC_body* LINK);
    void gen0t(oprange fop, structure* fsp, struct LOC_body* LINK);
    void gen1t(oprange fop, long fp2, structure* fsp, struct LOC_body* LINK);
    void gen2t(oprange fop, long fp1, long fp2, structure* fsp, struct LOC_body* LINK);
    void load(struct LOC_body* LINK);
    void store(attr* fattr, struct LOC_body* LINK);
    void loadaddress(struct LOC_body* LINK);
    void genfjp(long faddr, struct LOC_body* LINK);
    void genujpxjp(oprange fop, long fp2, struct LOC_body* LINK);
    void gencupent(oprange fop, long fp1, long fp2, struct LOC_body* LINK);
    void checkbnds(structure* fsp, struct LOC_body* LINK);
    void putlabel(long labname, struct LOC_body* LINK);
    void selector(long* fsys, identifier* fcp, struct LOC_statement* LINK);
    void variable(long* fsys, struct LOC_call* LINK);
    void getputresetrewrite(struct LOC_call* LINK);

    void read(struct LOC_call* LINK);
    void write(struct LOC_call* LINK);
    void pack(struct LOC_call* LINK);
    void unpack(struct LOC_call* LINK);
    void new_(struct LOC_call* LINK);
    void mark__(struct LOC_call* LINK);
    void release__(struct LOC_call* LINK);
    void abs_(struct LOC_call* LINK);
    void sqr_(struct LOC_call* LINK);
    void trunc_(struct LOC_call* LINK);
    void odd_(struct LOC_call* LINK);
    void ord_(struct LOC_call* LINK);
    void chr_(struct LOC_call* LINK);
    void predsucc(struct LOC_call* LINK);
    void eof_(struct LOC_call* LINK);
    void callnonstandard(struct LOC_call* LINK);
    void call(long* fsys_, identifier* fcp_, struct LOC_statement* LINK);

    void factor(long* fsys, struct LOC_term* LINK);
    void term(long* fsys, struct LOC_simpleexpression* LINK);
    void simpleexpression(long* fsys, struct LOC_expression* LINK);

    void expression(long* fsys, struct LOC_statement* LINK);
    void assignment(identifier* fcp, struct LOC_statement* LINK);
    void gotostatement(struct LOC_statement* LINK);
    void compoundstatement(struct LOC_statement* LINK);
    void ifstatement(struct LOC_statement* LINK);
    void casestatement(struct LOC_statement* LINK);
    void repeatstatement(struct LOC_statement* LINK);
    void whilestatement(struct LOC_statement* LINK);
    void forstatement(struct LOC_statement* LINK);
    void withstatement(struct LOC_statement* LINK);
    void statement(long* fsys_, struct LOC_body* LINK);
    void body(long* fsys, struct LOC_block* LINK);
    void block(long* fsys_, symbol fsy, identifier* fprocp_);
    void programme(long* fsys);

    void stdnames(void);
    void enterstdtypes(void);
    void entstdnames(void);
    void enterundecl(void);




};



#endif /* __PASCALPPARSER_H__ */