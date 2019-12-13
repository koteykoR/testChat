#ifndef FENSERVEUR_H
#define FENSERVEUR_H

#include <QWidget>
#include <QtNetwork>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>

class FenServeur : public QWidget
{
    Q_OBJECT

public:
    FenServeur();
    void messageOutput(const QString &message);
private slots:
    void newConnection();
    void dataReceived();
    void diconnectionClient();

private:
    QLabel *serverCondition;
    QPushButton *quitButton;

    QTcpServer *server;
    QList<QTcpSocket *> clients;
    quint16 messageSize;
};





#endif // FENSERVEUR_H
