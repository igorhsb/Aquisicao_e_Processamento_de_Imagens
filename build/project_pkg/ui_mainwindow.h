/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QGroupBox *mainGpBox;
    QFrame *viewFrame;
    QLabel *dispositivoLbl;
    QLabel *dispositivoslbl;
    QComboBox *dispositivosCB;
    QFrame *viewFrame_2;
    QTextBrowser *txtBox;
    QLabel *lblConnect;
    QLabel *dispositivoLbl_2;
    QGroupBox *dispositivoGpBox;
    QGroupBox *imuGP;
    QLabel *yawLBL;
    QLabel *pitchSLBL;
    QLabel *yawSLBL;
    QLabel *rollSLBL;
    QLabel *pitchLBL;
    QLabel *rollLBL;
    QComboBox *vModeCB;
    QLabel *vModeLbl;
    QLabel *vModeLbl_2;
    QComboBox *dispositivosCB_2;
    QLabel *dispositivoslbl_2;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QComboBox *vModeCB_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 720);
        MainWindow->setMaximumSize(QSize(1920, 1080));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mainGpBox = new QGroupBox(centralWidget);
        mainGpBox->setObjectName(QString::fromUtf8("mainGpBox"));
        mainGpBox->setGeometry(QRect(0, -11, 1280, 661));
        mainGpBox->setAutoFillBackground(false);
        mainGpBox->setAlignment(Qt::AlignCenter);
        viewFrame = new QFrame(mainGpBox);
        viewFrame->setObjectName(QString::fromUtf8("viewFrame"));
        viewFrame->setGeometry(QRect(10, 150, 630, 391));
        viewFrame->setBaseSize(QSize(640, 480));
        viewFrame->setAutoFillBackground(true);
        viewFrame->setFrameShape(QFrame::StyledPanel);
        viewFrame->setFrameShadow(QFrame::Raised);
        dispositivoLbl = new QLabel(mainGpBox);
        dispositivoLbl->setObjectName(QString::fromUtf8("dispositivoLbl"));
        dispositivoLbl->setGeometry(QRect(10, 130, 371, 20));
        dispositivoslbl = new QLabel(mainGpBox);
        dispositivoslbl->setObjectName(QString::fromUtf8("dispositivoslbl"));
        dispositivoslbl->setGeometry(QRect(10, 30, 81, 20));
        dispositivosCB = new QComboBox(mainGpBox);
        dispositivosCB->setObjectName(QString::fromUtf8("dispositivosCB"));
        dispositivosCB->setGeometry(QRect(10, 60, 161, 31));
        viewFrame_2 = new QFrame(mainGpBox);
        viewFrame_2->setObjectName(QString::fromUtf8("viewFrame_2"));
        viewFrame_2->setGeometry(QRect(650, 150, 620, 391));
        viewFrame_2->setBaseSize(QSize(640, 480));
        viewFrame_2->setAutoFillBackground(true);
        viewFrame_2->setFrameShape(QFrame::StyledPanel);
        viewFrame_2->setFrameShadow(QFrame::Raised);
        txtBox = new QTextBrowser(mainGpBox);
        txtBox->setObjectName(QString::fromUtf8("txtBox"));
        txtBox->setGeometry(QRect(1040, 50, 231, 61));
        lblConnect = new QLabel(mainGpBox);
        lblConnect->setObjectName(QString::fromUtf8("lblConnect"));
        lblConnect->setGeometry(QRect(1040, 20, 231, 20));
        dispositivoLbl_2 = new QLabel(mainGpBox);
        dispositivoLbl_2->setObjectName(QString::fromUtf8("dispositivoLbl_2"));
        dispositivoLbl_2->setGeometry(QRect(650, 130, 371, 20));
        dispositivoGpBox = new QGroupBox(mainGpBox);
        dispositivoGpBox->setObjectName(QString::fromUtf8("dispositivoGpBox"));
        dispositivoGpBox->setGeometry(QRect(10, 545, 271, 111));
        QFont font;
        font.setPointSize(14);
        dispositivoGpBox->setFont(font);
        imuGP = new QGroupBox(dispositivoGpBox);
        imuGP->setObjectName(QString::fromUtf8("imuGP"));
        imuGP->setGeometry(QRect(10, 30, 241, 81));
        yawLBL = new QLabel(imuGP);
        yawLBL->setObjectName(QString::fromUtf8("yawLBL"));
        yawLBL->setGeometry(QRect(10, 60, 121, 20));
        QFont font1;
        font1.setPointSize(11);
        yawLBL->setFont(font1);
        pitchSLBL = new QLabel(imuGP);
        pitchSLBL->setObjectName(QString::fromUtf8("pitchSLBL"));
        pitchSLBL->setEnabled(true);
        pitchSLBL->setGeometry(QRect(150, 30, 66, 20));
        pitchSLBL->setFont(font1);
        yawSLBL = new QLabel(imuGP);
        yawSLBL->setObjectName(QString::fromUtf8("yawSLBL"));
        yawSLBL->setEnabled(true);
        yawSLBL->setGeometry(QRect(150, 60, 66, 20));
        yawSLBL->setFont(font1);
        rollSLBL = new QLabel(imuGP);
        rollSLBL->setObjectName(QString::fromUtf8("rollSLBL"));
        rollSLBL->setEnabled(true);
        rollSLBL->setGeometry(QRect(150, 0, 66, 20));
        rollSLBL->setFont(font1);
        pitchLBL = new QLabel(imuGP);
        pitchLBL->setObjectName(QString::fromUtf8("pitchLBL"));
        pitchLBL->setGeometry(QRect(10, 30, 121, 20));
        pitchLBL->setFont(font1);
        rollLBL = new QLabel(imuGP);
        rollLBL->setObjectName(QString::fromUtf8("rollLBL"));
        rollLBL->setGeometry(QRect(10, 0, 121, 20));
        rollLBL->setFont(font1);
        vModeCB = new QComboBox(mainGpBox);
        vModeCB->setObjectName(QString::fromUtf8("vModeCB"));
        vModeCB->setGeometry(QRect(514, 120, 121, 27));
        vModeLbl = new QLabel(mainGpBox);
        vModeLbl->setObjectName(QString::fromUtf8("vModeLbl"));
        vModeLbl->setGeometry(QRect(350, 130, 151, 20));
        vModeLbl_2 = new QLabel(mainGpBox);
        vModeLbl_2->setObjectName(QString::fromUtf8("vModeLbl_2"));
        vModeLbl_2->setGeometry(QRect(960, 130, 151, 20));
        dispositivosCB_2 = new QComboBox(mainGpBox);
        dispositivosCB_2->setObjectName(QString::fromUtf8("dispositivosCB_2"));
        dispositivosCB_2->setGeometry(QRect(650, 60, 161, 31));
        dispositivoslbl_2 = new QLabel(mainGpBox);
        dispositivoslbl_2->setObjectName(QString::fromUtf8("dispositivoslbl_2"));
        dispositivoslbl_2->setGeometry(QRect(650, 30, 81, 20));
        pushButton = new QPushButton(mainGpBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(330, 60, 131, 27));
        checkBox = new QCheckBox(mainGpBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(200, 60, 111, 22));
        vModeCB_2 = new QComboBox(mainGpBox);
        vModeCB_2->setObjectName(QString::fromUtf8("vModeCB_2"));
        vModeCB_2->setGeometry(QRect(1130, 120, 121, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        mainGpBox->setTitle(QString());
        dispositivoLbl->setText(QApplication::translate("MainWindow", "Dispositivo 1", 0, QApplication::UnicodeUTF8));
        dispositivoslbl->setText(QApplication::translate("MainWindow", "Dispositivos", 0, QApplication::UnicodeUTF8));
        dispositivosCB->clear();
        dispositivosCB->insertItems(0, QStringList()
         << QString()
         << QApplication::translate("MainWindow", "Laser", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "RGBD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "IMU", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Astra", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Zed", 0, QApplication::UnicodeUTF8)
        );
        lblConnect->setText(QApplication::translate("MainWindow", " -------------------------------------", 0, QApplication::UnicodeUTF8));
        dispositivoLbl_2->setText(QApplication::translate("MainWindow", "Dispositivo 2", 0, QApplication::UnicodeUTF8));
        dispositivoGpBox->setTitle(QApplication::translate("MainWindow", "Dados do Sistema", 0, QApplication::UnicodeUTF8));
        imuGP->setTitle(QString());
        yawLBL->setText(QApplication::translate("MainWindow", "Yaw:", 0, QApplication::UnicodeUTF8));
        pitchSLBL->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        yawSLBL->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        rollSLBL->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        pitchLBL->setText(QApplication::translate("MainWindow", "Pitch:", 0, QApplication::UnicodeUTF8));
        rollLBL->setText(QApplication::translate("MainWindow", "Roll:", 0, QApplication::UnicodeUTF8));
        vModeCB->clear();
        vModeCB->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "RGB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Depth", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "RGB/Depth", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Point Clould", 0, QApplication::UnicodeUTF8)
        );
        vModeLbl->setText(QApplication::translate("MainWindow", "Modo de Visualiza\303\247\303\243o", 0, QApplication::UnicodeUTF8));
        vModeLbl_2->setText(QApplication::translate("MainWindow", "Modo de Visualiza\303\247\303\243o", 0, QApplication::UnicodeUTF8));
        dispositivosCB_2->clear();
        dispositivosCB_2->insertItems(0, QStringList()
         << QString()
         << QApplication::translate("MainWindow", "Laser", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "RGBD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "IMU", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Astra", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Zed", 0, QApplication::UnicodeUTF8)
        );
        dispositivoslbl_2->setText(QApplication::translate("MainWindow", "Dispositivos", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Gerar Arquivos", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Sincronizar", 0, QApplication::UnicodeUTF8));
        vModeCB_2->clear();
        vModeCB_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "RGB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Depth", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "RGB/Depth", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Point Clould", 0, QApplication::UnicodeUTF8)
        );
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
