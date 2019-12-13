#include "fenclient.h"

FenClient::FenClient()
{
    setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(donneesRecues()));
    connect(socket,SIGNAL(connected()),this,SLOT(connecte()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(diconnect()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));

    messageSize =0;

}
void FenClient::on_buttonConnection_clicked()
{
    messagesList->append("<em> Идет попытка подключения ...</em>");
    buttonConnection->setEnabled(false);
    socket->abort();
    socket->connectToHost(serverIP->text(),serverPort->value());
}
void FenClient::on_buttonSend_clicked()
{
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);
        // готовим пакет
        QString messageAEnvoyer = tr("<strong>") + pseudo->text() +tr("</strong> : ") + message->text();
        out << (quint16) 0;
        out << messageAEnvoyer;
        out.device()->seek(0);
        out << (quint16) (packet.size() - sizeof(quint16));
        socket->write(packet); //отправляем пакет
        message->clear(); // Очистить область написания сообщения
        message->setFocus(); // помещаем курсор обратно внутрь

}

void FenClient::on_message_returnPressed()  // слот срабатывает, если нажат enter
{
    on_buttonSend_clicked();
}
// Мы получили пакет
void FenClient::donneesRecues()
{
    /* Тот же принцип, что и при получении пакета сервером:
        Мы пытаемся восстановить размер сообщения
        Как только мы его получим, мы ждем, чтобы получить все сообщение (в зависимости от объявленного размера sizeMessage)
        */
    QDataStream in(socket);

    if (messageSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> messageSize;
    }

    if (socket->bytesAvailable() < messageSize)
        return;

    QString messageRecu;
    in >> messageRecu;

        // отобразить сообщение в чате
    messagesList->append(messageRecu);

    //  Размер сообщения сбрасывается на 0 для получения будущих сообщений
    messageSize = 0;
}
// Этот слот вызывается при успешном соединении с сервером
void FenClient::connecte()
{
    messagesList->append(tr("<em>Успешное соединение !</em>"));
    buttonConnection->setEnabled(true);
}
// Этот слот вызывается при отключении от сервера
void FenClient::diconnect()
{
    messagesList->append(tr("<em>Отключен от сервера</em>"));
}
// Этот слот вызывается при возникновении ошибки
void FenClient::socketError(QAbstractSocket::SocketError error)
{
    switch(error) // Выводим сообщения в зависимости от сообщенной нам ошибки.
    {
        case QAbstractSocket::HostNotFoundError:
            messagesList->append(tr("<em>ОШИБКА: сервер не найден. Проверьте IP и порт.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            messagesList->append(tr("<em>ОШИБКА: сервер отклонил соединение. Проверьте, была ли запущена программа «сервер». Также проверьте IP и порт.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            messagesList->append(tr("<em> ОШИБКА: сервер разорвал соединение.</em>"));
            break;
        default:
            messagesList->append(tr("<em>Ошибка : ") + socket->errorString() + tr("</em>"));
    }

    buttonConnection->setEnabled(true);
}
