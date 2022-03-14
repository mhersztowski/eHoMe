#ifndef __STREAM_H__
#define __STREAM_H__

#include "Core/Types/Base.h"

/*
 */
#ifdef USE_DOUBLE
	#define readScalar			readDouble
	#define writeScalar			writeDouble
	#define readScalarArray		readDoubleArray
	#define writeScalarArray	writeDoubleArray
#else
	#define readScalar			readFloat
	#define writeScalar			writeFloat
	#define readScalarArray		readFloatArray
	#define writeScalarArray	writeFloatArray
#endif

/*
 */
template <int Capacity = 1024> class StreamString;

/*
 */
class Stream {
		
	public:
		
		// Stream
		virtual ~Stream() { }
		
		// stream status
		virtual int isOpened() const = 0;
		
		// stream read/write
		virtual size_t read(void *ptr,size_t size,size_t nmemb) const = 0;
		virtual size_t write(const void *ptr,size_t size,size_t nmemb) const = 0;
		
		// stream
		size_t readStream(const Stream *dest,size_t size,size_t nmemb) const;
		size_t writeStream(const Stream *src,size_t size,size_t nmemb) const;
		
		// bytes
		char readChar() const;
		int writeChar(char value) const;
		unsigned char readUChar() const;
		int writeUChar(unsigned char value) const;
		
		// short
		short readShortBig() const;
		int writeShortBig(short value) const;
		int readShortArrayBig(short *dest,int size) const;
		int writeShortArrayBig(const short *src,int size) const;
		
		short readShortLittle() const;
		int writeShortLittle(short value) const;
		int readShortArrayLittle(short *dest,int size) const;
		int writeShortArrayLittle(const short *src,int size) const;
		
		INLINE short readShort() const { return readShortLittle(); }
		INLINE int writeShort(short value) const { return writeShortLittle(value); }
		INLINE int readShortArray(short *dest,int size) const { return readShortArrayLittle(dest,size); }
		INLINE int writeShortArray(const short *src,int size) const { return writeShortArrayLittle(src,size); }
		
		// unsigned short
		unsigned short readUShortBig() const;
		int writeUShortBig(unsigned short value) const;
		int readUShortArrayBig(unsigned short *dest,int size) const;
		int writeUShortArrayBig(const unsigned short *src,int size) const;
		
		unsigned short readUShortLittle() const;
		int writeUShortLittle(unsigned short value) const;
		int readUShortArrayLittle(unsigned short *dest,int size) const;
		int writeUShortArrayLittle(const unsigned short *src,int size) const;
		
		INLINE unsigned short readUShort() const { return readUShortLittle(); }
		INLINE int writeUShort(unsigned short value) const { return writeUShortLittle(value); }
		INLINE int readUShortArray(unsigned short *dest,int size) const { return readUShortArrayLittle(dest,size); }
		INLINE int writeUShortArray(const unsigned short *src,int size) const { return writeUShortArrayLittle(src,size); }
		
		// int
		int readIntBig() const;
		int writeIntBig(int value) const;
		int readIntArrayBig(int *dest,int size) const;
		int writeIntArrayBig(const int *src,int size) const;
		
		int readIntLittle() const;
		int writeIntLittle(int value) const;
		int readIntArrayLittle(int *dest,int size) const;
		int writeIntArrayLittle(const int *src,int size) const;
		
		INLINE int readInt() const { return readIntLittle(); }
		INLINE int writeInt(int value) const { return writeIntLittle(value); }
		INLINE int readIntArray(int *dest,int size) const { return readIntArrayLittle(dest,size); }
		INLINE int writeIntArray(const int *src,int size) const { return writeIntArrayLittle(src,size); }
		
		// unsigned int
		unsigned int readUIntBig() const;
		int writeUIntBig(unsigned int value) const;
		int readUIntArrayBig(unsigned int *dest,int size) const;
		int writeUIntArrayBig(const unsigned int *src,int size) const;
		
		unsigned int readUIntLittle() const;
		int writeUIntLittle(unsigned int value) const;
		int readUIntArrayLittle(unsigned int *dest,int size) const;
		int writeUIntArrayLittle(const unsigned int *src,int size) const;
		
		INLINE unsigned int readUInt() const { return readUIntLittle(); }
		INLINE int writeUInt(unsigned int value) const { return writeUIntLittle(value); }
		INLINE int readUIntArray(unsigned int *dest,int size) const { return readUIntArrayLittle(dest,size); }
		INLINE int writeUIntArray(const unsigned int *src,int size) const { return writeUIntArrayLittle(src,size); }
		
		// long long
		long long readLongBig() const;
		int writeLongBig(long long value) const;
		int readLongArrayBig(long long *dest,int size) const;
		int writeLongArrayBig(const long long *src,int size) const;
		
		long long readLongLittle() const;
		int writeLongLittle(long long value) const;
		int readLongArrayLittle(long long *dest,int size) const;
		int writeLongArrayLittle(const long long *src,int size) const;
		
		INLINE long long readLong() const { return readLongLittle(); }
		INLINE int writeLong(long long value) const { return writeLongLittle(value); }
		INLINE int readLongArray(long long *dest,int size) const { return readLongArrayLittle(dest,size); }
		INLINE int writeLongArray(const long long *src,int size) const { return writeLongArrayLittle(src,size); }
		
		// float
		float readFloatBig() const;
		int writeFloatBig(float value) const;
		int readFloatArrayBig(float *dest,int size) const;
		int writeFloatArrayBig(const float *src,int size) const;
		
		float readFloatLittle() const;
		int writeFloatLittle(float value) const;
		int readFloatArrayLittle(float *dest,int size) const;
		int writeFloatArrayLittle(const float *src,int size) const;
		
		INLINE float readFloat() const { return readFloatLittle(); }
		INLINE int writeFloat(float value) const { return writeFloatLittle(value); }
		INLINE int readFloatArray(float *dest,int size) const { return readFloatArrayLittle(dest,size); }
		INLINE int writeFloatArray(const float *src,int size) const { return writeFloatArrayLittle(src,size); }
		
		// double
		double readDoubleBig() const;
		int writeDoubleBig(double value) const;
		int readDoubleArrayBig(double *dest,int size) const;
		int writeDoubleArrayBig(const double *src,int size) const;
		
		double readDoubleLittle() const;
		int writeDoubleLittle(double value) const;
		int readDoubleArrayLittle(double *dest,int size) const;
		int writeDoubleArrayLittle(const double *src,int size) const;
		
		INLINE double readDouble() const { return readDoubleLittle(); }
		INLINE int writeDouble(double value) const { return writeDoubleLittle(value); }
		INLINE int readDoubleArray(double *dest,int size) const { return readDoubleArrayLittle(dest,size); }
		INLINE int writeDoubleArray(const double *src,int size) const { return writeDoubleArrayLittle(src,size); }
		
		// binary strings
		const StreamString<> readString() const;
		int readString(char *str,int size) const;
		int writeString(const char *str) const;
		
		// strings
		const StreamString<> readToken() const;
		int readToken(char *str,int size) const;
		const StreamString<> readLine() const;
		int readLine(char *str,int size) const;
		const StreamString<> gets() const;
		int puts(const char *str) const;
		int printf(const char *format,...) const;
};

/*
 */
template <int Capacity> class StreamString {
		
	private:
		
		friend class Stream;
		friend class Buffer;
		
		StreamString() : length(0), capacity(Capacity), data(stack_data) {
			data[length] = '\0';
		}
		StreamString(int length) : length(length), capacity(Capacity), data(stack_data) {
			if(length + 1 > capacity) {
				capacity = length + 1;
				data = new char[capacity];
			}	
			data[length] = '\0';
		}
		
		void append(char c) {
			if(length + 2 > capacity) {
				capacity = (length + 2) * 2;
				char *new_data = new char[capacity];
				for(int i = 0; i < length; i++) {
					new_data[i] = data[i];
				}
				if(data != stack_data) delete [] data;
				data = new_data;
			}
			data[length++] = c;
			data[length] = '\0';
		}
		
	public:
		
		StreamString(const StreamString &s) : length(s.length), capacity(Capacity), data(stack_data) {
			if(length + 1 > capacity) {
				capacity = length + 1;
				data = new char[capacity];
			}
			for(int i = 0; i < length; i++) {
				data[i] = s.data[i];
			}
			data[length] = '\0';
		}
		~StreamString() {
			if(data != stack_data) delete [] data;
		}
		
		INLINE const char *get() const { return data; }
		INLINE operator const char*() const { return data; }
		INLINE operator const void*() const { return data; }
		
		INLINE char get(int index) const {
			assert((unsigned int)index < (unsigned int)length && "StreamString::get(): bad index");
			return data[index];
		}
		
		INLINE char operator[](int index) const {
			assert((unsigned int)index < (unsigned int)length && "StreamString::operator[](): bad index");
			return data[index];
		}
		
		INLINE int size() const { return length; }
		INLINE int empty() const { return (length == 0); }
		
	private:
		
		int length;
		int capacity;
		char *data;
		char stack_data[Capacity];
};

#endif /* __STREAM_H__ */
