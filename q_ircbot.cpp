#include "q_ircbot.h"

Q_IRCBot::Q_IRCBot(QObject* parent):QObject(parent), ircBotSocket(new QTcpSocket(this)) {
    qDebug() << "IRCBot obj created";
}

Q_IRCBot::Q_IRCBot(QString const &server, int port,
                   QString const &username, QString const &realname,
                   QString const &password, QString const &nickname,
                   QString const &first_channel, QObject* parent):
        QObject(parent),
        server(server), port(port),
        username(username), realname(realname),
        password(password), nickname(nickname),
        first_channel(first_channel),
        channelList({}),
        ircBotSocket(new QTcpSocket(this)) {
        connect(ircBotSocket.get(), SIGNAL(connected()), this, SLOT(connected()));
        connect(ircBotSocket.get(), SIGNAL(disconnected()), this, SLOT(disconnected()));
        ircBotSocket->connectToHost(server, port);

        qDebug() << "IRCBot obj created";
}

void Q_IRCBot::Listen() const {
    connect(ircBotSocket.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Q_IRCBot::secureNick() const {
        QByteArray package;
        package.append("\x02/msg NickServ");
        package.append("identify <");
        package.append(password);
        package.append(">x02.\r\n");
        ircBotSocket->write(package);
        ircBotSocket->flush();
}

void Q_IRCBot::sendMessage(QString const &message,
                           QString const &channel) const {
    QByteArray package;
    package.append("PRIVMSG #");
    package.append(channel);
    package.append(" :");
    package.append(message);
    package.append("\r\n");
    ircBotSocket->write(package);
    ircBotSocket->flush();
}

void Q_IRCBot::setNick(QString const &nickname) const{
    QByteArray package;
    package.append("NICK ");
    package.append(nickname);
    package.append("\r\n");
    ircBotSocket->write(package);
    ircBotSocket->flush();
}

void Q_IRCBot::setUser() const {
    QByteArray package;
    package.append("USER ");
    package.append(username);
    package.append(" 8 * : ");
    package.append(realname);
    ircBotSocket->write(package);
    ircBotSocket->flush();
}

void Q_IRCBot::joinChannel(QString const &channel) const {
    channelList.push_back(channel);
    QByteArray package;
    package.append("JOIN #");
    package.append(channel);
    package.append("\r\n");
    ircBotSocket->write(package);
    ircBotSocket->flush();
}

void Q_IRCBot::PONG() const {
    QByteArray package;
    package.append("PONG ");
    package.append(server);
    package.append("\r\n");
    ircBotSocket->write(package);
    ircBotSocket->flush();
}

void Q_IRCBot::login() const {
    setUser();
    secureNick();
    setNick(nickname);
}

void Q_IRCBot::connected() const {
    login();
    joinChannel(first_channel);
}

void Q_IRCBot::disconnected() const
{}

void Q_IRCBot::readyRead() const {
    QByteArray array;
    while(!(array = ircBotSocket->readLine()).isNull()) {
        qDebug() << array;
        std::string s(array);
        if (s.find("PING") != std::string::npos) {
            PONG();
            this->sendMessage("Just stop it, brah", first_channel);
            qDebug() << "PongedBack";
        }
    }
}

