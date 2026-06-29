/*
*********************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the ActiveQt framework of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
*********************************************************************
*/

/********************************************************************************
** Form generated from reading UI file 'qaxselect.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QAXSELECT_H
#define UI_QAXSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>
#include "ActiveQt/QAxWidget"

QT_BEGIN_NAMESPACE

class Ui_QAxSelect
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLineEdit *filterLineEdit;
    QListView *ActiveXList;
    QHBoxLayout *unnamed;
    QLabel *TextLabel1;
    QLineEdit *ActiveX;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QAxSelect)
    {
        if (QAxSelect->objectName().isEmpty())
            QAxSelect->setObjectName(QStringLiteral("QAxSelect"));
        QAxSelect->setSizeGripEnabled(true);
        horizontalLayout = new QHBoxLayout(QAxSelect);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        filterLineEdit = new QLineEdit(QAxSelect);
        filterLineEdit->setObjectName(QStringLiteral("filterLineEdit"));
        filterLineEdit->setClearButtonEnabled(true);

        verticalLayout->addWidget(filterLineEdit);

        ActiveXList = new QListView(QAxSelect);
        ActiveXList->setObjectName(QStringLiteral("ActiveXList"));

        verticalLayout->addWidget(ActiveXList);

        unnamed = new QHBoxLayout();
        unnamed->setSpacing(6);
        unnamed->setContentsMargins(0, 0, 0, 0);
        unnamed->setObjectName(QStringLiteral("unnamed"));
        TextLabel1 = new QLabel(QAxSelect);
        TextLabel1->setObjectName(QStringLiteral("TextLabel1"));

        unnamed->addWidget(TextLabel1);

        ActiveX = new QLineEdit(QAxSelect);
        ActiveX->setObjectName(QStringLiteral("ActiveX"));

        unnamed->addWidget(ActiveX);


        verticalLayout->addLayout(unnamed);


        horizontalLayout->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(QAxSelect);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);

#ifndef QT_NO_SHORTCUT
        TextLabel1->setBuddy(ActiveX);
#endif // QT_NO_SHORTCUT

        retranslateUi(QAxSelect);

        QMetaObject::connectSlotsByName(QAxSelect);
    } // setupUi

    void retranslateUi(QDialog *QAxSelect)
    {
        QAxSelect->setWindowTitle(QApplication::translate("QAxSelect", "Select ActiveX Control", 0));
        filterLineEdit->setPlaceholderText(QApplication::translate("QAxSelect", "Filter", 0));
        TextLabel1->setText(QApplication::translate("QAxSelect", "COM &Object:", 0));
    } // retranslateUi

};

namespace Ui {
    class QAxSelect: public Ui_QAxSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QAXSELECT_H
