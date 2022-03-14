#ifndef __PROPERTY_TYPE_H__
#define __PROPERTY_TYPE_H__

#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"


class PropertyType {

public:

    enum Kind {
        BOOL,
        INT,
        FLOAT,
        STRING,
        NODE
    };

    enum Direction {
        IN,
        OUT,
        INOUT,
        FIELD
    };

    PropertyType(const char* name, Kind type, Direction dir) {
        this->name = name;
        this->kind = type;
        this->dir = dir;
    }

	const String& getName() { return name; }
    Kind getType() { return kind; }
    Direction getDirection() { return dir; }

private:
	String name;
    Kind kind;
    Direction dir;

};



#endif /* __PROPERTY_TYPE_H__ */
