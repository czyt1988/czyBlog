/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "SAChart2D.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    SAChart2D *chart;
    QGridLayout *gridLayout;
    QPushButton *pushButton_1Simple;
    QCheckBox *checkBox_legend;
    QPushButton *pushButton_legendBottom;
    QPushButton *pushButton_legendTop;
    QPushButton *pushButton_legendLeft;
    QPushButton *pushButton_legendRight;
    QPushButton *pushButton_2axis;
    QComboBox *comboBox_theme;
    QPushButton *pushButton_addBar;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_effect;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(662, 300);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        chart = new SAChart2D(Widget);
        chart->setObjectName(QStringLiteral("chart"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chart->sizePolicy().hasHeightForWidth());
        chart->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(chart);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_1Simple = new QPushButton(Widget);
        pushButton_1Simple->setObjectName(QStringLiteral("pushButton_1Simple"));

        gridLayout->addWidget(pushButton_1Simple, 1, 0, 1, 1);

        checkBox_legend = new QCheckBox(Widget);
        checkBox_legend->setObjectName(QStringLiteral("checkBox_legend"));

        gridLayout->addWidget(checkBox_legend, 1, 1, 1, 1);

        pushButton_legendBottom = new QPushButton(Widget);
        pushButton_legendBottom->setObjectName(QStringLiteral("pushButton_legendBottom"));

        gridLayout->addWidget(pushButton_legendBottom, 1, 3, 1, 1);

        pushButton_legendTop = new QPushButton(Widget);
        pushButton_legendTop->setObjectName(QStringLiteral("pushButton_legendTop"));

        gridLayout->addWidget(pushButton_legendTop, 1, 2, 1, 1);

        pushButton_legendLeft = new QPushButton(Widget);
        pushButton_legendLeft->setObjectName(QStringLiteral("pushButton_legendLeft"));

        gridLayout->addWidget(pushButton_legendLeft, 1, 4, 1, 1);

        pushButton_legendRight = new QPushButton(Widget);
        pushButton_legendRight->setObjectName(QStringLiteral("pushButton_legendRight"));

        gridLayout->addWidget(pushButton_legendRight, 1, 5, 1, 1);

        pushButton_2axis = new QPushButton(Widget);
        pushButton_2axis->setObjectName(QStringLiteral("pushButton_2axis"));

        gridLayout->addWidget(pushButton_2axis, 2, 1, 1, 1);

        comboBox_theme = new QComboBox(Widget);
        comboBox_theme->setObjectName(QStringLiteral("comboBox_theme"));

        gridLayout->addWidget(comboBox_theme, 2, 0, 1, 1);

        pushButton_addBar = new QPushButton(Widget);
        pushButton_addBar->setObjectName(QStringLiteral("pushButton_addBar"));

        gridLayout->addWidget(pushButton_addBar, 2, 2, 1, 1);

        pushButton_clear = new QPushButton(Widget);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));

        gridLayout->addWidget(pushButton_clear, 2, 3, 1, 1);

        pushButton_effect = new QPushButton(Widget);
        pushButton_effect->setObjectName(QStringLiteral("pushButton_effect"));

        gridLayout->addWidget(pushButton_effect, 2, 4, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        pushButton_1Simple->setText(QApplication::translate("Widget", "1.\347\256\200\345\215\225\347\232\204\346\255\243\345\274\246\346\233\262\347\272\277\347\273\230\345\210\266", 0));
        checkBox_legend->setText(QApplication::translate("Widget", "\345\233\276\344\276\213", 0));
        pushButton_legendBottom->setText(QApplication::translate("Widget", "\344\270\213", 0));
        pushButton_legendTop->setText(QApplication::translate("Widget", "\344\270\212", 0));
        pushButton_legendLeft->setText(QApplication::translate("Widget", "\345\267\246", 0));
        pushButton_legendRight->setText(QApplication::translate("Widget", "\345\217\263", 0));
        pushButton_2axis->setText(QApplication::translate("Widget", "\346\267\273\345\212\240\345\217\246\345\244\226\345\235\220\346\240\207\347\263\273", 0));
        pushButton_addBar->setText(QApplication::translate("Widget", "\346\267\273\345\212\240bar", 0));
        pushButton_clear->setText(QApplication::translate("Widget", "\346\270\205\351\231\244\346\211\200\346\234\211\345\233\276\350\241\250", 0));
        pushButton_effect->setText(QApplication::translate("Widget", "\346\200\247\350\203\275\346\265\213\350\257\225", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
