#ifndef FENCLIENT_H
#define FENCLIENT_H


#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include "ui_FenClient.h"


class FenClient : public QWidget, private Ui::FenClient
{
    Q_OBJECT

    public:
        FenClient();

    private slots:
        void on_buttonConnection_clicked();
        void on_buttonSend_clicked();
        void on_message_returnPressed();
        void donneesRecues();
        void connecte();
        void diconnect();
        void socketError(QAbstractSocket::SocketError error);

private:
        QTcpSocket *socket;
        quint16 messageSize;
};
#endif // FENCLIENT_H
