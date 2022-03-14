#ifndef __STRING_H__
#define __STRING_H__

#include "Base.h"

/*
 */
class String;
template <int Capacity = 256> class StringStack;
template <int Capacity = 256> class StringArray;

/*
 */
class String {
		
	public:
		
		String();
		String(const String &s);
		explicit String(const char *s);
		~String();
		
		String &operator=(const char *s);
		String &operator=(const String &s);
		
		String &operator+=(char c);
		String &operator+=(const char *s);
		String &operator+=(const String &s);
		
		INLINE const char *get() const { return data; }
		INLINE operator const char*() const { return data; }
		INLINE operator const void*() const { return data; }
		
		INLINE char &get(int index) {
			assert((unsigned int)index < (unsigned int)length && "String::get(): bad index");
			return data[index];
		}
		INLINE char get(int index) const {
			assert((unsigned int)index < (unsigned int)length && "String::get(): bad index");
			return data[index];
		}
		
		INLINE char &operator[](int index) {
			assert((unsigned int)index < (unsigned int)length && "String::operator[](): bad index");
			return data[index];
		}
		INLINE char operator[](int index) const {
			assert((unsigned int)index < (unsigned int)length && "String::operator[](): bad index");
			return data[index];
		}
		
		INLINE int size() const { return length; }
		INLINE int empty() const { return (length == 0); }
		
		void resize(int size);
		void allocate(int size);
		void reserve(int size);
		
		void clear();
		void destroy();
		
		int find(char c) const;
		int find(const char *s) const;
		int find(const String &s) const;
		
		int rfind(char c) const;
		int rfind(const char *s) const;
		int rfind(const String &s) const;
		
		void copy(const char *s,int len = -1);
		void copy(const String &s,int len = -1);
		
		void append(char c);
		void append(const char *s,int len = -1);
		void append(const String &s,int len = -1);
		
		void append(int pos,char c);
		void append(int pos,const char *s,int len = -1);
		void append(int pos,const String &s,int len = -1);
		
		void remove();
		void remove(int pos,int len = 1);
		
		void lower();
		void upper();
		
		void vprintf(const char *format,va_list argptr);
		void printf(const char *format,...);
		
		int vscanf(const char *format,va_list argptr);
		int scanf(const char *format,...);
		
		static int isdigit(int code);
		static int islower(int code);
		static int isupper(int code);
		static int isalpha(int code);
		
		static int atoi(const char *str);
		static long long atol(const char *str);
		static float atof(const char *str);
		static double atod(const char *str);
		
		static StringStack<> memory(size_t memory);
		
		static StringStack<> lower(const char *str);
		static StringStack<> upper(const char *str);
		
		static StringStack<> format(const char *format,va_list argptr);
		static StringStack<> format(const char *format,...);
		
		static int vsscanf(const char *str,const char *format,va_list argptr);
		static int sscanf(const char *str,const char *format,...);
		
		static StringStack<> trim(const char *str,const char *symbols = 0);
		static StringStack<> replace(const char *str,const char *before,const char *after);
		
		static StringArray<> split(const char *str,const char *delimiters);
		static StringStack<> substr(const char *str,int pos,int len = -1);
		static StringStack<> addslashes(const char *str);
		static StringStack<> stripslashes(const char *str);
		
		static StringStack<> dirname(const char *str);
		static StringStack<> basename(const char *str);
		static StringStack<> pathname(const char *str);
		
		static StringStack<> absname(const char *path,const char *str);
		static StringStack<> relname(const char *path,const char *str);
		static StringStack<> extension(const char *str,const char *ext);
		
		static StringStack<> unicodeToUtf8(const wchar_t *src);
		static StringStack<> unicodeToUtf8(const unsigned int *src);
		static int utf8ToUnicode(const char *src,unsigned int &code);
		static int utf8ToUnicode(const char *src,wchar_t *dest,int size);
		static int utf8ToUnicode(const char *src,unsigned int *dest,int size);
		
		static int utf8strlen(const char *str);
		static StringStack<> utf8substr(const char *str,int pos,int len = -1);
		
		static int match(const char *pattern,const char *str);
		
		static const String null;
		
	protected:
		
		enum {
			CAPACITY = 4,
			DYNAMIC_FLAG = (int)(1 << (sizeof(int) * 8 - 1)),
			CAPACITY_MASK = ~(int)DYNAMIC_FLAG,
		};
		
		void do_copy(const char *s,int len);
		void do_append(int pos,char c);
		void do_append(int pos,const char *s,int len);
		void do_memcpy(char *dest,const char *src,int size);
		
		int length;
		int capacity;
		char *data;
		
		char stack_data[CAPACITY];
};

/*
 */
class StringPtr {
		
	public:
		
		StringPtr();
		StringPtr(const String &s);
		StringPtr(const StringPtr &s);
		explicit StringPtr(const char *s);
		~StringPtr();
		
		StringPtr &operator=(const char *s);
		StringPtr &operator=(const String &s);
		StringPtr &operator=(const StringPtr &s);
		
		StringPtr &operator+=(const char *s);
		StringPtr &operator+=(const String &s);
		
		INLINE const char *get() const { return (string) ? string->get() : String::null; }
		INLINE const String &getString() const { return (string) ? *string : String::null; }
		INLINE operator const char*() const { return (string) ? string->get() : String::null; }
		INLINE operator const void*() const { return (string) ? string->get() : String::null; }
		
		INLINE int size() const { return (string) ? string->size() : 0; }
		INLINE int empty() const { return (string) ? string->empty() : 1; }
		
		INLINE void clear() { delete string; string = 0; }
		
	private:
		
		String *string;
};

/*
 */
template <int Capacity> class StringStack : public String {
		
	public:
		
		StringStack() {
			capacity = Capacity;
			data = stack_data;
			data[length] = '\0';
		}
		StringStack(const String &s) {
			capacity = Capacity;
			data = stack_data;
			copy(s);
		}
		StringStack(const StringStack &s) {
			capacity = Capacity;
			data = stack_data;
			copy(s);
		}
		explicit StringStack(const char *s) {
			capacity = Capacity;
			data = stack_data;
			copy(s);
		}
		~StringStack() { }
		
		StringStack &operator=(const char *s) {
			if(data == s) return *this;
			copy(s);
			return *this;
		}
		StringStack &operator=(const String &s) {
			if(this == &s) return *this;
			copy(s);
			return *this;
		}
		StringStack &operator=(const StringStack &s) {
			if(this == &s) return *this;
			copy(s);
			return *this;
		}
		
	private:
		
		char stack_data[Capacity];
};

/*
 */
template <int Capacity> class StringArray {
		
	private:
		
		friend class String;
		
		StringArray(int size) : data_length(size), data(stack_data), indices_length(0), indices_capacity(Capacity), indices(stack_indices) {
			if(data_length + 1 > Capacity) {
				data = new char[data_length + 1];
			}
			data[data_length] = '\0';
		}
		
		void append(int index) {
			if(indices_length + 1 > indices_capacity) {
				indices_capacity = (indices_length + 1) * 2;
				int *new_indices = new int[indices_capacity];
				for(int i = 0; i < indices_length; i++) {
					new_indices[i] = indices[i];
				}
				if(indices != stack_indices) delete [] indices;
				indices = new_indices;
			}
			indices[indices_length++] = index;
		}
		
	public:
		
		StringArray(const StringArray &s) : data_length(s.data_length), data(stack_data), indices_length(s.indices_length), indices_capacity(Capacity), indices(stack_indices) {
			if(data_length + 1 > Capacity) {
				data = new char[data_length + 1];
			}
			for(int i = 0; i < data_length; i++) {
				data[i] = s.data[i];
			}
			data[data_length] = '\0';
			if(indices_length > indices_capacity) {
				indices_capacity = indices_length;
				indices = new int[indices_capacity];
			}
			for(int i = 0; i < indices_length; i++) {
				indices[i] = s.indices[i];
			}
		}
		~StringArray() {
			if(data != stack_data) delete [] data;
			if(indices != stack_indices) delete [] indices;
		}
		
		INLINE const char *operator[](int index) const {
			assert((unsigned int)index < (unsigned int)indices_length && "StringArray::operator[](): bad index");
			return data + indices[index];
		}
		
		INLINE int size() const { return indices_length; }
		INLINE int empty() const { return (indices_length == 0); }
		
	private:
		
		int data_length;
		char *data;
		char stack_data[Capacity];
		
		int indices_length;
		int indices_capacity;
		int *indices;
		int stack_indices[Capacity];
};

/*
 */
StringStack<> operator+(const String &s0,const String &s1);
StringStack<> operator+(const String &s0,const char *s1);
StringStack<> operator+(const char *s0,const String &s1);
StringStack<> operator+(const String &s0,char c1);
StringStack<> operator+(char c0,const String &s1);

/*
 */
int operator==(const String &s0,const String &s1);
int operator==(const String &s0,const char *s1);
int operator==(const char *s0,const String &s1);

int operator!=(const String &s0,const String &s1);
int operator!=(const String &s0,const char *s1);
int operator!=(const char *s0,const String &s1);

int operator<(const String &s0,const String &s1);
int operator<(const String &s0,const char *s1);
int operator<(const char *s0,const String &s1);

int operator>(const String &s0,const String &s1);
int operator>(const String &s0,const char *s1);
int operator>(const char *s0,const String &s1);

int operator<=(const String &s0,const String &s1);
int operator<=(const String &s0,const char *s1);
int operator<=(const char *s0,const String &s1);

int operator>=(const String &s0,const String &s1);
int operator>=(const String &s0,const char *s1);
int operator>=(const char *s0,const String &s1);

/*
 */
int operator==(const StringPtr &s0,const StringPtr &s1);
int operator==(const StringPtr &s0,const char *s1);
int operator==(const char *s0,const StringPtr &s1);

int operator!=(const StringPtr &s0,const StringPtr &s1);
int operator!=(const StringPtr &s0,const char *s1);
int operator!=(const char *s0,const StringPtr &s1);

#endif /* __STRING_H__ */
