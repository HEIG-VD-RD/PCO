#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <pcosynchro/pcothread.h>
#include <QVector>
#include <pcosynchro/pcothread.h>
#include "threadmanager.h"


class ThreadManager;

class MyThread{
    friend class ThreadManager;
    private:
    // Variable pour compter les itérations effectués
    long long unsigned int nbComputed = 0;

    // Charset fixe du début du mot de passe
    QString firstCharset = QString("");

    // Pointeur vers le thread
    PcoThread* pcoThread = nullptr;

    // Pointeur vers le ThreadManager
    ThreadManager* manager  = nullptr;
    
    public:
    /**
     * \brief Constructeur de MyThread qui prend en paramètre le premier charset à tester et le ThreadManager
     * \param firstCharset le charset fixe du debut du mot de passe
     * \param manager le ThreadManager qui contient les informations sur le mot de passe à trouver
    */
    MyThread(QString firstCharset, ThreadManager* manager) : firstCharset(firstCharset), manager(manager) {}

    /**
     * \brief Constructeur par défaut de MyThread
    */
    MyThread() {}

    /**
     * \brief Methode utilisée par le thread pour tester toutes les combinaisons de mot de passe avec le charset fixe et le charset complet du ThreadManager
    */
    void tryAllCombinations();
};

/**
 * \brief Fonction qui génère le prochain mot de passe à tester
 * \param currentPasswordString le mot de passe actuel
 * \param lengthOfComb la longueur du mot de passe
 * \param indexes le vecteur des indexes des caractères du mot de passe
 * \param charset le charset complet
*/
inline static void generateNewPasswordToTry(QString& currentPasswordString, unsigned& lengthOfComb, QVector<unsigned> &indexes, const QString& charset);
#endif
