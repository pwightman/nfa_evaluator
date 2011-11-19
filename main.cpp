#include <QtCore/QCoreApplication>
#include <QHash>
#include <QSet>

#include "nfa.h"

int main()
{
    // QCoreApplication a(argc, argv);
    // return a.exec();
    QSet<QString>* Q = new QSet<QString>();
    QString q0("");
    QSet<QString>* sigma = new QSet<QString>();
    QHash<QPair<QString, QString>*, QSet<QString>*>* delta = new QHash<QPair<QString, QString>*, QSet<QString>*>();
    QSet<QString>* f = new QSet<QString>();

    Q->insert(QString("0"));
    Q->insert(QString("1"));
    Q->insert(QString("2"));

    q0 = QString("0");

    sigma->insert(QString("0"));
    sigma->insert(QString("1"));

    QPair<QString, QString>* pair = new QPair<QString, QString>();
    QSet<QString>* set = new QSet<QString>();

    pair->first = QString("0");
    pair->second = QString("1");
    set->insert(QString("1"));
    set->insert(QString("2"));

    delta->insert(pair, set);

    pair = new QPair<QString, QString>();
    pair->first = QString("1");
    pair->second = QString("1");
    set = new QSet<QString>();
    set->insert(QString("2"));

    delta->insert(pair, set);

    pair = new QPair<QString, QString>();
    pair->first = QString("2");
    pair->second = QString("0");
    set = new QSet<QString>();
    set->insert(QString("1"));

    delta->insert(pair, set);

    Nfa* nfa = Nfa::createNfa(Q, q0, sigma, delta, f);
    nfa->runNfa();
}
