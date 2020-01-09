#ifndef Q_IRCBOT_H
#define Q_IRCBOT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <iostream>
#include <QtCore/QCoreApplication>
#include <QStringList>
#include <iostream>
#include <memory>

class Q_IRCBot : public QObject {

    Q_OBJECT
    QString server;
    size_t port;
    QString username;
    QString realname;
    QString password;
    QString nickname;
    QString first_channel;
    mutable QStringList  channelList;
    std::unique_ptr<QTcpSocket> ircBotSocket;

public:
    explicit Q_IRCBot(QObject* parent = nullptr);

    Q_IRCBot          (QString const &server, int port,
                       QString const &username, QString const &realname,
                       QString const &password, QString const &nickname,
                       QString const &channel, QObject* parent = nullptr);

    void Listen      () const;

    void sendMessage (QString const &message, QString const &channel) const;
    void joinChannel (QString const &chanell) const;

signals:

public slots:
private:
    void PONG() const;
    void setNick     (QString const &nickname) const;
    void secureNick  () const;
    void setUser     () const;
    void login       () const;


private slots:
    void connected   () const;
    void disconnected() const;
    void readyRead   () const;
};

#endif // Q_IRCBOT_H
