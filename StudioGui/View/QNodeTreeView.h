#ifndef QNODETREEVIEW_H
#define QNODETREEVIEW_H

#include <QTreeWidget>
#include "Core/Types/Node.h"


class QNodeTreeView : public QTreeWidget {
	Q_OBJECT

public:

	void setRootNode(Node* node);

protected:
	Node* root_node;
};

#endif // QNODETREEVIEW_H
