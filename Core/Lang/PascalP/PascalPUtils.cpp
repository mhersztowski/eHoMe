#include "PascalPUtils.h"

int P_peek(FILE*  f)
{
    int ch;

    ch = fgetc(f);
    if (ch == EOF)
        return EOF;
    ungetc(ch, f);
    return (ch == '\n') ? ' ' : ch;
}


/* Check if at end of file, using Pascal "eof" semantics.  End-of-file for
   stdin is broken; remove the special case for it to be broken in a
   different way. */

int P_eof(FILE*  f)
{
    register int ch;

    if (feof(f))
        return 1;
    ch = fgetc(f);
    if (ch == EOF)
        return 1;
    ungetc(ch, f);
    return 0;
}


/* Check if at end of line (or end of entire file). */

int P_eoln(FILE*  f)
{
    register int ch;

    ch = fgetc(f);
    if (ch == EOF)
        return 1;
    ungetc(ch, f);
    return (ch == '\n');
}


int P_setequal(register long* s1, register long* s2)              /* s1 = s2 */
{
    register int size = *s1++;
    if (*s2++ != size)
        return 0;
    while (--size >= 0) {
        if (*s1++ != *s2++)
            return 0;
    }
    return 1;
}

int P_subset(register long*  s1, register long* s2)             /* s1 <= s2 */
{
    register int sz1 = *s1++, sz2 = *s2++;
    if (sz1 > sz2)
        return 0;
    while (--sz1 >= 0) {
        if (*s1++ & ~*s2++)
            return 0;
    }
    return 1;
}

long* P_setcpy(register long* d, register long*  s)                /* d := s */
{
    register long* save_d = d;

//#ifdef SETCPY_MEMCPY
    memcpy(d, s, (*s + 1) * sizeof(long));
//#else
//    register int i = *s + 1;
//    while (--i >= 0)
//        *d++ = *s++;
//#endif
    return save_d;
}

long* P_addset(register long*  s, register unsigned val)              /* s := s + [val] */
{
    register long* sbase = s;
    register int bit, size;
    bit = val % SETBITS;
    val /= SETBITS;
    size = *s;
    if (++val > size) {
        s += size;
        while (val > size)
            *++s = 0, size++;
        *sbase = size;
    }
    else
        s += val;
    *s |= 1L << bit;
    return sbase;
}

/* s is a "smallset", i.e., a 32-bit or less set stored
   directly in a long. */
long* P_expset(register long* d, register long s)                /* d := s */
{
    if (s) {
        d[1] = s;
        *d = 1;
    }
    else
        *d = 0;
    return d;
}

long* P_setunion(register long* d, register long* s1, register long* s2)         /* d := s1 + s2 */
{
    long* dbase = d++;
    register int sz1 = *s1++, sz2 = *s2++;
    while (sz1 > 0 && sz2 > 0) {
        *d++ = *s1++ | *s2++;
        sz1--, sz2--;
    }
    while (--sz1 >= 0)
        *d++ = *s1++;
    while (--sz2 >= 0)
        *d++ = *s2++;
    *dbase = d - dbase - 1;
    return dbase;
}

long* P_setdiff(register long* d, register long* s1, register long* s2)          /* d := s1 - s2 */
{
    long* dbase = d++;
    register int sz1 = *s1++, sz2 = *s2++;
    while (--sz1 >= 0 && --sz2 >= 0)
        *d++ = *s1++ & ~*s2++;
    if (sz1 >= 0) {
        while (sz1-- >= 0)
            *d++ = *s1++;
    }
    while (--d > dbase && !*d);
    *dbase = d - dbase;
    return dbase;
}

long* P_setint(register long* d, register long* s1, register long* s2)           /* d := s1 * s2 */
{
    long* dbase = d++;
    register int sz1 = *s1++, sz2 = *s2++;
    while (--sz1 >= 0 && --sz2 >= 0)
        *d++ = *s1++ & *s2++;
    while (--d > dbase && !*d);
    *dbase = d - dbase;
    return dbase;
}

int P_inset(register unsigned val, register long*  s)                 /* val IN s */
{
    register int bit;
    bit = val % SETBITS;
    val /= SETBITS;
    if (val < *s++ && ((1L << bit) & s[val]))
        return 1;
    return 0;
}

