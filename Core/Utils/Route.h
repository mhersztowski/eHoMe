#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "Core/Utils/Singleton.h"
#include "Core/Types/Str.h"
#include "Core/Types/Vector.h"

#include "Core/Types/Node.h"
#include "Core/Types/Property.h"


class NodePos {
public:
	Node* node;
	int pos_x;
	int pos_y;
};

class RoutePos {
public:
	Property* from;
	Property* to;
	int pos_x;
	int pos_y;
};

class Route {

public:

	NodePos* getNodePos(Node* node);
	void addNodePos(Node* node, int pos_x, int pos_y);
	void removeNodePos(Node* node);

	RoutePos* getRoutePos(Property* from, Property* to);
	void addRoutePos(Property* from, Property* to, int pos_x, int pos_y);
	void removeRoutePos(Property* from, Property* to);

protected:
	Vector<NodePos*> node_pos_list;
	Vector<RoutePos*> route_pos_list;
};


#endif /* __ROUTE_H__ */
