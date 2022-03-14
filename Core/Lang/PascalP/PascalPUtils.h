#ifndef __PASCALPUTILS_H__
#define __PASCALPUTILS_H__

#include "Core/Types/Base.h"

typedef struct {
    FILE* f;
    //FILEBUFNC(f, char);
    //char name[_FNSIZE];
} _TEXT;

int P_peek(FILE*);
int P_eof(FILE*);
int P_eoln(FILE*);

typedef signed   char schar;
typedef unsigned char uchar;

#define SETBITS  32

int P_setequal(long*, long*);
int P_subset(long*, long*);
long* P_setcpy(long*, long*);

long* P_addset(long*, unsigned);
long* P_expset(long*, long);
long* P_setdiff(long*, long*, long*);
long* P_setint(long*, long*, long*);
long* P_setunion(long*, long*, long*);
int   P_inset(unsigned, long*);

#endif /* __PASCALPUTILS_H__ */
