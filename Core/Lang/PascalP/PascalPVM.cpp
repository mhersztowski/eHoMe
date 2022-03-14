#include "PascalPVM.h"
#include "PascalPUtils.h"


void PascalPVM::errori(char* string)
{
    //printf("\n%.25s\n", string);
    //pmd();
    //longjmp(_JL1, 1);
}  /*errori*/

PascalPVM::address PascalPVM::base(long ld)
{
    address ad;

    ad = mp;
    while (ld > 0) {
        ad = store[ad + 1].vm;
        ld--;
    }
    return ad;
} 


void PascalPVM::compare()
{
    /*comparing is only correct if result by comparing integers will be*/
    long i1 = store[sp].va;
    long i2 = store[sp + 1].va;
    long i = 0;
    bool b = true;

    while (b && i != q) {
        if (store[i1 + i].vi == store[i2 + i].vi)
            i++;
        else
            b = false;
    }
}  /*compare*/

void PascalPVM::callsp() {
	
	switch (q) {

    case 14:   /*sin*/
        store[sp].vr = sin(store[sp].vr);
        break;

    case 15:   /*cos*/
        store[sp].vr = cos(store[sp].vr);
        break;

    case 16:   /*exp*/
        store[sp].vr = exp(store[sp].vr);
        break;

    case 17:   /*log*/
        store[sp].vr = log(store[sp].vr);
        break;

    case 18:   /*sqt*/
        store[sp].vr = sqrt(store[sp].vr);
        break;

    case 19:   /*atn*/
        store[sp].vr = atan(store[sp].vr);
        break;

	}

}



void PascalPVM::run()
{
    _REC_code* WITH;

    address ad;
    bool b;
    long i, i1, i2;

    pc = 0;
    sp = -1;
    mp = 0;
    np = MAXSTK + 1;
    ep = 5;
    interpreting = true;

    while (interpreting) {

        WITH = &code[pc / 2];
        
        if (pc & 1) {
            op = WITH->op2;
            p = WITH->p2;
            q = WITH->q2;
        }
        else {
            op = WITH->op1;
            p = WITH->p1;
            q = WITH->q1;
        }
        pc++;

        switch (op) {

        case 0:   /*lod*/
            ad = base(p) + q;
            sp++;
            store[sp] = store[ad];
            break;

        case 1:   /*ldo*/
            sp++;
            store[sp] = store[q];
            break;

        case 2:   /*str*/
            store[base(p) + q] = store[sp];
            sp--;
            break;

        case 3:   /*sro*/
            store[q] = store[sp];
            sp--;
            break;

        case 4:   /*lda*/
            sp++;
            store[sp].va = base(p) + q;
            break;

        case 5:   /*lao*/
            sp++;
            store[sp].va = q;
            break;

        case 6:   /*sto*/
            store[store[sp - 1].va] = store[sp];
            sp -= 2;
            break;

        case 7:   /*ldc*/
            sp++;
            if (p == 1)
                store[sp].vi = q;
            else {
                if (p == 6)
                    store[sp].vc = q;
                else {
                    if (p == 3)
                        store[sp].vb = (q == 1);
                    else   /* load nil */
                        store[sp].va = MAXSTR;
                }
            }
            break;

        case 8:   /*lci*/
            sp++;
            store[sp] = store[q];
            break;

        case 9:   /*ind*/
            ad = store[sp].va + q;
            /* q is a number of storage units */
            store[sp] = store[ad];
            break;

        case 10:   /*inc*/
            store[sp].vi += q;
            break;

        case 11:   /*mst*/
          /*p=level of calling procedure minus level of called
                              procedure + 1;  set dl and sl, increment sp*/
                              /* then length of this element is
                                 max(intsize,realsize,boolsize,charsize,ptrsize */
            store[sp + 2].vm = base(p);
            /* the length of this element is ptrsize */
            store[sp + 3].vm = mp;
            /* idem */
            store[sp + 4].vm = ep;
            /* idem */
            sp += 5;
            break;

        case 12:   /*cup*/
          /*p=no of locations for parameters, q=entry point*/
            mp = sp - p - 4;
            store[mp + 4].vm = pc;
            pc = q;
            break;

        case 13:   /*ent*/
            if (p == 1) {
                sp = mp + q;   /*q = length of dataseg*/
                if (sp > np)
                    errori(" store overflow\t         ");
            }
            else {
                ep = sp + q;
                if (ep > np)
                    errori(" store overflow\t         ");
            }
            break;
            /*q = max space required on stack*/

        case 14:   /*ret*/
            switch (p) {

            case 0:
                sp = mp - 1;
                break;

            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                sp = mp;
                break;
            }
            pc = store[mp + 4].vm;
            ep = store[mp + 3].vm;
            mp = store[mp + 2].vm;
            break;

        case 15:   /*csp*/
            callsp();
            break;

        case 16:   /*ixa*/
            i = store[sp].vi;
            sp--;
            store[sp].va += q * i;
            break;

        case 17:   /*equ*/
            sp--;
            switch (p) {
                case 1: store[sp].vb = (store[sp].vi == store[sp + 1].vi); break;
                case 0: store[sp].vb = (store[sp].va == store[sp + 1].va); break;
                case 6: store[sp].vb = (store[sp].vc == store[sp + 1].vc); break;
                case 2: store[sp].vb = (store[sp].vr == store[sp + 1].vr); break; 
                case 3: store[sp].vb = (store[sp].vb == store[sp + 1].vb); break;
                case 4: store[sp].vb = P_setequal(store[sp].vs, store[sp + 1].vs); break;
                case 5: compare(); store[sp].vb = b; break;
            }/*case p*/
            break;

        case 18:   /*neq*/
            sp--;
            switch (p) {
                case 0: store[sp].vb = (store[sp].va != store[sp + 1].va); break;
                case 1: store[sp].vb = (store[sp].vi != store[sp + 1].vi); break;
                case 6: store[sp].vb = (store[sp].vc != store[sp + 1].vc); break;
                case 2: store[sp].vb = (store[sp].vr != store[sp + 1].vr); break;
                case 3: store[sp].vb = (store[sp].vb != store[sp + 1].vb); break;
                case 4: store[sp].vb = !P_setequal(store[sp].vs, store[sp + 1].vs); break;
                case 5: compare(); store[sp].vb = !b; break;
            }/*case p*/
            break;

        case 19:   /*geq*/
            sp--;
            switch (p) {
                case 0: errori(" <,<=,>,>= for address   "); break;
                case 1: store[sp].vb = (store[sp].vi >= store[sp + 1].vi); break;
                case 6: store[sp].vb = (store[sp].vc >= store[sp + 1].vc); break;
                case 2: store[sp].vb = (store[sp].vr >= store[sp + 1].vr); break;
                case 3: store[sp].vb = (store[sp].vb >= store[sp + 1].vb); break;
                case 4: store[sp].vb = P_subset(store[sp + 1].vs, store[sp].vs); break;
                case 5: compare(); store[sp].vb = (b || store[i1 + i].vi >= store[i2 + i].vi); break;
                }/*case p*/
            break;

        case 20:   /*grt*/
            sp--;
            switch (p) {
                case 0: errori(" <,<=,>,>= for address   "); break;
                case 1: store[sp].vb = (store[sp].vi > store[sp + 1].vi); break;
                case 6: store[sp].vb = (store[sp].vc > store[sp + 1].vc); break;
                case 2: store[sp].vb = (store[sp].vr > store[sp + 1].vr); break;
                case 3: store[sp].vb = (store[sp].vb > store[sp + 1].vb); break;
                case 4: errori(" set inclusion\t          "); break;
                case 5: compare(); store[sp].vb = (!b && store[i1 + i].vi > store[i2 + i].vi); break;
            }/*case p*/
            break;

        case 21:   /*leq*/
            sp--;
            switch (p) {
                case 0: errori(" <,<=,>,>= for address   "); break;
                case 1: store[sp].vb = (store[sp].vi <= store[sp + 1].vi); break;
                case 6: store[sp].vb = (store[sp].vc <= store[sp + 1].vc); break;
                case 2: store[sp].vb = (store[sp].vr <= store[sp + 1].vr); break;
                case 3: store[sp].vb = (store[sp].vb <= store[sp + 1].vb); break;
                case 4: store[sp].vb = P_subset(store[sp].vs, store[sp + 1].vs); break;
                case 5: compare(); store[sp].vb = (b || store[i1 + i].vi <= store[i2 + i].vi); break;
            }/*case p*/
            break;

        case 22:   /*les*/
            sp--;
            switch (p) {
                case 0: errori(" <,<=,>,>= for address   "); break;
                case 1: store[sp].vb = (store[sp].vi < store[sp + 1].vi); break;
                case 6: store[sp].vb = (store[sp].vc < store[sp + 1].vc); break;
                case 2: store[sp].vb = (store[sp].vr < store[sp + 1].vr); break;
                case 3: store[sp].vb = (store[sp].vb < store[sp + 1].vb); break;
                case 5: compare(); store[sp].vb = (!b && store[i1 + i].vi < store[i2 + i].vi); break;
            }/*case p*/
            break;

        case 23:   /*ujp*/
            pc = q;
            break;

        case 24:   /*fjp*/
            if (!store[sp].vb)
                pc = q;
            sp--;
            break;

        case 25:   /*xjp*/
            pc = store[sp].vi + q;
            sp--;
            break;

        case 95:   /*chka*/
            if (store[sp].va < np || store[sp].va > MAXSTR - q)
                errori(" bad pointer value       ");
            break;

        case 96:
        case 97:
        case 98:
        case 99:
        case 26:   /*chk*/
            if (store[sp].vi < store[q - 1].vi || store[sp].vi > store[q].vi)
                errori(" value out of range      ");
            break;

        case 27:   /*eof*/
            /*
            i = store[sp].vi;
            if (i == inputadr)
                store[sp].vb = P_eof(stdin);
            else
                errori(" code in error\t          ");
            */

            break;

        case 28:   /*adi*/
            sp--;
            store[sp].vi += store[sp + 1].vi;
            break;

        case 29:   /*adr*/
            sp--;
            store[sp].vr += store[sp + 1].vr;
            break;

        case 30:   /*sbi*/
            sp--;
            store[sp].vi -= store[sp + 1].vi;
            break;

        case 31:   /*sbr*/
            sp--;
            store[sp].vr -= store[sp + 1].vr;
            break;

        case 32:   /*sgs*/
            settype SET;
            P_setcpy(store[sp].vs, P_addset(P_expset(SET, 0), store[sp].vi));
            break;

        case 33:   /*flt*/
            store[sp].vr = store[sp].vi;
            break;

        case 34:   /*flo*/
            store[sp - 1].vr = store[sp - 1].vi;
            break;

        case 35:   /*trc*/
            store[sp].vi = (long)store[sp].vr;
            break;

        case 36:   /*ngi*/
            store[sp].vi = -store[sp].vi;
            break;

        case 37:   /*ngr*/
            store[sp].vr = -store[sp].vr;
            break;

        case 38:   /*sqi*/
        {
            long TEMP = store[sp].vi;
            store[sp].vi = TEMP * TEMP;
            break;
        }
        case 39:   /*sqr*/
        {
            double TEMP1 = store[sp].vr;
            store[sp].vr = TEMP1 * TEMP1;
            break;
        }
        case 40:   /*abi*/
            store[sp].vi = labs(store[sp].vi);
            break;

        case 41:   /*abr*/
            store[sp].vr = fabs(store[sp].vr);
            break;

        case 42:   /*not*/
            store[sp].vb = !store[sp].vb;
            break;

        case 43:   /*and*/
            sp--;
            store[sp].vb = (store[sp].vb && store[sp + 1].vb);
            break;

        case 44:   /*ior*/
            sp--;
            store[sp].vb = (store[sp].vb || store[sp + 1].vb);
            break;

        case 45:   /*dif*/
            sp--;
            P_setdiff(store[sp].vs, store[sp].vs, store[sp + 1].vs);
            break;

        case 46:   /*int*/
            sp--;
            P_setint(store[sp].vs, store[sp].vs, store[sp + 1].vs);
            break;

        case 47:   /*uni*/
            sp--;
            P_setunion(store[sp].vs, store[sp].vs, store[sp + 1].vs);
            break;

        case 48:   /*inn*/
            sp--;
            i = store[sp].vi;
            store[sp].vb = P_inset(i, store[sp + 1].vs);
            break;

        case 49:   /*mod*/
            sp--;
            store[sp].vi %= store[sp + 1].vi;
            break;

        case 50:   /*odd*/
            store[sp].vb = store[sp].vi & 1;
            break;

        case 51:   /*mpi*/
            sp--;
            store[sp].vi *= store[sp + 1].vi;
            break;

        case 52:   /*mpr*/
            sp--;
            store[sp].vr *= store[sp + 1].vr;
            break;

        case 53:   /*dvi*/
            sp--;
            store[sp].vi /= store[sp + 1].vi;
            break;

        case 54:   /*dvr*/
            sp--;
            store[sp].vr /= store[sp + 1].vr;
            break;

        case 55:   /*mov*/
            long FORLIM;
            i1 = store[sp - 1].va;
            i2 = store[sp].va;
            sp -= 2;
            FORLIM = q;
            for (i = 0; i < FORLIM; i++) {
                store[i1 + i] = store[i2 + i];
                /* q is a number of storage units */
            }
            break;

        case 56:   /*lca*/
            sp++;
            store[sp].va = q;
            break;

        case 57:   /*dec*/
            store[sp].vi -= q;
            break;

        case 58:   /*stp*/
            interpreting = false;
            break;

        case 59:   /*ord*/
            break;
            /*only used to change the tagfield*/

        case 60:   /*chr*/
            break;

        case 61:   /*ujc*/
            errori(" case - error\t           ");
            break;
        }
    }  /*while interpreting*/

}