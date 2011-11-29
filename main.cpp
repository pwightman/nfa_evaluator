#include <QtCore/QCoreApplication>
#include <QHash>
#include <QSet>
#include <QTime>
#include "stdio.h"

#include "nfa.h"

int main()
{
    // QCoreApplication a(argc, argv);
    // return a.exec();
    QSet<QString>* Q = new QSet<QString>();
    QString q0("");
    QSet<QString>* sigma = new QSet<QString>();
    QHash<QPair<QString, QString>, QSet<QString>*>* delta = new QHash<QPair<QString, QString>, QSet<QString>*>();
    QSet<QString>* f = new QSet<QString>();

    Q->insert(QString("S1"));
    Q->insert(QString("S2"));
    Q->insert(QString("S3"));

    q0 = QString("S1");

    sigma->insert(QString("0"));
    sigma->insert(QString("1"));

    f->insert(QString("S3"));

    QPair<QString, QString> pair1;
    QPair<QString, QString> pair2;
    QPair<QString, QString> pair3;
    QSet<QString>* set = new QSet<QString>();

    pair1.first = QString("S1");
    pair1.second = QString("@");
    set->insert(QString("S2"));
    set->insert(QString("S3"));

    delta->insert(pair1, set);

    pair2.first = QString("S2");
    pair2.second = QString("1");
    set = new QSet<QString>();
    set->insert(QString("S3"));

    delta->insert(pair2, set);

    pair3.first = QString("S3");
    pair3.second = QString("0");
    set = new QSet<QString>();
    set->insert(QString("S2"));

    delta->insert(pair3, set);

    Nfa* nfa = Nfa::createNfa(Q, q0, sigma, delta, f);
    QTime t;
    t.start();
    nfa->runNfa(QString("0101"));
    printf("Linear run time: %d ms\n", t.elapsed());
    QTime t2;
    t2.start();
    nfa->runNfaP(QString("0101"));
    printf("Parallel run time: %d ms\n", t2.elapsed());
}
