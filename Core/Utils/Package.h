#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "Core/Utils/Singleton.h"
#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"
#include "Core/Utils/Buffer.h"

class Package {
public:

	enum {
		PKG_SEND_CLIENTINFO,
		PKG_RECEIVE_CLIENTINFO,
		PKG_SEND_TYPEINFO,
		PKG_RECEIVE_TYPEINFO,
		PKG_SEND_WORLD,
		PKG_RECEIVE_WORLD,
	};

	Package(const char* from, const char* to, int packageId, Buffer* buffer) {
		this->from = from; this->to = to; this->packageId = packageId; this->buffer = buffer;
	}

	const String& getFrom() { return from;  }
	const String& getTo() { return to; }
	int getId() { return packageId; }
	Buffer* getBuffer() { return buffer; }

private:
	String from;
	String to;
	int packageId;
	Buffer* buffer;
};


#endif /* __PACKAGE_H__ */
