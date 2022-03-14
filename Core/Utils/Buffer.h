#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "Stream.h"

/*
 */
#ifdef getc
	#undef getc
#endif

/*
 */
class Buffer : public Stream {
		
	public:
		
		Buffer();
		Buffer(Buffer &buffer);
		virtual ~Buffer();
		
		Buffer &operator=(Buffer &buffer);
		
		// resize buffer
		void resize(int size) const;
		
		// allocate buffer
		void allocate(int size) const;
		
		// reseve buffer
		void reserve(int size) const;
		
		// clear buffer
		void clear() const;
		
		// buffer size
		int getSize() const;
		
		// buffer data
		unsigned char *getData() const;
		
		// stdio
		int eof() const;
		int getc() const;
		int seekSet(int offset) const;
		int seekEnd(int offset) const;
		int seekCur(int offset) const;
		int tell() const;
		int flush() const;
		
		// decode buffer
		int decode(const char *src) const;
		const StreamString<> encode() const;
		
		// stream status
		virtual int isOpened() const;
		
		// stream read/write
		virtual size_t read(void *ptr,size_t size,size_t nmemb) const;
		virtual size_t write(const void *ptr,size_t size,size_t nmemb) const;
		
	private:
		
		struct BufferData;	// buffer implementation
		BufferData *data;
};

#endif /* __BUFFER_H__ */
