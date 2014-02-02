/********************************************************************************
** Form generated from reading UI file 'panel_test.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANEL_TEST_H
#define UI_PANEL_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PanelTest
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;

    void setupUi(QWidget *PanelTest)
    {
        if (PanelTest->objectName().isEmpty())
            PanelTest->setObjectName(QStringLiteral("PanelTest"));
        PanelTest->resize(400, 300);
        gridLayout = new QGridLayout(PanelTest);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableView = new QTableView(PanelTest);
        tableView->setObjectName(QStringLiteral("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);


        retranslateUi(PanelTest);

        QMetaObject::connectSlotsByName(PanelTest);
    } // setupUi

    void retranslateUi(QWidget *PanelTest)
    {
        PanelTest->setWindowTitle(QApplication::translate("PanelTest", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class PanelTest: public Ui_PanelTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANEL_TEST_H
