#ifndef NFA_H
#define NFA_H

#include <QHash>
#include <QSet>

class Nfa
{
public:
    static Nfa createNfa(QString Q, QString q0, QSet<QString> sigma, QHash<QPair<QString, QString>, QString> delta, QSet<QString> f);
    bool acceptString(QString string);

private:
    QSet<QString> Q;
    QString q0;
    QSet<QString> sigma;
    QHash<QPair<QString, QString>, QString> delta;
    QSet<QString> f;
    Nfa();
};

#endif // NFA_H
