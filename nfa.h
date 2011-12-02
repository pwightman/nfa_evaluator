#ifndef NFA_H
#define NFA_H

#define VERBOSE 1

#include <QHash>
#include <QSet>
#include <QString>
#include <QPair>

class Nfa
{
public:
    // TODO: Figure out how to make private.
    // Nfa(QSet<QString>* Q, QString q0, QSet<QString>* sigma, QHash<QPair<QString, QString>, QSet<QString>*>* delta, QSet<QString>* f);
    Nfa();
    void addTransition(QString source, QString destination, QString path);
    void makeInitial(QString node);
    void makeFinal(QString node);
    bool isValidString(QString string, bool isParallel);

    /*
     * Prints a string that python can read in as its own native data structures
     * (Used for testing)
     */
    QString toPyString();

private:
    // Private member variables.
    QSet<QString>* Q;
    QString q0;
    QSet<QString>* sigma;
    QHash<QPair<QString, QString>, QSet<QString>*>* delta;
    QHash<QPair<QString, QString>, QSet<QString>*>* reverseDelta;
    QSet<QString>* f;

    // Private methods.
    void debugPrintDelta();
    void debugPrintSet(QSet<QString>* set);
    QSet<QString>* runNfa(QString string);
    QSet<QString>* runNfaP(QString string);
};

#endif // NFA_H
