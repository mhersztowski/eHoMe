#include "Core/Utils/Route.h"

NodePos* Route::getNodePos(Node* node) {
	for (int i = 0; i < node_pos_list.size(); i++) {
		if (node_pos_list[i]->node == node) {
			return node_pos_list[i];
		}
	}
	return NULL;
}

void Route::addNodePos(Node* node, int pos_x, int pos_y) {
	NodePos* node1 = getNodePos(node);

	if (node1 != NULL)
		return;

	node1 = new NodePos();
	node1->node = node;
	node1->pos_x = pos_x;
	node1->pos_y = pos_y;

	node_pos_list.append(node1);
}

void Route::removeNodePos(Node* node) {

	for (int i = route_pos_list.size() -1; i >= 0; i--) {
		RoutePos* rpos = route_pos_list[i];
		if (rpos->from->getOwner() == node || rpos->to->getOwner() == node) {
			route_pos_list.remove(i, 1);
		}
	}

	for (int i = 0; i < node_pos_list.size(); i++) {
		if (node_pos_list[i]->node == node) {
			node_pos_list.remove(i, 1);
			break;
		}
	}

}

RoutePos* Route::getRoutePos(Property* from, Property* to) {
	for (int i = 0; i < route_pos_list.size(); i++) {
		if (route_pos_list[i]->from == from && route_pos_list[i]->to == to) {
			return route_pos_list[i];
		}
	}
	return NULL;
}

void Route::addRoutePos(Property* from, Property* to, int pos_x, int pos_y) {
	RoutePos* rpos = getRoutePos(from, to);

	if (rpos != NULL)
		return;

	rpos = new RoutePos();
	rpos->from = from;
	rpos->to = to;
	rpos->pos_x = pos_x;
	rpos->pos_y = pos_y;
	route_pos_list.append(rpos);
}

void Route::removeRoutePos(Property* from, Property* to) {
	for (int i = 0; i < route_pos_list.size(); i++) {
		if (route_pos_list[i]->from == from && route_pos_list[i]->to == to) {
			route_pos_list.remove(i, 1);
			break;
		}
	}
}

