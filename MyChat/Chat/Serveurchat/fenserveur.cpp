#include "fenserveur.h"

FenServeur::FenServeur()
{
    serverCondition = new QLabel;
    quitButton = new QPushButton("Выйти");
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(serverCondition);
    layout->addWidget(quitButton);

    setLayout(layout);

    setWindowTitle("Chat");

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any,50885))
    {
        serverCondition->setText(" сервер не может быть запущен правильно, потому что :<br />"+server->errorString());

    }
    else
    {
        serverCondition->setText("Сервер был запущен. Порт: <strong>"+ QString::number(server->serverPort()) +
        "</strong>.<br />Клиенты теперь могут подключиться.");
        connect(server, SIGNAL(newConnection()),this,SLOT(newConnection()));

    }

    messageSize =0;

}

void FenServeur::newConnection()
{
    messageOutput("<em>Клиент подключился</em>");

    QTcpSocket *nouveauClient = server->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()),this, SLOT(dataReceived()));
    connect(nouveauClient, SIGNAL(disconnected()),this, SLOT(diconnectionClient()));

}
void FenServeur::dataReceived()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket ==0)
    {
        return;
    }
    QDataStream in(socket);
    if (messageSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }
        in >> messageSize;
    }
    if (socket->bytesAvailable()< messageSize)
    {
        return;
    }
    QString message;
    in >>message;

    messageOutput(message);
    messageSize =0;

}
void FenServeur::diconnectionClient()
{
    messageOutput("<em>Клиент отключился</em>");

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
    {
        return;
    }
    clients.removeOne(socket);
    socket->deleteLater();
}
void FenServeur::messageOutput(const QString &message)
{

    QByteArray  packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out <<(quint16)0;
    out <<message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    for (int i=0; i < clients.size();i++)
    {
        clients[i]->write(packet);
    }
}
