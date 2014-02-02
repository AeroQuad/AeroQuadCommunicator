/********************************************************************************
** Form generated from reading UI file 'menu_main.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_MAIN_H
#define UI_MENU_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuMain
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *MenuMain)
    {
        if (MenuMain->objectName().isEmpty())
            MenuMain->setObjectName(QStringLiteral("MenuMain"));
        MenuMain->resize(486, 287);
        gridLayout = new QGridLayout(MenuMain);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(MenuMain);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font;
        font.setPointSize(21);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 4, 1, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 0, 1, 1);

        label = new QLabel(MenuMain);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(500, 200));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/resources/AQ.png")));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 3, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 3, 3, 1, 1);


        retranslateUi(MenuMain);

        QMetaObject::connectSlotsByName(MenuMain);
    } // setupUi

    void retranslateUi(QWidget *MenuMain)
    {
        MenuMain->setWindowTitle(QApplication::translate("MenuMain", "Form", 0));
        label_2->setText(QApplication::translate("MenuMain", "Under Construction", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MenuMain: public Ui_MenuMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_MAIN_H
