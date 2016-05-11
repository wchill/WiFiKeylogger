#include "multiinput.h"
#include "ui_multiinput.h"
#include <QtNetwork>
#include <QString>
#include <iostream>
#include <fstream>
#include <Qfile>
#include <QTextStream>
using namespace std;


MultiInput::MultiInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiInput),
    networkSession(0)
{

    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readStream()));
    //connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    //        this, SLOT(displayError(QAbstractSocket::SocketError)));

    // WASDFER12345 Ctrl Tab Esc Space
    keyRemap[0x57] = 0x11;
    keyRemap[0x41] = 0x1E;
    keyRemap[0x53] = 0x1F;
    keyRemap[0x44] = 0x20;
    keyRemap[0x46] = 0x21;
    keyRemap[0x45] = 0x12;
    keyRemap[0x52] = 0x13;
    keyRemap[0x31] = 0x02;
    keyRemap[0x32] = 0x03;
    keyRemap[0x33] = 0x04;
    keyRemap[0x34] = 0x05;
    keyRemap[0x35] = 0x06;

    // keyRemap[0x01000022] = 0x1D;
    keyRemap[0x01000021] = 0x1D;

    keyRemap[0x01000001] = 0x0F;
    keyRemap[0x01000000] = 0x01;
    keyRemap[0x20] = 0x39;
}

int MultiInput::QtToDirectInput(qint64 key) {
    auto it = keyRemap.find(key);
    if (it == keyRemap.end()) {
        return -1;
    } else {
        return it->second;
    }
}

void MultiInput::socketConnected()
{
    ui->keyEventLog->append("Connected to host");
    tcpSocket->write("hello\r\n");
    enableDisconnect();
}

void MultiInput::displayError(QAbstractSocket::SocketError socketError)
{
    ui->keyEventLog->append("Connection error.");
    tcpSocket->close();
    enableConnect();
}

void MultiInput::updatePressedKeys(QKeyEvent *event)
{
    QString str = "";
    for(auto it = keys.begin(); it != keys.end(); it++) {
        if((*it).second)
            str += QKeySequence((*it).first).toString() + " ";
            //str += "0x" + QString::number((*it).first, 16) + " ";
    }
    ui->keysPressed->setText(str);
}
/*
QFile* MultiInput::openFile()
{
    QFile *file = new QFile("C:/logfile.txt");
    return file;
}
*/
void MultiInput::readStream()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    PACKET packetbuf[30000];
    PACKET packet;
    qint64 index = 0;
    while(tcpSocket->bytesAvailable() >= 2) {
        char ch;
        int bytesRead = tcpSocket->read(&ch, sizeof(ch));
        if (bytesRead == sizeof(ch)) {
            packet.key = ch;
        }
        bytesRead = tcpSocket->read(&ch, sizeof(ch));
        if (bytesRead == sizeof(ch)) {
            packet.press_rel = ch;
        }
        packet.time = QDateTime::currentDateTime();
        packetbuf[index] = packet;
        index++;
    }
    writetoFile(packetbuf, index);
    index = 0;
}

MultiInput::~MultiInput()
{
    delete ui;
}

void MultiInput::writetoFile(PACKET *buf, int index)
{
        //open file
        QString outputFilename = "C:/QT/Results1.txt";
        QFile outputFile(outputFilename);
        if (outputFile.exists())
            outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
        else
            outputFile.open(QIODevice::WriteOnly);

    //    if(!outputFile.isOpen()){
    //        qDebug() << "- Error, unable to open" << outputFilename << "for output";
    //    }

        //make file stream
        QTextStream outStream(&outputFile);
        //QString pr;
        QDateTime time;
        for(int i=0; i<index-1; i++)
        {
            qDebug() << buf[i].key;
            outStream << buf[i].key << "  ";
            time = buf[i].time;
            if(buf[i].press_rel == '0'){
                outStream << "release" << "  " << time.toString() << endl;
                qDebug() << "release" << "  " << time.toString() << endl;
            }
            else
            {
                outStream << "press" << "  " << time.toString() << endl;
                qDebug() << "press" << "  " << time.toString() << endl;
            }

            //qDebug() << pr << time.toString();
            //outStream << pr << "  " << time.toString() << endl;
        }

        outputFile.close();
}

void MultiInput::keyPressEvent(QKeyEvent *event)
{
    keys[event->key()] = true;
    updatePressedKeys(event);
    if(tcpSocket->isOpen()) {
        int k = QtToDirectInput(event->key());
        if(k != -1) {
            char buf[32];
            buf[0] = '+';
            buf[1] = (char) k;
            buf[2] = '\r';
            buf[3] = '\n';
            buf[4] = 0;
            tcpSocket->write(buf);
        }
    }
}

void MultiInput::keyReleaseEvent(QKeyEvent *event)
{
    keys[event->key()] = false;
    updatePressedKeys(event);
    if(tcpSocket->isOpen()) {
        int k = QtToDirectInput(event->key());
        if(k != -1) {
            char buf[32];
            buf[0] = '-';
            buf[1] = (char) k;
            buf[2] = '\r';
            buf[3] = '\n';
            buf[4] = 0;
            tcpSocket->write(buf);
        }
    }
}

void MultiInput::on_connectButton_clicked()
{
    if(!tcpSocket->isOpen()) {
        ui->connectButton->setEnabled(false);
        tcpSocket->connectToHost(ui->hostname->text(), ui->port->value());
    } else {
        tcpSocket->close();
        ui->keyEventLog->append("Disconnected from host.");
        enableConnect();
    }
}

void MultiInput::enableConnect()
{
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText("Connect");
    ui->networkStatus->setText("Not Connected");
    ui->hostname->setEnabled(true);
    ui->port->setEnabled(true);
}

void MultiInput::enableDisconnect()
{
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText("Disconnect");
    ui->networkStatus->setText("Connected to " + ui->hostname->text() + ":" + ui->port->text());
    ui->hostname->setEnabled(false);
    ui->port->setEnabled(false);
}

