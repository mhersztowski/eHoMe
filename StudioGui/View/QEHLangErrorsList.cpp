#include "StudioGui/View/QEHLangErrorsList.h"

#include <QTreeWidgetItem>

void QEHLangErrorsList::update(EHLang::EHLang* lang) {
    this->clear();

    this->setColumnCount(3);

    for (int i =0; i < lang->parser->errors->errors.size(); i++) {
        EHLang::Errors::ErrorItem* item =  lang->parser->errors->errors[i];

        QTreeWidgetItem* titem = new QTreeWidgetItem();
        QString msg = QString::fromWCharArray(item->msg);
        titem->setText(0, msg);
        this->addTopLevelItem(titem);

    }

}
