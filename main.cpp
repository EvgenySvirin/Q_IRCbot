#include <QCoreApplication>
#include <q_ircbot.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Q_IRCBot bot("irc.freenode.net", 6667, "BAUTMAN", "BAUTMAN", "passwordword", "BAUTMAN", "botkila");
    bot.Listen();
    return a.exec();
}
