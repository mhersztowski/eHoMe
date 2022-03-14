#include "PascalPLex.h"
#include "PascalPUtils.h"

PascalPGlobals::PascalPGlobals() {
	long i;

	for (i = ORDMINCHAR; i <= ORDMAXCHAR; i++)
		chartype[i] = ILLEGAL;

	chartype['a'] = LETTER;
	chartype['b'] = LETTER;
	chartype['c'] = LETTER;
	chartype['d'] = LETTER;
	chartype['e'] = LETTER;
	chartype['f'] = LETTER;
	chartype['g'] = LETTER;
	chartype['h'] = LETTER;
	chartype['i'] = LETTER;
	chartype['j'] = LETTER;
	chartype['k'] = LETTER;
	chartype['l'] = LETTER;
	chartype['m'] = LETTER;
	chartype['n'] = LETTER;
	chartype['o'] = LETTER;
	chartype['p'] = LETTER;
	chartype['q'] = LETTER;
	chartype['r'] = LETTER;
	chartype['s'] = LETTER;
	chartype['t'] = LETTER;
	chartype['u'] = LETTER;
	chartype['v'] = LETTER;
	chartype['w'] = LETTER;
	chartype['x'] = LETTER;
	chartype['y'] = LETTER;
	chartype['z'] = LETTER;
	chartype['0'] = NUMBER;
	chartype['1'] = NUMBER;
	chartype['2'] = NUMBER;
	chartype['3'] = NUMBER;
	chartype['4'] = NUMBER;
	chartype['5'] = NUMBER;
	chartype['6'] = NUMBER;
	chartype['7'] = NUMBER;
	chartype['8'] = NUMBER;
	chartype['9'] = NUMBER;
	chartype['+'] = SPECIAL;
	chartype['-'] = SPECIAL;
	chartype['*'] = SPECIAL;
	chartype['/'] = SPECIAL;
	chartype['('] = CHLPAREN;
	chartype[')'] = SPECIAL;
	chartype['$'] = SPECIAL;
	chartype['='] = SPECIAL;
	chartype[' '] = CHSPACE;
	chartype[','] = SPECIAL;
	chartype['.'] = CHPERIOD;
	chartype['\''] = CHSTRQUO;
	chartype['['] = SPECIAL;
	chartype[']'] = SPECIAL;
	chartype[':'] =  CHCOLON;
	chartype['^'] = SPECIAL;
	chartype[';'] = SPECIAL;
	chartype['<'] = CHLT;
	chartype['>'] = CHGT;

	//reserved words
	rw.append(String("if"));
	rw.append(String("do"));
	rw.append(String("of"));
	rw.append(String("to"));
	rw.append(String("in"));
	rw.append(String("or"));
	rw.append(String("end"));
	rw.append(String("for"));
	rw.append(String("var"));
	rw.append(String("div"));
	rw.append(String("mod"));
	rw.append(String("set"));
	rw.append(String("and"));
	rw.append(String("not"));
	rw.append(String("then"));
	rw.append(String("else"));
	rw.append(String("with"));
	rw.append(String("goto"));
	rw.append(String("case"));
	rw.append(String("type"));
	rw.append(String("file"));
	rw.append(String("begin"));
	rw.append(String("until"));
	rw.append(String("while"));
	rw.append(String("array"));
	rw.append(String("const"));
	rw.append(String("label"));
	rw.append(String("repeat"));
	rw.append(String("record"));
	rw.append(String("downto"));
	rw.append(String("packed"));
	rw.append(String("forward"));
	rw.append(String("program"));
	rw.append(String("function"));
	rw.append(String("procedure"));

	rsy[0] = IFSY;
	rsy[1] = DOSY;
	rsy[2] = OFSY;
	rsy[3] = TOSY;
	rsy[4] = RELOP;
	rsy[5] = ADDOP;
	rsy[6] = ENDSY;
	rsy[7] = FORSY;
	rsy[8] = VARSY;
	rsy[9] = MULOP;
	rsy[10] = MULOP;
	rsy[11] = SETSY;
	rsy[12] = MULOP;
	rsy[13] = NOTSY;
	rsy[14] = THENSY;
	rsy[15] = ELSESY;
	rsy[16] = WITHSY;
	rsy[17] = GOTOSY;
	rsy[18] = CASESY;
	rsy[19] = TYPESY;
	rsy[20] = FILESY;
	rsy[21] = BEGINSY;
	rsy[22] = UNTILSY;
	rsy[23] = WHILESY;
	rsy[24] = ARRAYSY;
	rsy[25] = CONSTSY;
	rsy[26] = LABELSY;
	rsy[27] = REPEATSY;
	rsy[28] = RECORDSY;
	rsy[29] = DOWNTOSY;
	rsy[30] = PACKEDSY;
	rsy[31] = FORWARDSY;
	rsy[32] = PROGSY;
	rsy[33] = FUNCSY;
	rsy[34] = PROCSY;

	for (i = 0; i <= 34; i++)   /*nr of res words*/
		rop[i] = NOOP;

	rop[4] = INOP;
	rop[9] = IDIV;
	rop[10] = IMOD;
	rop[5] = OROP;
	rop[12] = ANDOP;

	digmax = STRGLGTH - 1;

}

PascalPLex::PascalPLex() : errors(false) {
	mxint10 = MAXINT / 10;
}

void PascalPLex::error(long ferrnr)
{
	errors = true;

}

void PascalPLex::skip(long* fsys)
{
	/*skip input string until relevant symbol found*/
	/*skip*/
	if (src->ch == src->EOS)
		return;

	while ((!P_inset(sym, fsys)) & (!(src->ch == src->EOS)))
		insymbol();

	if (!P_inset(sym, fsys))
		insymbol();
}

void PascalPLex::skipwhitespace() {
	while (src->ch == ' ' || src->ch == '\t')
		src->next();
}

void PascalPLex::options()
{
	/*options*/
	do {
		src->next();
		if (src->ch != '*') {
			if (src->ch == 't') {
				src->next();
				prtables = (src->ch == '+');
			}
			else if (src->ch == 'l') {
				src->next();
				list = (src->ch == '+');
				if (!list)
					putchar('\n');
			}
			else if (src->ch == 'd') {
				src->next();
				debug = (src->ch == '+');
			}
			else if (src->ch == 'c') {
				src->next();
				prcode = (src->ch == '+');
				
			}
			src->next();
		}
	} while (src->ch == ',');

}

void PascalPLex::insymbol()
{
_L1:

	skipwhitespace();

	if (PascalPGlobals::get()->chartype[src->ch] == PascalPGlobals::ILLEGAL) {
		sym = PascalPGlobals::OTHERSY;
		op = PascalPGlobals::NOOP;
		error(399);
		src->next();
		return;
	}

	switch (PascalPGlobals::get()->chartype[src->ch]) {
	
		case PascalPGlobals::LETTER: {

			do {
				id.append(src->ch);
				src->next();
			} while (PascalPGlobals::get()->chartype[src->ch] == PascalPGlobals::LETTER);

			int rwindex = PascalPGlobals::get()->rw.findIndex(id);
			if (rwindex >= 0) {
				sym = PascalPGlobals::get()->rsy[rwindex];
				op = PascalPGlobals::get()->rop[rwindex];
			}
			else {
				sym = PascalPGlobals::IDENT;
				op = PascalPGlobals::NOOP;
			}

			break;
		}

		case PascalPGlobals::NUMBER: {
			char digit[STRGLGTH];
			long i, k;
			PascalPGlobals::constant* lvp;

			op = PascalPGlobals::NOOP;

			i = 0;
			do {
				i++;
				if (i <= PascalPGlobals::get()->digmax)
					digit[i - 1] = src->ch;
				src->next();
			} while (PascalPGlobals::get()->chartype[src->ch] == PascalPGlobals::NUMBER);

			if (((src->ch == '.') & (src->forward() != '.')) || src->ch == 'e') {
				k = i;
				if (src->ch == '.') {
					k++;
					if (k <= PascalPGlobals::get()->digmax)
						digit[k - 1] = src->ch;

					src->next();   /*if ch = '.' then begin ch := ':'; goto 3 end;*/
					if (PascalPGlobals::get()->chartype[src->ch] != PascalPGlobals::NUMBER)
						error(201);
					else {
						do {
							k++;
							if (k <= PascalPGlobals::get()->digmax)
								digit[k - 1] = src->ch;
							src->next();
						} while (PascalPGlobals::get()->chartype[src->ch] == PascalPGlobals::NUMBER);
					}
				}
				if (src->ch == 'e') {
					k++;
					if (k <= PascalPGlobals::get()->digmax)
						digit[k - 1] = src->ch;

					src->next();
					if (src->ch == '+' || src->ch == '-') {
						k++;
						if (k <= PascalPGlobals::get()->digmax)
							digit[k - 1] = src->ch;
						src->next();
					}
					if (PascalPGlobals::get()->chartype[src->ch] != PascalPGlobals::NUMBER)
						error(201);
					else {
						do {
							k++;
							if (k <= PascalPGlobals::get()->digmax)
								digit[k - 1] = src->ch;
							src->next();
						} while (PascalPGlobals::get()->chartype[src->ch] == PascalPGlobals::NUMBER);
					}
				}
				/* p2c: pcom.p, line 454:
				 * Note: No SpecialMalloc form known for CONSTANT.REEL [187] */
				lvp = (PascalPGlobals::constant *) malloc(sizeof(PascalPGlobals::constant));
				sym = PascalPGlobals::REALCONST;
				lvp->cclass = PascalPGlobals::reel;
				for (i = 0; i < STRGLGTH; i++)
					lvp->UU.rval[i] = ' ';
				if (k <= PascalPGlobals::get()->digmax) {
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
				if (i > PascalPGlobals::get()->digmax) {
					error(203);
					val.UU.ival = 0;
				}
				else {
					val.UU.ival = 0;
					for (k = 0; k < i; k++) {
						if (val.UU.ival <= mxint10)
							val.UU.ival = val.UU.ival * 10 + (digit[k] - '0');
						else {
							error(203);
							val.UU.ival = 0;
						}
					}
					sym = PascalPGlobals::INTCONST;
				}
			}
			break;
		}

		case PascalPGlobals::CHSTRQUO: {
			char string[STRGLGTH];
			PascalPGlobals::constant* lvp;

			lgth = 0;
			sym = PascalPGlobals::STRINGCONST;
			op = PascalPGlobals::NOOP;
			do {
				do {
					src->next();
					lgth++;
					if (lgth <= STRGLGTH)
						string[lgth - 1] = src->ch;
				} while (!(src->ch == PascalPSource::EOL || src->ch == '\''));
				if (src->ch == PascalPSource::EOL)
					error(202);
				else
					src->next();
			} while (src->ch == '\'');
			lgth--;   /*now lgth = nr of chars in string*/
			if (lgth == 0)
				error(205);
			else {
				if (lgth == 1)
					val.UU.ival = string[0];
				else {
					lvp = (PascalPGlobals::constant*) malloc(sizeof(PascalPGlobals::constant));
					lvp->cclass = PascalPGlobals::strg;
					if (lgth > STRGLGTH) {
						error(399);
						lgth = STRGLGTH;
					}
					lvp->UU.U2.slgth = lgth;
					for (long i = 0; i < lgth; i++)
						lvp->UU.U2.sval[i] = string[i];
					val.UU.valp = lvp;
				}
			}
			break;
		}

		case PascalPGlobals::CHCOLON: {
			op = PascalPGlobals::NOOP;
			src->next();
			if (src->ch == '=') {
				sym = PascalPGlobals::BECOMES;
				src->next();
			}
			else
				sym = PascalPGlobals::COLON;
			break;
		}

		case PascalPGlobals::CHPERIOD: {
			op = PascalPGlobals::NOOP;
			src->next();
			if (src->ch == '.') {
				sym = PascalPGlobals::COLON;
				src->next();
			}
			else
				sym = PascalPGlobals::PERIOD;
			break;
		}

		case PascalPGlobals::CHLT: {
			src->next();
			sym = PascalPGlobals::RELOP;
			if (src->ch == '=') {
				op = PascalPGlobals::LEOP;
				src->next();
			}
			else {
				if (src->ch == '>') {
					op = PascalPGlobals::NEOP;
					src->next();
				}
				else
					op = PascalPGlobals::LTOP;
			}
			break;
		}

		case PascalPGlobals::CHGT: {
			src->next();
			sym = PascalPGlobals::RELOP;
			if (src->ch == '=') {
				op = PascalPGlobals::GEOP;
				src->next();
			}
			else
				op = PascalPGlobals::GTOP;
			break;
		}

		case PascalPGlobals::CHLPAREN: {
			src->next();
			if (src->ch == '*') {
				src->next();
				if (src->ch == '$')
					options();
				do {
					while ((src->ch != '*') & (!(src->ch == PascalPSource::EOS)))
						src->next();
					src->next();
				} while (!((src->ch == ')') | (src->ch == PascalPSource::EOS)));
				src->next();
				goto _L1;
			}
			sym = PascalPGlobals::LPARENT;
			op = PascalPGlobals::NOOP;
			break;
		}
	}
}

char PascalPStringSource::next() {
	src_pos++;

	if (ch == EOL) {
		pos = 0;
		line++;
	}
	else if (ch != EOS) {
		pos++;
	}

	if (src_pos >= src_size) {
		ch = EOS;
		return EOS;
	}

	if (src[src_pos] == '\r') {
		if (src_pos < src_size - 1) {
			if (src[src_pos + 1] == '\n') {
				src_pos++;
				ch = EOL;
				return EOL;
			}
		}
	}

	ch = src[src_pos];
	return ch;
}

char PascalPStringSource::forward() {
	if (src_pos >= src_size)
		return EOS;
	return src[src_pos + 1];
}

