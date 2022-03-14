#include "Str.h"

/*
 */
const String String::null;

/*
 */
String::String() : length(0), capacity(CAPACITY), data(stack_data) {
	data[length] = '\0';
}

String::String(const String &s) : length(0), capacity(CAPACITY), data(stack_data) {
	copy(s);
}

String::String(const char *s) : length(0), capacity(CAPACITY), data(stack_data) {
	copy(s);
}

String::~String() {
	destroy();
}

/*
 */
String &String::operator=(const char *s) {
	if(data == s) return *this;
	if(s == NULL) clear();
	else do_copy(s,(int)strlen(s));
	return *this;
}

String &String::operator=(const String &s) {
	if(this == &s) return *this;
	do_copy(s,s.length);
	return *this;
}

/*
 */
String &String::operator+=(char c) {
	do_append(length,c);
	return *this;
}

String &String::operator+=(const char *s) {
	if(s == NULL) return *this;
	do_append(length,s,(int)strlen(s));
	return *this;
}

String &String::operator+=(const String &s) {
	do_append(length,s.data,s.length);
	return *this;
}

/*
 */
void String::resize(int size) {
	allocate(size);
	length = size;
	data[length] = '\0';
}

void String::allocate(int size) {
	if(size + 1 <= (capacity & CAPACITY_MASK)) return;
	int dynamic = (capacity & DYNAMIC_FLAG);
	capacity = (size + 1) | DYNAMIC_FLAG;
	char *new_data = new char[size + 1];
	do_memcpy(new_data,data,length);
	if(dynamic) delete [] data;
	data = new_data;
	data[length] = '\0';
}

void String::reserve(int size) {
	if(size + 1 <= (capacity & CAPACITY_MASK)) return;
	int dynamic = (capacity & DYNAMIC_FLAG);
	capacity = (size * 2 + 1) | DYNAMIC_FLAG;
	char *new_data = new char[size * 2 + 1];
	do_memcpy(new_data,data,length);
	if(dynamic) delete [] data;
	data = new_data;
	data[length] = '\0';
}

/*
 */
void String::clear() {
	length = 0;
	data[length] = '\0';
}

void String::destroy() {
	int dynamic = (capacity & DYNAMIC_FLAG);
	length = 0;
	capacity = CAPACITY;
	if(dynamic) delete [] data;
	data = stack_data;
	data[length] = '\0';
}

/*
 */
int String::find(char c) const {
	for(int i = 0; i < length; i++) if(data[i] == c) return i;
	return -1;
}

int String::find(const char *s) const {
	if(s == NULL) return -1;
	int len = (int)strlen(s);
	for(int i = 0; i < length; i++) if(!strncmp(data + i,s,len)) return i;
	return -1;
}

int String::find(const String &s) const {
	for(int i = 0; i < length; i++) if(!strncmp(data + i,s.data,s.length)) return i;
	return -1;
}

/*
 */
int String::rfind(char c) const {
	for(int i = length - 1; i >= 0; i--) if(data[i] == c) return i;
	return -1;
}

int String::rfind(const char *s) const {
	if(s == NULL) return -1;
	int len = (int)strlen(s);
	for(int i = length - 1; i >= 0; i--) if(!strncmp(data + i,s,len)) return i;
	return -1;
}

int String::rfind(const String &s) const {
	for(int i = length - 1; i >= 0; i--) if(!strncmp(data + i,s.data,s.length)) return i;
	return -1;
}

/*
 */
void String::copy(const char *s,int len) {
	if(s == NULL) {
		clear();
	} else {
		if(len == -1) len = (int)strlen(s);
		do_copy(s,len);
	}
}

void String::copy(const String &s,int len) {
	if(len == -1) len = s.length;
	do_copy(s.data,len);
}

/*
 */
void String::append(char c) {
	do_append(length,c);
}

void String::append(const char *s,int len) {
	if(s == NULL) return;
	if(len == -1) len = (int)strlen(s);
	do_append(length,s,len);
}

void String::append(const String &s,int len) {
	if(len == -1) len = s.length;
	do_append(length,s.data,len);
}

void String::append(int pos,char c) {
	do_append(pos,c);
}

void String::append(int pos,const char *s,int len) {
	if(s == NULL) return;
	if(len == -1) len = (int)strlen(s);
	do_append(pos,s,len);
}

void String::append(int pos,const String &s,int len) {
	if(len == -1) len = s.length;
	do_append(pos,s.data,len);
}

/*
 */
void String::remove() {
	assert(length > 0 && "String::remove(): bad length");
	data[--length] = '\0';
}

void String::remove(int pos,int len) {
	assert(pos >= 0 && pos <= length && "String::remove(): bad position");
	assert(len >= 0 && pos + len <= length && "String::remove(): bad length");
	do_memcpy(data + pos,data + pos + len,length - pos - len);
	length -= len;
	data[length] = '\0';
}

/*
 */
void String::do_copy(const char *s,int len) {
	length = len;
	if(length < (capacity & CAPACITY_MASK)) {
		do_memcpy(data,s,length);
	} else {
		int dynamic = (capacity & DYNAMIC_FLAG);
		capacity = (length + 1) | DYNAMIC_FLAG;
		char *new_data = new char[length + 1];
		do_memcpy(new_data,s,length);
		if(dynamic) delete [] data;
		data = new_data;
	}
	data[length] = '\0';
}

void String::do_append(int pos,char c) {
	assert(pos >= 0 && pos <= length && "String::do_append(): bad position");
	int new_length = length + 1;
	if(new_length < (capacity & CAPACITY_MASK)) {
		do_memcpy(data + pos + 1,data + pos,length - pos);
		data[pos] = c;
	} else {
		int dynamic = (capacity & DYNAMIC_FLAG);
		capacity = (new_length * 2 + 1) | DYNAMIC_FLAG;
		char *new_data = new char[new_length * 2 + 1];
		do_memcpy(new_data,data,pos);
		new_data[pos] = c;
		do_memcpy(new_data + pos + 1,data + pos,length - pos);
		if(dynamic) delete [] data;
		data = new_data;
	}
	length = new_length;
	data[length] = '\0';
}

void String::do_append(int pos,const char *s,int len) {
	assert(pos >= 0 && pos <= length && "String::do_append(): bad position");
	int new_length = length + len;
	if(new_length < (capacity & CAPACITY_MASK)) {
		do_memcpy(data + pos + len,data + pos,length - pos);
		do_memcpy(data + pos,s,len);
	} else {
		int dynamic = (capacity & DYNAMIC_FLAG);
		capacity = (new_length * 2 + 1) | DYNAMIC_FLAG;
		char *new_data = new char[new_length * 2 + 1];
		do_memcpy(new_data,data,pos);
		do_memcpy(new_data + pos,s,len);
		do_memcpy(new_data + pos + len,data + pos,length - pos);
		if(dynamic) delete [] data;
		data = new_data;
	}
	length = new_length;
	data[length] = '\0';
}

void String::do_memcpy(char *dest,const char *src,int size) {
	if(dest > src && (int)(dest - src) < size) {
		dest += size - 1;
		src += size - 1;
		for(int i = size; i > 0; i--) {
			*dest-- = *src--;
		}
	} else {
		#ifdef _WEBGL
			memcpy(dest,src,size);
		#else
			if(size & ~15) {
				for(size_t i = size >> 4; i > 0; i--) {
					*(unsigned int*)(dest + 0) = *(const unsigned int*)(src + 0);
					*(unsigned int*)(dest + 4) = *(const unsigned int*)(src + 4);
					*(unsigned int*)(dest + 8) = *(const unsigned int*)(src + 8);
					*(unsigned int*)(dest + 12) = *(const unsigned int*)(src + 12);
					dest += 16;
					src += 16;
				}
				size &= 15;
			}
			if(size & ~3) {
				for(size_t i = size >> 2; i > 0; i--) {
					*(unsigned int*)dest = *(const unsigned int*)src;
					dest += 4;
					src += 4;
				}
				size &= 3;
			}
			if(size) {
				for(int i = size; i > 0; i--) {
					*dest++ = *src++;
				}
			}
		#endif
	}
}

/*
 */
void String::lower() {
	char *s = data;
	while(*s) {
		char c = *s;
		if(c >= 'A' && c <= 'Z') *s += ('a' - 'A');
		s++;
	}
}

void String::upper() {
	char *s = data;
	while(*s) {
		char c = *s;
		if(c >= 'a' && c <= 'z') *s -= ('a' - 'A');
		s++;
	}
}

/*
 */
void String::vprintf(const char *format,va_list argptr) {
	clear();
	char buf[1024];
	StringStack<> token;
	const char *f = format;
	while(*f) {
		if(*f == '%') {
			token.clear();
			token.append(*f++);
			int is_long = 0;
			while(*f && strchr("0123456789.-+#lL ",*f)) {
				if(*f == 'l' || *f == 'L') is_long = 1;
				token.append(*f++);
			}
			token.append(*f);
			buf[0] = '\0';
			if(strchr("cdiouxX",*f)) {
				if(is_long) sprintf(buf,token,va_arg(argptr,long long));
				else sprintf(buf,token,va_arg(argptr,int));
			} else if(strchr("fgGeE",*f)) {
				sprintf(buf,token,va_arg(argptr,double));
			} else if(strchr("s",*f)) {
				append(va_arg(argptr,const char*));
			} else if(strchr("p",*f)) {
				sprintf(buf,token,va_arg(argptr,const void*));
			} else if(strchr("%",*f)) {
				strcpy(buf,"%");
			} else {
				break;
			}
			append(buf);
			if(*f) f++;
		} else {
			append(*f++);
		}
	}
}

void String::printf(const char *format,...) {
	va_list argptr;
	va_start(argptr,format);
	vprintf(format,argptr);
	va_end(argptr);
}

/*
 */
int String::vscanf(const char *format,va_list argptr) {
	int ret = 0;
	StringStack<> token;
	const char *s = data;
	const char *f = format;
	const char *delimiters = " \t\n\r";
	while(*s && *f) {
		while(*f && strchr(delimiters,*f)) f++;
		if(*f == '%') {
			while(*s && strchr(delimiters,*s)) s++;
			if(*s == '\0') break;
			f++;
			token.clear();
			int is_long = 0;
			while(*f && strchr("0123456789.lL ",*f)) {
				if(*f == 'l' || *f == 'L') is_long = 1;
				token.append(*f++);
			}
			int length = atoi(token);
			token.clear();
			if(strchr("dDiI",*f)) {
				if(length == 0) while(*s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				else while(length-- && *s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				if(is_long) *((long long*)va_arg(argptr,long long*)) = atoi(token);
				else *((int*)va_arg(argptr,int*)) = atoi(token);
				ret++;
			} else if(strchr("xX",*f)) {
				if(length == 0) while(*s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				else while(length-- && *s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				if(is_long) *((long long*)va_arg(argptr,long long*)) = atol("0x" + token);
				else *((int*)va_arg(argptr,int*)) = atoi("0x" + token);
				ret++;
			} else if(strchr("fFgGeE",*f)) {
				if(length == 0) while(*s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				else while(length-- && *s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				if(is_long) *((double*)va_arg(argptr,double*)) = atod(token);
				else *((float*)va_arg(argptr,float*)) = atof(token);
				ret++;
			} else if(strchr("sS",*f)) {
				char *d = (char*)va_arg(argptr,char*);
				if(length == 0) while(*s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				else while(length-- && *s && strchr(delimiters,*s) == NULL && *s != *(f + 1)) token.append(*s++);
				*d = '\0';
				ret++;
			} else if(strchr("cC",*f)) {
				*((char*)va_arg(argptr,char*)) = *s++;
				ret++;
			} else {
				break;
			}
			f++;
		} else {
			while(*s && strchr(delimiters,*s)) s++;
			while(*f && *s && *f == *s) {
				if(strchr(delimiters,*f)) break;
				f++;
				s++;
			}
			if(*f != '%' && *f != *s) break;
		}
	}
	return ret;
}

int String::scanf(const char *format,...) {
	va_list argptr;
	va_start(argptr,format);
	int ret = vscanf(format,argptr);
	va_end(argptr);
	return ret;
}

/******************************************************************************\
*
* String functions
*
\******************************************************************************/

/*
 */
StringStack<> operator+(const String &s0,const String &s1) {
	StringStack<> ret = s0;
	ret.append(s1);
	return ret;
}

StringStack<> operator+(const String &s0,const char *s1) {
	StringStack<> ret = s0;
	ret.append(s1);
	return ret;
}

StringStack<> operator+(const char *s0,const String &s1) {
	StringStack<> ret = StringStack<>(s0);
	ret.append(s1);
	return ret;
}

StringStack<> operator+(const String &s0,char c1) {
	StringStack<> ret = s0;
	ret.append(c1);
	return ret;
}

StringStack<> operator+(char c0,const String &s1) {
	StringStack<> ret;
	ret.append(c0);
	ret.append(s1);
	return ret;
}

/*
 */
#define DECLARE_STRING_COMPARE(OP) \
int operator OP(const String &s0,const String &s1) { \
	return (strcmp(s0.get(),s1.get()) OP 0); \
} \
int operator OP(const String &s0,const char *s1) { \
	if(s1 == NULL) return 0; \
	return (strcmp(s0.get(),s1) OP 0); \
} \
int operator OP(const char *s0,const String &s1) { \
	if(s0 == NULL) return 0; \
	return (strcmp(s0,s1.get()) OP 0); \
}

DECLARE_STRING_COMPARE(==)
DECLARE_STRING_COMPARE(!=)
DECLARE_STRING_COMPARE(<)
DECLARE_STRING_COMPARE(>)
DECLARE_STRING_COMPARE(<=)
DECLARE_STRING_COMPARE(>=)

#undef DECLARE_STRING_COMPARE

/******************************************************************************\
*
* String static functions
*
\******************************************************************************/

/*
 */
int String::isdigit(int code) {
	return (code >= '0' && code <= '9');
}

int String::islower(int code) {
	return (code >= 'a' && code <= 'z');
}

int String::isupper(int code) {
	return (code >= 'A' && code <= 'Z');
}

int String::isalpha(int code) {
	return ((code >= 'a' && code <= 'z') || (code >= 'A' && code <= 'Z'));
}

/*
 */
int String::atoi(const char *str) {
	int ret = 0;
	int negative = 0;
	const char *s = str;
	while(*s == ' ' || *s == '\t') {
		s++;
	}
	if(*s == '-' || *s == '+') {
		if(*s == '-') negative = 1;
		s++;
	}
	if(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
		s += 2;
		while(*s) {
			char c = *s++;
			if(c >= '0' && c <= '9') ret = (ret << 4) + (c - '0');
			else if(c >= 'a' && c <= 'f') ret = (ret << 4) + (c - 'a' + 10);
			else if(c >= 'A' && c <= 'F') ret = (ret << 4) + (c - 'A' + 10);
			else break;
		}
	} else {
		while(*s) {
			char c = *s++;
			if(c < '0' || c > '9') break;
			ret = ret * 10 + (c - '0');
		}
	}
	if(negative) return -ret;
	return ret;
}

long long String::atol(const char *str) {
	long long ret = 0;
	int negative = 0;
	const char *s = str;
	while(*s == ' ' || *s == '\t') {
		s++;
	}
	if(*s == '-' || *s == '+') {
		if(*s == '-') negative = 1;
		s++;
	}
	if(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
		s += 2;
		while(*s) {
			char c = *s++;
			if(c >= '0' && c <= '9') ret = (ret << 4) + (c - '0');
			else if(c >= 'a' && c <= 'f') ret = (ret << 4) + (c - 'a' + 10);
			else if(c >= 'A' && c <= 'F') ret = (ret << 4) + (c - 'A' + 10);
			else break;
		}
	} else {
		while(*s) {
			char c = *s++;
			if(c < '0' || c > '9') break;
			ret = ret * 10 + (c - '0');
		}
	}
	if(negative) return -ret;
	return ret;
}

float String::atof(const char *str) {
	return (float)atod(str);
}

double String::atod(const char *str) {
	double ret = 0.0;
	int negative = 0;
	const char *s = str;
	while(*s == ' ' || *s == '\t') {
		s++;
	}
	if(*s == '-' || *s == '+') {
		if(*s == '-') negative = 1;
		s++;
	}
	if(*s >= '0' && *s <= '9') {
		while(*s) {
			char c = *s;
			if(c < '0' || c > '9') break;
			ret = ret * 10.0 + (c - '0');
			s++;
		}
	}
	if(*s == '.') {
		s++;
		if(*s >= '0' && *s <= '9') {
			double frac = 0.0;
			double scale = 0.1;
			while(*s) {
				char c = *s;
				if(c < '0' || c > '9') break;
				frac += scale * (c - '0');
				scale *= 0.1;
				s++;
			}
			ret += frac;
		}
	}
	if(*s == 'e' || *s == 'E') {
		s++;
		double exp = 0.0;
		int negative = 0;
		if(*s == '-' || *s == '+') {
			if(*s == '-') negative = 1;
			s++;
		}
		if(*s >= '0' && *s <= '9') {
			while(*s) {
				char c = *s++;
				if(c < '0' || c > '9') break;
				exp = exp * 10.0 + (c - '0');
			}
		}
		if(negative) exp = -exp;
		ret *= pow(10.0,exp);
	}
	if(negative) return -ret;
	return ret;
}

/*
 */
StringStack<> String::memory(size_t size) {
	StringStack<> ret;
	if(size < 1024) ret.printf("%dB",size);
	else if(size < 1024 * 1024) ret.printf("%.1fKB",size / 1024.0);
	else if(size < 1024 * 1024 * 1024) ret.printf("%.1fMB",size / (1024.0 * 1024.0));
	else ret.printf("%.1fGB",size / (1024.0 * 1024.0 * 1024.0));
	return ret;
}

/*
 */
StringStack<> String::lower(const char *str) {
	StringStack<> ret = StringStack<>(str);
	ret.lower();
	return ret;
}

StringStack<> String::upper(const char *str) {
	StringStack<> ret = StringStack<>(str);
	ret.upper();
	return ret;
}

/*
 */
StringStack<> String::format(const char *format,va_list argptr) {
	StringStack<> ret;
	ret.vprintf(format,argptr);
	return ret;
}

StringStack<> String::format(const char *format,...) {
	StringStack<> ret;
	va_list argptr;
	va_start(argptr,format);
	ret.vprintf(format,argptr);
	va_end(argptr);
	return ret;
}

/*
 */
int String::vsscanf(const char *str,const char *format,va_list argptr) {
	return StringStack<>(str).vscanf(format,argptr);
}

int String::sscanf(const char *str,const char *format,...) {
	va_list argptr;
	va_start(argptr,format);
	int ret = StringStack<>(str).vscanf(format,argptr);
	va_end(argptr);
	return ret;
}

/*
 */
StringStack<> String::trim(const char *str,const char *symbols) {
	if(symbols == NULL) symbols = " ";
	const char *s = str;
	while(*s && strchr(symbols,*s) != NULL) s++;
	StringStack<> ret = StringStack<>(s);
	str = ret.get();
	s = str + ret.size() - 1;
	while(s > str && strchr(symbols,*s--) != NULL) ret.remove();
	return ret;
}

StringStack<> String::replace(const char *str,const char *before,const char *after) {
	StringStack<> ret;
	const char *s = str;
	int length = (int)strlen(before);
	while(*s) {
		if(length && !strncmp(s,before,length)) {
			ret += after;
			s += length;
		} else {
			ret += *s++;
		}
	}
	return ret;
}

/*
 */
StringArray<> String::split(const char *str,const char *delimiters) {
	StringArray<> ret((int)strlen(str));
	const char *s = str;
	char *d = ret.data;
	int index = -1;
	while(*s) {
		if(strchr(delimiters,*s)) {
			while(*s && strchr(delimiters,*s)) {
				*d++ = '\0';
				s++;
			}
			if(index != -1) ret.append(index);
			index = -1;
		} else {
			if(*s == '\\' && *(s + 1) != '\0' && strchr(delimiters,*(s + 1))) s++;
			if(index == -1) index = (int)(d - ret.data);
			*d++ = *s++;
		}
	}
	if(index != -1) ret.append(index);
	*d = '\0';
	return ret;
}

StringStack<> String::substr(const char *str,int pos,int len) {
	StringStack<> ret;
	int length = (int)strlen(str);
	if(len == -1) len = length - pos;
	for(int i = 0; i < len; i++) {
		if(pos + i < 0) ret += ' ';
		else if(pos + i < length) ret += str[pos + i];
		else ret += ' ';
	}
	return ret;
}

StringStack<> String::addslashes(const char *str) {
	StringStack<> ret;
	const char *s = str;
	while(*s) {
		if(*s == '\t') ret += "\\t";
		else if(*s == '\n') ret += "\\n";
		else if(*s == '\r') ret += "\\r";
		else if(*s == '"') ret += "\\\"";
		else if(*s == '\\') ret += "\\\\";
		else ret += *s;
		s++;
	}
	return ret;
}

StringStack<> String::stripslashes(const char *str) {
	StringStack<> ret;
	const char *s = str;
	while(*s) {
		if(*s == '\\') {
			s++;
			if(*s == 't') ret += '\t';
			else if(*s == 'n') ret += '\n';
			else if(*s == 'r') ret += '\r';
			else if(*s == '"') ret += '\"';
			else if(*s == '\\') ret += '\\';
			else if(*s == '0' && (*(s + 1) >= '0' && *(s + 1) <= '7') && (*(s + 2) >= '0' && *(s + 2) <= '7')) {
				ret += (*(s + 1) - '0') * 8 + (*(s + 2) - '0');
				s += 2;
			}
			else ret += String::format("\\%c",*s);
			s++;
		} else {
			ret += *s++;
		}
	}
	return ret;
}

/*
 */
StringStack<> String::dirname(const char *str) {
	const char *s = strrchr(str,'/');
	if(s == NULL) s = strrchr(str,'\\');
	if(s == NULL) return StringStack<>();
	return substr(str,0,(int)(s - str) + 1);
}

StringStack<> String::basename(const char *str) {
	const char *s = strrchr(str,'/');
	if(s == NULL) s = strrchr(str,'\\');
	if(s == NULL) return StringStack<>(str);
	return StringStack<>(s + 1);
}

StringStack<> String::pathname(const char *str) {
	StringStack<> ret;
	const char *s = str;
	while(1) {
		if(*s == '/' && ret.size() > 0 && ret[ret.size() - 1] == '/') {
			s++;
		} else if(!strncmp(s,"./",2)) {
			s += 2;
		} else if(!strncmp(s,"../",3)) {
			s += 3;
			if(ret.size() > 0 && ret[ret.size() - 1] == '/' && (ret.size() < 3 || strncmp(ret.get() + ret.size() - 3,"../",3))) {
				#ifdef _WIN32
					if(ret.size() != 3 || isalpha(ret[0]) == 0 || ret[1] != ':' || ret[2] != '/') {
				#else
					if(ret.size() != 1 || ret[0] != '/') {
				#endif
					ret.remove();
					int pos = ret.rfind('/');
					if(pos == -1) ret.clear();
					else ret.remove(pos + 1,ret.size() - pos - 1);
				}
			} else {
				#ifdef _WIN32
					if(ret.size() < 3 || isalpha(ret[0]) == 0 || ret[1] != ':' || ret[2] != '/') {
						ret.append("../");
					}
				#else
					if(ret.size() < 1 || ret[0] != '/') {
						ret.append("../");
					}
				#endif
			}
		} else {
			const char *slash = strchr(s,'/');
			if(slash == NULL) break;
			ret.append(s,(int)(slash - s) + 1);
			s = slash + 1;
		}
	}
	return ret;
}

StringStack<> String::absname(const char *path,const char *str) {
	StringStack<> ret;
	#ifdef _WIN32
		const char *s = str;
		if(*s == '\0' || *(s + 1) != ':') ret = path;
	#else
		ret = path;
	#endif
	ret += str;
	return ret;
}

StringStack<> String::relname(const char *path,const char *str) {
	StringStack<> ret;
	const char *p = path;
	const char *s = str;
	while(*p && *p == *s) {
		p++;
		s++;
	}
	while(p > path && *p != '/') p--;
	while(s > str && *s != '/') s--;
	if(*p != '/' || *s != '/') return StringStack<>(str);
	while(*p) {
		if(*++p == '/') ret += "../";
	}
	ret += s + 1;
	return ret;
}

StringStack<> String::extension(const char *str,const char *ext) {
	StringStack<> ret(str);
	int pos = ret.rfind('.');
	if(pos != -1 && pos > ret.rfind('/') && pos > ret.rfind('\\')) {
		if(lower(substr(ret,pos)) != ext) {
			ret.remove(pos,ret.size() - pos);
			ret += ext;
		}
	} else {
		ret += ext;
	}
	return ret;
}

/******************************************************************************\
*
* String unicode functions
*
\******************************************************************************/

/*
 */
StringStack<> String::unicodeToUtf8(const wchar_t *src) {
	StringStack<> ret;
	const wchar_t *s = src;
	while(*s) {
		unsigned int code = *s++;
		if(code < 0x80) {
			ret += code & 0x7f;
		} else if(code < 0x800) {
			ret += 0xc0 | (code >> 6);
			ret += 0x80 | (code & 0x3f);
		} else if(code < 0x10000) {
			ret += 0xe0 | (code >> 12);
			ret += 0x80 | ((code >> 6) & 0x3f);
			ret += 0x80 | (code & 0x3f);
		}
	}
	return ret;
}

StringStack<> String::unicodeToUtf8(const unsigned int *src) {
	StringStack<> ret;
	const unsigned int *s = src;
	while(*s) {
		unsigned int code = *s++;
		if(code < 0x80) {
			ret += code & 0x7f;
		} else if(code < 0x800) {
			ret += 0xc0 | (code >> 6);
			ret += 0x80 | (code & 0x3f);
		} else if(code < 0x10000) {
			ret += 0xe0 | (code >> 12);
			ret += 0x80 | ((code >> 6) & 0x3f);
			ret += 0x80 | (code & 0x3f);
		} else if(code < 0x200000) {
			ret += 0xf0 | (code >> 18);
			ret += 0x80 | ((code >> 12) & 0x3f);
			ret += 0x80 | ((code >> 6) & 0x3f);
			ret += 0x80 | (code & 0x3f);
		}
	}
	return ret;
}

int String::utf8ToUnicode(const char *src,unsigned int &code) {
	const unsigned char *s = (const unsigned char*)src;
	code = *s;
	if(code > 0xf0) {
		code = (unsigned int)(*s++ & 0x07) << 18;
		if(*s) code |= (unsigned int)(*s++ & 0x3f) << 12;
		if(*s) code |= (unsigned int)(*s++ & 0x3f) << 6;
		if(*s) code |= (unsigned int)(*s++ & 0x3f);
	} else if(code > 0xe0) {
		code = (unsigned int)(*s++ & 0x0f) << 12;
		if(*s) code |= (unsigned int)(*s++ & 0x3f) << 6;
		if(*s) code |= (unsigned int)(*s++ & 0x3f);
	} else if(code > 0xc0) {
		code = (unsigned int)(*s++ & 0x1f) << 6;
		if(*s) code |= (unsigned int)(*s++ & 0x3f);
	} else {
		s++;
	}
	return (int)(s - (const unsigned char*)src);
}

int String::utf8ToUnicode(const char *src,unsigned int *dest,int size) {
	const char *s = src;
	unsigned int *d = dest;
	unsigned int code = 0;
	while(*s && --size) {
		s += utf8ToUnicode(s,code);
		*d++ = code;
	}
	*d = '\0';
	return (int)(d - dest);
}

int String::utf8ToUnicode(const char *src,wchar_t *dest,int size) {
	const char *s = src;
	wchar_t *d = dest;
	unsigned int code = 0;
	while(*s && --size) {
		s += utf8ToUnicode(s,code);
		if(code < 0x10000) *d++ = (wchar_t)code;
		else *d++ = 0x25a1;
	}
	*d = '\0';
	return (int)(d - dest);
}

/*
 */
int String::utf8strlen(const char *str) {
	const unsigned char *s = (const unsigned char*)str;
	int length = 0;
	while(*s) {
		int code = *s++;
		if(code > 0xf0) {
			if(*s) s++;
			if(*s) s++;
			if(*s) s++;
		} else if(code > 0xe0) {
			if(*s) s++;
			if(*s) s++;
		} else if(code > 0xc0) {
			if(*s) s++;
		}
		length++;
	}
	return length;
}

StringStack<> String::utf8substr(const char *str,int pos,int len) {
	StringStack<> ret;
	const unsigned char *s = (const unsigned char*)str;
	int length = 0;
	while(*s) {
		if(length == pos) str = (const char*)s;
		int code = *s++;
		if(code > 0xf0) {
			if(*s) s++;
			if(*s) s++;
			if(*s) s++;
		} else if(code > 0xe0) {
			if(*s) s++;
			if(*s) s++;
		} else if(code > 0xc0) {
			if(*s) s++;
		}
		length++;
	}
	s = (const unsigned char*)str;
	if(len == -1) len = length - pos;
	for(int i = 0; i < len; i++) {
		if(pos + i < 0) ret += ' ';
		else if(pos + i < length) {
			int code = *s++;
			ret += (char)code;
			if(code > 0xf0) {
				if(*s) ret += *s++;
				if(*s) ret += *s++;
				if(*s) ret += *s++;
			} else if(code > 0xe0) {
				if(*s) ret += *s++;
				if(*s) ret += *s++;
			} else if(code > 0xc0) {
				if(*s) ret += *s++;
			}
		}
		else ret += ' ';
	}
	return ret;
}

/******************************************************************************\
*
* String compare functions
*
\******************************************************************************/

/*
 */
int String::match(const char *pattern,const char *str) {
	int ret = 1;
	const char *p = pattern;
	const char *s = str;
	while(*p) {
		if(*p == '*' && ret) {
			if(*(p + 1) == '\0' || *(p + 1) == '|') break;
			p++;
			int len = 0;
			while(p[len] && p[len] != '|') len++;
			while(*s) s++;
			if(s - str >= len) s -= len;
		} else if(*p == '|') {
			if(ret) break;
			p++;
			s = str;
			ret = 1;
		} else {
			if(*p != '?' && *p != *s) ret = 0;
			if(*s) s++;
			p++;
		}
	}
	if(*s != '\0' && *p == '\0') ret = 0;
	return ret;
}

/******************************************************************************\
*
* StringPtr
*
\******************************************************************************/

/*
 */
StringPtr::StringPtr() : string(NULL) {
	
}

StringPtr::StringPtr(const String &s) : string(NULL) {
	if(s.size()) string = new String(s);
}

StringPtr::StringPtr(const StringPtr &s) : string(NULL) {
	if(s.string && s.string->size()) string = new String(*s.string);
}

StringPtr::StringPtr(const char *s) : string(NULL) {
	if(s && strlen(s)) string = new String(s);
}

StringPtr::~StringPtr() {
	delete string;
}

/*
 */
StringPtr &StringPtr::operator=(const char *s) {
	if(s && strlen(s)) {
		if(string == NULL) string = new String(s);
		else *string = s;
	} else {
		clear();
	}
	return *this;
}
StringPtr &StringPtr::operator=(const String &s) {
	if(s.size()) {
		if(string == NULL) string = new String(s);
		else *string = s;
	} else {
		clear();
	}
	return *this;
}
StringPtr &StringPtr::operator=(const StringPtr &s) {
	if(this == &s) return *this;
	if(s.string && s.string->size()) {
		if(string == NULL) string = new String(*s.string);
		else *string = *s.string;
	} else {
		clear();
	}
	return *this;
}

/*
 */
StringPtr &StringPtr::operator+=(const char *s) {
	if(s && strlen(s)) {
		if(string == NULL) string = new String(s);
		else *string += s;
	}
	return *this;
}
StringPtr &StringPtr::operator+=(const String &s) {
	if(s.size()) {
		if(string == NULL) string = new String(s);
		else *string += s;
	}
	return *this;
}

/*
 */
#define DECLARE_STRING_PTR_COMPARE(OP) \
int operator OP(const StringPtr &s0,const StringPtr &s1) { \
	return (s0.getString() OP s1.getString()); \
} \
int operator OP(const StringPtr &s0,const char *s1) { \
	return (s0.getString() OP s1); \
} \
int operator OP(const char *s0,const StringPtr &s1) { \
	return (s0 OP s1.getString()); \
}

DECLARE_STRING_PTR_COMPARE(==)
DECLARE_STRING_PTR_COMPARE(!=)

#undef DECLARE_STRING_PTR_COMPARE
