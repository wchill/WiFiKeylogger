/********************************************************************************
** Form generated from reading UI file 'multiinput.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIINPUT_H
#define UI_MULTIINPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MultiInput
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *hostname;
    QSpinBox *port;
    QPushButton *connectButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *keysPressed;
    QTextEdit *keyEventLog;
    QLabel *networkStatus;

    void setupUi(QWidget *MultiInput)
    {
        if (MultiInput->objectName().isEmpty())
            MultiInput->setObjectName(QStringLiteral("MultiInput"));
        MultiInput->resize(400, 300);
        verticalLayout = new QVBoxLayout(MultiInput);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(MultiInput);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        hostname = new QLineEdit(MultiInput);
        hostname->setObjectName(QStringLiteral("hostname"));
        hostname->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(hostname);

        port = new QSpinBox(MultiInput);
        port->setObjectName(QStringLiteral("port"));
        port->setMinimum(1);
        port->setMaximum(65535);
        port->setValue(8000);

        horizontalLayout_4->addWidget(port);

        connectButton = new QPushButton(MultiInput);
        connectButton->setObjectName(QStringLiteral("connectButton"));

        horizontalLayout_4->addWidget(connectButton);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(MultiInput);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        keysPressed = new QLineEdit(MultiInput);
        keysPressed->setObjectName(QStringLiteral("keysPressed"));
        keysPressed->setReadOnly(true);

        horizontalLayout->addWidget(keysPressed);


        verticalLayout->addLayout(horizontalLayout);

        keyEventLog = new QTextEdit(MultiInput);
        keyEventLog->setObjectName(QStringLiteral("keyEventLog"));
        keyEventLog->setReadOnly(true);

        verticalLayout->addWidget(keyEventLog);

        networkStatus = new QLabel(MultiInput);
        networkStatus->setObjectName(QStringLiteral("networkStatus"));

        verticalLayout->addWidget(networkStatus);


        retranslateUi(MultiInput);

        QMetaObject::connectSlotsByName(MultiInput);
    } // setupUi

    void retranslateUi(QWidget *MultiInput)
    {
        MultiInput->setWindowTitle(QApplication::translate("MultiInput", "MultiInput", 0));
        label_2->setText(QApplication::translate("MultiInput", "Host : Port", 0));
        hostname->setText(QApplication::translate("MultiInput", "localhost", 0));
        connectButton->setText(QApplication::translate("MultiInput", "Connect", 0));
        label->setText(QApplication::translate("MultiInput", "Keys Pressed", 0));
        networkStatus->setText(QApplication::translate("MultiInput", "Not Connected", 0));
    } // retranslateUi

};

namespace Ui {
    class MultiInput: public Ui_MultiInput {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIINPUT_H
