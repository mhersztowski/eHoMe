#ifndef QEHLANGERRORSLIST_H
#define QEHLANGERRORSLIST_H

#include <QTreeWidget>
#include "StudioCore/EHLang/EHLang.h"


class QEHLangErrorsList : public QTreeWidget {

public:
    QEHLangErrorsList() {

    }

    void update(EHLang::EHLang* lang);

};


#endif // QEHLANGERRORSLIST_H
