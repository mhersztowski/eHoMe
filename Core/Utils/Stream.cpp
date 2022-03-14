#include "Stream.h"

/******************************************************************************\
*
* Stream read/write
*
\******************************************************************************/

/*
 */
size_t Stream::readStream(const Stream *dest,size_t size,size_t nmemb) const {
	unsigned char buf[4096];
	if(size * nmemb > sizeof(buf)) {
		unsigned char *buf = new unsigned char[size * nmemb];
		size_t ret = read(buf,size,nmemb);
		if(ret) dest->write(buf,size,ret);
		delete [] buf;
		return ret;
	}
	size_t ret = read(buf,size,nmemb);
	if(ret) dest->write(buf,size,ret);
	return ret;
}

size_t Stream::writeStream(const Stream *src,size_t size,size_t nmemb) const {
	unsigned char buf[4096];
	if(size * nmemb > sizeof(buf)) {
		unsigned char *buf = new unsigned char[size * nmemb];
		size_t ret = src->read(buf,size,nmemb);
		if(ret) write(buf,size,ret);
		delete [] buf;
		return ret;
	}
	size_t ret = src->read(buf,size,nmemb);
	if(ret) write(buf,size,ret);
	return ret;
}

/******************************************************************************\
*
* Endian correct read/write
*
\******************************************************************************/

/*
 */
char Stream::readChar() const {
	char ret = 0;
	read(&ret,sizeof(char),1);
	return ret;
}

int Stream::writeChar(char value) const {
	return (write(&value,sizeof(char),1) == 1);
}

/*
 */
unsigned char Stream::readUChar() const {
	unsigned char ret = 0;
	read(&ret,sizeof(unsigned char),1);
	return ret;
}

int Stream::writeUChar(unsigned char value) const {
	return (write(&value,sizeof(unsigned char),1) == 1);
}

/*
 */
#define DECLARE_READ_DIRECT(TYPE,NAME) \
TYPE Stream::NAME() const { \
	TYPE ret = 0; \
	read(&ret,sizeof(TYPE),1); \
	return ret; \
}

#define DECLARE_READ_REVERSE(TYPE,NAME) \
TYPE Stream::NAME() const { \
	TYPE ret,buf = 0; \
	read(&buf,sizeof(TYPE),1); \
	const unsigned char *s = (const unsigned char*)&buf; \
	unsigned char *d = (unsigned char*)&ret + sizeof(TYPE) - 1; \
	for(int i = 0; i < (int)sizeof(TYPE); i++) { \
		*d-- = *s++; \
	} \
	return ret; \
}

#define DECLARE_READ_ARRAY_DIRECT(TYPE,NAME) \
int Stream::NAME(TYPE *dest,int size) const { \
	return ((int)read(dest,sizeof(TYPE),size) == size); \
}

#define DECLARE_READ_ARRAY_REVERSE(TYPE,NAME) \
int Stream::NAME(TYPE *dest,int size) const { \
	int ret = ((int)read(dest,sizeof(TYPE),size) == size); \
	for(int i = 0; i < size; i++) { \
		TYPE buf = dest[i]; \
		const unsigned char *s = (const unsigned char*)&buf; \
		unsigned char *d = (unsigned char*)&dest[i] + sizeof(TYPE) - 1; \
		for(int j = 0; j < (int)sizeof(TYPE); j++) { \
			*d-- = *s++; \
		} \
	} \
	return ret; \
}

/*
 */
#define DECLARE_WRITE_DIRECT(TYPE,NAME) \
int Stream::NAME(TYPE value) const { \
	return (write(&value,sizeof(TYPE),1) == 1); \
}

#define DECLARE_WRITE_REVERSE(TYPE,NAME) \
int Stream::NAME(TYPE value) const { \
	TYPE buf; \
	const unsigned char *s = (const unsigned char*)&value; \
	unsigned char *d = (unsigned char*)&buf + sizeof(TYPE) - 1; \
	for(int i = 0; i < (int)sizeof(TYPE); i++) { \
		*d-- = *s++; \
	} \
	return (write(&buf,sizeof(TYPE),1) == 1); \
}

#define DECLARE_WRITE_ARRAY_DIRECT(TYPE,NAME) \
int Stream::NAME(const TYPE *src,int size) const { \
	return ((int)write(src,sizeof(TYPE),size) == size); \
}

#define DECLARE_WRITE_ARRAY_REVERSE(TYPE,NAME) \
int Stream::NAME(const TYPE *src,int size) const { \
	for(int i = 0; i < size; i++) { \
		TYPE buf; \
		const unsigned char *s = (const unsigned char*)&src[i]; \
		unsigned char *d = (unsigned char*)&buf + sizeof(TYPE) - 1; \
		for(int j = 0; j < (int)sizeof(TYPE); j++) { \
			*d-- = *s++; \
		} \
		if(write(&buf,sizeof(TYPE),1) != 1) return 0; \
	} \
	return 1; \
}

/*
 */
#ifdef USE_BIG_ENDIAN
	
	/* short
	 */
	DECLARE_READ_DIRECT(short,readShortBig)
	DECLARE_WRITE_DIRECT(short,writeShortBig)
	DECLARE_READ_ARRAY_DIRECT(short,readShortArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(short,writeShortArrayBig)
	
	DECLARE_READ_REVERSE(short,readShortLittle)
	DECLARE_WRITE_REVERSE(short,writeShortLittle)
	DECLARE_READ_ARRAY_REVERSE(short,readShortArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(short,writeShortArrayLittle)
	
	/* unsigned short
	 */
	DECLARE_READ_DIRECT(unsigned short,readUShortBig)
	DECLARE_WRITE_DIRECT(unsigned short,writeUShortBig)
	DECLARE_READ_ARRAY_DIRECT(unsigned short,readUShortArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(unsigned short,writeUShortArrayBig)
	
	DECLARE_READ_REVERSE(unsigned short,readUShortLittle)
	DECLARE_WRITE_REVERSE(unsigned short,writeUShortLittle)
	DECLARE_READ_ARRAY_REVERSE(unsigned short,readUShortArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(unsigned short,writeUShortArrayLittle)
	
	/* int
	 */
	DECLARE_READ_DIRECT(int,readIntBig)
	DECLARE_WRITE_DIRECT(int,writeIntBig)
	DECLARE_READ_ARRAY_DIRECT(int,readIntArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(int,writeIntArrayBig)
	
	DECLARE_READ_REVERSE(int,readIntLittle)
	DECLARE_WRITE_REVERSE(int,writeIntLittle)
	DECLARE_READ_ARRAY_REVERSE(int,readIntArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(int,writeIntArrayLittle)
	
	/* unsigned int
	 */
	DECLARE_READ_DIRECT(unsigned int,readUIntBig)
	DECLARE_WRITE_DIRECT(unsigned int,writeUIntBig)
	DECLARE_READ_ARRAY_DIRECT(unsigned int,readUIntArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(unsigned int,writeUIntArrayBig)
	
	DECLARE_READ_REVERSE(unsigned int,readUIntLittle)
	DECLARE_WRITE_REVERSE(unsigned int,writeUIntLittle)
	DECLARE_READ_ARRAY_REVERSE(unsigned int,readUIntArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(unsigned int,writeUIntArrayLittle)
	
	/* long long
	 */
	DECLARE_READ_DIRECT(long long,readLongBig)
	DECLARE_WRITE_DIRECT(long long,writeLongBig)
	DECLARE_READ_ARRAY_DIRECT(long long,readLongArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(long long,writeLongArrayBig)
	
	DECLARE_READ_REVERSE(long long,readLongLittle)
	DECLARE_WRITE_REVERSE(long long,writeLongLittle)
	DECLARE_READ_ARRAY_REVERSE(long long,readLongArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(long long,writeLongArrayLittle)
	
	/* float
	 */
	DECLARE_READ_DIRECT(float,readFloatBig)
	DECLARE_WRITE_DIRECT(float,writeFloatBig)
	DECLARE_READ_ARRAY_DIRECT(float,readFloatArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(float,writeFloatArrayBig)
	
	DECLARE_READ_REVERSE(float,readFloatLittle)
	DECLARE_WRITE_REVERSE(float,writeFloatLittle)
	DECLARE_READ_ARRAY_REVERSE(float,readFloatArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(float,writeFloatArrayLittle)
	
	/* double
	 */
	DECLARE_READ_DIRECT(double,readDoubleBig)
	DECLARE_WRITE_DIRECT(double,writeDoubleBig)
	DECLARE_READ_ARRAY_DIRECT(double,readDoubleArrayBig)
	DECLARE_WRITE_ARRAY_DIRECT(double,writeDoubleArrayBig)
	
	DECLARE_READ_REVERSE(double,readDoubleLittle)
	DECLARE_WRITE_REVERSE(double,writeDoubleLittle)
	DECLARE_READ_ARRAY_REVERSE(double,readDoubleArrayLittle)
	DECLARE_WRITE_ARRAY_REVERSE(double,writeDoubleArrayLittle)
	
#else
	
	/* short
	 */
	DECLARE_READ_REVERSE(short,readShortBig)
	DECLARE_WRITE_REVERSE(short,writeShortBig)
	DECLARE_READ_ARRAY_REVERSE(short,readShortArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(short,writeShortArrayBig)
	
	DECLARE_READ_DIRECT(short,readShortLittle)
	DECLARE_WRITE_DIRECT(short,writeShortLittle)
	DECLARE_READ_ARRAY_DIRECT(short,readShortArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(short,writeShortArrayLittle)
	
	/* unsigned short
	 */
	DECLARE_READ_REVERSE(unsigned short,readUShortBig)
	DECLARE_WRITE_REVERSE(unsigned short,writeUShortBig)
	DECLARE_READ_ARRAY_REVERSE(unsigned short,readUShortArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(unsigned short,writeUShortArrayBig)
	
	DECLARE_READ_DIRECT(unsigned short,readUShortLittle)
	DECLARE_WRITE_DIRECT(unsigned short,writeUShortLittle)
	DECLARE_READ_ARRAY_DIRECT(unsigned short,readUShortArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(unsigned short,writeUShortArrayLittle)
	
	/* int
	 */
	DECLARE_READ_REVERSE(int,readIntBig)
	DECLARE_WRITE_REVERSE(int,writeIntBig)
	DECLARE_READ_ARRAY_REVERSE(int,readIntArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(int,writeIntArrayBig)
	
	DECLARE_READ_DIRECT(int,readIntLittle)
	DECLARE_WRITE_DIRECT(int,writeIntLittle)
	DECLARE_READ_ARRAY_DIRECT(int,readIntArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(int,writeIntArrayLittle)
	
	/* unsigned int
	 */
	DECLARE_READ_REVERSE(unsigned int,readUIntBig)
	DECLARE_WRITE_REVERSE(unsigned int,writeUIntBig)
	DECLARE_READ_ARRAY_REVERSE(unsigned int,readUIntArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(unsigned int,writeUIntArrayBig)
	
	DECLARE_READ_DIRECT(unsigned int,readUIntLittle)
	DECLARE_WRITE_DIRECT(unsigned int,writeUIntLittle)
	DECLARE_READ_ARRAY_DIRECT(unsigned int,readUIntArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(unsigned int,writeUIntArrayLittle)
	
	/* long long
	 */
	DECLARE_READ_REVERSE(long long,readLongBig)
	DECLARE_WRITE_REVERSE(long long,writeLongBig)
	DECLARE_READ_ARRAY_REVERSE(long long,readLongArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(long long,writeLongArrayBig)
	
	DECLARE_READ_DIRECT(long long,readLongLittle)
	DECLARE_WRITE_DIRECT(long long,writeLongLittle)
	DECLARE_READ_ARRAY_DIRECT(long long,readLongArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(long long,writeLongArrayLittle)
	
	/* float
	 */
	DECLARE_READ_REVERSE(float,readFloatBig)
	DECLARE_WRITE_REVERSE(float,writeFloatBig)
	DECLARE_READ_ARRAY_REVERSE(float,readFloatArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(float,writeFloatArrayBig)
	
	DECLARE_READ_DIRECT(float,readFloatLittle)
	DECLARE_WRITE_DIRECT(float,writeFloatLittle)
	DECLARE_READ_ARRAY_DIRECT(float,readFloatArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(float,writeFloatArrayLittle)
	
	/* double
	 */
	DECLARE_READ_REVERSE(double,readDoubleBig)
	DECLARE_WRITE_REVERSE(double,writeDoubleBig)
	DECLARE_READ_ARRAY_REVERSE(double,readDoubleArrayBig)
	DECLARE_WRITE_ARRAY_REVERSE(double,writeDoubleArrayBig)
	
	DECLARE_READ_DIRECT(double,readDoubleLittle)
	DECLARE_WRITE_DIRECT(double,writeDoubleLittle)
	DECLARE_READ_ARRAY_DIRECT(double,readDoubleArrayLittle)
	DECLARE_WRITE_ARRAY_DIRECT(double,writeDoubleArrayLittle)
	
#endif /* USE_BIG_ENDIAN */

#undef DECLARE_READ_DIRECT
#undef DECLARE_READ_REVERSE
#undef DECLARE_READ_ARRAY_DIRECT
#undef DECLARE_READ_ARRAY_REVERSE

#undef DECLARE_WRITE_DIRECT
#undef DECLARE_WRITE_REVERSE
#undef DECLARE_WRITE_ARRAY_DIRECT
#undef DECLARE_WRITE_ARRAY_REVERSE

/******************************************************************************\
*
* Binary strings
*
\******************************************************************************/

/*
 */
const StreamString<> Stream::readString() const {
	int length = readInt();
    if(length <= 0) throw("Stream::readString(): can't read string\n");
	StreamString<> ret(length - 1);
    if((int)read(ret.data,sizeof(char),length) != length) throw("Stream::readString(): can't read string\n");
    if(ret.data[length - 1] != '\0') throw("Stream::readString(): can't read string\n");
	return ret;
}

int Stream::readString(char *str,int size) const {
	int length = readInt();
	if(length <= 0 || length > size) return 0;
	if((int)read(str,sizeof(char),length) != length) return 0;
	if(str[length - 1] != '\0') return 0;
	return 1;
}

/*
 */
int Stream::writeString(const char *str) const {
	int length = (int)strlen(str) + 1;
	if(writeInt(length) == 0) return 0;
	return ((int)write(str,sizeof(char),length) == length);
}

/******************************************************************************\
*
* Ascii strings
*
\******************************************************************************/

/*
 */
const StreamString<> Stream::readToken() const {
	char c = 0;
	StreamString<> ret;
	while(read(&c,sizeof(char),1) == 1) {
		if(strchr(" \t\n\r",c)) continue;
		if(c != '"') ret.append(c);
		break;
	}
	if(c == '"') {
		while(read(&c,sizeof(char),1) == 1) {
			if(c == '"' && (ret.empty() || ret[ret.size() - 1] != '\\')) break;
			ret.append(c);
		}
	} else {
		while(read(&c,sizeof(char),1) == 1) {
			if(strchr(" \t\n\r",c)) break;
			ret.append(c);
		}
	}
	return ret;
}

int Stream::readToken(char *str,int size) const {
	char c = 0;
	char *s = str;
	while(s - str < size - 1 && read(&c,sizeof(char),1) == 1) {
		if(strchr(" \t\n\r",c)) continue;
		if(c != '"') *s++ = c;
		break;
	}
	if(c == '"') {
		while(s - str < size - 1 && read(&c,sizeof(char),1) == 1) {
			if(c == '"' && (s == str || *(s - 1) != '\\')) break;
			*s++ = c;
		}
	} else {
		while(s - str < size - 1 && read(&c,sizeof(char),1) == 1) {
			if(strchr(" \t\n\r",c)) break;
			*s++ = c;
		}
	}
	*s = '\0';
	return (int)(s - str);
}

/*
 */
const StreamString<> Stream::readLine() const {
	char c = 0;
	StreamString<> ret;
	while(read(&c,sizeof(char),1) == 1) {
		if(c != '\r') ret.append(c);
		if(c == '\n') break;
	}
	return ret;
}

int Stream::readLine(char *str,int size) const {
	char c = 0;
	char *s = str;
	while(s - str < size - 1 && read(&c,sizeof(char),1) == 1) {
		if(c != '\r') *s++ = c;
		if(c == '\n') break;
	}
	*s = '\0';
	return (int)(s - str);
}

/*
 */
const StreamString<> Stream::gets() const {
	char c = 0;
	StreamString<> ret;
	while(read(&c,sizeof(char),1) == 1) {
		ret.append(c);
	}
	return ret;
}

/*
 */
int Stream::puts(const char *str) const {
	int length = (int)strlen(str);
	return ((int)write(str,sizeof(char),length) == length);
}

int Stream::printf(const char *format,...) const {
	char buf[4096];
	va_list argptr;
	va_start(argptr,format);
	#ifdef _WIN32
		_vsnprintf(buf,sizeof(buf),format,argptr);
	#else
		vsnprintf(buf,sizeof(buf),format,argptr);
	#endif
	buf[sizeof(buf) - 1] = '\0';
	va_end(argptr);
	return puts(buf);
}
