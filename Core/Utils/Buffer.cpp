#include "Buffer.h"

/*
 */
struct Buffer::BufferData {
	int size;
	int capacity;
	unsigned char *data;
	unsigned char *ptr;
};

/*
 */
Buffer::Buffer() {
	data = new BufferData();
	data->size = 0;
	data->capacity = 0;
	data->data = NULL;
	data->ptr = NULL;
}

Buffer::Buffer(Buffer &buffer) {
	data = new BufferData();
	data->size = buffer.data->size;
	data->capacity = buffer.data->capacity;
	data->data = buffer.data->data;
	data->ptr = buffer.data->ptr;
	buffer.data->size = 0;
	buffer.data->capacity = 0;
	buffer.data->data = NULL;
	buffer.data->ptr = NULL;
}

Buffer::~Buffer() {
	delete [] data->data;
	delete data;
}

/*
 */
Buffer &Buffer::operator=(Buffer &buffer) {
	if(this == &buffer) return *this;
	assert(data->data == NULL && "Buffer::operator=(): buffer is not empty");
	BufferData *d = data;
	data = buffer.data;
	buffer.data = d;
	return *this;
}

/*
 */
void Buffer::resize(int size) const {
	allocate(size);
	data->size = size;
}

void Buffer::allocate(int size) const {
	if(size <= data->capacity) return;
	data->capacity = size;
	unsigned char *new_data = new unsigned char[data->capacity];
	memcpy(new_data,data->data,sizeof(unsigned char) * data->size);
	data->ptr = new_data + (data->ptr - data->data);
	delete [] data->data;
	data->data = new_data;
}

void Buffer::reserve(int size) const {
	if(size <= data->capacity) return;
	data->capacity = size * 2 + 1;
	unsigned char *new_data = new unsigned char[data->capacity];
	memcpy(new_data,data->data,sizeof(unsigned char) * data->size);
	data->ptr = new_data + (data->ptr - data->data);
	delete [] data->data;
	data->data = new_data;
}

/*
 */
void Buffer::clear() const {
	data->ptr = data->data;
	data->size = 0;
}

int Buffer::getSize() const {
	return data->size;
}

unsigned char *Buffer::getData() const {
	return data->data;
}

/*
 */
int Buffer::eof() const {
	return (data->ptr - data->data >= data->size);
}

int Buffer::getc() const {
	if(data->ptr - data->data >= data->size) return EOF;
	return *(data->ptr)++;
}

int Buffer::seekSet(int offset) const {
	data->ptr = data->data + offset;
	assert(data->ptr >= data->data && data->ptr <= data->data + data->size && "Buffer::seekSet(): bad offset");
	return 1;
}

int Buffer::seekEnd(int offset) const {
	data->ptr = data->data + data->size - offset;
	assert(data->ptr >= data->data && data->ptr <= data->data + data->size && "Buffer::seekEnd(): bad offset");
	return 1;
}

int Buffer::seekCur(int offset) const {
	data->ptr += offset;
	assert(data->ptr >= data->data && data->ptr <= data->data + data->size && "Buffer::seekCur(): bad offset");
	return 1;
}

int Buffer::tell() const {
	return (int)(data->ptr - data->data);
}

int Buffer::flush() const {
	return 1;
}

/*
 */
int Buffer::decode(const char *src) const {
	int size = (int)strlen(src);
	clear();
	allocate(size);
	unsigned char *d = data->data;
	const unsigned char *s = (const unsigned char*)src;
	static const char base64_values[] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
		-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	};
	while(*s) {
		*d++ = ((base64_values[s[0]] << 2) & 0xfc) | ((base64_values[s[1]] >> 4) & 0x03);
		if(s[2] == '=') break;
		*d++ = ((base64_values[s[1]] & 0x0f) << 4) | ((base64_values[s[2]] >> 2) & 0x0f);
		if(s[3] == '=') break;
		*d++ = ((base64_values[s[2]] & 0x03) << 6) | (base64_values[s[3]] & 0x3f);
		s += 4;
	}
	data->size = (int)(d - data->data);
	return (data->size > 0);
}

const StreamString<> Buffer::encode() const {
	StreamString<> ret;
	int size = data->size;
	const unsigned char *s = data->data;
	static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	while(size > 0) {
		if(size >= 3) {
			ret.append(base64_chars[(s[0] >> 2) & 0x3f]);
			ret.append(base64_chars[((s[0] & 0x03) << 4) | ((s[1] >> 4) & 0x0f)]);
			ret.append(base64_chars[((s[1] & 0x0f) << 2) | ((s[2] >> 6) & 0x03)]);
			ret.append(base64_chars[(s[2]) & 0x3f]);
			size -= 3;
			s += 3;
		} else if(size == 2) {
			ret.append(base64_chars[(s[0] >> 2) & 0x3f]);
			ret.append(base64_chars[((s[0] & 0x03) << 4) | ((s[1] >> 4) & 0x0f)]);
			ret.append(base64_chars[((s[1] & 0x0f) << 2)]);
			ret.append('=');
			break;
		} else {
			ret.append(base64_chars[(s[0] >> 2) & 0x3f]);
			ret.append(base64_chars[(s[0] & 0x03) << 4]);
			ret.append('=');
			ret.append('=');
			break;
		}
	}
	return ret;
}

/*
 */
int Buffer::isOpened() const {
	return 1;
}

/*
 */
size_t Buffer::read(void *ptr,size_t size,size_t nmemb) const {
	size_t bytes = size * nmemb;
	size_t bytes_to_end = data->size - (data->ptr - data->data);
	if(bytes <= bytes_to_end) {
		memcpy(ptr,data->ptr,bytes);
		data->ptr += bytes;
		return nmemb;
	}
	memcpy(ptr,data->ptr,bytes_to_end);
	data->ptr += bytes_to_end;
	return bytes_to_end / size;
}

size_t Buffer::write(const void *ptr,size_t size,size_t nmemb) const {
	size_t bytes = size * nmemb;
	size_t bytes_to_end = data->size - (data->ptr - data->data);
	if(bytes <= bytes_to_end) {
		memcpy(data->ptr,ptr,bytes);
		data->ptr += bytes;
		return nmemb;
	}
	reserve(data->size + (int)(bytes - bytes_to_end));
	resize(data->size + (int)(bytes - bytes_to_end));
	memcpy(data->ptr,ptr,bytes);
	data->ptr += bytes;
	return nmemb;
}
