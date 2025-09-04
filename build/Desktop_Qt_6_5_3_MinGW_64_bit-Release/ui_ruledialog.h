/********************************************************************************
** Form generated from reading UI file 'ruledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RULEDIALOG_H
#define UI_RULEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_RuleDialog
{
public:

    void setupUi(QDialog *RuleDialog)
    {
        if (RuleDialog->objectName().isEmpty())
            RuleDialog->setObjectName("RuleDialog");
        RuleDialog->resize(400, 300);

        retranslateUi(RuleDialog);

        QMetaObject::connectSlotsByName(RuleDialog);
    } // setupUi

    void retranslateUi(QDialog *RuleDialog)
    {
        RuleDialog->setWindowTitle(QCoreApplication::translate("RuleDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RuleDialog: public Ui_RuleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RULEDIALOG_H
