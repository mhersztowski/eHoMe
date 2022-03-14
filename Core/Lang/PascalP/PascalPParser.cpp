#include "PascalPParser.h"
/*
#include "PascalPLex.h"
#include "PascalPSymTab.h"
#include "PascalPVM.h"
*/


/*-------------------------------------------------------------------------*/
void PascalPParser::mark_(long** p)
{
}


void PascalPParser::release_(long* p)
{
}


void PascalPParser::endofline(void)
{
	long lastpos, freepos, currpos, currnmr, f, k, FORLIM;
	_REC_errlist* WITH;

	/*endofline*/
	if (errinx > 0) {   /*output error messages*/
		printf("%6ld%9s", linecount, " ****  ");
		lastpos = 0;
		freepos = 1;
		FORLIM = errinx;
		for (k = 0; k < FORLIM; k++) {
			WITH = &errlist[k];
			currpos = WITH->pos;
			currnmr = WITH->nmr;
			if (currpos == lastpos)
				putchar(',');
			else {
				while (freepos < currpos) {
					putchar(' ');
					freepos++;
				}
				putchar('^');
				lastpos = currpos;
			}
			if (currnmr < 10)
				f = 1;
			else if (currnmr < 100)
				f = 2;
			else
				f = 3;
			printf("%*ld", (int)f, currnmr);
			freepos += f + 1;
		}
		putchar('\n');
		errinx = 0;
	}
	linecount++;
	if (list & (!P_eof(stdin))) {
		printf("%6ld%2s", linecount, "  ");
		if (dp)
			printf("%7d", lc);
		else
			printf("%7d", ic);
		putchar(' ');
	}
	chcnt = 0;
}


void PascalPParser::error(long ferrnr)
{
	/*error*/
	if (errinx >= 9) {
		errlist[9].nmr = 255;
		errinx = 10;
	}
	else {
		errinx++;
		errlist[errinx - 1].nmr = ferrnr;
	}
	errlist[errinx - 1].pos = chcnt;
	input_ok = false;
}


void PascalPParser::nextch(struct LOC_insymbol* LINK)
{
	if (eol) {
		if (list)
			putchar('\n');
		endofline();
	}
	if (P_eof(stdin)) {
		printf("   *** eof encountered\n");
		LINK->test = false;
		return;
	}
	eol = P_eoln(stdin);
	ch = fgetc(stdin);
	if (ch == '\n')
		ch = ' ';
	else
		if (isupper(ch))
			ch = tolower(ch);
	if (list)
		putchar(ch);
	chcnt++;
}

void PascalPParser::options(struct LOC_insymbol* LINK)
{
	/*options*/
	do {
		nextch(LINK);
		if (ch != '*') {
			if (ch == 't') {
				nextch(LINK);
				prtables = (ch == '+');
			}
			else {
				if (ch == 'l') {
					nextch(LINK);
					list = (ch == '+');
					if (!list)
						putchar('\n');
				}
				else {
					if (ch == 'd') {
						nextch(LINK);
						debug = (ch == '+');
					}
					else {
						if (ch == 'c') {
							nextch(LINK);
							prcode = (ch == '+');
						}
					}
				}
			}
			nextch(LINK);
		}
	} while (ch == ',');
}


void PascalPParser::insymbol(void)
{
	/*read next basic symbol of source program and return its
	description in the global variables sy, op, id, val and lgth*/
	struct LOC_insymbol V;
	long i, k;
	char digit[strglgth];
	char string[strglgth];
	constant* lvp;
	long FORLIM;

	/*insymbol*/
_L1:
	do {
		while ((ch == ' ' || ch == '\t') && !eol)
			nextch(&V);
		V.test = eol;
		if (V.test)
			nextch(&V);
	} while (V.test);
	/*
	  if (P_eof (stdin)) {
		sy = othersy;
		op = noop;
		error(399);
		return;
	  }
	*/
	if (chartp[ch] == illegal) {
		sy = othersy;
		op = noop;
		error(399);
		nextch(&V);
		return;
	}
	switch (chartp[ch]) {

	case letter:
		k = 0;
		do {
			if (k < 8) {
				k++;
				id[k - 1] = ch;
			}
			nextch(&V);
		} while (((1L << ((long)chartp[ch])) & ((1L << ((long)special)) |
			(1L << ((long)illegal)) | (1L << ((long)chstrquo)) |
			(1L << ((long)chcolon)) | (1L << ((long)chperiod)) |
			(1L << ((long)chlt)) | (1L << ((long)chgt)) |
			(1L << ((long)chlparen)) | (1L << ((long)chspace)))) == 0);
		if (k >= kk)
			kk = k;
		else {
			do {
				id[kk - 1] = ' ';
				kk--;
			} while (kk != k);
		}
		FORLIM = frw[k] - 2;
		for (i = frw[k - 1] - 1; i <= FORLIM; i++) {
			if (!strncmp(rw[i], id, sizeof(alpha))) {
				sy = rsy[i];
				op = rop[i];
				goto _L2;
			}
		}
		sy = ident;
		op = noop;
	_L2:;
		break;

	case number:
		op = noop;
		i = 0;
		do {
			i++;
			if (i <= digmax)
				digit[i - 1] = ch;
			nextch(&V);
		} while (chartp[ch] == number);
		if (((ch == '.') & (P_peek(stdin) != '.')) || ch == 'e') {
			k = i;
			if (ch == '.') {
				k++;
				if (k <= digmax)
					digit[k - 1] = ch;
				nextch(&V);   /*if ch = '.' then begin ch := ':'; goto 3 end;*/
				if (chartp[ch] != number)
					error(201);
				else {
					do {
						k++;
						if (k <= digmax)
							digit[k - 1] = ch;
						nextch(&V);
					} while (chartp[ch] == number);
				}
			}
			if (ch == 'e') {
				k++;
				if (k <= digmax)
					digit[k - 1] = ch;
				nextch(&V);
				if (ch == '+' || ch == '-') {
					k++;
					if (k <= digmax)
						digit[k - 1] = ch;
					nextch(&V);
				}
				if (chartp[ch] != number)
					error(201);
				else {
					do {
						k++;
						if (k <= digmax)
							digit[k - 1] = ch;
						nextch(&V);
					} while (chartp[ch] == number);
				}
			}
			/* p2c: pcom.p, line 454:
			 * Note: No Specialmalloc form known for CONSTANT.REEL [187] */
			lvp = (constant*) malloc(sizeof(constant));
			sy = realconst;
			lvp->cclass = reel;
			for (i = 0; i < strglgth; i++)
				lvp->UU.rval[i] = ' ';
			if (k <= digmax) {
				for (i = 2; i <= k + 1; i++)
					lvp->UU.rval[i - 1] = digit[i - 2];
			}
			else {
				error(203);
				lvp->UU.rval[1] = '0';
				lvp->UU.rval[2] = '.';
				lvp->UU.rval[3] = '0';
			}
			val.UU.valp = lvp;
		}
		else {
			if (i > digmax) {
				error(203);
				val.UU.ival = 0;
			}
			else {
				val.UU.ival = 0;
				for (k = 0; k < i; k++) {
					if (val.UU.ival <= mxint10)
						val.UU.ival = val.UU.ival * 10 + ordint[digit[k]];
					else {
						error(203);
						val.UU.ival = 0;
					}
				}
				sy = intconst;
			}
		}
		break;

	case chstrquo:
		lgth = 0;
		sy = stringconst;
		op = noop;
		do {
			do {
				nextch(&V);
				lgth++;
				if (lgth <= strglgth)
					string[lgth - 1] = ch;
			} while (!(eol || ch == '\''));
			if (eol)
				error(202);
			else
				nextch(&V);
		} while (ch == '\'');
		lgth--;   /*now lgth = nr of chars in string*/
		if (lgth == 0)
			error(205);
		else {
			if (lgth == 1)
				val.UU.ival = string[0];
			else {
				lvp = (constant*) malloc(sizeof(constant));
				lvp->cclass = strg;
				if (lgth > strglgth) {
					error(399);
					lgth = strglgth;
				}
				lvp->UU.U2.slgth = lgth;
				FORLIM = lgth;
				for (i = 0; i < FORLIM; i++)
					lvp->UU.U2.sval[i] = string[i];
				val.UU.valp = lvp;
			}
		}
		break;

	case chcolon:
		op = noop;
		nextch(&V);
		if (ch == '=') {
			sy = becomes;
			nextch(&V);
		}
		else
			sy = colon;
		break;

	case chperiod:
		op = noop;
		nextch(&V);
		if (ch == '.') {
			sy = colon;
			nextch(&V);
		}
		else
			sy = period;
		break;

	case chlt:
		nextch(&V);
		sy = relop;
		if (ch == '=') {
			op = leop;
			nextch(&V);
		}
		else {
			if (ch == '>') {
				op = neop;
				nextch(&V);
			}
			else
				op = ltop;
		}
		break;

	case chgt:
		nextch(&V);
		sy = relop;
		if (ch == '=') {
			op = geop;
			nextch(&V);
		}
		else
			op = gtop;
		break;

	case chlparen:
		nextch(&V);
		if (ch == '*') {
			nextch(&V);
			if (ch == '$')
				options(&V);
			do {
				while ((ch != '*') & (!P_eof(stdin)))
					nextch(&V);
				nextch(&V);
			} while (!((ch == ')') | P_eof(stdin)));
			nextch(&V);
			goto _L1;
		}
		sy = lparent;
		op = noop;
		break;

	case special:
		sy = ssy[ch];
		op = sop[ch];
		nextch(&V);
		break;

	case chspace:
		sy = othersy;
		break;
	}/*case*/

  /* p2c: pcom.p, line 493:
   * Note: No Specialmalloc form known for CONSTANT.STRG [187] */
}


void PascalPParser::enterid(identifier* fcp)
{
	/*enter id pointed at by fcp into the name-table,
	 which on each declaration level is organised as
	 an unbalanced binary tree*/
	alpha nam;
	identifier* lcp, * lcp1;
	bool lleft;

	/*enterid*/
	memcpy(nam, fcp->name, sizeof(alpha));
	lcp = display[top].fname;
	if (lcp == NULL)
		display[top].fname = fcp;
	else {
		do {
			lcp1 = lcp;
			if (!strncmp(lcp->name, nam, sizeof(alpha)))
			{   /*name conflict, follow right link*/
				error(101);
				lcp = lcp->rlink;
				lleft = false;
			}
			else {
				if (strncmp(lcp->name, nam, sizeof(alpha)) < 0) {
					lcp = lcp->rlink;
					lleft = false;
				}
				else {
					lcp = lcp->llink;
					lleft = true;
				}
			}
		} while (lcp != NULL);
		if (lleft)
			lcp1->llink = fcp;
		else
			lcp1->rlink = fcp;
	}
	fcp->llink = NULL;
	fcp->rlink = NULL;
}


void PascalPParser::searchsection(identifier* fcp, identifier** fcp1)
{
	/*to find record fields and forward declared procedure id's
	 --> procedure proceduredeclaration
	 --> procedure selector*/
	 /*searchsection*/
	while (fcp != NULL) {
		if (!strncmp(fcp->name, id, sizeof(alpha)))
			goto _L1;
		if (strncmp(fcp->name, id, sizeof(alpha)) < 0)
			fcp = fcp->rlink;
		else
			fcp = fcp->llink;
	}
_L1:
	*fcp1 = fcp;
}


void PascalPParser::searchid(setofids fidcls, identifier** fcp)
{
	identifier* lcp;

	/*searchid*/
	for (disx = top; disx >= 0; disx--) {
		lcp = display[disx].fname;
		while (lcp != NULL) {
			if (strncmp(lcp->name, id, sizeof(alpha))) {
				if (strncmp(lcp->name, id, sizeof(alpha)) < 0)
					lcp = lcp->rlink;
				else
					lcp = lcp->llink;
				continue;
			}
			if (((1L << lcp->klass) & fidcls) != 0)
				goto _L1;
			if (prterr)
				error(103);
			lcp = lcp->rlink;
		}
	}
	/*search not successful; suppress error message in case
	 of forward referenced type id in pointer type definition
	 --> procedure simpletype*/
	if (prterr) {
		error(104);
		/*to avoid returning nil, reference an entry
		 for an undeclared id of appropriate class
		 --> procedure enterundecl*/
		if (((1L << ((long)types)) & fidcls) != 0)
			lcp = utypptr;
		else {
			if (((1L << ((long)vars)) & fidcls) != 0)
				lcp = uvarptr;
			else {
				if (((1L << ((long)field)) & fidcls) != 0)
					lcp = ufldptr;
				else {
					if (((1L << ((long)konst)) & fidcls) != 0)
						lcp = ucstptr;
					else {
						if (((1L << ((long)proc)) & fidcls) != 0)
							lcp = uprcptr;
						else
							lcp = ufctptr;
					}
				}
			}
		}
	}
_L1:
	*fcp = lcp;
}


void PascalPParser::getbounds(structure* fsp, long* fmin, long* fmax)
{
	/*get internal bounds of subrange or scalar type*/
	/*assume fsp<>intptr and fsp<>realptr*/
	/*getbounds*/
	*fmin = 0;
	*fmax = 0;
	if (fsp == NULL)
		return;
	if ((structform)fsp->form == subrange) {
		*fmin = fsp->UU.U1.min.UU.ival;
		*fmax = fsp->UU.U1.max.UU.ival;
		return;
	}
	if (fsp == charptr) {
		*fmin = ordminchar;
		*fmax = ordmaxchar;
	}
	else {
		if (fsp->UU.U0.UU.fconst != NULL)
			*fmax = fsp->UU.U0.UU.fconst->UU.values.UU.ival;
	}
}


long PascalPParser::alignquot(structure* fsp)
{
	long Result;

	/*alignquot*/
	Result = 1;
	if (fsp == NULL)
		return Result;
	switch ((structform)fsp->form) {

	case scalar:
		if (fsp == intptr)
			Result = intal;
		else if (fsp == boolptr)
			Result = boolal;
		else if ((declkind)fsp->UU.U0.scalkind == declared)
			Result = intal;
		else if (fsp == charptr)
			Result = charal;
		else if (fsp == realptr)
			Result = realal;
		else
			Result = parmal;
		break;

	case subrange:
		Result = alignquot(fsp->UU.U1.rangetype);
		break;

	case pointer:
		Result = adral;
		break;

	case power:
		Result = setal;
		break;

	case files:
		Result = fileal;
		break;

	case arrays:
		Result = alignquot(fsp->UU.U4.aeltype);
		break;

	case records:
		Result = recal;
		break;

	case variant:
	case tagfld:
		error(501);
		break;
	}
	return Result;

	/*parmptr*/
}


void PascalPParser::align(structure* fsp, addrrange* flc)
{
	long k, l;

	/*align*/
	k = alignquot(fsp);
	l = *flc - 1;
	*flc = l + k - (k + l) % k;
}



//Local void followctp(identifier* fp, struct LOC_printtables* LINK);
//Local void markctp(identifier* fp);

void PascalPParser::markstp(structure* fp)
{
	/*mark data structures, prevent cycles*/
	/*markstp*/
	if (fp == NULL)
		return;
	fp->marked = true;
	switch ((structform)fp->form) {   /*with*/

	case scalar:
		/* blank case */
		break;

	case subrange:
		markstp(fp->UU.U1.rangetype);
		break;

	case pointer:
		/* blank case */
		break;

		/*don't mark eltype: cycle possible; will be marked
				  anyway, if fp = true*/
	case power:
		markstp(fp->UU.elset);
		break;

	case arrays:
		markstp(fp->UU.U4.aeltype);
		markstp(fp->UU.U4.inxtype);
		break;

	case records:
		markctp(fp->UU.U5.fstfld);
		markstp(fp->UU.U5.recvar);
		break;

	case files:
		markstp(fp->UU.filtype);
		break;

	case tagfld:
		markstp(fp->UU.U7.fstvar);
		break;

	case variant:
		markstp(fp->UU.U8.nxtvar);
		markstp(fp->UU.U8.subvar);
		break;
	}/*case*/
}

void PascalPParser::markctp(identifier* fp)
{
	/*markctp*/
	if (fp == NULL)
		return;
	markctp(fp->llink);
	markctp(fp->rlink);
	markstp(fp->idtype);
}


void PascalPParser::marker(struct LOC_printtables* LINK)
{
	/*mark data structure entries to avoid multiple printout*/
	long i, FORLIM;

	/*marker*/
	FORLIM = LINK->lim;
	for (i = top; i >= FORLIM; i--)
		markctp(display[i].fname);
}

void PascalPParser::followstp(structure* fp, struct LOC_printtables* LINK)
{
	/*followstp*/
	if (fp == NULL)
		return;
	if (!fp->marked) {
		return;
	}  /*if marked*/
	fp->marked = false;
	printf("%4c%6ld%10d", ' ', (long)fp, fp->size);
	switch ((structform)fp->form) {

	case scalar:
		printf("%10s", "scalar");
		if ((declkind)fp->UU.U0.scalkind == standard)
			printf("%10s", "standard");
		else
			printf("%10s%4c%6ld", "declared", ' ', (long)fp->UU.U0.UU.fconst);
		putchar('\n');
		break;

	case subrange:
		printf("%10s%4c%6ld", "subrange", ' ', (long)fp->UU.U1.rangetype);
		if (fp->UU.U1.rangetype != realptr)
			printf("%12ld%12ld", fp->UU.U1.min.UU.ival, fp->UU.U1.max.UU.ival);
		else {
			if (fp->UU.U1.min.UU.valp != NULL && fp->UU.U1.max.UU.valp != NULL)
				printf(" %.9s %.9s",
					fp->UU.U1.min.UU.valp->UU.rval,
					fp->UU.U1.max.UU.valp->UU.rval);
			/* p2c: pcom.p, line 742: Note:
			 * Format for packed-array-of-char will work only if width < length [321] */
			 /* p2c: pcom.p, line 743: Note:
			  * Format for packed-array-of-char will work only if width < length [321] */
		}
		putchar('\n');
		followstp(fp->UU.U1.rangetype, LINK);
		break;

	case pointer:
		printf("%10s%4c%6ld\n", "pointer", ' ', (long)fp->UU.eltype);
		break;

	case power:
		printf("%10s%4c%6ld\n", "set", ' ', (long)fp->UU.elset);
		followstp(fp->UU.elset, LINK);
		break;

	case arrays:
		printf("%10s%4c%6ld%4c%6ld\n",
			"array", ' ', (long)fp->UU.U4.aeltype, ' ',
			(long)fp->UU.U4.inxtype);
		followstp(fp->UU.U4.aeltype, LINK);
		followstp(fp->UU.U4.inxtype, LINK);
		break;

	case records:
		printf("%10s%4c%6ld%4c%6ld\n",
			"record", ' ', (long)fp->UU.U5.fstfld, ' ',
			(long)fp->UU.U5.recvar);
		followctp(fp->UU.U5.fstfld, LINK);
		followstp(fp->UU.U5.recvar, LINK);
		break;

	case files:
		printf("%10s%4c%6ld", "file", ' ', (long)fp->UU.filtype);
		followstp(fp->UU.filtype, LINK);
		break;

	case tagfld:
		printf("%10s%4c%6ld%4c%6ld\n",
			"tagfld", ' ', (long)fp->UU.U7.tagfieldp, ' ',
			(long)fp->UU.U7.fstvar);
		followstp(fp->UU.U7.fstvar, LINK);
		break;

	case variant:
		printf("%10s%4c%6ld%4c%6ld%12ld\n",
			"variant", ' ', (long)fp->UU.U8.nxtvar, ' ',
			(long)fp->UU.U8.subvar, fp->UU.U8.varval.UU.ival);
		followstp(fp->UU.U8.nxtvar, LINK);
		followstp(fp->UU.U8.subvar, LINK);
		break;
	}/*case*/
}

void PascalPParser::followctp(identifier* fp, struct LOC_printtables* LINK)
{
	long i;
	constant* WITH1;
	long FORLIM;

	/*followctp*/
	if (fp == NULL)
		return;
	printf("%4c%6ld  %.8s%4c%6ld%4c%6ld%4c%6ld",
		' ', (long)fp, fp->name, ' ', (long)fp->llink, ' ', (long)fp->rlink,
		' ', (long)fp->idtype);
	switch ((idclass)fp->klass) {   /*case*/

	case types:
		printf("%10s", "type");
		break;

	case konst:
		printf("%10s%4c%6ld", "constant", ' ', (long)fp->next);
		if (fp->idtype != NULL) {
			if (fp->idtype == realptr) {
				if (fp->UU.values.UU.valp != NULL)
					printf(" %.9s", fp->UU.values.UU.valp->UU.rval);
				/* p2c: pcom.p, line 789: Note:
				 * Format for packed-array-of-char will work only if width < length [321] */
			}
			else {
				if ((structform)fp->idtype->form == arrays) {   /*stringconst*/
					if (fp->UU.values.UU.valp != NULL) {
						putchar(' ');
						WITH1 = fp->UU.values.UU.valp;
						FORLIM = WITH1->UU.U2.slgth;
						for (i = 0; i < FORLIM; i++)
							putchar(WITH1->UU.U2.sval[i]);
					}
				}
				else
					printf("%12ld", fp->UU.values.UU.ival);
			}
		}
		break;

	case vars:
		printf("%10s", "variable");
		if ((idkind)fp->UU.U2.vkind == actual)
			printf("%10s", "actual");
		else
			printf("%10s", "formal");
		printf("%4c%6ld%12d%4c%6d",
			' ', (long)fp->next, fp->UU.U2.vlev, ' ', fp->UU.U2.vaddr);
		break;

	case field:
		printf("%10s%4c%6ld%4c%6d",
			"field", ' ', (long)fp->next, ' ', fp->UU.fldaddr);
		break;

	case proc:
	case func:
		if ((idclass)fp->klass == proc)
			printf("%10s", "procedure");
		else
			printf("%10s", "function");
		if ((declkind)fp->UU.U4.pfdeckind == standard)
			printf("%10s%10d", "standard", fp->UU.U4.UU.key);
		else {
			printf("%10s%4c%6ld", "declared", ' ', (long)fp->next);
			printf("%12d%4c%6ld",
				fp->UU.U4.UU.U1.pflev, ' ', fp->UU.U4.UU.U1.pfname);
			if ((idkind)fp->UU.U4.UU.U1.pfkind == actual) {
				printf("%10s", "actual");
				if (fp->UU.U4.UU.U1.UU.U0.forwdecl)
					printf("%10s", "forward");
				else
					printf("%10s", "notforward");
				if (fp->UU.U4.UU.U1.UU.U0.externl)
					printf("%10s", "extern");
				else
					printf("%10s", "not extern");
			}
			else
				printf("%10s", "formal");
		}
		break;
	}
	putchar('\n');
	followctp(fp->llink, LINK);
	followctp(fp->rlink, LINK);
	followstp(fp->idtype, LINK);   /*with*/
}


void PascalPParser::printtables(bool fb)
{
	/*print data structure and name table*/
	struct LOC_printtables V;
	disprange i, FORLIM;

	/*printtables*/
	printf("\n\n\n");
	if (fb)
		V.lim = 0;
	else {
		V.lim = top;
		printf(" local");
	}
	printf(" tables \n\n");
	marker(&V);
	FORLIM = V.lim;
	for (i = top; i >= FORLIM; i--)
		followctp(display[i].fname, &V);
	putchar('\n');
	if (!eol)
		printf("%*c", (int)(chcnt + 16), ' ');
}


void PascalPParser::genlabel(long* nxtlab)
{
	/*genlabel*/
	intlabel++;
	*nxtlab = intlabel;
}


//Static void block(long* fsys, symbol fsy, identifier* fprocp);







//Local void typ(long* fsys, structure** fsp, addrrange* fsize, struct LOC_block* LINK);

void PascalPParser::skip(long* fsys, struct LOC_block* LINK)
{
	/*skip input string until relevant symbol found*/
	/*skip*/
	if (P_eof(stdin))
		return;
	while ((!P_inset(sy, fsys)) & (!P_eof(stdin)))
		insymbol();
	if (!P_inset(sy, fsys))
		insymbol();
}

void PascalPParser::constant_(long* fsys, structure** fsp, valu* fvalu, struct LOC_block* LINK)
{
	structure* lsp;
	identifier* lcp;
	enum {
		none, pos_, neg
	} sign;
	constant* lvp;
	char i;
	setofsys SET;

	/*constant*/
	lsp = NULL;
	fvalu->UU.ival = 0;
	if (!P_inset(sy, constbegsys)) {
		error(50);
		skip(P_setunion(SET, fsys, constbegsys), LINK);
	}
	if (P_inset(sy, constbegsys)) {
		if (sy == stringconst) {
			if (lgth == 1)
				lsp = charptr;
			else {
				lsp = (structure*) malloc(sizeof(structure));
				lsp->UU.U4.aeltype = charptr;
				lsp->UU.U4.inxtype = NULL;
				lsp->size = lgth;
				lsp->form = arrays;
			}
			*fvalu = val;
			insymbol();
		}
		else {
			sign = none;
			if (sy == addop &&
				((1L << ((long)op)) & ((1L << ((long)plus)) | (1L << ((long)minus)))) !=
				0) {
				if (op == plus)
					sign = pos_;
				else
					sign = neg;
				insymbol();
			}
			if (sy == ident) {
				searchid(1L << ((long)konst), &lcp);
				lsp = lcp->idtype;
				*fvalu = lcp->UU.values;
				if (sign != none) {
					if (lsp == intptr) {
						if (sign == neg)
							fvalu->UU.ival = -fvalu->UU.ival;
					}
					else {
						if (lsp == realptr) {
							if (sign == neg) {
								lvp = (constant*) malloc(sizeof(constant));
								if (fvalu->UU.valp->UU.rval[0] == '-')
									lvp->UU.rval[0] = '+';
								else
									lvp->UU.rval[0] = '-';
								for (i = 1; i < strglgth; i++)
									lvp->UU.rval[i] = fvalu->UU.valp->UU.rval[i];
								fvalu->UU.valp = lvp;
							}
							/* p2c: pcom.p, line 903:
							 * Note: No Specialmalloc form known for CONSTANT.REEL [187] */
						}
						else
							error(105);
					}
				}
				insymbol();
			}
			else {
				if (sy == intconst) {
					if (sign == neg)
						val.UU.ival = -val.UU.ival;
					lsp = intptr;
					*fvalu = val;
					insymbol();
				}
				else {
					if (sy == realconst) {
						if (sign == neg)
							val.UU.valp->UU.rval[0] = '-';
						lsp = realptr;
						*fvalu = val;
						insymbol();
					}
					else {
						error(106);
						skip(fsys, LINK);
					}
				}
			}
		}
		if (!P_inset(sy, fsys)) {
			error(6);
			skip(fsys, LINK);
		}
	}
	*fsp = lsp;

	/* p2c: pcom.p, line 877:
	 * Note: No Specialmalloc form known for STRUCTURE.ARRAYS [187] */
}

bool PascalPParser::equalbounds(structure* fsp1, structure* fsp2, struct LOC_block* LINK)
{
	long lmin1, lmin2, lmax1, lmax2;

	/*equalbounds*/
	if (fsp1 == NULL || fsp2 == NULL)
		return true;
	else {
		getbounds(fsp1, &lmin1, &lmax1);
		getbounds(fsp2, &lmin2, &lmax2);
		return (lmin1 == lmin2 && lmax1 == lmax2);
	}
}

bool PascalPParser::comptypes(structure* fsp1, structure* fsp2, struct LOC_block* LINK)
{
	/*decide whether structures pointed at by fsp1 and fsp2 are compatible*/
	bool Result;
	identifier* nxt1, * nxt2;
	bool comp;
	testpointer* ltestp1, * ltestp2, * WITH;

	/*comptypes*/
	if (fsp1 == fsp2)
		return true;
	else {
		if (fsp1 != NULL && fsp2 != NULL) {
			if ((structform)fsp1->form == (structform)fsp2->form) {
				switch ((structform)fsp1->form) {

				case scalar:
					Result = false;
					break;

					/* identical scalars declared on different levels are
					 not recognized to be compatible*/
				case subrange:
					Result = comptypes(fsp1->UU.U1.rangetype, fsp2->UU.U1.rangetype,
						LINK);
					break;

				case pointer:
					comp = false;
					ltestp1 = globtestp;
					ltestp2 = globtestp;
					while (ltestp1 != NULL) {
						WITH = ltestp1;
						if (WITH->elt1 == fsp1->UU.eltype && WITH->elt2 == fsp2->UU.eltype)
							comp = true;
						ltestp1 = WITH->lasttestp;
					}
					if (!comp) {
						ltestp1 = (testpointer*) malloc(sizeof(testpointer));
						ltestp1->elt1 = fsp1->UU.eltype;
						ltestp1->elt2 = fsp2->UU.eltype;
						ltestp1->lasttestp = globtestp;
						globtestp = ltestp1;
						comp = comptypes(fsp1->UU.eltype, fsp2->UU.eltype, LINK);
					}
					Result = comp;
					globtestp = ltestp2;
					break;

				case power:
					Result = comptypes(fsp1->UU.elset, fsp2->UU.elset, LINK);
					break;

				case arrays:
					comp = comptypes(fsp1->UU.U4.aeltype, fsp2->UU.U4.aeltype, LINK) &
						comptypes(fsp1->UU.U4.inxtype, fsp2->UU.U4.inxtype, LINK);
					Result = (comp && fsp1->size == fsp2->size) & equalbounds(
						fsp1->UU.U4.inxtype, fsp2->UU.U4.inxtype, LINK);
					break;

				case records:
					nxt1 = fsp1->UU.U5.fstfld;
					nxt2 = fsp2->UU.U5.fstfld;
					comp = true;
					while (nxt1 != NULL && nxt2 != NULL) {
						comp &= comptypes(nxt1->idtype, nxt2->idtype, LINK);
						nxt1 = nxt1->next;
						nxt2 = nxt2->next;
					}
					Result = (comp && nxt1 == NULL && nxt2 == NULL &&
						fsp1->UU.U5.recvar == NULL && fsp2->UU.U5.recvar == NULL);
					break;

					/*identical records are recognized to be compatible
					 iff no variants occur*/
				case files:
					Result = comptypes(fsp1->UU.filtype, fsp2->UU.filtype, LINK);
					break;
				}/*case*/
				return Result;
			}
			else {  /*fsp1^.form <> fsp2^.form*/
				if ((structform)fsp1->form == subrange)
					return (comptypes(fsp1->UU.U1.rangetype, fsp2, LINK));
				else {
					if ((structform)fsp2->form == subrange)
						return (comptypes(fsp1, fsp2->UU.U1.rangetype, LINK));
					else
						return false;
				}
			}
		}
		else
			return true;
	}
	return Result;
}

bool PascalPParser::string(structure* fsp, struct LOC_block* LINK)
{
	bool Result;

	/*string*/
	Result = false;
	if (fsp == NULL)
		return Result;
	if ((structform)fsp->form == arrays) {
		if (comptypes(fsp->UU.U4.aeltype, charptr, LINK))
			return true;
	}
	return Result;
}

void PascalPParser::simpletype(long* fsys, structure** fsp, addrrange* fsize, struct LOC_typ* LINK)
{
	structure* lsp, * lsp1;
	identifier* lcp, * lcp1;
	disprange ttop;
	long lcnt;
	valu lvalu;
	setofsys SET, SET1, SET2, SET3;

	/*simpletype*/
	*fsize = 1;
	if (!P_inset(sy, simptypebegsys)) {
		error(1);
		skip(P_setunion(SET, fsys, simptypebegsys), LINK->LINK);
	}
	if (!P_inset(sy, simptypebegsys)) {
		*fsp = NULL;
		return;
	}
	if (sy == lparent) {
		ttop = top;   /*decl. consts local to innermost block*/
		while ((where)display[top].occur != blck)
			top--;
		/* p2c: pcom.p, line 1040:
		 * Note: No Specialmalloc form known for STRUCTURE.SCALAR.DECLARED [187] */
		lsp = (structure*) malloc(sizeof(structure));
		lsp->size = intsize;
		lsp->form = scalar;
		lsp->UU.U0.scalkind = declared;
		lcp1 = NULL;
		lcnt = 0;
		do {
			insymbol();
			if (sy == ident) {
				lcp = (identifier*) malloc(sizeof(identifier));
				memcpy(lcp->name, id, sizeof(alpha));
				lcp->idtype = lsp;
				lcp->next = lcp1;
				lcp->UU.values.UU.ival = lcnt;
				lcp->klass = konst;
				enterid(lcp);
				lcnt++;
				lcp1 = lcp;
				insymbol();
			}
			else
				error(2);
			/* p2c: pcom.p, line 1048:
			 * Note: No Specialmalloc form known for IDENTIFIER.KONST [187] */
			if (!P_inset(sy, P_setunion(SET1, fsys, P_expset(SET,
				(1L << ((long)comma)) | (1L << ((long)rparent)))))) {
				error(6);
				skip(P_setunion(SET3, fsys, P_expset(SET2,
					(1L << ((long)comma)) | (1L << ((long)rparent)))),
					LINK->LINK);
			}
		} while (sy == comma);
		lsp->UU.U0.UU.fconst = lcp1;
		top = ttop;
		if (sy == rparent)
			insymbol();
		else
			error(4);
	}
	else {
		if (sy == ident) {
			searchid((1L << ((long)types)) | (1L << ((long)konst)), &lcp);
			insymbol();
			if ((idclass)lcp->klass == konst) {
				lsp = (structure*) malloc(sizeof(structure));
				lsp->UU.U1.rangetype = lcp->idtype;
				lsp->form = subrange;
				if (string(lsp->UU.U1.rangetype, LINK->LINK)) {
					error(148);
					lsp->UU.U1.rangetype = NULL;
				}
				lsp->UU.U1.min = lcp->UU.values;
				lsp->size = intsize;
				if (sy == colon)
					insymbol();
				else
					error(5);
				constant_(fsys, &lsp1, &lvalu, LINK->LINK);
				lsp->UU.U1.max = lvalu;
				if (lsp->UU.U1.rangetype != lsp1)
					error(107);
			}
			else {
				lsp = lcp->idtype;
				if (lsp != NULL)
					*fsize = lsp->size;
			}
			/* p2c: pcom.p, line 1070:
			 * Note: No Specialmalloc form known for STRUCTURE.SUBRANGE [187] */
		}  /*sy = ident*/
		else {
			lsp = (structure*) malloc(sizeof(structure));
			lsp->form = subrange;
			constant_(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)colon))),
				&lsp1, &lvalu, LINK->LINK);
			if (string(lsp1, LINK->LINK)) {
				error(148);
				lsp1 = NULL;
			}
			lsp->UU.U1.rangetype = lsp1;
			lsp->UU.U1.min = lvalu;
			lsp->size = intsize;
			if (sy == colon)
				insymbol();
			else
				error(5);
			constant_(fsys, &lsp1, &lvalu, LINK->LINK);
			lsp->UU.U1.max = lvalu;
			if (lsp->UU.U1.rangetype != lsp1)
				error(107);
		}
		if (lsp != NULL) {
			if ((structform)lsp->form == subrange) {
				if (lsp->UU.U1.rangetype != NULL) {
					if (lsp->UU.U1.rangetype == realptr)
						error(399);
					else {
						if (lsp->UU.U1.min.UU.ival > lsp->UU.U1.max.UU.ival)
							error(102);
					}
				}
			}
		}
	}
	*fsp = lsp;
	if (!P_inset(sy, fsys)) {
		error(6);
		skip(fsys, LINK->LINK);
	}

	/* p2c: pcom.p, line 1088:
	 * Note: No Specialmalloc form known for STRUCTURE.SUBRANGE [187] */
}

void PascalPParser::fieldlist(long* fsys, structure** frecvar, struct LOC_typ* LINK)
{
	identifier* lcp, * lcp1, * nxt, * nxt1;
	structure* lsp, * lsp1, * lsp2, * lsp3, * lsp4;
	addrrange minsize, maxsize, lsize;
	valu lvalu;
	long SET[(long)casesy / 32 + 2];
	setofsys SET1;
	long SET2[(long)casesy / 32 + 2];
	setofsys SET3;
	identifier* WITH;
	structure* WITH1;
	long SET4[(long)ofsy / 32 + 2];
	setofsys SET5, SET6, SET7, SET8;

	/*fieldlist*/
	nxt1 = NULL;
	lsp = NULL;
	P_addset(P_expset(SET, 0), (long)ident);
	if (!P_inset(sy, P_setunion(SET1, fsys, P_addset(SET, (long)casesy)))) {
		error(19);
		P_addset(P_expset(SET2, 0), (long)ident);
		skip(P_setunion(SET3, fsys, P_addset(SET2, (long)casesy)), LINK->LINK);
	}
	while (sy == ident) {   /*while*/
		nxt = nxt1;
		do {
			if (sy == ident) {
				lcp = (identifier*) malloc(sizeof(identifier));
				memcpy(lcp->name, id, sizeof(alpha));
				lcp->idtype = NULL;
				lcp->next = nxt;
				lcp->klass = field;
				nxt = lcp;
				enterid(lcp);
				insymbol();
			}
			else
				error(2);
			/* p2c: pcom.p, line 1124:
			 * Note: No Specialmalloc form known for IDENTIFIER.FIELD [187] */
			if ((unsigned long)sy >= 32 ||
				((1L << ((long)sy)) & ((1L << ((long)comma)) | (1L << ((long)colon)))) ==
				0) {
				error(6);
				P_addset(P_expset(SET, 0), (long)comma);
				P_addset(SET, (long)colon);
				P_addset(SET, (long)semicolon);
				skip(P_setunion(SET1, fsys, P_addset(SET, (long)casesy)), LINK->LINK);
			}
			LINK->LINK->test = (sy != comma);
			if (!LINK->LINK->test)
				insymbol();
		} while (!LINK->LINK->test);
		if (sy == colon)
			insymbol();
		else
			error(5);
		P_addset(P_expset(SET, 0), (long)casesy);
		typ(P_setunion(SET1, fsys, P_addset(SET, (long)semicolon)), &lsp, &lsize,
			LINK->LINK);
		while (nxt != nxt1) {
			WITH = nxt;
			align(lsp, &LINK->displ);
			WITH->idtype = lsp;
			WITH->UU.fldaddr = LINK->displ;
			nxt = WITH->next;
			LINK->displ += lsize;
		}
		nxt1 = lcp;
		while (sy == semicolon) {
			insymbol();
			P_addset(P_expset(SET, 0), (long)ident);
			P_addset(SET, (long)casesy);
			if (!P_inset(sy, P_setunion(SET1, fsys, P_addset(SET, (long)semicolon)))) {
				error(19);
				P_addset(P_expset(SET2, 0), (long)ident);
				skip(P_setunion(SET3, fsys, P_addset(SET2, (long)casesy)), LINK->LINK);
			}
		}
	}
	nxt = NULL;
	while (nxt1 != NULL) {
		WITH = nxt1;
		lcp = WITH->next;
		WITH->next = nxt;
		nxt = nxt1;
		nxt1 = lcp;
	}
	if (sy != casesy) {
		*frecvar = NULL;
		return;
	}
	/* p2c: pcom.p, line 1160:
	 * Note: No Specialmalloc form known for STRUCTURE.TAGFLD [187] */
	lsp = (structure*) malloc(sizeof(structure));
	lsp->UU.U7.tagfieldp = NULL;
	lsp->UU.U7.fstvar = NULL;
	lsp->form = tagfld;
	*frecvar = lsp;
	insymbol();
	if (sy == ident) {
		lcp = (identifier*) malloc(sizeof(identifier));
		memcpy(lcp->name, id, sizeof(alpha));
		lcp->idtype = NULL;
		lcp->klass = field;
		lcp->next = NULL;
		lcp->UU.fldaddr = LINK->displ;
		enterid(lcp);
		insymbol();
		if (sy == colon)
			insymbol();
		else
			error(5);
		if (sy == ident) {
			searchid(1L << ((long)types), &lcp1);
			lsp1 = lcp1->idtype;
			if (lsp1 != NULL) {
				align(lsp1, &LINK->displ);
				lcp->UU.fldaddr = LINK->displ;
				LINK->displ += lsp1->size;
				if (((structform)lsp1->form <= subrange) | string(lsp1, LINK->LINK)) {
					if (comptypes(realptr, lsp1, LINK->LINK))
						error(109);
					else if (string(lsp1, LINK->LINK))
						error(399);
					lcp->idtype = lsp1;
					lsp->UU.U7.tagfieldp = lcp;
				}
				else
					error(110);
			}
			insymbol();
		}
		else {
			error(2);
			P_addset(P_expset(SET4, 0), (long)ofsy);
			skip(P_setunion(SET1, fsys, P_addset(SET4, (long)lparent)), LINK->LINK);
		}
	}
	else {
		error(2);
		P_addset(P_expset(SET4, 0), (long)ofsy);
		skip(P_setunion(SET1, fsys, P_addset(SET4, (long)lparent)), LINK->LINK);
	}
	/* p2c: pcom.p, line 1166:
	 * Note: No Specialmalloc form known for IDENTIFIER.FIELD [187] */
	lsp->size = LINK->displ;
	if (sy == ofsy)
		insymbol();
	else
		error(8);
	lsp1 = NULL;
	minsize = LINK->displ;
	maxsize = LINK->displ;
	do {
		lsp2 = NULL;
		if (!P_inset(sy, P_setunion(SET3, fsys,
			P_expset(SET1, 1L << ((long)semicolon))))) {
			do {
				constant_(P_setunion(SET6, fsys, P_expset(SET5,
					(1L << ((long)comma)) | (1L << ((long)colon)) |
					(1L << ((long)lparent)))), &lsp3, &lvalu,
					LINK->LINK);
				if (lsp->UU.U7.tagfieldp != NULL) {
					if (!comptypes(lsp->UU.U7.tagfieldp->idtype, lsp3, LINK->LINK))
						error(111);
				}
				/* p2c: pcom.p, line 1202:
				 * Note: No Specialmalloc form known for STRUCTURE.VARIANT [187] */
				lsp3 = (structure*) malloc(sizeof(structure));
				lsp3->UU.U8.nxtvar = lsp1;
				lsp3->UU.U8.subvar = lsp2;
				lsp3->UU.U8.varval = lvalu;
				lsp3->form = variant;
				lsp4 = lsp1;
				while (lsp4 != NULL) {
					WITH1 = lsp4;
					if (WITH1->UU.U8.varval.UU.ival == lvalu.UU.ival)
						error(178);
					lsp4 = WITH1->UU.U8.nxtvar;
				}
				lsp1 = lsp3;
				lsp2 = lsp3;
				LINK->LINK->test = (sy != comma);
				if (!LINK->LINK->test)
					insymbol();
			} while (!LINK->LINK->test);
			if (sy == colon)
				insymbol();
			else
				error(5);
			if (sy == lparent)
				insymbol();
			else
				error(9);
			fieldlist(P_setunion(SET6, fsys, P_expset(SET5,
				(1L << ((long)rparent)) | (1L << ((long)semicolon)))),
				&lsp2, LINK);
			if (LINK->displ > maxsize)
				maxsize = LINK->displ;
			while (lsp3 != NULL) {
				lsp4 = lsp3->UU.U8.subvar;
				lsp3->UU.U8.subvar = lsp2;
				lsp3->size = LINK->displ;
				lsp3 = lsp4;
			}
			if (sy == rparent) {
				insymbol();
				if (!P_inset(sy,
					P_setunion(SET6, fsys,
						P_expset(SET5, 1L << ((long)semicolon))))) {
					error(6);
					skip(P_setunion(SET8, fsys, P_expset(SET7, 1L << ((long)semicolon))),
						LINK->LINK);
				}
			}
			else
				error(4);
		}
		LINK->LINK->test = (sy != semicolon);
		if (!LINK->LINK->test) {
			LINK->displ = minsize;
			insymbol();
		}
	} while (!LINK->LINK->test);
	LINK->displ = maxsize;
	lsp->UU.U7.fstvar = lsp1;
}

void PascalPParser::typ(long* fsys, structure** fsp, addrrange* fsize, struct LOC_block* LINK)
{
	struct LOC_typ V;
	structure* lsp, * lsp1, * lsp2;
	disprange oldtop;
	identifier* lcp;
	addrrange lsize;
	long lmin, lmax;
	setofsys SET;
	_REC_display* WITH;
	long SET1[(long)ofsy / 32 + 2];
	setofsys SET2;
	long SET3[(long)endsy / 32 + 2];
	setofsys SET4;

	V.LINK = LINK;
	/*typ*/
	if (!P_inset(sy, typebegsys)) {
		error(10);
		skip(P_setunion(SET, fsys, typebegsys), LINK);
	}
	if (P_inset(sy, typebegsys)) {
		if (P_inset(sy, simptypebegsys))
			simpletype(fsys, fsp, fsize, &V);
		else {
			/*^*/
			if (sy == arrow) {
				lsp = (structure*) malloc(sizeof(structure));
				*fsp = lsp;
				lsp->UU.eltype = NULL;
				lsp->size = ptrsize;
				lsp->form = pointer;
				insymbol();
				if (sy == ident) {
					prterr = false;   /*no error if search not successful*/
					searchid(1L << ((long)types), &lcp);
					prterr = true;
					if (lcp == NULL) {   /*forward referenced type id*/
						lcp = (identifier*) malloc(sizeof(identifier));
						memcpy(lcp->name, id, sizeof(alpha));
						lcp->idtype = lsp;
						lcp->next = fwptr;
						lcp->klass = types;
						fwptr = lcp;
					}
					else {
						if (lcp->idtype != NULL) {
							if ((structform)lcp->idtype->form == files)
								error(108);
							else
								lsp->UU.eltype = lcp->idtype;
						}
					}
					/* p2c: pcom.p, line 1262:
					 * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
					insymbol();
				}
				else
					error(2);
			}
			else {
				if (sy == packedsy) {
					insymbol();
					if (!P_inset(sy, typedels)) {
						error(10);
						skip(P_setunion(SET, fsys, typedels), LINK);
					}
				}
				/*array*/
				if (sy == arraysy) {
					insymbol();
					if (sy == lbrack)
						insymbol();
					else
						error(11);
					lsp1 = NULL;
					do {
						lsp = (structure*) malloc(sizeof(structure));
						lsp->UU.U4.aeltype = lsp1;
						lsp->UU.U4.inxtype = NULL;
						lsp->form = arrays;
						lsp1 = lsp;
						P_addset(P_expset(SET1, 0), (long)comma);
						P_addset(SET1, (long)rbrack);
						simpletype(P_setunion(SET, fsys, P_addset(SET1, (long)ofsy)),
							&lsp2, &lsize, &V);
						lsp1->size = lsize;
						if (lsp2 != NULL) {
							if ((structform)lsp2->form <= subrange) {
								if (lsp2 == realptr) {
									error(109);
									lsp2 = NULL;
								}
								else {
									if (lsp2 == intptr) {
										error(149);
										lsp2 = NULL;
									}
								}
								lsp->UU.U4.inxtype = lsp2;
							}
							else {
								error(113);
								lsp2 = NULL;
							}
						}
						LINK->test = (sy != comma);
						if (!LINK->test)
							insymbol();
					} while (!LINK->test);
					/* p2c: pcom.p, line 1292:
					 * Note: No Specialmalloc form known for STRUCTURE.ARRAYS [187] */
					if (sy == rbrack)
						insymbol();
					else
						error(12);
					if (sy == ofsy)
						insymbol();
					else
						error(8);
					typ(fsys, &lsp, &lsize, LINK);
					do {
						lsp2 = lsp1->UU.U4.aeltype;
						lsp1->UU.U4.aeltype = lsp;
						if (lsp1->UU.U4.inxtype != NULL) {
							getbounds(lsp1->UU.U4.inxtype, &lmin, &lmax);
							align(lsp, &lsize);
							lsize *= lmax - lmin + 1;
							lsp1->size = lsize;
						}
						lsp = lsp1;
						lsp1 = lsp2;
					} while (lsp1 != NULL);
				}
				else {
					/*record*/
					if (sy == recordsy) {
						insymbol();
						oldtop = top;
						if (top < displimit) {
							top++;
							WITH = &display[top];
							WITH->fname = NULL;
							WITH->flabel = NULL;
							WITH->occur = rec;
						}
						else
							error(250);
						V.displ = 0;
						fieldlist(P_setunion(SET4,
							P_setdiff(SET2, fsys, P_expset(SET, 1L << ((long)semicolon))),
							P_addset(P_expset(SET3, 0), (long)endsy)), &lsp1, &V);
						/* p2c: pcom.p, line 1343:
						 * Note: No Specialmalloc form known for STRUCTURE.RECORDS [187] */
						lsp = (structure*) malloc(sizeof(structure));
						lsp->UU.U5.fstfld = display[top].fname;
						lsp->UU.U5.recvar = lsp1;
						lsp->size = V.displ;
						lsp->form = records;
						top = oldtop;
						if (sy == endsy)
							insymbol();
						else
							error(13);
					}
					else {
						/*set*/
						if (sy == setsy) {
							insymbol();
							if (sy == ofsy)
								insymbol();
							else
								error(8);
							simpletype(fsys, &lsp1, &lsize, &V);
							if (lsp1 != NULL) {
								if ((structform)lsp1->form > subrange) {
									error(115);
									lsp1 = NULL;
								}
								else {
									if (lsp1 == realptr) {
										error(114);
										lsp1 = NULL;
									}
									else if (lsp1 == intptr) {
										error(169);
										lsp1 = NULL;
									}
									else {
										getbounds(lsp1, &lmin, &lmax);
										if (lmin < setlow || lmax > sethigh)
											error(169);
									}
								}
							}
							/* p2c: pcom.p, line 1369:
							 * Note: No Specialmalloc form known for STRUCTURE.POWER [187] */
							lsp = (structure*) malloc(sizeof(structure));
							lsp->UU.elset = lsp1;
							lsp->size = setsize;
							lsp->form = power;
						}
						else {
							/*file*/
							if (sy == filesy) {
								insymbol();
								error(399);
								skip(fsys, LINK);
								lsp = NULL;
							}
						}
					}
				}
				*fsp = lsp;
			}
			/* p2c: pcom.p, line 1254:
			 * Note: No Specialmalloc form known for STRUCTURE.POINTER [187] */
		}
		if (!P_inset(sy, fsys)) {
			error(6);
			skip(fsys, LINK);
		}
	}
	else
		*fsp = NULL;
	if (*fsp == NULL)
		*fsize = 1;
	else
		*fsize = (*fsp)->size;
}

void PascalPParser::labeldeclaration(struct LOC_block* LINK)
{
	labl* llp;
	bool redef;
	long lbname;
	_REC_display* WITH;
	setofsys SET, SET1, SET2, SET3;

	/* labeldeclaration */
	do {
		if (sy == intconst) {
			WITH = &display[top];
			llp = WITH->flabel;
			redef = false;
			while (llp != NULL && !redef) {
				if (llp->labval != val.UU.ival)
					llp = llp->nextlab;
				else {
					redef = true;
					error(166);
				}
			}
			if (!redef) {
				llp = (labl*) malloc(sizeof(labl));
				llp->labval = val.UU.ival;
				genlabel(&lbname);
				llp->defined_ = false;
				llp->nextlab = WITH->flabel;
				llp->labname = lbname;
				WITH->flabel = llp;
			}
			insymbol();
		}
		else
			error(15);
		if (!P_inset(sy, P_setunion(SET1, LINK->fsys, P_expset(SET,
			(1L << ((long)comma)) | (1L << ((long)semicolon)))))) {
			error(6);
			skip(P_setunion(SET3, LINK->fsys, P_expset(SET2,
				(1L << ((long)comma)) | (1L << ((long)semicolon)))),
				LINK);
		}
		LINK->test = (sy != comma);
		if (!LINK->test)
			insymbol();
	} while (!LINK->test);
	if (sy == semicolon)
		insymbol();
	else
		error(14);
}

void PascalPParser::constdeclaration(struct LOC_block* LINK)
{
	identifier* lcp;
	structure* lsp;
	valu lvalu;
	setofsys SET, SET1, SET2, SET3;

	/*constdeclaration*/
	if (sy != ident) {
		error(2);
		skip(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)ident))),
			LINK);
	}
	while (sy == ident) {
		lcp = (identifier*) malloc(sizeof(identifier));
		memcpy(lcp->name, id, sizeof(alpha));
		lcp->idtype = NULL;
		lcp->next = NULL;
		lcp->klass = konst;
		insymbol();
		if (sy == relop && op == eqop)
			insymbol();
		else
			error(16);
		constant_(P_setunion(SET1, LINK->fsys,
			P_expset(SET, 1L << ((long)semicolon))), &lsp,
			&lvalu, LINK);
		enterid(lcp);
		lcp->idtype = lsp;
		lcp->UU.values = lvalu;
		if (sy == semicolon) {
			insymbol();
			if (!P_inset(sy, P_setunion(SET1, LINK->fsys,
				P_expset(SET, 1L << ((long)ident))))) {
				error(6);
				skip(P_setunion(SET3, LINK->fsys, P_expset(SET2, 1L << ((long)ident))),
					LINK);
			}
		}
		else
			error(14);
	}
	/* p2c: pcom.p, line 1423:
	 * Note: No Specialmalloc form known for IDENTIFIER.KONST [187] */
}

void PascalPParser::typedeclaration(struct LOC_block* LINK)
{
	identifier* lcp, * lcp1, * lcp2;
	structure* lsp;
	addrrange lsize;
	setofsys SET, SET1, SET2, SET3;

	/*typedeclaration*/
	if (sy != ident) {
		error(2);
		skip(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)ident))),
			LINK);
	}
	while (sy == ident) {
		lcp = (identifier*) malloc(sizeof(identifier));
		memcpy(lcp->name, id, sizeof(alpha));
		lcp->idtype = NULL;
		lcp->klass = types;
		insymbol();
		if (sy == relop && op == eqop)
			insymbol();
		else
			error(16);
		typ(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)semicolon))),
			&lsp, &lsize, LINK);
		enterid(lcp);
		lcp->idtype = lsp;
		/*has any forward reference been satisfied:*/
		lcp1 = fwptr;
		while (lcp1 != NULL) {
			if (!strncmp(lcp1->name, lcp->name, sizeof(alpha))) {
				lcp1->idtype->UU.eltype = lcp->idtype;
				if (lcp1 != fwptr)
					lcp2->next = lcp1->next;
				else
					fwptr = lcp1->next;
			}
			else
				lcp2 = lcp1;
			lcp1 = lcp1->next;
		}
		if (sy == semicolon) {
			insymbol();
			if (!P_inset(sy, P_setunion(SET1, LINK->fsys,
				P_expset(SET, 1L << ((long)ident))))) {
				error(6);
				skip(P_setunion(SET3, LINK->fsys, P_expset(SET2, 1L << ((long)ident))),
					LINK);
			}
		}
		else
			error(14);
	}
	/* p2c: pcom.p, line 1446:
	 * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
	if (fwptr == NULL)
		return;
	error(117);
	putchar('\n');
	do {
		printf(" type-id %.8s\n", fwptr->name);
		fwptr = fwptr->next;
	} while (fwptr != NULL);
	if (!eol)
		printf("%*c", (int)(chcnt + 16), ' ');
}

void PascalPParser::vardeclaration(struct LOC_block* LINK)
{
	identifier* lcp, * nxt;
	structure* lsp;
	addrrange lsize;
	identifier* WITH;
	setofsys SET, SET1, SET2, SET3, SET4, SET5;

	/*vardeclaration*/
	nxt = NULL;
	do {
		do {
			if (sy == ident) {
				lcp = (identifier*) malloc(sizeof(identifier));
				memcpy(lcp->name, id, sizeof(alpha));
				lcp->next = nxt;
				lcp->klass = vars;
				lcp->idtype = NULL;
				lcp->UU.U2.vkind = actual;
				lcp->UU.U2.vlev = level;
				enterid(lcp);
				nxt = lcp;
				insymbol();
			}
			else
				error(2);
			/* p2c: pcom.p, line 1489:
			 * Note: No Specialmalloc form known for IDENTIFIER.VARS [187] */
			if (!P_inset(sy, P_setunion(SET2, P_setunion(SET1, LINK->fsys,
				P_expset(SET, (1L << ((long)comma)) | (1L << ((long)colon)))),
				typedels))) {
				error(6);
				skip(P_setunion(SET5, P_setunion(SET4, LINK->fsys, P_expset(SET3,
					(1L << ((long)comma)) | (1L << ((long)colon)) |
					(1L << ((long)semicolon)))), typedels), LINK);
			}
			LINK->test = (sy != comma);
			if (!LINK->test)
				insymbol();
		} while (!LINK->test);
		if (sy == colon)
			insymbol();
		else
			error(5);
		typ(P_setunion(SET2, P_setunion(SET1, LINK->fsys,
			P_expset(SET, 1L << ((long)semicolon))),
			typedels), &lsp, &lsize, LINK);
		while (nxt != NULL) {
			WITH = nxt;
			align(lsp, &lc);
			WITH->idtype = lsp;
			WITH->UU.U2.vaddr = lc;
			lc += lsize;
			nxt = WITH->next;
		}
		if (sy == semicolon) {
			insymbol();
			if (!P_inset(sy, P_setunion(SET1, LINK->fsys,
				P_expset(SET, 1L << ((long)ident))))) {
				error(6);
				skip(P_setunion(SET3, LINK->fsys, P_expset(SET2, 1L << ((long)ident))),
					LINK);
			}
		}
		else
			error(14);
	} while (!((sy != ident) & (!P_inset(sy, typedels))));
	if (fwptr == NULL)
		return;
	error(117);
	putchar('\n');
	do {
		printf(" type-id %.8s\n", fwptr->name);
		fwptr = fwptr->next;
	} while (fwptr != NULL);
	if (!eol)
		printf("%*c", (int)(chcnt + 16), ' ');
}

void PascalPParser::parameterlist(long* fsy, identifier** fpar, struct LOC_procdeclaration* LINK)
{
	identifier* lcp, * lcp1, * lcp2, * lcp3;
	structure* lsp;
	idkind lkind;
	addrrange llc, lsize;
	long count;
	setofsys SET, SET1, SET2, SET3, SET4;
	identifier* WITH;

	/*parameterlist*/
	lcp1 = NULL;
	if (!P_inset(sy,
		P_setunion(SET1, fsy, P_expset(SET, 1L << ((long)lparent))))) {
		error(7);
		skip(P_setunion(SET4, P_setunion(SET2, LINK->LINK->fsys, fsy),
			P_expset(SET3, 1L << ((long)lparent))), LINK->LINK);
	}
	if (sy != lparent) {
		*fpar = NULL;
		return;
	}
	if (LINK->forw)
		error(119);
	insymbol();
	if ((unsigned long)sy >= 32 ||
		((1L << ((long)sy)) & ((1L << ((long)ident)) | (1L << ((long)varsy)) |
			(1L << ((long)procsy)) | (1L << ((long)funcsy)))) == 0) {
		error(7);
		skip(P_setunion(SET1, LINK->LINK->fsys,
			P_expset(SET, (1L << ((long)ident)) | (1L << ((long)rparent)))),
			LINK->LINK);
	}
	while ((unsigned long)sy < 32 &&
		((1L << ((long)sy)) & ((1L << ((long)ident)) | (1L << ((long)varsy)) |
			(1L << ((long)procsy)) | (1L << ((long)funcsy)))) != 0)
	{   /*while*/
		if (sy == procsy) {
			error(399);
			do {
				insymbol();
				if (sy == ident) {
					lcp = (identifier*) malloc(sizeof(identifier));
					memcpy(lcp->name, id, sizeof(alpha));
					lcp->idtype = NULL;
					lcp->next = lcp1;
					lcp->UU.U4.UU.U1.pflev = level;   /*beware of parameter procedures*/
					lcp->klass = proc;
					lcp->UU.U4.pfdeckind = declared;
					lcp->UU.U4.UU.U1.pfkind = formal;
					enterid(lcp);
					lcp1 = lcp;
					align(parmptr, &lc);
					/*lc := lc + some size */
					insymbol();
				}
				else
					error(2);
				/* p2c: pcom.p, line 1550: Note:
				 * No Specialmalloc form known for IDENTIFIER.PROC.DECLARED.FORMAL [187] */
				if (!P_inset(sy, P_setunion(SET1, LINK->LINK->fsys, P_expset(SET,
					(1L << ((long)comma)) | (1L << ((long)semicolon)) |
					(1L << ((long)rparent)))))) {
					error(7);
					skip(P_setunion(SET3, LINK->LINK->fsys,
						P_expset(SET2,
							(1L << ((long)comma)) | (1L << ((long)semicolon)) |
							(1L << ((long)rparent)))), LINK->LINK);
				}
			} while (sy == comma);
		}
		else {
			if (sy == funcsy) {
				error(399);
				lcp2 = NULL;
				do {
					insymbol();
					if (sy == ident) {
						lcp = (identifier*) malloc(sizeof(identifier));
						memcpy(lcp->name, id, sizeof(alpha));
						lcp->idtype = NULL;
						lcp->next = lcp2;
						lcp->UU.U4.UU.U1.pflev = level;   /*beware param funcs*/
						lcp->klass = func;
						lcp->UU.U4.pfdeckind = declared;
						lcp->UU.U4.UU.U1.pfkind = formal;
						enterid(lcp);
						lcp2 = lcp;
						align(parmptr, &lc);
						/*lc := lc + some size*/
						insymbol();
					}
					/* p2c: pcom.p, line 1573: Note:
					 * No Specialmalloc form known for IDENTIFIER.FUNC.DECLARED.FORMAL [187] */
					if (!P_inset(sy, P_setunion(SET1,
						P_expset(SET, (1L << ((long)comma)) | (1L << ((long)colon))),
						LINK->LINK->fsys))) {
						error(7);
						skip(P_setunion(SET3, LINK->LINK->fsys, P_expset(SET2,
							(1L << ((long)comma)) | (1L << ((long)semicolon)) |
							(1L << ((long)rparent)))), LINK->LINK);
					}
				} while (sy == comma);
				if (sy == colon) {
					insymbol();
					if (sy == ident) {
						searchid(1L << ((long)types), &lcp);
						lsp = lcp->idtype;
						if (lsp != NULL) {
							if (((1L << lsp->form) & ((1L << ((long)scalar)) |
								(1L << ((long)subrange)) | (1L << ((long)pointer)))) == 0) {
								error(120);
								lsp = NULL;
							}
						}
						lcp3 = lcp2;
						while (lcp2 != NULL) {
							lcp2->idtype = lsp;
							lcp = lcp2;
							lcp2 = lcp2->next;
						}
						lcp->next = lcp1;
						lcp1 = lcp3;
						insymbol();
					}
					else
						error(2);
					if (!P_inset(sy, P_setunion(SET1, LINK->LINK->fsys, P_expset(SET,
						(1L << ((long)semicolon)) | (1L << ((long)rparent)))))) {
						error(7);
						skip(P_setunion(SET3, LINK->LINK->fsys, P_expset(SET2,
							(1L << ((long)semicolon)) | (1L << ((long)rparent)))),
							LINK->LINK);
					}
				}
				else
					error(5);
			}
			else {
				if (sy == varsy) {
					lkind = formal;
					insymbol();
				}
				else
					lkind = actual;
				lcp2 = NULL;
				count = 0;
				do {
					if (sy == ident) {
						lcp = (identifier*) malloc(sizeof(identifier));
						memcpy(lcp->name, id, sizeof(alpha));
						lcp->idtype = NULL;
						lcp->klass = vars;
						lcp->UU.U2.vkind = lkind;
						lcp->next = lcp2;
						lcp->UU.U2.vlev = level;
						enterid(lcp);
						lcp2 = lcp;
						count++;
						insymbol();
					}
					/* p2c: pcom.p, line 1621:
					 * Note: No Specialmalloc form known for IDENTIFIER.VARS [187] */
					if (!P_inset(sy, P_setunion(SET1,
						P_expset(SET, (1L << ((long)comma)) | (1L << ((long)colon))),
						LINK->LINK->fsys))) {
						error(7);
						skip(P_setunion(SET3, LINK->LINK->fsys, P_expset(SET2,
							(1L << ((long)comma)) | (1L << ((long)semicolon)) |
							(1L << ((long)rparent)))), LINK->LINK);
					}
					LINK->LINK->test = (sy != comma);
					if (!LINK->LINK->test)
						insymbol();
				} while (!LINK->LINK->test);
				if (sy == colon) {
					insymbol();
					if (sy == ident) {
						searchid(1L << ((long)types), &lcp);
						lsp = lcp->idtype;
						lsize = ptrsize;
						if (lsp != NULL) {
							if (lkind == actual) {
								if ((structform)lsp->form <= power)
									lsize = lsp->size;
								else if ((structform)lsp->form == files)
									error(121);
							}
						}
						align(parmptr, &lsize);
						lcp3 = lcp2;
						align(parmptr, &lc);
						lc += count * lsize;
						llc = lc;
						while (lcp2 != NULL) {
							lcp = lcp2;
							lcp2->idtype = lsp;
							llc -= lsize;
							lcp2->UU.U2.vaddr = llc;
							lcp2 = lcp2->next;
						}
						lcp->next = lcp1;
						lcp1 = lcp3;
						insymbol();
					}
					else
						error(2);
					if (!P_inset(sy, P_setunion(SET1, LINK->LINK->fsys, P_expset(SET,
						(1L << ((long)semicolon)) | (1L << ((long)rparent)))))) {
						error(7);
						skip(P_setunion(SET3, LINK->LINK->fsys, P_expset(SET2,
							(1L << ((long)semicolon)) | (1L << ((long)rparent)))),
							LINK->LINK);
					}
				}
				else
					error(5);
			}
		}
		if (sy == semicolon) {
			insymbol();
			if (!P_inset(sy, P_setunion(SET1, LINK->LINK->fsys, P_expset(SET,
				(1L << ((long)ident)) | (1L << ((long)varsy)) |
				(1L << ((long)procsy)) | (1L << ((long)funcsy)))))) {
				error(7);
				skip(P_setunion(SET3, LINK->LINK->fsys, P_expset(SET2,
					(1L << ((long)ident)) | (1L << ((long)rparent)))),
					LINK->LINK);
			}
		}
	}
	if (sy == rparent) {
		insymbol();
		if (!P_inset(sy, P_setunion(SET, fsy, LINK->LINK->fsys))) {
			error(6);
			skip(P_setunion(SET1, fsy, LINK->LINK->fsys), LINK->LINK);
		}
	}
	else
		error(4);
	lcp3 = NULL;
	/*reverse pointers and reserve local cells for copies of multiple
	 values*/
	while (lcp1 != NULL) {
		WITH = lcp1;
		lcp2 = WITH->next;
		WITH->next = lcp3;
		if ((idclass)WITH->klass == vars) {
			if (WITH->idtype != NULL) {
				if ((idkind)WITH->UU.U2.vkind == actual &&
					(structform)WITH->idtype->form > power) {
					align(WITH->idtype, &lc);
					WITH->UU.U2.vaddr = lc;
					lc += WITH->idtype->size;
				}
			}
		}
		lcp3 = lcp1;
		lcp1 = lcp2;
	}
	*fpar = lcp3;
}

void PascalPParser::procdeclaration(symbol fsy, struct LOC_block* LINK)
{
	struct LOC_procdeclaration V;
	char oldlev;
	identifier* lcp, * lcp1;
	structure* lsp;
	disprange oldtop;
	addrrange llc, lcm;
	long lbname, * markp;
	_REC_display* WITH;
	setofsys SET, SET1;

	V.LINK = LINK;
	/*procdeclaration*/
	llc = lc;
	lc = lcaftermarkstack;
	V.forw = false;
	if (sy == ident) {
		searchsection(display[top].fname, &lcp);   /*decide whether forw.*/
		if (lcp != NULL) {
			if ((idclass)lcp->klass == proc)
				V.forw = (lcp->UU.U4.UU.U1.UU.U0.forwdecl && fsy == procsy &&
					(idkind)lcp->UU.U4.UU.U1.pfkind == actual);
			else {
				if ((idclass)lcp->klass == func)
					V.forw = (lcp->UU.U4.UU.U1.UU.U0.forwdecl && fsy == funcsy &&
						(idkind)lcp->UU.U4.UU.U1.pfkind == actual);
				else
					V.forw = false;
			}
			if (!V.forw)
				error(160);
		}
		if (!V.forw) {
			if (fsy == procsy)
				lcp = (identifier*) malloc(sizeof(identifier));
			else
				lcp = (identifier*) malloc(sizeof(identifier));
			/* p2c: pcom.p, line 1719: Note:
			 * No Specialmalloc form known for IDENTIFIER.PROC.DECLARED.ACTUAL [187] */
			memcpy(lcp->name, id, sizeof(alpha));
			lcp->idtype = NULL;
			lcp->UU.U4.UU.U1.UU.U0.externl = false;
			lcp->UU.U4.UU.U1.pflev = level;
			genlabel(&lbname);

		
			lcp->UU.U4.pfdeckind = declared;
			lcp->UU.U4.UU.U1.pfkind = actual;
			lcp->UU.U4.UU.U1.pfname = lbname;
			if (fsy == procsy)
				lcp->klass = proc;
			else
				lcp->klass = func;
			enterid(lcp);
		}
		else {
			lcp1 = lcp->next;
			while (lcp1 != NULL) {
				if (lcp1->klass == vars) {
					if (lcp1->idtype != NULL) {
						lcm = lcp1->UU.U2.vaddr + lcp1->idtype->size;
						if (lcm > lc)
							lc = lcm;
					}
				}
				lcp1 = lcp1->next;
			}
		}
		insymbol();
	}
	else {
		error(2);
		lcp = ufctptr;
	}
	oldlev = level;
	oldtop = top;
	if (level < maxlevel)
		level++;
	else
		error(251);
	if (top < displimit) {
		top++;
		WITH = &display[top];
		if (V.forw)
			WITH->fname = lcp->next;
		else
			WITH->fname = NULL;
		WITH->flabel = NULL;
		WITH->occur = blck;
	}
	else
		error(250);
	if (fsy == procsy) {
		parameterlist(P_expset(SET, 1L << ((long)semicolon)), &lcp1, &V);
		if (!V.forw)
			lcp->next = lcp1;
	}
	else {
		parameterlist(P_expset(SET,
			(1L << ((long)semicolon)) | (1L << ((long)colon))),
			&lcp1, &V);
		if (!V.forw)
			lcp->next = lcp1;
		if (sy == colon) {
			insymbol();
			if (sy == ident) {
				if (V.forw)
					error(122);
				searchid(1L << ((long)types), &lcp1);
				lsp = lcp1->idtype;
				lcp->idtype = lsp;
				if (lsp != NULL) {
					if (((1L << lsp->form) & ((1L << ((long)scalar)) |
						(1L << ((long)subrange)) | (1L << ((long)pointer)))) == 0) {
						error(120);
						lcp->idtype = NULL;
					}
				}
				insymbol();
			}
			else {
				error(2);
				skip(P_setunion(SET1, LINK->fsys,
					P_expset(SET, 1L << ((long)semicolon))), LINK);
			}
		}
		else {
			if (!V.forw)
				error(123);
		}
	}
	if (sy == semicolon)
		insymbol();
	else
		error(14);
	if (sy == forwardsy) {
		if (V.forw)
			error(161);
		else
			lcp->UU.U4.UU.U1.UU.U0.forwdecl = true;
		insymbol();
		if (sy == semicolon)
			insymbol();
		else
			error(14);
		if (!P_inset(sy, LINK->fsys)) {
			error(6);
			skip(LINK->fsys, LINK);
		}
	}
	else {
		lcp->UU.U4.UU.U1.UU.U0.forwdecl = false;
		mark_(&markp);
		do {
			block(LINK->fsys, semicolon, lcp);
			if (sy == semicolon) {
				if (prtables)
					printtables(false);
				insymbol();
				if ((unsigned long)sy >= 32 ||
					((1L << ((long)sy)) & ((1L << ((long)beginsy)) |
						(1L << ((long)procsy)) | (1L << ((long)funcsy)))) == 0) {
					error(6);
					skip(LINK->fsys, LINK);
				}
			}
			else
				error(14);
		} while (!(((unsigned long)sy < 32 &&
			((1L << ((long)sy)) & ((1L << ((long)beginsy)) |
				(1L << ((long)procsy)) | (1L << ((long)funcsy)))) !=
			0) | P_eof(stdin)));
		release_(markp);   /* return local entries on runtime heap */
	}
	level = oldlev;
	top = oldtop;
	lc = llc;

	/* p2c: pcom.p, line 1719: Note:
	 * No Specialmalloc form known for IDENTIFIER.FUNC.DECLARED.ACTUAL [187] */
}

void PascalPParser::mes(long i, struct LOC_body* LINK)
{
	LINK->topnew += cdx[i];
	if (LINK->topnew > LINK->topmax)
		LINK->topmax = LINK->topnew;
}

void PascalPParser::putic(struct LOC_body* LINK)
{
	if (ic % 10 == 0)
		fprintf(prr.f, "i%5d\n", ic);
}

void PascalPParser::gen0(oprange fop, struct LOC_body* LINK)
{
	/*gen0*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s\n", mn[fop]);
	}
	ic++;
	mes(fop, LINK);
}

void PascalPParser::gen1(oprange fop, long fp2, struct LOC_body* LINK)
{
	long k;
	constant* WITH;
	long FORLIM;

	/*gen1*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s", mn[fop]);
		if (fop == 30) {
			fprintf(prr.f, "        %.4s\n", sna[fp2 - 1]);
			LINK->topnew += pdx[fp2 - 1];
			if (LINK->topnew > LINK->topmax)
				LINK->topmax = LINK->topnew;
		}
		else {
			if (fop == 38) {
				putc('\'', prr.f);
				WITH = LINK->cstptr[fp2 - 1];
				FORLIM = WITH->UU.U2.slgth;
				for (k = 0; k < FORLIM; k++)
					putc(WITH->UU.U2.sval[k], prr.f);
				for (k = WITH->UU.U2.slgth + 1; k <= strglgth; k++)
					putc(' ', prr.f);
				fprintf(prr.f, "'\n");
			}
			else if (fop == 42)
				fprintf(prr.f, "%c\n", (char)fp2);
			else
				fprintf(prr.f, "%12ld\n", fp2);
			mes(fop, LINK);
		}
	}
	ic++;
}

void PascalPParser::gen2(oprange fop, long fp1, long fp2, struct LOC_body* LINK)
{
	long k;
	constant* WITH;

	/*gen2*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s", mn[fop]);
		switch (fop) {

		case 45:
		case 50:
		case 54:
		case 56:
			fprintf(prr.f, " %3ld%8ld\n", fp1, fp2);
			break;

		case 47:
		case 48:
		case 49:
		case 52:
		case 53:
		case 55:
			putc((char)fp1, prr.f);
			if (fp1 == 'm')
				fprintf(prr.f, "%11ld", fp2);
			putc('\n', prr.f);
			break;

		case 51:
			switch (fp1) {

			case 1:
				fprintf(prr.f, "i %12ld\n", fp2);
				break;

			case 2:
				fprintf(prr.f, "r ");
				WITH = LINK->cstptr[fp2 - 1];
				for (k = 0; k < strglgth; k++)
					putc(WITH->UU.rval[k], prr.f);
				putc('\n', prr.f);
				break;

			case 3:
				fprintf(prr.f, "b %12ld\n", fp2);
				break;

			case 4:
				fprintf(prr.f, "n\n");
				break;

			case 6:
				fprintf(prr.f, "%3s%c'\n", "c '", (char)fp2);
				break;

			case 5:
				putc('(', prr.f);
				WITH = LINK->cstptr[fp2 - 1];
				for (k = setlow; k <= sethigh; k++) {
					if (P_inset(k, WITH->UU.pval))
						fprintf(prr.f, "%3ld", k);
				}
				fprintf(prr.f, ")\n");
				break;
			}
			break;
		}
	}
	ic++;
	mes(fop, LINK);
}

void PascalPParser::gentypindicator(structure* fsp, struct LOC_body* LINK)
{
	/*typindicator*/
	if (fsp == NULL)
		return;
	switch ((structform)fsp->form) {

	case scalar:
		if (fsp == intptr)
			putc('i', prr.f);
		else {
			if (fsp == boolptr)
				putc('b', prr.f);
			else {
				if (fsp == charptr)
					putc('c', prr.f);
				else {
					if ((declkind)fsp->UU.U0.scalkind == declared)
						putc('i', prr.f);
					else
						putc('r', prr.f);
				}
			}
		}
		break;

	case subrange:
		gentypindicator(fsp->UU.U1.rangetype, LINK);
		break;

	case pointer:
		putc('a', prr.f);
		break;

	case power:
		putc('s', prr.f);
		break;

	case records:
	case arrays:
		putc('m', prr.f);
		break;

	case files:
	case tagfld:
	case variant:
		error(500);
		break;
	}
}

void PascalPParser::gen0t(oprange fop, structure* fsp, struct LOC_body* LINK)
{
	/*gen0t*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s", mn[fop]);
		gentypindicator(fsp, LINK);
		putc('\n', prr.f);
	}
	ic++;
	mes(fop, LINK);
}

void PascalPParser::gen1t(oprange fop, long fp2, structure* fsp, struct LOC_body* LINK)
{
	/*gen1t*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s", mn[fop]);
		gentypindicator(fsp, LINK);
		fprintf(prr.f, "%11ld\n", fp2);
	}
	ic++;
	mes(fop, LINK);
}

void PascalPParser::gen2t(oprange fop, long fp1, long fp2, structure* fsp, struct LOC_body* LINK)
{
	/*gen2t*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s", mn[fop]);
		gentypindicator(fsp, LINK);
		fprintf(prr.f, "%*ld%8ld\n", (labs(fp1) > 99) * 5 + 3, fp1, fp2);
	}
	ic++;
	mes(fop, LINK);
}

void PascalPParser::load(struct LOC_body* LINK)
{
	/*load*/
	if (gattr.typtr == NULL)
		return;
	switch (gattr.kind) {

	case cst:
		if ((structform)gattr.typtr->form == scalar && gattr.typtr != realptr) {
			if (gattr.typtr == boolptr)   /*ldc*/
				gen2(51, 3, gattr.UU.cval.UU.ival, LINK);
			else {
				if (gattr.typtr == charptr)   /*ldc*/
					gen2(51, 6, gattr.UU.cval.UU.ival, LINK);
				else   /*ldc*/
					gen2(51, 1, gattr.UU.cval.UU.ival, LINK);
			}
		}
		else {
			if (gattr.typtr == nilptr)   /*ldc*/
				gen2(51, 4, 0, LINK);
			else {
				if (LINK->cstptrix >= cstoccmax)
					error(254);
				else {
					LINK->cstptrix++;
					LINK->cstptr[LINK->cstptrix - 1] = gattr.UU.cval.UU.valp;
					if (gattr.typtr == realptr)   /*ldc*/
						gen2(51, 2, LINK->cstptrix, LINK);
					else   /*ldc*/
						gen2(51, 5, LINK->cstptrix, LINK);
				}
			}
		}
		break;

	case varbl:
		switch (gattr.UU.U1.access) {

		case drct:
			if (gattr.UU.U1.UU.U0.vlevel <= 1)   /*ldo*/
				gen1t(39, gattr.UU.U1.UU.U0.dplmt, gattr.typtr, LINK);
			else   /*lod*/
				gen2t(54, level - gattr.UU.U1.UU.U0.vlevel, gattr.UU.U1.UU.U0.dplmt,
					gattr.typtr, LINK);
			break;

		case indrct:   /*ind*/
			gen1t(35, gattr.UU.U1.UU.idplmt, gattr.typtr, LINK);
			break;

		case inxd:
			error(400);
			break;
		}
		break;

	case expr:
		/* blank case */
		break;
	}
	gattr.kind = expr;
}

void PascalPParser::store(attr* fattr, struct LOC_body* LINK)
{
	/*store*/
	if (fattr->typtr == NULL)
		return;
	switch (fattr->UU.U1.access) {

	case drct:
		if (fattr->UU.U1.UU.U0.vlevel <= 1)   /*sro*/
			gen1t(43, fattr->UU.U1.UU.U0.dplmt, fattr->typtr, LINK);
		else   /*str*/
			gen2t(56, level - fattr->UU.U1.UU.U0.vlevel, fattr->UU.U1.UU.U0.dplmt,
				fattr->typtr, LINK);
		break;

	case indrct:
		if (fattr->UU.U1.UU.idplmt != 0)
			error(400);
		else   /*sto*/
			gen0t(26, fattr->typtr, LINK);
		break;

	case inxd:
		error(400);
		break;
	}
}

void PascalPParser::loadaddress(struct LOC_body* LINK)
{
	/*loadaddress*/
	if (gattr.typtr == NULL)
		return;
	switch (gattr.kind) {

	case cst:
		if (string(gattr.typtr, LINK->LINK)) {
			if (LINK->cstptrix >= cstoccmax)
				error(254);
			else {
				LINK->cstptrix++;
				LINK->cstptr[LINK->cstptrix - 1] = gattr.UU.cval.UU.valp;   /*lca*/
				gen1(38, LINK->cstptrix, LINK);
			}
		}
		else
			error(400);
		break;

	case varbl:
		switch (gattr.UU.U1.access) {

		case drct:
			if (gattr.UU.U1.UU.U0.vlevel <= 1)   /*lao*/
				gen1(37, gattr.UU.U1.UU.U0.dplmt, LINK);
			else   /*lda*/
				gen2(50, level - gattr.UU.U1.UU.U0.vlevel, gattr.UU.U1.UU.U0.dplmt,
					LINK);
			break;

		case indrct:
			if (gattr.UU.U1.UU.idplmt != 0)   /*inc*/
				gen1t(34, gattr.UU.U1.UU.idplmt, nilptr, LINK);
			break;

		case inxd:
			error(400);
			break;
		}
		break;

	case expr:
		error(400);
		break;
	}
	gattr.kind = varbl;
	gattr.UU.U1.access = indrct;
	gattr.UU.U1.UU.idplmt = 0;
}


void PascalPParser::genfjp(long faddr, struct LOC_body* LINK)
{
	/*genfjp*/
	load(LINK);
	if (gattr.typtr != NULL) {
		if (gattr.typtr != boolptr)
			error(144);
	}
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s%8s%4ld\n", mn[33], " l", faddr);
	}
	ic++;
	mes(33, LINK);
}

void PascalPParser::genujpxjp(oprange fop, long fp2, struct LOC_body* LINK)
{
	/*genujpxjp*/
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s%8s%4ld\n", mn[fop], " l", fp2);
	}
	ic++;
	mes(fop, LINK);
}


void PascalPParser::gencupent(oprange fop, long fp1, long fp2, struct LOC_body* LINK)
{
	if (prcode) {
		putic(LINK);
		fprintf(prr.f, "%.4s%4ld%4c%4ld\n", mn[fop], fp1, 'l', fp2);
	}
	ic++;
	mes(fop, LINK);
}


void PascalPParser::checkbnds(structure* fsp, struct LOC_body* LINK)
{
	long lmin, lmax;

	/*checkbnds*/
	if (fsp == NULL)
		return;
	if (fsp == intptr)
		return;
	if (fsp == realptr)
		return;
	if ((structform)fsp->form <= subrange) {
		getbounds(fsp, &lmin, &lmax);   /*chk*/
		gen2t(45, lmin, lmax, fsp, LINK);
	}
}


void PascalPParser::putlabel(long labname, struct LOC_body* LINK)
{
	/*putlabel*/
	if (prcode)
		fprintf(prr.f, "l%4ld\n", labname);
}

void PascalPParser::selector(long* fsys, identifier* fcp, struct LOC_statement* LINK)
{
	attr lattr;
	identifier* lcp;
	addrrange lsize;
	long lmin, lmax;
	structure* WITH;
	_REC_display* WITH1;
	setofsys SET, SET1;

	/*selector*/
	gattr.typtr = fcp->idtype;
	gattr.kind = varbl;
	switch ((idclass)fcp->klass) {   /*with*/

	case vars:
		if ((idkind)fcp->UU.U2.vkind == actual) {
			gattr.UU.U1.access = drct;
			gattr.UU.U1.UU.U0.vlevel = fcp->UU.U2.vlev;
			gattr.UU.U1.UU.U0.dplmt = fcp->UU.U2.vaddr;
		}
		else {   /*lod*/
			gen2t(54, level - fcp->UU.U2.vlev, fcp->UU.U2.vaddr, nilptr, LINK->LINK);
			gattr.UU.U1.access = indrct;
			gattr.UU.U1.UU.idplmt = 0;
		}
		break;

	case field:
		WITH1 = &display[disx];
		if ((where)WITH1->occur == crec) {
			gattr.UU.U1.access = drct;
			gattr.UU.U1.UU.U0.vlevel = WITH1->UU.U1.clev;
			gattr.UU.U1.UU.U0.dplmt = WITH1->UU.U1.cdspl + fcp->UU.fldaddr;
		}
		else {
			if (level == 1)   /*ldo*/
				gen1t(39, WITH1->UU.vdspl, nilptr, LINK->LINK);
			else   /*lod*/
				gen2t(54, 0, WITH1->UU.vdspl, nilptr, LINK->LINK);
			gattr.UU.U1.access = indrct;
			gattr.UU.U1.UU.idplmt = fcp->UU.fldaddr;
		}
		break;

	case func:
		if ((declkind)fcp->UU.U4.pfdeckind == standard) {
			error(150);
			gattr.typtr = NULL;
		}
		else {
			if ((idkind)fcp->UU.U4.UU.U1.pfkind == formal)
				error(151);
			else {
				if (fcp->UU.U4.UU.U1.pflev + 1 != level ||
					LINK->LINK->LINK->fprocp != fcp)
					error(177);
			}
			gattr.UU.U1.access = drct;
			gattr.UU.U1.UU.U0.vlevel = fcp->UU.U4.UU.U1.pflev + 1;
			gattr.UU.U1.UU.U0.dplmt = 0;   /*impl. relat. addr. of fct. result*/
		}
		break;
	}/*case*/
	if (!P_inset(sy, P_setunion(SET, selectsys, fsys))) {
		error(59);
		skip(P_setunion(SET1, selectsys, fsys), LINK->LINK->LINK);
	}
	while (P_inset(sy, selectsys)) {
		/*[*/
		if (sy == lbrack) {
			do {
				lattr = gattr;
				if (lattr.typtr != NULL) {
					if ((structform)lattr.typtr->form != arrays) {
						error(138);
						lattr.typtr = NULL;
					}
				}
				loadaddress(LINK->LINK);
				insymbol();
				expression(P_setunion(SET1, fsys,
					P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rbrack)))),
					LINK);
				load(LINK->LINK);
				if (gattr.typtr != NULL) {
					if ((structform)gattr.typtr->form != scalar)
						error(113);
					else if (!comptypes(gattr.typtr, intptr, LINK->LINK->LINK))
						gen0t(58, gattr.typtr, LINK->LINK);
				}
				if (lattr.typtr != NULL) {
					WITH = lattr.typtr;
					if (comptypes(WITH->UU.U4.inxtype, gattr.typtr, LINK->LINK->LINK)) {
						if (WITH->UU.U4.inxtype != NULL) {
							getbounds(WITH->UU.U4.inxtype, &lmin, &lmax);
							if (debug)   /*chk*/
								gen2t(45, lmin, lmax, intptr, LINK->LINK);
							if (lmin > 0)   /*dec*/
								gen1t(31, lmin, intptr, LINK->LINK);
							else if (lmin < 0)
								gen1t(34, -lmin, intptr, LINK->LINK);
							/*or simply gen1(31,lmin)*/
						}
					}
					else
						error(139);
					gattr.typtr = WITH->UU.U4.aeltype;
					gattr.kind = varbl;
					gattr.UU.U1.access = indrct;
					gattr.UU.U1.UU.idplmt = 0;
					if (gattr.typtr != NULL) {
						lsize = gattr.typtr->size;
						align(gattr.typtr, &lsize);   /*ixa*/
						gen1(36, lsize, LINK->LINK);
					}
				}
			} while (sy == comma);
			if (sy == rbrack)
				insymbol();
			else
				error(12);
		}  /*if sy = lbrack*/
		else {
			/*.*/
			if (sy == period) {
				if (gattr.typtr != NULL) {
					if ((structform)gattr.typtr->form != records) {
						error(140);
						gattr.typtr = NULL;
					}
				}
				insymbol();
				if (sy == ident) {   /*with gattr*/
					if (gattr.typtr != NULL) {
						searchsection(gattr.typtr->UU.U5.fstfld, &lcp);
						if (lcp == NULL) {
							error(152);
							gattr.typtr = NULL;
						}
						else {
							gattr.typtr = lcp->idtype;
							switch (gattr.UU.U1.access) {

							case drct:
								gattr.UU.U1.UU.U0.dplmt += lcp->UU.fldaddr;
								break;

							case indrct:
								gattr.UU.U1.UU.idplmt += lcp->UU.fldaddr;
								break;

							case inxd:
								error(400);
								break;
							}
						}
					}
					insymbol();
				}  /*sy = ident*/
				else
					error(2);
			}  /*if sy = period*/
			else {
				/*^*/
				if (gattr.typtr != NULL) {
					WITH = gattr.typtr;
					if ((structform)WITH->form == pointer) {
						load(LINK->LINK);
						gattr.typtr = WITH->UU.eltype;
						if (debug)   /*chk*/
							gen2t(45, 1, maxaddr, nilptr, LINK->LINK);
						gattr.kind = varbl;
						gattr.UU.U1.access = indrct;
						gattr.UU.U1.UU.idplmt = 0;
					}
					else {
						if ((structform)WITH->form == files)
							gattr.typtr = WITH->UU.filtype;
						else
							error(141);
					}
				}
				insymbol();
			}
		}
		if (!P_inset(sy, P_setunion(SET, fsys, selectsys))) {
			error(6);
			skip(P_setunion(SET1, fsys, selectsys), LINK->LINK->LINK);
		}
	}  /*while*/

	/*ord*/
	/*inc*/
}


void PascalPParser::variable(long* fsys, struct LOC_call* LINK)
{
	identifier* lcp;

	/*variable*/
	if (sy == ident) {
		searchid((1L << ((long)vars)) | (1L << ((long)field)), &lcp);
		insymbol();
	}
	else {
		error(2);
		lcp = uvarptr;
	}
	selector(fsys, lcp, LINK->LINK);
}

void PascalPParser::getputresetrewrite(struct LOC_call* LINK)
{
	setofsys SET, SET1;

	/*getputresetrewrite*/
	variable(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)rparent))),
		LINK);
	loadaddress(LINK->LINK->LINK);
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form != files)
			error(116);
	}
	if (LINK->lkey <= 2)   /*csp*/
		gen1(30, LINK->lkey, LINK->LINK->LINK);
	else
		error(399);
	/*get,put*/
}

void PascalPParser::read(struct LOC_call* LINK)
{
	levrange llev;
	addrrange laddr;
	structure* lsp;
	setofsys SET, SET1;

	/*read*/
	llev = 1;
	laddr = lcaftermarkstack;
	if (sy == lparent) {
		insymbol();
		variable(P_setunion(SET1, LINK->fsys, P_expset(SET,
			(1L << ((long)comma)) | (1L << ((long)rparent)))),
			LINK);
		lsp = gattr.typtr;
		LINK->LINK->LINK->LINK->test = false;
		if (lsp != NULL) {
			if ((structform)lsp->form == files) {
				if (lsp->UU.filtype == charptr) {
					llev = gattr.UU.U1.UU.U0.vlevel;
					laddr = gattr.UU.U1.UU.U0.dplmt;
				}
				else
					error(399);
				if (sy == rparent) {
					if (LINK->lkey == 5)
						error(116);
					LINK->LINK->LINK->LINK->test = true;
				}
				else {
					if (sy != comma) {
						error(116);
						skip(P_setunion(SET1, LINK->fsys,
							P_expset(SET,
								(1L << ((long)comma)) | (1L << ((long)rparent)))),
							LINK->LINK->LINK->LINK);
					}
				}
				if (sy == comma) {
					insymbol();
					variable(P_setunion(SET1, LINK->fsys,
						P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
						LINK);
				}
				else
					LINK->LINK->LINK->LINK->test = true;
			}
		}
		if (!LINK->LINK->LINK->LINK->test) {
			do {
				loadaddress(LINK->LINK->LINK);   /*lda*/
				gen2(50, level - llev, laddr, LINK->LINK->LINK);
				if (gattr.typtr != NULL) {
					if ((structform)gattr.typtr->form <= subrange) {
						if (comptypes(intptr, gattr.typtr, LINK->LINK->LINK->LINK))
							/*csp*/
							gen1(30, 3, LINK->LINK->LINK);
						else {
							if (comptypes(realptr, gattr.typtr, LINK->LINK->LINK->LINK))
								/*csp*/
								gen1(30, 4, LINK->LINK->LINK);
							else {
								if (comptypes(charptr, gattr.typtr, LINK->LINK->LINK->LINK))
									/*csp*/
									gen1(30, 5, LINK->LINK->LINK);
								else
									error(399);
								/*rdc*/
							}
							/*rdr*/
						}
						/*rdi*/
					}
					else
						error(116);
				}
				LINK->LINK->LINK->LINK->test = (sy != comma);
				if (!LINK->LINK->LINK->LINK->test) {
					insymbol();
					variable(P_setunion(SET1, LINK->fsys,
						P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
						LINK);
				}
			} while (!LINK->LINK->LINK->LINK->test);
		}
		if (sy == rparent)
			insymbol();
		else
			error(4);
	}
	else if (LINK->lkey == 5)
		error(116);
	if (LINK->lkey == 11) {   /*lda*/
		gen2(50, level - llev, laddr, LINK->LINK->LINK);   /*csp*/
		/*rln*/
		gen1(30, 21, LINK->LINK->LINK);
	}
}

void PascalPParser::write(struct LOC_call* LINK)
{
	structure* lsp;
	bool default_;
	char llkey;
	levrange llev;
	addrrange laddr, len;
	setofsys SET, SET1;

	/*write*/
	llkey = LINK->lkey;
	llev = 1;
	laddr = lcaftermarkstack + charmax;
	if (sy == lparent) {
		insymbol();
		expression(P_setunion(SET1, LINK->fsys,
			P_expset(SET, (1L << ((long)comma)) | (1L << ((long)colon)) |
				(1L << ((long)rparent)))), LINK->LINK);
		lsp = gattr.typtr;
		LINK->LINK->LINK->LINK->test = false;
		if (lsp != NULL) {
			if ((structform)lsp->form == files) {
				if (lsp->UU.filtype == charptr) {
					llev = gattr.UU.U1.UU.U0.vlevel;
					laddr = gattr.UU.U1.UU.U0.dplmt;
				}
				else
					error(399);
				if (sy == rparent) {
					if (llkey == 6)
						error(116);
					LINK->LINK->LINK->LINK->test = true;
				}
				else {
					if (sy != comma) {
						error(116);
						skip(P_setunion(SET1, LINK->fsys,
							P_expset(SET,
								(1L << ((long)comma)) | (1L << ((long)rparent)))),
							LINK->LINK->LINK->LINK);
					}
				}
				if (sy == comma) {
					insymbol();
					expression(P_setunion(SET1, LINK->fsys, P_expset(SET,
						(1L << ((long)comma)) | (1L << ((long)colon)) |
						(1L << ((long)rparent)))), LINK->LINK);
				}
				else
					LINK->LINK->LINK->LINK->test = true;
			}
		}
		if (!LINK->LINK->LINK->LINK->test) {
			do {
				lsp = gattr.typtr;
				if (lsp != NULL) {
					if ((structform)lsp->form <= subrange)
						load(LINK->LINK->LINK);
					else
						loadaddress(LINK->LINK->LINK);
				}
				if (sy == colon) {
					insymbol();
					expression(P_setunion(SET1, LINK->fsys, P_expset(SET,
						(1L << ((long)comma)) | (1L << ((long)colon)) |
						(1L << ((long)rparent)))), LINK->LINK);
					if (gattr.typtr != NULL) {
						if (gattr.typtr != intptr)
							error(116);
					}
					load(LINK->LINK->LINK);
					default_ = false;
				}
				else
					default_ = true;
				if (sy == colon) {
					insymbol();
					expression(P_setunion(SET1, LINK->fsys,
						P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
						LINK->LINK);
					if (gattr.typtr != NULL) {
						if (gattr.typtr != intptr)
							error(116);
					}
					if (lsp != realptr)
						error(124);
					load(LINK->LINK->LINK);
					error(399);
				}
				else {
					if (lsp == intptr) {
						if (default_)   /*ldc*/
							gen2(51, 1, 10, LINK->LINK->LINK);
						/*lda*/
						gen2(50, level - llev, laddr, LINK->LINK->LINK);   /*csp*/
						/*wri*/
						gen1(30, 6, LINK->LINK->LINK);
					}
					else {
						if (lsp == realptr) {
							if (default_)   /*ldc*/
								gen2(51, 1, 20, LINK->LINK->LINK);
							/*lda*/
							gen2(50, level - llev, laddr, LINK->LINK->LINK);   /*csp*/
							/*wrr*/
							gen1(30, 8, LINK->LINK->LINK);
						}
						else {
							if (lsp == charptr) {
								if (default_)   /*ldc*/
									gen2(51, 1, 1, LINK->LINK->LINK);
								/*lda*/
								gen2(50, level - llev, laddr, LINK->LINK->LINK);   /*csp*/
								/*wrc*/
								gen1(30, 9, LINK->LINK->LINK);
							}
							else {
								if (lsp != NULL) {
									if ((structform)lsp->form == scalar)
										error(399);
									else {
										if (string(lsp, LINK->LINK->LINK->LINK)) {
											len = lsp->size;
											if (default_)   /*ldc*/
												gen2(51, 1, len, LINK->LINK->LINK);
											/*ldc*/
											gen2(51, 1, len, LINK->LINK->LINK);   /*lda*/
											gen2(50, level - llev, laddr, LINK->LINK->LINK);
											/*csp*/
											/*wrs*/
											gen1(30, 10, LINK->LINK->LINK);
										}
										else
											error(116);
									}
								}
							}
						}
					}
				}
				LINK->LINK->LINK->LINK->test = (sy != comma);
				if (!LINK->LINK->LINK->LINK->test) {
					insymbol();
					expression(P_setunion(SET1, LINK->fsys, P_expset(SET,
						(1L << ((long)comma)) | (1L << ((long)colon)) |
						(1L << ((long)rparent)))), LINK->LINK);
				}
			} while (!LINK->LINK->LINK->LINK->test);
		}
		if (sy == rparent)
			insymbol();
		else
			error(4);
	}
	else if (LINK->lkey == 6)
		error(116);
	if (llkey == 12) {   /*writeln*/
		gen2(50, level - llev, laddr, LINK->LINK->LINK);   /*csp*/
		/*wln*/
		gen1(30, 22, LINK->LINK->LINK);
	}
	/*lda*/
}

void PascalPParser::pack(struct LOC_call* LINK)
{
	structure* lsp, * lsp1;
	setofsys SET, SET1;
	structure* WITH;

	/*pack*/
	error(399);
	variable(P_setunion(SET1, LINK->fsys,
		P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
		LINK);
	lsp = NULL;
	lsp1 = NULL;
	if (gattr.typtr != NULL) {
		WITH = gattr.typtr;
		if ((structform)WITH->form == arrays) {
			lsp = WITH->UU.U4.inxtype;
			lsp1 = WITH->UU.U4.aeltype;
		}
		else
			error(116);
	}
	if (sy == comma)
		insymbol();
	else
		error(20);
	expression(P_setunion(SET1, LINK->fsys, P_expset(SET,
		(1L << ((long)comma)) | (1L << ((long)rparent)))),
		LINK->LINK);
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form != scalar)
			error(116);
		else {
			if (!comptypes(lsp, gattr.typtr, LINK->LINK->LINK->LINK))
				error(116);
		}
	}
	if (sy == comma)
		insymbol();
	else
		error(20);
	variable(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)rparent))),
		LINK);
	if (gattr.typtr == NULL)
		return;
	WITH = gattr.typtr;
	if ((structform)WITH->form == arrays) {
		if ((!comptypes(WITH->UU.U4.aeltype, lsp1, LINK->LINK->LINK->LINK)) |
			(!comptypes(WITH->UU.U4.inxtype, lsp, LINK->LINK->LINK->LINK)))
			error(116);
	}
	else
		error(116);
}

void PascalPParser::unpack(struct LOC_call* LINK)
{
	structure* lsp, * lsp1;
	setofsys SET, SET1;
	structure* WITH;

	/*unpack*/
	error(399);
	variable(P_setunion(SET1, LINK->fsys,
		P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
		LINK);
	lsp = NULL;
	lsp1 = NULL;
	if (gattr.typtr != NULL) {
		WITH = gattr.typtr;
		if ((structform)WITH->form == arrays) {
			lsp = WITH->UU.U4.inxtype;
			lsp1 = WITH->UU.U4.aeltype;
		}
		else
			error(116);
	}
	if (sy == comma)
		insymbol();
	else
		error(20);
	variable(P_setunion(SET1, LINK->fsys,
		P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
		LINK);
	if (gattr.typtr != NULL) {
		WITH = gattr.typtr;
		if ((structform)WITH->form == arrays) {
			if ((!comptypes(WITH->UU.U4.aeltype, lsp1, LINK->LINK->LINK->LINK)) |
				(!comptypes(WITH->UU.U4.inxtype, lsp, LINK->LINK->LINK->LINK)))
				error(116);
		}
		else
			error(116);
	}
	if (sy == comma)
		insymbol();
	else
		error(20);
	expression(P_setunion(SET1, LINK->fsys,
		P_expset(SET, 1L << ((long)rparent))), LINK->LINK);
	if (gattr.typtr == NULL)
		return;
	if ((structform)gattr.typtr->form != scalar)
		error(116);
	else {
		if (!comptypes(lsp, gattr.typtr, LINK->LINK->LINK->LINK))
			error(116);
	}
}

void PascalPParser::new_(struct LOC_call* LINK)
{
	structure* lsp, * lsp1;
	long varts;
	addrrange lsize;
	valu lval;
	setofsys SET, SET1;
	structure* WITH;

	/*new*/
	variable(P_setunion(SET1, LINK->fsys,
		P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
		LINK);
	loadaddress(LINK->LINK->LINK);
	lsp = NULL;
	varts = 0;
	lsize = 0;
	if (gattr.typtr != NULL) {
		WITH = gattr.typtr;
		if ((structform)WITH->form == pointer) {
			if (WITH->UU.eltype != NULL) {
				lsize = WITH->UU.eltype->size;
				if ((structform)WITH->UU.eltype->form == records)
					lsp = WITH->UU.eltype->UU.U5.recvar;
			}
		}
		else
			error(116);
	}
	while (sy == comma) {   /*while*/
		insymbol();
		constant_(P_setunion(SET1, LINK->fsys, P_expset(SET,
			(1L << ((long)comma)) | (1L << ((long)rparent)))),
			&lsp1, &lval, LINK->LINK->LINK->LINK);
		varts++;
		/*check to insert here: is constant in tagfieldtype range*/
		if (lsp == NULL)
			error(158);
		else {
			if ((structform)lsp->form != tagfld)
				error(162);
			else {
				if (lsp->UU.U7.tagfieldp != NULL) {
					if (string(lsp1, LINK->LINK->LINK->LINK) || lsp1 == realptr)
						error(159);
					else {
						if (comptypes(lsp->UU.U7.tagfieldp->idtype, lsp1,
							LINK->LINK->LINK->LINK)) {
							lsp1 = lsp->UU.U7.fstvar;
							while (lsp1 != NULL) {
								WITH = lsp1;
								if (WITH->UU.U8.varval.UU.ival == lval.UU.ival) {
									lsize = WITH->size;
									lsp = WITH->UU.U8.subvar;
									goto _L1;
								}
								lsp1 = WITH->UU.U8.nxtvar;
							}
							lsize = lsp->size;
							lsp = NULL;
						}
						else
							error(116);
					}
				}
			}
		}
	_L1:;
	}
	/*ldc*/
	gen2(51, 1, lsize, LINK->LINK->LINK);   /*csp*/
	/*new*/
	gen1(30, 12, LINK->LINK->LINK);
}

void PascalPParser::mark__(struct LOC_call* LINK)
{
	setofsys SET, SET1;

	/*mark*/
	variable(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)rparent))),
		LINK);
	if (gattr.typtr == NULL)
		return;
	if ((structform)gattr.typtr->form == pointer) {
		loadaddress(LINK->LINK->LINK);   /*csp*/
		/*sav*/
		gen1(30, 23, LINK->LINK->LINK);
	}
	else
		error(116);
}

void PascalPParser::release__(struct LOC_call* LINK)
{
	setofsys SET, SET1;

	/*release*/
	variable(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)rparent))),
		LINK);
	if (gattr.typtr == NULL)
		return;
	if ((structform)gattr.typtr->form == pointer) {
		load(LINK->LINK->LINK);   /*csp*/
		/*rst*/
		gen1(30, 13, LINK->LINK->LINK);
	}
	else
		error(116);
}



void PascalPParser::abs_(struct LOC_call* LINK)
{
	/*abs*/
	if (gattr.typtr == NULL)
		return;
	if (gattr.typtr == intptr) {   /*abi*/
		gen0(0, LINK->LINK->LINK);
		return;
	}
	if (gattr.typtr == realptr)   /*abr*/
		gen0(1, LINK->LINK->LINK);
	else {
		error(125);
		gattr.typtr = intptr;
	}
}

void PascalPParser::sqr_(struct LOC_call* LINK)
{
	/*sqr*/
	if (gattr.typtr == NULL)
		return;
	if (gattr.typtr == intptr) {   /*sqi*/
		gen0(24, LINK->LINK->LINK);
		return;
	}
	if (gattr.typtr == realptr)   /*sqr*/
		gen0(25, LINK->LINK->LINK);
	else {
		error(125);
		gattr.typtr = intptr;
	}
}

void PascalPParser::trunc_(struct LOC_call* LINK)
{
	/*trunc*/
	if (gattr.typtr != NULL) {   /*trc*/
		if (gattr.typtr != realptr)
			error(125);
	}
	gen0(27, LINK->LINK->LINK);
	gattr.typtr = intptr;
}

void PascalPParser::odd_(struct LOC_call* LINK)
{
	/*odd*/
	if (gattr.typtr != NULL) {   /*odd*/
		if (gattr.typtr != intptr)
			error(125);
	}
	gen0(20, LINK->LINK->LINK);
	gattr.typtr = boolptr;
}

 void PascalPParser::ord_(struct LOC_call* LINK)
{
	/*ord*/
	if (gattr.typtr != NULL) {   /*ord*/
		if ((structform)gattr.typtr->form >= power)
			error(125);
	}
	gen0t(58, gattr.typtr, LINK->LINK->LINK);
	gattr.typtr = intptr;
}

void PascalPParser::chr_(struct LOC_call* LINK)
{
	/*chr*/
	if (gattr.typtr != NULL) {   /*chr*/
		if (gattr.typtr != intptr)
			error(125);
	}
	gen0(59, LINK->LINK->LINK);
	gattr.typtr = charptr;
}

void PascalPParser::predsucc(struct LOC_call* LINK)
{
	/*predsucc*/
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form != scalar)
			error(125);
	}
	if (LINK->lkey == 7)   /*dec*/
		gen1t(31, 1, gattr.typtr, LINK->LINK->LINK);
	else   /*inc*/
		gen1t(34, 1, gattr.typtr, LINK->LINK->LINK);
}

void PascalPParser::eof_(struct LOC_call* LINK)
{
	setofsys SET, SET1;

	/*eof*/
	if (sy == lparent) {
		insymbol();
		variable(P_setunion(SET1, LINK->fsys,
			P_expset(SET, 1L << ((long)rparent))), LINK);
		if (sy == rparent)
			insymbol();
		else
			error(4);
	}
	else {
		gattr.typtr = textptr;
		gattr.kind = varbl;
		gattr.UU.U1.access = drct;
		gattr.UU.U1.UU.U0.vlevel = 1;
		gattr.UU.U1.UU.U0.dplmt = lcaftermarkstack;
	}
	loadaddress(LINK->LINK->LINK);
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form != files)
			error(125);
	}
	if (LINK->lkey == 9)   /*eof*/
		gen0(8, LINK->LINK->LINK);
	else   /*csp*/
		gen1(30, 14, LINK->LINK->LINK);
	gattr.typtr = boolptr;

	/*eln*/
}



void PascalPParser::callnonstandard(struct LOC_call* LINK)
{
	identifier* nxt, * lcp;
	structure* lsp;
	idkind lkind;
	bool lb;
	addrrange locpar, llc;
	identifier* WITH;
	setofsys SET, SET1, SET2, SET3;

	/*callnonstandard*/
	locpar = 0;
	WITH = LINK->fcp;
	nxt = WITH->next;
	lkind = (idkind)WITH->UU.U4.UU.U1.pfkind;
	if (!WITH->UU.U4.UU.U1.UU.U0.externl)   /*mst*/
		gen1(41, level - WITH->UU.U4.UU.U1.pflev, LINK->LINK->LINK);
	if (sy == lparent) {   /*if lparent*/
		llc = lc;
		do {
			lb = false;   /*decide whether proc/func must be passed*/
			if (lkind == actual) {
				if (nxt == NULL)
					error(126);
				else
					lb = (((1L << nxt->klass) &
						((1L << ((long)proc)) | (1L << ((long)func)))) != 0);
			}
			else
				error(399);
			/*For formal proc/func, lb is false and expression
			 will be called, which will always interpret a proc/func id
			 at its beginning as a call rather than a parameter passing.
			 In this implementation, parameter procedures/functions
			 are therefore not allowed to have procedure/function
			 parameters*/
			insymbol();
			if (lb) {   /*pass function or procedure*/
				error(399);
				if (sy != ident) {
					error(2);
					skip(P_setunion(SET1, LINK->fsys, P_expset(SET,
						(1L << ((long)comma)) | (1L << ((long)rparent)))),
						LINK->LINK->LINK->LINK);
				}
				else {
					if ((idclass)nxt->klass == proc)
						searchid(1L << ((long)proc), &lcp);
					else {
						searchid(1L << ((long)func), &lcp);
						if (!comptypes(lcp->idtype, nxt->idtype, LINK->LINK->LINK->LINK))
							error(128);
					}
					insymbol();
					if (!P_inset(sy, P_setunion(SET1, LINK->fsys, P_expset(SET,
						(1L << ((long)comma)) | (1L << ((long)rparent)))))) {
						error(6);
						skip(P_setunion(SET3, LINK->fsys,
							P_expset(SET2,
								(1L << ((long)comma)) | (1L << ((long)rparent)))),
							LINK->LINK->LINK->LINK);
					}
				}
			}  /*if lb*/
			else {
				expression(P_setunion(SET1, LINK->fsys,
					P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rparent)))),
					LINK->LINK);
				if (gattr.typtr != NULL) {
					if (lkind == actual) {
						if (nxt != NULL) {
							lsp = nxt->idtype;
							if (lsp != NULL) {
								if ((idkind)nxt->UU.U2.vkind == actual) {
									if ((structform)lsp->form <= power) {
										load(LINK->LINK->LINK);
										if (debug)
											checkbnds(lsp, LINK->LINK->LINK);
										if (comptypes(realptr, lsp, LINK->LINK->LINK->LINK) &&
											gattr.typtr == intptr)
										{   /*flt*/
											gen0(10, LINK->LINK->LINK);
											gattr.typtr = realptr;
										}
										locpar += lsp->size;
										align(parmptr, &locpar);
									}
									else {
										loadaddress(LINK->LINK->LINK);
										locpar += ptrsize;
										align(parmptr, &locpar);
									}
								}
								else {
									if (gattr.kind == varbl) {
										loadaddress(LINK->LINK->LINK);
										locpar += ptrsize;
										align(parmptr, &locpar);
									}
									else
										error(154);
								}
								if (!comptypes(lsp, gattr.typtr, LINK->LINK->LINK->LINK))
									error(142);
							}
						}
					}
				}
			}
			if (lkind == actual && nxt != NULL)
				nxt = nxt->next;
		} while (sy == comma);
		lc = llc;
		if (sy == rparent)
			insymbol();
		else
			error(4);
	}
	if (lkind == actual) {
		if (nxt != NULL)
			error(126);
		WITH = LINK->fcp;
		if (WITH->UU.U4.UU.U1.UU.U0.externl)   /*csp*/
			gen1(30, WITH->UU.U4.UU.U1.pfname, LINK->LINK->LINK);
		else   /*cup*/
			gencupent(46, locpar, WITH->UU.U4.UU.U1.pfname, LINK->LINK->LINK);
	}
	gattr.typtr = LINK->fcp->idtype;

	/*lkind = formal*/
	/*pass formal param*/
}

void PascalPParser::call(long* fsys_, identifier* fcp_, struct LOC_statement* LINK)
{
	struct LOC_call V;
	setofsys SET, SET1;

	V.LINK = LINK;
	/*call*/
	P_setcpy(V.fsys, fsys_);
	V.fcp = fcp_;
	if ((declkind)V.fcp->UU.U4.pfdeckind != standard) {
		callnonstandard(&V);
		return;
	}  /*standard procedures and functions*/
	V.lkey = V.fcp->UU.U4.UU.key;
	if ((idclass)V.fcp->klass == proc) {
		if (((1L << V.lkey) & 0x1860) == 0) {
			if (sy == lparent)
				insymbol();
			else
				error(9);
		}
		switch (V.lkey) {

		case 1:
		case 2:
		case 3:
		case 4:
			getputresetrewrite(&V);
			break;

		case 5:
		case 11:
			read(&V);
			break;

		case 6:
		case 12:
			write(&V);
			break;

		case 7:
			pack(&V);
			break;

		case 8:
			unpack(&V);
			break;

		case 9:
			new_(&V);
			break;

		case 10:
			release__(&V);
			break;

		case 13:
			mark__(&V);
			break;
		}
		if (((1L << V.lkey) & 0x1860) != 0)
			return;
		if (sy == rparent)
			insymbol();
		else
			error(4);
		return;
	}
	if (V.lkey <= 8) {
		if (sy == lparent)
			insymbol();
		else
			error(9);
		expression(P_setunion(SET1, V.fsys, P_expset(SET, 1L << ((long)rparent))),
			LINK);
		load(LINK->LINK);
	}
	switch (V.lkey) {

	case 1:
		abs_(&V);
		break;

	case 2:
		sqr_(&V);
		break;

	case 3:
		trunc_(&V);
		break;

	case 4:
		odd_(&V);
		break;

	case 5:
		ord_(&V);
		break;

	case 6:
		chr_(&V);
		break;

	case 7:
	case 8:
		predsucc(&V);
		break;

	case 9:
	case 10:
		eof_(&V);
		break;
	}
	if (V.lkey > 8)
		return;
	if (sy == rparent)
		insymbol();
	else
		error(4);
}


void PascalPParser::factor(long* fsys, struct LOC_term* LINK)
{
	identifier* lcp;
	constant* lvp;
	bool varpart;
	setty cstpart;
	structure* lsp;
	setofsys SET;
	structure* WITH;
	setofsys SET1;

	/*factor*/
	if (!P_inset(sy, facbegsys)) {
		error(58);
		skip(P_setunion(SET, fsys, facbegsys), LINK->LINK->LINK->LINK->LINK->LINK);
		gattr.typtr = NULL;
	}
	while (P_inset(sy, facbegsys)) {
		switch (sy) {   /*case*/

		/*id*/
		case ident:
			searchid((1L << ((long)konst)) | (1L << ((long)vars)) |
				(1L << ((long)field)) | (1L << ((long)func)), &lcp);
			insymbol();
			if ((idclass)lcp->klass == func) {
				call(fsys, lcp, LINK->LINK->LINK->LINK);
				gattr.kind = expr;
				if (gattr.typtr != NULL) {
					if ((structform)gattr.typtr->form == subrange)
						gattr.typtr = gattr.typtr->UU.U1.rangetype;
				}
			}
			else {
				if ((idclass)lcp->klass == konst) {
					gattr.typtr = lcp->idtype;
					gattr.kind = cst;
					gattr.UU.cval = lcp->UU.values;
				}
				else {
					selector(fsys, lcp, LINK->LINK->LINK->LINK);
					if (gattr.typtr != NULL) {   /*elim.subr.types to*/
						WITH = gattr.typtr;
						if ((structform)WITH->form == subrange)
							gattr.typtr = WITH->UU.U1.rangetype;
					}
					/*simplify later tests*/
				}
			}
			break;

			/*cst*/
		case intconst:
			gattr.typtr = intptr;
			gattr.kind = cst;
			gattr.UU.cval = val;
			insymbol();
			break;

		case realconst:
			gattr.typtr = realptr;
			gattr.kind = cst;
			gattr.UU.cval = val;
			insymbol();
			break;

		case stringconst:
			if (lgth == 1)
				gattr.typtr = charptr;
			else {
				lsp = (structure*) malloc(sizeof(structure));
				lsp->UU.U4.aeltype = charptr;
				lsp->form = arrays;
				lsp->UU.U4.inxtype = NULL;
				lsp->size = lgth;
				gattr.typtr = lsp;
			}
			gattr.kind = cst;
			gattr.UU.cval = val;
			insymbol();
			break;

			/* ( */
		case lparent:
			insymbol();
			expression(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)rparent))),
				LINK->LINK->LINK->LINK);
			if (sy == rparent)
				insymbol();
			else
				error(4);
			break;

			/*not*/
		case notsy:
			insymbol();
			factor(fsys, LINK);
			load(LINK->LINK->LINK->LINK->LINK);   /*not*/
			gen0(19, LINK->LINK->LINK->LINK->LINK);
			if (gattr.typtr != NULL) {
				if (gattr.typtr != boolptr) {
					error(135);
					gattr.typtr = NULL;
				}
			}
			break;

			/*[*/
		case lbrack:
			insymbol();
			P_expset(cstpart, 0);
			varpart = false;
			/* p2c: pcom.p, line 2831:
			 * Note: No Specialmalloc form known for STRUCTURE.POWER [187] */
			lsp = (structure*) malloc(sizeof(structure));
			lsp->UU.elset = NULL;
			lsp->size = setsize;
			lsp->form = power;
			if (sy == rbrack) {
				gattr.typtr = lsp;
				gattr.kind = cst;
				insymbol();
			}
			else {
				do {
					expression(P_setunion(SET1, fsys,
						P_expset(SET, (1L << ((long)comma)) | (1L << ((long)rbrack)))),
						LINK->LINK->LINK->LINK);
					if (gattr.typtr != NULL) {
						if ((structform)gattr.typtr->form != scalar) {
							error(136);
							gattr.typtr = NULL;
						}
						else {
							if (comptypes(lsp->UU.elset, gattr.typtr,
								LINK->LINK->LINK->LINK->LINK->LINK)) {
								if (gattr.kind == cst) {
									if ((unsigned long)gattr.UU.cval.UU.ival > sethigh)
										error(304);
									else
										P_addset(cstpart, gattr.UU.cval.UU.ival);
								}
								else {
									load(LINK->LINK->LINK->LINK->LINK);
									if (!comptypes(gattr.typtr, intptr,
										LINK->LINK->LINK->LINK->LINK->LINK))
										/*ord*/
										gen0t(58, gattr.typtr, LINK->LINK->LINK->LINK->LINK);
									/*sgs*/
									gen0(23, LINK->LINK->LINK->LINK->LINK);
									if (varpart)   /*uni*/
										gen0(28, LINK->LINK->LINK->LINK->LINK);
									else
										varpart = true;
								}
								lsp->UU.elset = gattr.typtr;
								gattr.typtr = lsp;
							}
							else
								error(137);
						}
					}
					LINK->LINK->LINK->LINK->LINK->LINK->test = (sy != comma);
					if (!LINK->LINK->LINK->LINK->LINK->LINK->test)
						insymbol();
				} while (!LINK->LINK->LINK->LINK->LINK->LINK->test);
				if (sy == rbrack)
					insymbol();
				else
					error(12);
			}
			if (varpart) {
				if (*cstpart != 0) {
					lvp = (constant*) malloc(sizeof(constant));
					P_setcpy(lvp->UU.pval, cstpart);
					lvp->cclass = pset;
					if (LINK->LINK->LINK->LINK->LINK->cstptrix == cstoccmax)
						error(254);
					else {
						LINK->LINK->LINK->LINK->LINK->cstptrix++;
						LINK->LINK->LINK->LINK->LINK->cstptr[LINK->LINK->LINK->LINK->
							LINK->cstptrix - 1] = lvp;
						/*ldc*/
						gen2(51, 5, LINK->LINK->LINK->LINK->LINK->cstptrix,
							LINK->LINK->LINK->LINK->LINK);
						/*uni*/
						gen0(28, LINK->LINK->LINK->LINK->LINK);
						gattr.kind = expr;
					}
				}
				/* p2c: pcom.p, line 2875:
				 * Note: No Specialmalloc form known for CONSTANT.PSET [187] */
			}
			else {
				lvp = (constant*) malloc(sizeof(constant));
				P_setcpy(lvp->UU.pval, cstpart);
				lvp->cclass = pset;
				gattr.UU.cval.UU.valp = lvp;
			}
			break;
		}
		if (!P_inset(sy, fsys)) {
			error(6);
			skip(P_setunion(SET, fsys, facbegsys),
				LINK->LINK->LINK->LINK->LINK->LINK);
		}
	}  /*while*/

  /* p2c: pcom.p, line 2807:
   * Note: No Specialmalloc form known for STRUCTURE.ARRAYS [187] */
   /* p2c: pcom.p, line 2887:
	* Note: No Specialmalloc form known for CONSTANT.PSET [187] */
}

void PascalPParser::term(long* fsys, struct LOC_simpleexpression* LINK)
{
	struct LOC_term V;
	attr lattr;
	operator_ lop;
	setofsys SET, SET1;

	V.LINK = LINK;
	/*term*/
	factor(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)mulop))), &V);
	while (sy == mulop) {
		load(LINK->LINK->LINK->LINK);
		lattr = gattr;
		lop = op;
		insymbol();
		factor(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)mulop))), &V);
		load(LINK->LINK->LINK->LINK);
		if (lattr.typtr == NULL || gattr.typtr == NULL) {
			gattr.typtr = NULL;
			continue;
		}
		switch (lop) {

			/***/
		case mul:
			if (lattr.typtr == intptr && gattr.typtr == intptr)   /*mpi*/
				gen0(15, LINK->LINK->LINK->LINK);
			else {
				if (lattr.typtr == intptr) {   /*flo*/
					gen0(9, LINK->LINK->LINK->LINK);
					lattr.typtr = realptr;
				}
				else {
					if (gattr.typtr == intptr) {   /*flt*/
						gen0(10, LINK->LINK->LINK->LINK);
						gattr.typtr = realptr;
					}
				}
				if (lattr.typtr == realptr && gattr.typtr == realptr)   /*mpr*/
					gen0(16, LINK->LINK->LINK->LINK);
				else {
					if (((structform)lattr.typtr->form == power) & comptypes(lattr.typtr,
						gattr.typtr, LINK->LINK->LINK->LINK->LINK))
						/*int*/
						gen0(12, LINK->LINK->LINK->LINK);
					else {
						error(134);
						gattr.typtr = NULL;
					}
				}
			}
			break;

			/* / */
		case rdiv:
			if (gattr.typtr == intptr) {   /*flt*/
				gen0(10, LINK->LINK->LINK->LINK);
				gattr.typtr = realptr;
			}
			if (lattr.typtr == intptr) {   /*flo*/
				gen0(9, LINK->LINK->LINK->LINK);
				lattr.typtr = realptr;
			}
			if (lattr.typtr == realptr && gattr.typtr == realptr)   /*dvr*/
				gen0(7, LINK->LINK->LINK->LINK);
			else {
				error(134);
				gattr.typtr = NULL;
			}
			break;

			/*div*/
		case idiv:
			if (lattr.typtr == intptr && gattr.typtr == intptr)   /*dvi*/
				gen0(6, LINK->LINK->LINK->LINK);
			else {
				error(134);
				gattr.typtr = NULL;
			}
			break;

			/*mod*/
		case imod:
			if (lattr.typtr == intptr && gattr.typtr == intptr)   /*mod*/
				gen0(14, LINK->LINK->LINK->LINK);
			else {
				error(134);
				gattr.typtr = NULL;
			}
			break;

			/*and*/
		case andop:
			if (lattr.typtr == boolptr && gattr.typtr == boolptr)   /*and*/
				gen0(4, LINK->LINK->LINK->LINK);
			else {
				error(134);
				gattr.typtr = NULL;
			}
			break;
		}/*case*/
	}  /*while*/
}

void PascalPParser::simpleexpression(long* fsys, struct LOC_expression* LINK)
{
	struct LOC_simpleexpression V;
	attr lattr;
	operator_ lop;
	bool signed_;
	setofsys SET, SET1;

	V.LINK = LINK;
	/*simpleexpression*/
	signed_ = false;
	if (sy == addop &&
		((1L << ((long)op)) & ((1L << ((long)plus)) | (1L << ((long)minus)))) != 0) {
		signed_ = (op == minus);
		insymbol();
	}
	term(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)addop))), &V);
	if (signed_) {
		load(LINK->LINK->LINK);
		if (gattr.typtr == intptr)   /*ngi*/
			gen0(17, LINK->LINK->LINK);
		else {
			if (gattr.typtr == realptr)   /*ngr*/
				gen0(18, LINK->LINK->LINK);
			else {
				error(134);
				gattr.typtr = NULL;
			}
		}
	}
	while (sy == addop) {
		load(LINK->LINK->LINK);
		lattr = gattr;
		lop = op;
		insymbol();
		term(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)addop))), &V);
		load(LINK->LINK->LINK);
		if (lattr.typtr == NULL || gattr.typtr == NULL) {
			gattr.typtr = NULL;
			continue;
		}
		switch (lop) {

			/*+*/
		case plus:
			if (lattr.typtr == intptr && gattr.typtr == intptr)   /*adi*/
				gen0(2, LINK->LINK->LINK);
			else {
				if (lattr.typtr == intptr) {   /*flo*/
					gen0(9, LINK->LINK->LINK);
					lattr.typtr = realptr;
				}
				else {
					if (gattr.typtr == intptr) {   /*flt*/
						gen0(10, LINK->LINK->LINK);
						gattr.typtr = realptr;
					}
				}
				if (lattr.typtr == realptr && gattr.typtr == realptr)   /*adr*/
					gen0(3, LINK->LINK->LINK);
				else if (((structform)lattr.typtr->form == power) &
					comptypes(lattr.typtr, gattr.typtr, LINK->LINK->LINK->LINK))
					gen0(28, LINK->LINK->LINK);
				else {
					error(134);
					gattr.typtr = NULL;
				}
			}
			break;

			/*-*/
		case minus:
			if (lattr.typtr == intptr && gattr.typtr == intptr)   /*sbi*/
				gen0(21, LINK->LINK->LINK);
			else {
				if (lattr.typtr == intptr) {   /*flo*/
					gen0(9, LINK->LINK->LINK);
					lattr.typtr = realptr;
				}
				else {
					if (gattr.typtr == intptr) {   /*flt*/
						gen0(10, LINK->LINK->LINK);
						gattr.typtr = realptr;
					}
				}
				if (lattr.typtr == realptr && gattr.typtr == realptr)   /*sbr*/
					gen0(22, LINK->LINK->LINK);
				else {
					if (((structform)lattr.typtr->form == power) &
						comptypes(lattr.typtr, gattr.typtr, LINK->LINK->LINK->LINK))
						/*dif*/
						gen0(5, LINK->LINK->LINK);
					else {
						error(134);
						gattr.typtr = NULL;
					}
				}
			}
			break;

			/*or*/
		case orop:
			if (lattr.typtr == boolptr && gattr.typtr == boolptr)   /*ior*/
				gen0(13, LINK->LINK->LINK);
			else {
				error(134);
				gattr.typtr = NULL;
			}
			break;
		}/*case*/
	}  /*while*/

	/*uni*/
}

void PascalPParser::expression(long* fsys, struct LOC_statement* LINK)
{
	struct LOC_expression V;
	attr lattr;
	operator_ lop;
	char typind;
	addrrange lsize;
	setofsys SET, SET1;

	V.LINK = LINK;
	/*expression*/
	simpleexpression(P_setunion(SET1, fsys, P_expset(SET, 1L << ((long)relop))),
		&V);
	if (sy != relop) {
		return;
	}  /*sy = relop*/
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form <= power)
			load(LINK->LINK);
		else
			loadaddress(LINK->LINK);
	}
	lattr = gattr;
	lop = op;
	if (lop == inop) {
		if (!comptypes(gattr.typtr, intptr, LINK->LINK->LINK))   /*ord*/
			gen0t(58, gattr.typtr, LINK->LINK);
	}
	insymbol();
	simpleexpression(fsys, &V);
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form <= power)
			load(LINK->LINK);
		else
			loadaddress(LINK->LINK);
	}
	if (lattr.typtr != NULL && gattr.typtr != NULL) {
		if (lop == inop) {
			if ((structform)gattr.typtr->form == power) {
				if (comptypes(lattr.typtr, gattr.typtr->UU.elset, LINK->LINK->LINK))
					/*inn*/
					gen0(11, LINK->LINK);
				else {
					error(129);
					gattr.typtr = NULL;
				}
			}
			else {
				error(130);
				gattr.typtr = NULL;
			}
		}
		else {
			if (lattr.typtr != gattr.typtr) {
				if (lattr.typtr == intptr) {   /*flo*/
					gen0(9, LINK->LINK);
					lattr.typtr = realptr;
				}
				else {
					if (gattr.typtr == intptr) {   /*flt*/
						gen0(10, LINK->LINK);
						gattr.typtr = realptr;
					}
				}
			}
			if (comptypes(lattr.typtr, gattr.typtr, LINK->LINK->LINK)) {
				lsize = lattr.typtr->size;
				switch ((structform)lattr.typtr->form) {

				case scalar:
					if (lattr.typtr == realptr)
						typind = 'r';
					else {
						if (lattr.typtr == boolptr)
							typind = 'b';
						else {
							if (lattr.typtr == charptr)
								typind = 'c';
							else
								typind = 'i';
						}
					}
					break;

				case pointer:
					if (((1L << ((long)lop)) & ((1L << ((long)ltop)) | (1L << ((long)leop)) |
						(1L << ((long)gtop)) | (1L << ((long)geop)))) != 0)
						error(131);
					typind = 'a';
					break;

				case power:
					if (((1L << ((long)lop)) &
						((1L << ((long)ltop)) | (1L << ((long)gtop)))) != 0)
						error(132);
					typind = 's';
					break;

				case arrays:
					if (!string(lattr.typtr, LINK->LINK->LINK))
						error(134);
					typind = 'm';
					break;

				case records:
					error(134);
					typind = 'm';
					break;

				case files:
					error(133);
					typind = 'f';
					break;
				}
				switch (lop) {

				case ltop:   /*les*/
					gen2(53, typind, lsize, LINK->LINK);
					break;

				case leop:   /*leq*/
					gen2(52, typind, lsize, LINK->LINK);
					break;

				case gtop:   /*grt*/
					gen2(49, typind, lsize, LINK->LINK);
					break;

				case geop:   /*geq*/
					gen2(48, typind, lsize, LINK->LINK);
					break;

				case neop:   /*neq*/
					gen2(55, typind, lsize, LINK->LINK);
					break;

				case eqop:   /*equ*/
					gen2(47, typind, lsize, LINK->LINK);
					break;
				}
			}
			else
				error(129);
		}
	}
	gattr.typtr = boolptr;
	gattr.kind = expr;
}

void PascalPParser::assignment(identifier* fcp, struct LOC_statement* LINK)
{
	attr lattr;
	setofsys SET, SET1;

	/*assignment*/
	selector(P_setunion(SET1, LINK->fsys, P_expset(SET, 1L << ((long)becomes))),
		fcp, LINK);
	if (sy != becomes) {
		error(51);
		return;
	}  /*sy = becomes*/
	if (gattr.typtr != NULL) {
		if (gattr.UU.U1.access != drct || (structform)gattr.typtr->form > power)
			loadaddress(LINK->LINK);
	}
	lattr = gattr;
	insymbol();
	expression(LINK->fsys, LINK);
	if (gattr.typtr != NULL) {
		if ((structform)gattr.typtr->form <= power)
			load(LINK->LINK);
		else
			loadaddress(LINK->LINK);
	}
	if (lattr.typtr == NULL || gattr.typtr == NULL)
		return;
	if (comptypes(realptr, lattr.typtr, LINK->LINK->LINK) &&
		gattr.typtr == intptr)
	{   /*flt*/
		gen0(10, LINK->LINK);
		gattr.typtr = realptr;
	}
	if (!comptypes(lattr.typtr, gattr.typtr, LINK->LINK->LINK)) {
		error(129);
		return;
	}
	switch ((structform)lattr.typtr->form) {

	case scalar:
	case subrange:
		if (debug)
			checkbnds(lattr.typtr, LINK->LINK);
		store(&lattr, LINK->LINK);
		break;

	case pointer:
		if (debug)   /*chk*/
			gen2t(45, 0, maxaddr, nilptr, LINK->LINK);
		store(&lattr, LINK->LINK);
		break;

	case power:
		store(&lattr, LINK->LINK);
		break;

	case arrays:
	case records:   /*mov*/
		gen1(40, lattr.typtr->size, LINK->LINK);
		break;

	case files:
		error(146);
		break;
	}
}

void PascalPParser::gotostatement(struct LOC_statement* LINK)
{
	labl* llp;
	bool found;
	disprange ttop, ttop1;
	labl* WITH;

	/*gotostatement*/
	if (sy != intconst) {
		error(15);
		return;
	}
	found = false;
	ttop = top;
	while ((where)display[ttop].occur != blck)
		ttop--;
	ttop1 = ttop;
	do {
		llp = display[ttop].flabel;
		while (llp != NULL && !found) {
			WITH = llp;
			if (WITH->labval == val.UU.ival) {
				found = true;
				if (ttop == ttop1)   /*ujp*/
					genujpxjp(57, WITH->labname, LINK->LINK);
				else   /*goto leads out of procedure*/
					error(399);
			}
			else
				llp = WITH->nextlab;
		}
		ttop--;
	} while (!(found || ttop == 0));
	if (!found)
		error(167);
	insymbol();
}

void PascalPParser::compoundstatement(struct LOC_statement* LINK)
{
	long SET[(long)endsy / 32 + 2];
	setofsys SET1;

	/*compoundstatemenet*/
	do {
		do {
			P_addset(P_expset(SET, 0), (long)semicolon);
			statement(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)endsy)),
				LINK->LINK);
		} while (P_inset(sy, statbegsys));
		LINK->LINK->LINK->test = (sy != semicolon);
		if (!LINK->LINK->LINK->test)
			insymbol();
	} while (!LINK->LINK->LINK->test);
	if (sy == endsy)
		insymbol();
	else
		error(13);
}

void PascalPParser::ifstatement(struct LOC_statement* LINK)
{
	long lcix1, lcix2;
	long SET[(long)thensy / 32 + 2];
	setofsys SET1;
	long SET2[(long)elsesy / 32 + 2];

	/*ifstatement*/
	expression(P_setunion(SET1, LINK->fsys,
		P_addset(P_expset(SET, 0), (long)thensy)), LINK);
	genlabel(&lcix1);
	genfjp(lcix1, LINK->LINK);
	if (sy == thensy)
		insymbol();
	else
		error(52);
	statement(P_setunion(SET1, LINK->fsys,
		P_addset(P_expset(SET2, 0), (long)elsesy)),
		LINK->LINK);
	if (sy != elsesy) {
		putlabel(lcix1, LINK->LINK);
		return;
	}
	genlabel(&lcix2);   /*ujp*/
	genujpxjp(57, lcix2, LINK->LINK);
	putlabel(lcix1, LINK->LINK);
	insymbol();
	statement(LINK->fsys, LINK->LINK);
	putlabel(lcix2, LINK->LINK);
}

void PascalPParser::casestatement(struct LOC_statement* LINK)
{
	structure* lsp, * lsp1;
	caseinfo* fstptr, * lpt1, * lpt2, * lpt3;
	valu lval;
	long laddr, lcix, lcix1, lmin, lmax;
	long SET[(long)ofsy / 32 + 2];
	setofsys SET1, SET2;
	caseinfo* WITH;
	setofsys SET3, SET4;

	/*casestatement*/
	P_addset(P_expset(SET, 0), (long)ofsy);
	P_addset(SET, (long)comma);
	expression(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)colon)), LINK);
	load(LINK->LINK);
	genlabel(&lcix);
	lsp = gattr.typtr;
	if (lsp != NULL) {   /*ujp*/
		if ((structform)lsp->form != scalar || lsp == realptr) {
			error(144);
			lsp = NULL;
		}
		else if (!comptypes(lsp, intptr, LINK->LINK->LINK))
			gen0t(58, lsp, LINK->LINK);
	}
	genujpxjp(57, lcix, LINK->LINK);
	if (sy == ofsy)
		insymbol();
	else
		error(8);
	fstptr = NULL;
	genlabel(&laddr);
	do {
		lpt3 = NULL;
		genlabel(&lcix1);
		if (sy != (long)endsy && sy != (long)semicolon) {
			do {
				constant_(P_setunion(SET2, LINK->fsys,
					P_expset(SET1, (1L << ((long)comma)) | (1L << ((long)colon)))),
					&lsp1, &lval, LINK->LINK->LINK);
				if (lsp != NULL) {
					if (comptypes(lsp, lsp1, LINK->LINK->LINK)) {
						lpt1 = fstptr;
						lpt2 = NULL;
						while (lpt1 != NULL) {
							WITH = lpt1;
							if (WITH->cslab <= lval.UU.ival) {
								if (WITH->cslab == lval.UU.ival)
									error(156);
								goto _L1;
							}
							lpt2 = lpt1;
							lpt1 = WITH->next;
						}
					_L1:
						lpt3 = (caseinfo*) malloc(sizeof(caseinfo));
						lpt3->next = lpt1;
						lpt3->cslab = lval.UU.ival;
						lpt3->csstart = lcix1;
						if (lpt2 == NULL)
							fstptr = lpt3;
						else
							lpt2->next = lpt3;
					}
					else
						error(147);
				}
				LINK->LINK->LINK->test = (sy != comma);
				if (!LINK->LINK->LINK->test)
					insymbol();
			} while (!LINK->LINK->LINK->test);
			if (sy == colon)
				insymbol();
			else
				error(5);
			putlabel(lcix1, LINK->LINK);
			do {
				statement(P_setunion(SET4, LINK->fsys,
					P_expset(SET3, 1L << ((long)semicolon))),
					LINK->LINK);
			} while (P_inset(sy, statbegsys));
			if (lpt3 != NULL)   /*ujp*/
				genujpxjp(57, laddr, LINK->LINK);
		}
		LINK->LINK->LINK->test = (sy != semicolon);
		if (!LINK->LINK->LINK->test)
			insymbol();
	} while (!LINK->LINK->LINK->test);
	putlabel(lcix, LINK->LINK);
	if (fstptr != NULL) {
		lmax = fstptr->cslab;
		/*reverse pointers*/
		lpt1 = fstptr;
		fstptr = NULL;
		do {
			lpt2 = lpt1->next;
			lpt1->next = fstptr;
			fstptr = lpt1;
			lpt1 = lpt2;
		} while (lpt1 != NULL);
		lmin = fstptr->cslab;
		if (lmax - lmin < cixmax) {   /*chk*/
			gen2t(45, lmin, lmax, intptr, LINK->LINK);   /*ldc*/
			gen2(51, 1, lmin, LINK->LINK);   /*sbi*/
			gen0(21, LINK->LINK);
			genlabel(&lcix);   /*xjp*/
			genujpxjp(44, lcix, LINK->LINK);
			putlabel(lcix, LINK->LINK);
			do {
				WITH = fstptr;
				while (WITH->cslab > lmin) {   /*ujc error*/
					gen0(60, LINK->LINK);
					lmin++;
				}
				/*ujp*/
				genujpxjp(57, WITH->csstart, LINK->LINK);
				fstptr = WITH->next;
				lmin++;
			} while (fstptr != NULL);
			putlabel(laddr, LINK->LINK);
		}
		else
			error(157);
	}
	if (sy == endsy)
		insymbol();
	else
		error(13);

	/*ord*/
}

void PascalPParser::repeatstatement(struct LOC_statement* LINK)
{
	long laddr;
	long SET[(long)untilsy / 32 + 2];
	setofsys SET1;

	/*repeatstatement*/
	genlabel(&laddr);
	putlabel(laddr, LINK->LINK);
	do {
		P_addset(P_expset(SET, 0), (long)semicolon);
		statement(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)untilsy)),
			LINK->LINK);
		if (P_inset(sy, statbegsys))
			error(14);
	} while (P_inset(sy, statbegsys));
	while (sy == semicolon) {
		insymbol();
		do {
			P_addset(P_expset(SET, 0), (long)semicolon);
			statement(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)untilsy)),
				LINK->LINK);
			if (P_inset(sy, statbegsys))
				error(14);
		} while (P_inset(sy, statbegsys));
	}
	if (sy != untilsy) {
		error(53);
		return;
	}
	insymbol();
	expression(LINK->fsys, LINK);
	genfjp(laddr, LINK->LINK);
}

void PascalPParser::whilestatement(struct LOC_statement* LINK)
{
	long laddr, lcix;
	long SET[(long)dosy / 32 + 2];
	setofsys SET1;

	/*whilestatement*/
	genlabel(&laddr);
	putlabel(laddr, LINK->LINK);
	expression(P_setunion(SET1, LINK->fsys,
		P_addset(P_expset(SET, 0), (long)dosy)), LINK);
	genlabel(&lcix);
	genfjp(lcix, LINK->LINK);
	if (sy == dosy)
		insymbol();
	else
		error(54);
	statement(LINK->fsys, LINK->LINK);   /*ujp*/
	genujpxjp(57, laddr, LINK->LINK);
	putlabel(lcix, LINK->LINK);
}

void PascalPParser::forstatement(struct LOC_statement* LINK)
{
	attr lattr;
	symbol lsy;
	long lcix, laddr;
	addrrange llc;
	identifier* WITH;
	long SET[(long)downtosy / 32 + 2];
	setofsys SET1;
	long SET2[(long)dosy / 32 + 2];

	/*forstatement*/
	llc = lc;
	lattr.typtr = NULL;
	lattr.kind = varbl;
	lattr.UU.U1.access = drct;
	lattr.UU.U1.UU.U0.vlevel = level;
	lattr.UU.U1.UU.U0.dplmt = 0;
	if (sy == ident) {
		searchid(1L << ((long)vars), &LINK->lcp);
		WITH = LINK->lcp;
		lattr.typtr = WITH->idtype;
		lattr.kind = varbl;
		if ((idkind)WITH->UU.U2.vkind == actual) {
			lattr.UU.U1.access = drct;
			lattr.UU.U1.UU.U0.vlevel = WITH->UU.U2.vlev;
			lattr.UU.U1.UU.U0.dplmt = WITH->UU.U2.vaddr;
		}
		else {
			error(155);
			lattr.typtr = NULL;
		}
		if (lattr.typtr != NULL) {
			if (((structform)lattr.typtr->form > subrange) | comptypes(realptr,
				lattr.typtr, LINK->LINK->LINK)) {
				error(143);
				lattr.typtr = NULL;
			}
		}
		insymbol();
	}
	else {
		error(2);
		P_addset(P_expset(SET, 0), (long)becomes);
		P_addset(SET, (long)tosy);
		P_addset(SET, (long)downtosy);
		skip(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)dosy)),
			LINK->LINK->LINK);
	}
	if (sy == becomes) {
		insymbol();
		P_addset(P_expset(SET, 0), (long)tosy);
		P_addset(SET, (long)downtosy);
		expression(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)dosy)), LINK);
		if (gattr.typtr != NULL) {
			if ((structform)gattr.typtr->form != scalar)
				error(144);
			else {
				if (comptypes(lattr.typtr, gattr.typtr, LINK->LINK->LINK)) {
					load(LINK->LINK);
					store(&lattr, LINK->LINK);
				}
				else
					error(145);
			}
		}
	}
	else {
		error(51);
		P_addset(P_expset(SET, 0), (long)tosy);
		P_addset(SET, (long)downtosy);
		skip(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)dosy)),
			LINK->LINK->LINK);
	}
	if (sy == (long)downtosy || sy == (long)tosy) {
		lsy = sy;
		insymbol();
		expression(P_setunion(SET1, LINK->fsys,
			P_addset(P_expset(SET2, 0), (long)dosy)), LINK);
		if (gattr.typtr != NULL) {
			if ((structform)gattr.typtr->form != scalar)
				error(144);
			else {
				if (comptypes(lattr.typtr, gattr.typtr, LINK->LINK->LINK)) {
					load(LINK->LINK);
					if (!comptypes(lattr.typtr, intptr, LINK->LINK->LINK))   /*ord*/
						gen0t(58, gattr.typtr, LINK->LINK);
					align(intptr, &lc);   /*str*/
					gen2t(56, 0, lc, intptr, LINK->LINK);
					genlabel(&laddr);
					putlabel(laddr, LINK->LINK);
					gattr = lattr;
					load(LINK->LINK);
					if (!comptypes(gattr.typtr, intptr, LINK->LINK->LINK))   /*ord*/
						gen0t(58, gattr.typtr, LINK->LINK);
					/*lod*/
					gen2t(54, 0, lc, intptr, LINK->LINK);
					lc += intsize;
					if (lc > LINK->LINK->lcmax)
						LINK->LINK->lcmax = lc;
					if (lsy == tosy)   /*leq*/
						gen2(52, 'i', 1, LINK->LINK);
					else   /*geq*/
						gen2(48, 'i', 1, LINK->LINK);
				}
				else
					error(145);
			}
		}
	}
	else {
		error(55);
		skip(P_setunion(SET1, LINK->fsys, P_addset(P_expset(SET2, 0), (long)dosy)),
			LINK->LINK->LINK);
	}
	genlabel(&lcix);   /*fjp*/
	genujpxjp(33, lcix, LINK->LINK);
	if (sy == dosy)
		insymbol();
	else
		error(54);
	statement(LINK->fsys, LINK->LINK);
	gattr = lattr;
	load(LINK->LINK);
	if (lsy == tosy)   /*inc*/
		gen1t(34, 1, gattr.typtr, LINK->LINK);
	else   /*dec*/
		gen1t(31, 1, gattr.typtr, LINK->LINK);
	store(&lattr, LINK->LINK);   /*ujp*/
	genujpxjp(57, laddr, LINK->LINK);
	putlabel(lcix, LINK->LINK);
	lc = llc;
}


void PascalPParser::withstatement(struct LOC_statement* LINK)
{
	identifier* lcp;
	disprange lcnt1;
	addrrange llc;
	long SET[(long)dosy / 32 + 2];
	setofsys SET1;
	_REC_display* WITH;

	/*withstatement*/
	lcnt1 = 0;
	llc = lc;
	do {
		if (sy == ident) {
			searchid((1L << ((long)vars)) | (1L << ((long)field)), &lcp);
			insymbol();
		}
		else {
			error(2);
			lcp = uvarptr;
		}
		P_addset(P_expset(SET, 0), (long)comma);
		selector(P_setunion(SET1, LINK->fsys, P_addset(SET, (long)dosy)), lcp,
			LINK);
		if (gattr.typtr != NULL) {
			if ((structform)gattr.typtr->form == records) {
				if (top < displimit) {
					top++;
					lcnt1++;
					WITH = &display[top];
					WITH->fname = gattr.typtr->UU.U5.fstfld;
					WITH->flabel = NULL;
					if (gattr.UU.U1.access == drct) {
						WITH = &display[top];
						WITH->occur = crec;
						WITH->UU.U1.clev = gattr.UU.U1.UU.U0.vlevel;
						WITH->UU.U1.cdspl = gattr.UU.U1.UU.U0.dplmt;
					}
					else {
						loadaddress(LINK->LINK);
						align(nilptr, &lc);   /*str*/
						gen2t(56, 0, lc, nilptr, LINK->LINK);
						WITH = &display[top];
						WITH->occur = vrec;
						WITH->UU.vdspl = lc;
						lc += ptrsize;
						if (lc > LINK->LINK->lcmax)
							LINK->LINK->lcmax = lc;
					}
				}
				else
					error(250);
			}
			else
				error(140);
		}
		LINK->LINK->LINK->test = (sy != comma);
		if (!LINK->LINK->LINK->test)
			insymbol();
	} while (!LINK->LINK->LINK->test);
	if (sy == dosy)
		insymbol();
	else
		error(54);
	statement(LINK->fsys, LINK->LINK);
	top -= lcnt1;
	lc = llc;
}


void PascalPParser::statement(long* fsys_, struct LOC_body* LINK)
{
	struct LOC_statement V;
	labl* llp, * WITH;
	setofsys SET, SET1;

	V.LINK = LINK;
	/*statement*/
	P_setcpy(V.fsys, fsys_);
	if (sy == intconst) {   /*label*/
		llp = display[level].flabel;
		while (llp != NULL) {
			WITH = llp;
			if (WITH->labval == val.UU.ival) {
				if (WITH->defined_)
					error(165);
				putlabel(WITH->labname, LINK);
				WITH->defined_ = true;
				goto _L1;
			}
			else {
				llp = WITH->nextlab;
				continue;
			}
		}
		error(167);
	_L1:
		insymbol();
		if (sy == colon)
			insymbol();
		else
			error(5);
	}
	if (!P_inset(sy, P_setunion(SET1, V.fsys,
		P_expset(SET, 1L << ((long)ident))))) {
		error(6);
		skip(V.fsys, LINK->LINK);
	}
	if (!P_inset(sy, P_setunion(SET1, statbegsys,
		P_expset(SET, 1L << ((long)ident)))))
		return;
	switch (sy) {

	case ident:
		searchid((1L << ((long)vars)) | (1L << ((long)field)) |
			(1L << ((long)func)) | (1L << ((long)proc)), &V.lcp);
		insymbol();
		if ((idclass)V.lcp->klass == proc)
			call(V.fsys, V.lcp, &V);
		else
			assignment(V.lcp, &V);
		break;

	case beginsy:
		insymbol();
		compoundstatement(&V);
		break;

	case gotosy:
		insymbol();
		gotostatement(&V);
		break;

	case ifsy:
		insymbol();
		ifstatement(&V);
		break;

	case casesy:
		insymbol();
		casestatement(&V);
		break;

	case whilesy:
		insymbol();
		whilestatement(&V);
		break;

	case repeatsy:
		insymbol();
		repeatstatement(&V);
		break;

	case forsy:
		insymbol();
		forstatement(&V);
		break;

	case withsy:
		insymbol();
		withstatement(&V);
		break;
	}
	if (sy != (long)untilsy && sy != (long)elsesy && sy != (long)endsy &&
		sy != (long)semicolon) {
		error(6);
		skip(V.fsys, LINK->LINK);
	}
}

void PascalPParser::body(long* fsys, struct LOC_block* LINK)
{
	struct LOC_body V;
	identifier* llcp;
	alpha saveid;
	/*allows referencing of noninteger constants by an index
	 (instead of a pointer), which can be stored in the p2-field
	 of the instruction record until writeout.
	 --> procedure load, procedure writeout*/
	long entname, segsize, stacktop;
	addrrange llc1;
	identifier* lcp;
	labl* llp;
	identifier* WITH;
	long SET[(long)endsy / 32 + 2];
	setofsys SET1;
	labl* WITH1;
	filerec* WITH2;

	V.LINK = LINK;
	/*body*/
	if (LINK->fprocp != NULL)
		entname = LINK->fprocp->UU.U4.UU.U1.pfname;
	else
		genlabel(&entname);
	V.cstptrix = 0;
	V.topnew = lcaftermarkstack;
	V.topmax = lcaftermarkstack;
	putlabel(entname, &V);
	genlabel(&segsize);
	genlabel(&stacktop);   /*ent1*/
	gencupent(32, 1, segsize, &V);   /*ent2*/
	gencupent(32, 2, stacktop, &V);
	if (LINK->fprocp != NULL) {   /*copy multiple values into local cells*/
		llc1 = lcaftermarkstack;
		lcp = LINK->fprocp->next;
		while (lcp != NULL) {
			WITH = lcp;
			align(parmptr, &llc1);
			if ((idclass)WITH->klass == vars) {
				if (WITH->idtype != NULL) {
					if ((structform)WITH->idtype->form > power) {
						if ((idkind)WITH->UU.U2.vkind == actual) {   /*lda*/
							gen2(50, 0, WITH->UU.U2.vaddr, &V);   /*lod*/
							gen2t(54, 0, llc1, nilptr, &V);   /*mov*/
							gen1(40, WITH->idtype->size, &V);
						}
						llc1 += ptrsize;
					}
					else
						llc1 += WITH->idtype->size;
				}
			}
			lcp = lcp->next;
		}
	}
	V.lcmax = lc;
	do {
		do {
			P_addset(P_expset(SET, 0), (long)semicolon);
			statement(P_setunion(SET1, fsys, P_addset(SET, (long)endsy)), &V);
		} while (P_inset(sy, statbegsys));
		LINK->test = (sy != semicolon);
		if (!LINK->test)
			insymbol();
	} while (!LINK->test);
	if (sy == endsy)
		insymbol();
	else
		error(13);
	llp = display[top].flabel;   /*test for undefined labels*/
	while (llp != NULL) {
		WITH1 = llp;
		if (!WITH1->defined_) {
			error(168);
			printf("\n label %12ld\n", WITH1->labval);
			printf("%*c", (int)(chcnt + 16), ' ');
		}
		llp = WITH1->nextlab;
	}
	if (LINK->fprocp != NULL) {
		if (LINK->fprocp->idtype == NULL)   /*ret*/
			gen1(42, 'p', &V);
		else   /*ret*/
			gen0t(42, LINK->fprocp->idtype, &V);
		align(parmptr, &V.lcmax);
		if (prcode) {
			fprintf(prr.f, "l%4ld=%12d\n", segsize, V.lcmax);
			fprintf(prr.f, "l%4ld=%12ld\n", stacktop, V.topmax);
		}
		return;
	}
	gen1(42, 'p', &V);
	align(parmptr, &V.lcmax);
	if (prcode) {
		fprintf(prr.f, "l%4ld=%12d\n", segsize, V.lcmax);
		fprintf(prr.f, "l%4ld=%12ld\n", stacktop, V.topmax);
		fprintf(prr.f, "q\n");
	}
	ic = 0;   /*mst*/
	/*generate call of main program; note that this call must be loaded
	  at absolute address zero*/
	gen1(41, 0, &V);   /*cup*/
	gencupent(46, 0, entname, &V);   /*stp*/
	gen0(29, &V);
	if (prcode)
		fprintf(prr.f, "q\n");
	memcpy(saveid, id, sizeof(alpha));
	while (fextfilep != NULL) {
		WITH2 = fextfilep;
		if (strncmp(WITH2->filename, "input   ", sizeof(alpha)) &&
			strncmp(WITH2->filename, "output  ", sizeof(alpha)) &&
			strncmp(WITH2->filename, "prd     ", sizeof(alpha)) &&
			strncmp(WITH2->filename, "prr     ", sizeof(alpha))) {
			memcpy(id, WITH2->filename, sizeof(alpha));
			searchid(1L << ((long)vars), &llcp);
			if (llcp->idtype != NULL) {
				if ((structform)llcp->idtype->form != files) {
					printf("\n%8cundeclared external file%.8s\n",
						' ', fextfilep->filename);
					printf("%*c", (int)(chcnt + 16), ' ');
				}
			}
		}
		fextfilep = fextfilep->nextfile;
	}
	memcpy(id, saveid, sizeof(alpha));
	if (prtables) {
		putchar('\n');
		printtables(true);
	}

	/*ret*/
}

#undef cstoccmax
#undef cixmax


void PascalPParser::block(long* fsys_, symbol fsy, identifier* fprocp_)
{
	struct LOC_block V;
	symbol lsy;
	long SET[(long)casesy / 32 + 2];
	setofsys SET1;

	/*block*/
	P_setcpy(V.fsys, fsys_);
	V.fprocp = fprocp_;
	dp = true;
	do {
		if (sy == labelsy) {
			insymbol();
			labeldeclaration(&V);
		}
		if (sy == constsy) {
			insymbol();
			constdeclaration(&V);
		}
		if (sy == typesy) {
			insymbol();
			typedeclaration(&V);
		}
		if (sy == varsy) {
			insymbol();
			vardeclaration(&V);
		}
		while ((unsigned long)sy < 32 &&
			((1L << ((long)sy)) &
				((1L << ((long)procsy)) | (1L << ((long)funcsy)))) != 0) {
			lsy = sy;
			insymbol();
			procdeclaration(lsy, &V);
		}
		if (sy != beginsy) {
			error(18);
			skip(V.fsys, &V);
		}
	} while (!(P_inset(sy, statbegsys) | P_eof(stdin)));
	dp = false;
	if (sy == beginsy)
		insymbol();
	else
		error(17);
	do {
		body(P_setunion(SET1, V.fsys, P_addset(P_expset(SET, 0), (long)casesy)),
			&V);
		if (sy != fsy) {
			error(6);
			skip(V.fsys, &V);
		}
	} while (!((sy == fsy) | P_inset(sy, blockbegsys) | P_eof(stdin)));
}

void PascalPParser::programme(long* fsys)
{
	filerec* extfp;

	/*programme*/
	if (sy == progsy) {
		insymbol();
		if (sy != ident)
			error(2);
		insymbol();
		if ((unsigned long)sy >= 32 ||
			((1L << ((long)sy)) &
				((1L << ((long)lparent)) | (1L << ((long)semicolon)))) == 0)
			error(14);
		if (sy == lparent) {
			do {
				insymbol();
				if (sy == ident) {
					extfp = (filerec*) malloc(sizeof(filerec));
					memcpy(extfp->filename, id, sizeof(alpha));
					extfp->nextfile = fextfilep;
					fextfilep = extfp;
					insymbol();
					if ((unsigned long)sy >= 32 ||
						((1L << ((long)sy)) &
							((1L << ((long)comma)) | (1L << ((long)rparent)))) == 0)
						error(20);
				}
				else
					error(2);
			} while (sy == comma);
			if (sy != rparent)
				error(4);
			insymbol();
		}
		if (sy != semicolon)
			error(14);
		else
			insymbol();
	}
	do {
		block(fsys, period, NULL);
		if (sy != period)
			error(21);
	} while (!((sy == period) | P_eof(stdin)));
	if (list)
		putchar('\n');
	if (errinx != 0) {
		list = false;
		endofline();
	}
}


void PascalPParser::stdnames(void)
{
	/*stdnames*/
	memcpy(na[0], "false   ", sizeof(alpha));
	memcpy(na[1], "true    ", sizeof(alpha));
	memcpy(na[2], "input   ", sizeof(alpha));
	memcpy(na[3], "output  ", sizeof(alpha));
	memcpy(na[4], "get     ", sizeof(alpha));
	memcpy(na[5], "put     ", sizeof(alpha));
	memcpy(na[6], "reset   ", sizeof(alpha));
	memcpy(na[7], "rewrite ", sizeof(alpha));
	memcpy(na[8], "read    ", sizeof(alpha));
	memcpy(na[9], "write   ", sizeof(alpha));
	memcpy(na[10], "pack    ", sizeof(alpha));
	memcpy(na[11], "unpack  ", sizeof(alpha));
	memcpy(na[12], "new     ", sizeof(alpha));
	memcpy(na[13], "release ", sizeof(alpha));
	memcpy(na[14], "readln  ", sizeof(alpha));
	memcpy(na[15], "writeln ", sizeof(alpha));
	memcpy(na[16], "abs     ", sizeof(alpha));
	memcpy(na[17], "sqr     ", sizeof(alpha));
	memcpy(na[18], "trunc   ", sizeof(alpha));
	memcpy(na[19], "odd     ", sizeof(alpha));
	memcpy(na[20], "ord     ", sizeof(alpha));
	memcpy(na[21], "chr     ", sizeof(alpha));
	memcpy(na[22], "pred    ", sizeof(alpha));
	memcpy(na[23], "succ    ", sizeof(alpha));
	memcpy(na[24], "eof     ", sizeof(alpha));
	memcpy(na[25], "eoln    ", sizeof(alpha));
	memcpy(na[26], "sin     ", sizeof(alpha));
	memcpy(na[27], "cos     ", sizeof(alpha));
	memcpy(na[28], "exp     ", sizeof(alpha));
	memcpy(na[29], "sqrt    ", sizeof(alpha));
	memcpy(na[30], "ln      ", sizeof(alpha));
	memcpy(na[31], "arctan  ", sizeof(alpha));
	memcpy(na[32], "prd     ", sizeof(alpha));
	memcpy(na[33], "prr     ", sizeof(alpha));
	memcpy(na[34], "mark    ", sizeof(alpha));
}


void PascalPParser::enterstdtypes(void)
{  /*type underlying:*/
	structure* WITH;

	/******************/

  /* p2c: pcom.p, line 3646:
   * Note: No Specialmalloc form known for STRUCTURE.SCALAR.STANDARD [187] */
   /*enterstdtypes*/
	intptr = (structure*) malloc(sizeof(structure));   /*integer*/
	WITH = intptr;
	/* p2c: pcom.p, line 3649:
	 * Note: No Specialmalloc form known for STRUCTURE.SCALAR.STANDARD [187] */
	WITH->size = intsize;
	WITH->form = scalar;
	WITH->UU.U0.scalkind = standard;
	realptr = (structure*) malloc(sizeof(structure));   /*real*/
	WITH = realptr;
	/* p2c: pcom.p, line 3652:
	 * Note: No Specialmalloc form known for STRUCTURE.SCALAR.STANDARD [187] */
	WITH->size = realsize;
	WITH->form = scalar;
	WITH->UU.U0.scalkind = standard;
	charptr = (structure*) malloc(sizeof(structure));   /*char*/
	WITH = charptr;
	/* p2c: pcom.p, line 3655:
	 * Note: No Specialmalloc form known for STRUCTURE.SCALAR.DECLARED [187] */
	WITH->size = charsize;
	WITH->form = scalar;
	WITH->UU.U0.scalkind = standard;
	boolptr = (structure*) malloc(sizeof(structure));   /*boolean*/
	WITH = boolptr;
	/* p2c: pcom.p, line 3658:
	 * Note: No Specialmalloc form known for STRUCTURE.POINTER [187] */
	WITH->size = boolsize;
	WITH->form = scalar;
	WITH->UU.U0.scalkind = declared;
	nilptr = (structure*) malloc(sizeof(structure));   /*nil*/
	WITH = nilptr;
	/* p2c: pcom.p, line 3661:
	 * Note: No Specialmalloc form known for STRUCTURE.SCALAR.STANDARD [187] */
	WITH->UU.eltype = NULL;
	WITH->size = ptrsize;
	WITH->form = pointer;
	parmptr = (structure*) malloc(sizeof(structure));   /*for alignment of parameters*/
	WITH = parmptr;
	/* p2c: pcom.p, line 3664:
	 * Note: No Specialmalloc form known for STRUCTURE.FILES [187] */
	WITH->size = parmsize;
	WITH->form = scalar;
	WITH->UU.U0.scalkind = standard;
	textptr = (structure*) malloc(sizeof(structure));   /*text*/
	WITH = textptr;
	WITH->UU.filtype = charptr;
	WITH->size = charsize;
	WITH->form = files;
}


void PascalPParser::entstdnames(void)
{  /*name:*/
	identifier* cp, * cp1;
	long i;

	/*******/

  /* p2c: pcom.p, line 3674:
   * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
   /*entstdnames*/
	cp = (identifier*) malloc(sizeof(identifier));   /*integer*/
	memcpy(cp->name, "integer ", sizeof(alpha));
	cp->idtype = intptr;
	cp->klass = types;
	enterid(cp);
	/* p2c: pcom.p, line 3678:
	 * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
	cp = (identifier*) malloc(sizeof(identifier));   /*real*/
	memcpy(cp->name, "real    ", sizeof(alpha));
	cp->idtype = realptr;
	cp->klass = types;
	enterid(cp);
	/* p2c: pcom.p, line 3682:
	 * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
	cp = (identifier*) malloc(sizeof(identifier));   /*char*/
	memcpy(cp->name, "char    ", sizeof(alpha));
	cp->idtype = charptr;
	cp->klass = types;
	enterid(cp);
	/* p2c: pcom.p, line 3686:
	 * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
	cp = (identifier*) malloc(sizeof(identifier));   /*boolean*/
	memcpy(cp->name, "boolean ", sizeof(alpha));
	cp->idtype = boolptr;
	cp->klass = types;
	enterid(cp);
	cp1 = NULL;
	for (i = 0; i <= 1; i++) {
		cp = (identifier*) malloc(sizeof(identifier));   /*false,true*/
		memcpy(cp->name, na[i], sizeof(alpha));
		cp->idtype = boolptr;
		cp->next = cp1;
		cp->UU.values.UU.ival = i;
		cp->klass = konst;
		enterid(cp);
		cp1 = cp;
	}
	/* p2c: pcom.p, line 3692:
	 * Note: No Specialmalloc form known for IDENTIFIER.KONST [187] */
	boolptr->UU.U0.UU.fconst = cp;
	/* p2c: pcom.p, line 3700:
	 * Note: No Specialmalloc form known for IDENTIFIER.KONST [187] */
	cp = (identifier*) malloc(sizeof(identifier));   /*nil*/
	memcpy(cp->name, "nil     ", sizeof(alpha));
	cp->idtype = nilptr;
	cp->next = NULL;
	cp->UU.values.UU.ival = 0;
	cp->klass = konst;
	enterid(cp);
	for (i = 3; i <= 4; i++) {
		cp = (identifier*) malloc(sizeof(identifier));   /*input,output*/
		memcpy(cp->name, na[i - 1], sizeof(alpha));
		cp->idtype = textptr;
		cp->klass = vars;
		cp->UU.U2.vkind = actual;
		cp->next = NULL;
		cp->UU.U2.vlev = 1;
		cp->UU.U2.vaddr = lcaftermarkstack + i - 3;
		enterid(cp);
	}
	/* p2c: pcom.p, line 3707:
	 * Note: No Specialmalloc form known for IDENTIFIER.VARS [187] */
	for (i = 33; i <= 34; i++) {
		cp = (identifier*) malloc(sizeof(identifier));   /*prd,prr files*/
		memcpy(cp->name, na[i - 1], sizeof(alpha));
		cp->idtype = textptr;
		cp->klass = vars;
		cp->UU.U2.vkind = actual;
		cp->next = NULL;
		cp->UU.U2.vlev = 1;
		cp->UU.U2.vaddr = lcaftermarkstack + i - 31;
		enterid(cp);
	}
	/* p2c: pcom.p, line 3716:
	 * Note: No Specialmalloc form known for IDENTIFIER.VARS [187] */
	for (i = 5; i <= 16; i++) {
		cp = (identifier*) malloc(sizeof(identifier));   /*get,put,reset*/
		/*rewrite,read*/
		memcpy(cp->name, na[i - 1], sizeof(alpha));
		cp->idtype = NULL;   /*write,pack*/
		cp->next = NULL;
		cp->UU.U4.UU.key = i - 4;   /*unpack,pack*/
		cp->klass = proc;
		cp->UU.U4.pfdeckind = standard;
		enterid(cp);
	}
	/* p2c: pcom.p, line 3725:
	 * Note: No Specialmalloc form known for IDENTIFIER.PROC.STANDARD [187] */
	 /* p2c: pcom.p, line 3733:
	  * Note: No Specialmalloc form known for IDENTIFIER.PROC.STANDARD [187] */
	cp = (identifier*) malloc(sizeof(identifier));
	memcpy(cp->name, na[34], sizeof(alpha));
	cp->idtype = NULL;
	cp->next = NULL;
	cp->UU.U4.UU.key = 13;
	cp->klass = proc;
	cp->UU.U4.pfdeckind = standard;
	enterid(cp);
	for (i = 17; i <= 26; i++) {
		cp = (identifier*) malloc(sizeof(identifier));   /*abs,sqr,trunc*/
		/*odd,ord,chr*/
		memcpy(cp->name, na[i - 1], sizeof(alpha));
		cp->idtype = NULL;   /*pred,succ,eof*/
		cp->next = NULL;
		cp->UU.U4.UU.key = i - 16;
		cp->klass = func;
		cp->UU.U4.pfdeckind = standard;
		enterid(cp);
	}
	/* p2c: pcom.p, line 3740:
	 * Note: No Specialmalloc form known for IDENTIFIER.FUNC.STANDARD [187] */
	 /* p2c: pcom.p, line 3748:
	  * Note: No Specialmalloc form known for IDENTIFIER.VARS [187] */
	cp = (identifier*) malloc(sizeof(identifier));   /*parameter of predeclared functions*/
	memcpy(cp->name, "        ", sizeof(alpha));
	cp->idtype = realptr;
	cp->klass = vars;
	cp->UU.U2.vkind = actual;
	cp->next = NULL;
	cp->UU.U2.vlev = 1;
	cp->UU.U2.vaddr = 0;
	for (i = 27; i <= 32; i++) {
		cp1 = (identifier*) malloc(sizeof(identifier));   /*sin,cos,exp*/
		/*sqrt,ln,arctan*/
		memcpy(cp1->name, na[i - 1], sizeof(alpha));
		cp1->idtype = realptr;
		cp1->next = cp;
		cp1->UU.U4.UU.U1.UU.U0.forwdecl = false;
		cp1->UU.U4.UU.U1.UU.U0.externl = true;
		cp1->UU.U4.UU.U1.pflev = 0;
		cp1->UU.U4.UU.U1.pfname = i - 12;
		cp1->klass = func;
		cp1->UU.U4.pfdeckind = declared;
		cp1->UU.U4.UU.U1.pfkind = actual;
		enterid(cp1);
	}
	/* p2c: pcom.p, line 3754: Note:
	 * No Specialmalloc form known for IDENTIFIER.FUNC.DECLARED.ACTUAL [187] */
}


void PascalPParser::enterundecl(void)
{
	identifier* WITH;

	/* p2c: pcom.p, line 3766:
	 * Note: No Specialmalloc form known for IDENTIFIER.TYPES [187] */
	 /*enterundecl*/
	utypptr = (identifier*) malloc(sizeof(identifier));
	WITH = utypptr;
	/* p2c: pcom.p, line 3769:
	 * Note: No Specialmalloc form known for IDENTIFIER.KONST [187] */
	memcpy(WITH->name, "        ", sizeof(alpha));
	WITH->idtype = NULL;
	WITH->klass = types;
	ucstptr = (identifier*) malloc(sizeof(identifier));
	WITH = ucstptr;
	/* p2c: pcom.p, line 3774:
	 * Note: No Specialmalloc form known for IDENTIFIER.VARS [187] */
	memcpy(WITH->name, "        ", sizeof(alpha));
	WITH->idtype = NULL;
	WITH->next = NULL;
	WITH->UU.values.UU.ival = 0;
	WITH->klass = konst;
	uvarptr = (identifier*) malloc(sizeof(identifier));
	WITH = uvarptr;
	/* p2c: pcom.p, line 3779:
	 * Note: No Specialmalloc form known for IDENTIFIER.FIELD [187] */
	memcpy(WITH->name, "        ", sizeof(alpha));
	WITH->idtype = NULL;
	WITH->UU.U2.vkind = actual;
	WITH->next = NULL;
	WITH->UU.U2.vlev = 0;
	WITH->UU.U2.vaddr = 0;
	WITH->klass = vars;
	ufldptr = (identifier*) malloc(sizeof(identifier));
	WITH = ufldptr;
	/* p2c: pcom.p, line 3784: Note:
	 * No Specialmalloc form known for IDENTIFIER.PROC.DECLARED.ACTUAL [187] */
	memcpy(WITH->name, "        ", sizeof(alpha));
	WITH->idtype = NULL;
	WITH->next = NULL;
	WITH->UU.fldaddr = 0;
	WITH->klass = field;
	uprcptr = (identifier*) malloc(sizeof(identifier));
	WITH = uprcptr;
	/* p2c: pcom.p, line 3790: Note:
	 * No Specialmalloc form known for IDENTIFIER.FUNC.DECLARED.ACTUAL [187] */
	memcpy(WITH->name, "        ", sizeof(alpha));
	WITH->idtype = NULL;
	WITH->UU.U4.UU.U1.UU.U0.forwdecl = false;
	WITH->next = NULL;
	WITH->UU.U4.UU.U1.UU.U0.externl = false;
	WITH->UU.U4.UU.U1.pflev = 0;
	genlabel(&WITH->UU.U4.UU.U1.pfname);
	WITH->klass = proc;
	WITH->UU.U4.pfdeckind = declared;
	WITH->UU.U4.UU.U1.pfkind = actual;
	ufctptr = (identifier*) malloc(sizeof(identifier));
	WITH = ufctptr;
	memcpy(WITH->name, "        ", sizeof(alpha));
	WITH->idtype = NULL;
	WITH->next = NULL;
	WITH->UU.U4.UU.U1.UU.U0.forwdecl = false;
	WITH->UU.U4.UU.U1.UU.U0.externl = false;
	WITH->UU.U4.UU.U1.pflev = 0;
	genlabel(&WITH->UU.U4.UU.U1.pfname);
	WITH->klass = func;
	WITH->UU.U4.pfdeckind = declared;
	WITH->UU.U4.UU.U1.pfkind = actual;
}




void PascalPParser::initscalars()
{
	/*initscalars*/
	fwptr = NULL;
	prtables = false;
	list = true;
	prcode = true;
	debug = true;
	dp = true;
	prterr = true;
	errinx = 0;
	intlabel = 0;
	kk = 8;
	fextfilep = NULL;
	lc = lcaftermarkstack + filebuffer;
	/* note in the above reservation of buffer store for 2 text files */
	ic = 3;
	eol = true;
	linecount = 0;
	ch = ' ';
	chcnt = 0;
	globtestp = NULL;
	mxint10 = maxint / 10;
	digmax = strglgth - 1;
}

setofsys PascalPParser::constbegsys, PascalPParser::simptypebegsys, PascalPParser::typebegsys, PascalPParser::blockbegsys,
PascalPParser::selectsys, PascalPParser::facbegsys, PascalPParser::statbegsys, PascalPParser::typedels;

chtp PascalPParser::chartp[256];
alpha PascalPParser::rw[35];
char PascalPParser::frw[9];
symbol PascalPParser::rsy[35];
symbol PascalPParser::ssy[256];
operator_ PascalPParser::rop[35];
operator_ PascalPParser::sop[256];
alpha PascalPParser::na[35];
char PascalPParser::mn[61][4];
char PascalPParser::sna[23][4];
schar PascalPParser::cdx[61];
schar PascalPParser::pdx[23];
long PascalPParser::ordint[256];

long PascalPParser::intlabel, PascalPParser::mxint10, PascalPParser::digmax;


void PascalPParser::initsets()
{
	setofsys SET;

	/*initsets*/
	P_expset(constbegsys, (1L << ((long)addop)) | (1L << ((long)intconst)) |
		(1L << ((long)realconst)) |
		(1L << ((long)stringconst)) | (1L << ((long)ident)));
	P_setunion(simptypebegsys, P_expset(SET, 1L << ((long)lparent)),
		constbegsys);
	P_setunion(typebegsys,
		P_expset(SET, (1L << ((long)arrow)) | (1L << ((long)packedsy)) |
			(1L << ((long)arraysy)) | (1L << ((long)recordsy)) |
			(1L << ((long)setsy)) | (1L << ((long)filesy))),
		simptypebegsys);
	P_expset(typedels, (1L << ((long)arraysy)) | (1L << ((long)recordsy)) |
		(1L << ((long)setsy)) | (1L << ((long)filesy)));
	P_expset(blockbegsys,
		(1L << ((long)labelsy)) | (1L << ((long)constsy)) |
		(1L << ((long)typesy)) | (1L << ((long)varsy)) | (1L << ((long)procsy)) |
		(1L << ((long)funcsy)) | (1L << ((long)beginsy)));
	P_expset(selectsys,
		(1L << ((long)arrow)) | (1L << ((long)period)) | (1L << ((long)lbrack)));
	P_expset(facbegsys,
		(1L << ((long)intconst)) | (1L << ((long)realconst)) |
		(1L << ((long)stringconst)) | (1L << ((long)ident)) |
		(1L << ((long)lparent)) | (1L << ((long)lbrack)) | (1L << ((long)notsy)));
	P_addset(P_expset(statbegsys, 0), (long)beginsy);
	P_addset(statbegsys, (long)gotosy);
	P_addset(statbegsys, (long)ifsy);
	P_addset(statbegsys, (long)whilesy);
	P_addset(statbegsys, (long)repeatsy);
	P_addset(statbegsys, (long)forsy);
	P_addset(statbegsys, (long)withsy);
	P_addset(statbegsys, (long)casesy);
}

void PascalPParser::reswords()
{
	/*reswords*/
	memcpy(rw[0], "if      ", sizeof(alpha));
	memcpy(rw[1], "do      ", sizeof(alpha));
	memcpy(rw[2], "of      ", sizeof(alpha));
	memcpy(rw[3], "to      ", sizeof(alpha));
	memcpy(rw[4], "in      ", sizeof(alpha));
	memcpy(rw[5], "or      ", sizeof(alpha));
	memcpy(rw[6], "end     ", sizeof(alpha));
	memcpy(rw[7], "for     ", sizeof(alpha));
	memcpy(rw[8], "var     ", sizeof(alpha));
	memcpy(rw[9], "div     ", sizeof(alpha));
	memcpy(rw[10], "mod     ", sizeof(alpha));
	memcpy(rw[11], "set     ", sizeof(alpha));
	memcpy(rw[12], "and     ", sizeof(alpha));
	memcpy(rw[13], "not     ", sizeof(alpha));
	memcpy(rw[14], "then    ", sizeof(alpha));
	memcpy(rw[15], "else    ", sizeof(alpha));
	memcpy(rw[16], "with    ", sizeof(alpha));
	memcpy(rw[17], "goto    ", sizeof(alpha));
	memcpy(rw[18], "case    ", sizeof(alpha));
	memcpy(rw[19], "type    ", sizeof(alpha));
	memcpy(rw[20], "file    ", sizeof(alpha));
	memcpy(rw[21], "begin   ", sizeof(alpha));
	memcpy(rw[22], "until   ", sizeof(alpha));
	memcpy(rw[23], "while   ", sizeof(alpha));
	memcpy(rw[24], "array   ", sizeof(alpha));
	memcpy(rw[25], "const   ", sizeof(alpha));
	memcpy(rw[26], "label   ", sizeof(alpha));
	memcpy(rw[27], "repeat  ", sizeof(alpha));
	memcpy(rw[28], "record  ", sizeof(alpha));
	memcpy(rw[29], "downto  ", sizeof(alpha));
	memcpy(rw[30], "packed  ", sizeof(alpha));
	memcpy(rw[31], "forward ", sizeof(alpha));
	memcpy(rw[32], "program ", sizeof(alpha));
	memcpy(rw[33], "function", sizeof(alpha));
	memcpy(rw[34], "procedur", sizeof(alpha));
	frw[0] = 1;
	frw[1] = 1;
	frw[2] = 7;
	frw[3] = 15;
	frw[4] = 22;
	frw[5] = 28;
	frw[6] = 32;
	frw[7] = 34;
	frw[8] = 36;
}

void PascalPParser::symbols()
{
	/*symbols*/
	rsy[0] = ifsy;
	rsy[1] = dosy;
	rsy[2] = ofsy;
	rsy[3] = tosy;
	rsy[4] = relop;
	rsy[5] = addop;
	rsy[6] = endsy;
	rsy[7] = forsy;
	rsy[8] = varsy;
	rsy[9] = mulop;
	rsy[10] = mulop;
	rsy[11] = setsy;
	rsy[12] = mulop;
	rsy[13] = notsy;
	rsy[14] = thensy;
	rsy[15] = elsesy;
	rsy[16] = withsy;
	rsy[17] = gotosy;
	rsy[18] = casesy;
	rsy[19] = typesy;
	rsy[20] = filesy;
	rsy[21] = beginsy;
	rsy[22] = untilsy;
	rsy[23] = whilesy;
	rsy[24] = arraysy;
	rsy[25] = constsy;
	rsy[26] = labelsy;
	rsy[27] = repeatsy;
	rsy[28] = recordsy;
	rsy[29] = downtosy;
	rsy[30] = packedsy;
	rsy[31] = forwardsy;
	rsy[32] = progsy;
	rsy[33] = funcsy;
	rsy[34] = procsy;
	ssy['+'] = addop;
	ssy['-'] = addop;
	ssy['*'] = mulop;
	ssy['/'] = mulop;
	ssy['('] = lparent;
	ssy[')'] = rparent;
	ssy['$'] = othersy;
	ssy['='] = relop;
	ssy[' '] = othersy;
	ssy[','] = comma;
	ssy['.'] = period;
	ssy['\''] = othersy;
	ssy['['] = lbrack;
	ssy[']'] = rbrack;
	ssy[':'] = colon;
	ssy['^'] = arrow;
	ssy['<'] = relop;
	ssy['>'] = relop;
	ssy[';'] = semicolon;
}

void PascalPParser::rators()
{
	long i;

	/*rators*/
	for (i = 0; i <= 34; i++)   /*nr of res words*/
		rop[i] = noop;
	rop[4] = inop;
	rop[9] = idiv;
	rop[10] = imod;
	rop[5] = orop;
	rop[12] = andop;
	for (i = ordminchar; i <= ordmaxchar; i++)
		sop[(char)i] = noop;
	sop['+'] = plus;
	sop['-'] = minus;
	sop['*'] = mul;
	sop['/'] = rdiv;
	sop['='] = eqop;
	sop['<'] = ltop;
	sop['>'] = gtop;
}

void PascalPParser::procmnemonics()
{
	/*procmnemonics*/
	memcpy(sna[0], " get", 4);
	memcpy(sna[1], " put", 4);
	memcpy(sna[2], " rdi", 4);
	memcpy(sna[3], " rdr", 4);
	memcpy(sna[4], " rdc", 4);
	memcpy(sna[5], " wri", 4);
	memcpy(sna[6], " wro", 4);
	memcpy(sna[7], " wrr", 4);
	memcpy(sna[8], " wrc", 4);
	memcpy(sna[9], " wrs", 4);
	memcpy(sna[10], " pak", 4);
	memcpy(sna[11], " new", 4);
	memcpy(sna[12], " rst", 4);
	memcpy(sna[13], " eln", 4);
	memcpy(sna[14], " sin", 4);
	memcpy(sna[15], " cos", 4);
	memcpy(sna[16], " exp", 4);
	memcpy(sna[17], " sqt", 4);
	memcpy(sna[18], " log", 4);
	memcpy(sna[19], " atn", 4);
	memcpy(sna[20], " rln", 4);
	memcpy(sna[21], " wln", 4);
	memcpy(sna[22], " sav", 4);
}

void PascalPParser::instrmnemonics()
{
	/*instrmnemonics*/
	memcpy(mn[0], " abi", 4);
	memcpy(mn[1], " abr", 4);
	memcpy(mn[2], " adi", 4);
	memcpy(mn[3], " adr", 4);
	memcpy(mn[4], " and", 4);
	memcpy(mn[5], " dif", 4);
	memcpy(mn[6], " dvi", 4);
	memcpy(mn[7], " dvr", 4);
	memcpy(mn[8], " eof", 4);
	memcpy(mn[9], " flo", 4);
	memcpy(mn[10], " flt", 4);
	memcpy(mn[11], " inn", 4);
	memcpy(mn[12], " int", 4);
	memcpy(mn[13], " ior", 4);
	memcpy(mn[14], " mod", 4);
	memcpy(mn[15], " mpi", 4);
	memcpy(mn[16], " mpr", 4);
	memcpy(mn[17], " ngi", 4);
	memcpy(mn[18], " ngr", 4);
	memcpy(mn[19], " not", 4);
	memcpy(mn[20], " odd", 4);
	memcpy(mn[21], " sbi", 4);
	memcpy(mn[22], " sbr", 4);
	memcpy(mn[23], " sgs", 4);
	memcpy(mn[24], " sqi", 4);
	memcpy(mn[25], " sqr", 4);
	memcpy(mn[26], " sto", 4);
	memcpy(mn[27], " trc", 4);
	memcpy(mn[28], " uni", 4);
	memcpy(mn[29], " stp", 4);
	memcpy(mn[30], " csp", 4);
	memcpy(mn[31], " dec", 4);
	memcpy(mn[32], " ent", 4);
	memcpy(mn[33], " fjp", 4);
	memcpy(mn[34], " inc", 4);
	memcpy(mn[35], " ind", 4);
	memcpy(mn[36], " ixa", 4);
	memcpy(mn[37], " lao", 4);
	memcpy(mn[38], " lca", 4);
	memcpy(mn[39], " ldo", 4);
	memcpy(mn[40], " mov", 4);
	memcpy(mn[41], " mst", 4);
	memcpy(mn[42], " ret", 4);
	memcpy(mn[43], " sro", 4);
	memcpy(mn[44], " xjp", 4);
	memcpy(mn[45], " chk", 4);
	memcpy(mn[46], " cup", 4);
	memcpy(mn[47], " equ", 4);
	memcpy(mn[48], " geq", 4);
	memcpy(mn[49], " grt", 4);
	memcpy(mn[50], " lda", 4);
	memcpy(mn[51], " ldc", 4);
	memcpy(mn[52], " leq", 4);
	memcpy(mn[53], " les", 4);
	memcpy(mn[54], " lod", 4);
	memcpy(mn[55], " neq", 4);
	memcpy(mn[56], " str", 4);
	memcpy(mn[57], " ujp", 4);
	memcpy(mn[58], " ord", 4);
	memcpy(mn[59], " chr", 4);
	memcpy(mn[60], " ujc", 4);
}

void PascalPParser::chartypes()
{
	long i;

	for (i = ordminchar; i <= ordmaxchar; i++)
		chartp[(char)i] = illegal;
	chartp['a'] = letter;
	chartp['b'] = letter;
	chartp['c'] = letter;
	chartp['d'] = letter;
	chartp['e'] = letter;
	chartp['f'] = letter;
	chartp['g'] = letter;
	chartp['h'] = letter;
	chartp['i'] = letter;
	chartp['j'] = letter;
	chartp['k'] = letter;
	chartp['l'] = letter;
	chartp['m'] = letter;
	chartp['n'] = letter;
	chartp['o'] = letter;
	chartp['p'] = letter;
	chartp['q'] = letter;
	chartp['r'] = letter;
	chartp['s'] = letter;
	chartp['t'] = letter;
	chartp['u'] = letter;
	chartp['v'] = letter;
	chartp['w'] = letter;
	chartp['x'] = letter;
	chartp['y'] = letter;
	chartp['z'] = letter;
	chartp['0'] = number;
	chartp['1'] = number;
	chartp['2'] = number;
	chartp['3'] = number;
	chartp['4'] = number;
	chartp['5'] = number;
	chartp['6'] = number;
	chartp['7'] = number;
	chartp['8'] = number;
	chartp['9'] = number;
	chartp['+'] = special;
	chartp['-'] = special;
	chartp['*'] = special;
	chartp['/'] = special;
	chartp['('] = chlparen;
	chartp[')'] = special;
	chartp['$'] = special;
	chartp['='] = special;
	chartp[' '] = chspace;
	chartp[','] = special;
	chartp['.'] = chperiod;
	chartp['\''] = chstrquo;
	chartp['['] = special;
	chartp[']'] = special;
	chartp[':'] = chcolon;
	chartp['^'] = special;
	chartp[';'] = special;
	chartp['<'] = chlt;
	chartp['>'] = chgt;
	ordint['0'] = 0;
	ordint['1'] = 1;
	ordint['2'] = 2;
	ordint['3'] = 3;
	ordint['4'] = 4;
	ordint['5'] = 5;
	ordint['6'] = 6;
	ordint['7'] = 7;
	ordint['8'] = 8;
	ordint['9'] = 9;
}

void PascalPParser::initdx()
{
	cdx[0] = 0;
	cdx[1] = 0;
	cdx[2] = -1;
	cdx[3] = -1;
	cdx[4] = -1;
	cdx[5] = -1;
	cdx[6] = -1;
	cdx[7] = -1;
	cdx[8] = 0;
	cdx[9] = 0;
	cdx[10] = 0;
	cdx[11] = -1;
	cdx[12] = -1;
	cdx[13] = -1;
	cdx[14] = -1;
	cdx[15] = -1;
	cdx[16] = -1;
	cdx[17] = 0;
	cdx[18] = 0;
	cdx[19] = 0;
	cdx[20] = 0;
	cdx[21] = -1;
	cdx[22] = -1;
	cdx[23] = 0;
	cdx[24] = 0;
	cdx[25] = 0;
	cdx[26] = -2;
	cdx[27] = 0;
	cdx[28] = -1;
	cdx[29] = 0;
	cdx[30] = 0;
	cdx[31] = 0;
	cdx[32] = 0;
	cdx[33] = -1;
	cdx[34] = 0;
	cdx[35] = 0;
	cdx[36] = -1;
	cdx[37] = 1;
	cdx[38] = 1;
	cdx[39] = 1;
	cdx[40] = -2;
	cdx[41] = 0;
	cdx[42] = 0;
	cdx[43] = -1;
	cdx[44] = -1;
	cdx[45] = 0;
	cdx[46] = 0;
	cdx[47] = -1;
	cdx[48] = -1;
	cdx[49] = -1;
	cdx[50] = 1;
	cdx[51] = 1;
	cdx[52] = -1;
	cdx[53] = -1;
	cdx[54] = 1;
	cdx[55] = -1;
	cdx[56] = -1;
	cdx[57] = 0;
	cdx[58] = 0;
	cdx[59] = 0;
	cdx[60] = 0;
	pdx[0] = -1;
	pdx[1] = -1;
	pdx[2] = -2;
	pdx[3] = -2;
	pdx[4] = -2;
	pdx[5] = -3;
	pdx[6] = -3;
	pdx[7] = -3;
	pdx[8] = -3;
	pdx[9] = -4;
	pdx[10] = 0;
	pdx[11] = -2;
	pdx[12] = -1;
	pdx[13] = 0;
	pdx[14] = 0;
	pdx[15] = 0;
	pdx[16] = 0;
	pdx[17] = 0;
	pdx[18] = 0;
	pdx[19] = 0;
	pdx[20] = -1;
	pdx[21] = -1;
	pdx[22] = -1;
}

void PascalPParser::inittables()
{
	/*inittables*/
	reswords();
	symbols();
	rators();
	instrmnemonics();
	procmnemonics();
	chartypes();
	initdx();
}


void PascalPParser::initPascalP() 
{
	initsets();
	inittables();
}


