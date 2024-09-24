    /**
  \file threadmanager.h
  \author Yann Thoma
  \date 24.02.2017
  \brief Classe pour reverser un hash md5 par brute force.


  Ce fichier contient la définition de la classe ThreadManager, qui permet de
  reverser un hash md5 par brute force.
*/

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QString>
#include "mythread.h"


class MyThread;
/**
 * \brief The ThreadManager class
 *
 * Cette classe tente de reverser un hash md5 par brute force.
 */
class ThreadManager: public QObject
{
friend class MyThread;
    Q_OBJECT
private:
// Charset commun à tous les threads
QString charset;

// Hash du mots de passe à reverser
QString hash;

// Sel du hash
QString salt;

// Mot de passe trouvé
QString result;

// Taille du mot de passe
unsigned passLength;

// Nombre total d'itérations à faire
long long unsigned int nbToCompute;

// Vecteur des myThreads contenant les threads
QVector<MyThread> myThreads;

public:
    /**
     * \brief ThreadManager Constructeur simple
     * \param parent Objet parent de l'interface
     */
    ThreadManager(QObject *parent);

    /**
     * \brief startHacking tâche qui s'occupe de trouver le hash md5.
     * \param charset QString tous les caractères possibles composant le mot de
     * passe
     * \param salt QString sel qui permet de modifier dynamiquement le hash
     * \param hash QString hash à reverser
     * \param nbChars taille du mot de passe
     * \param nbThreads nombre de threads qui doivent reverser le hash
     * \return Le hash trouvé, ou une chaine vide sinon
     *
     * Cette fonction exécute réellement la recherche.
     */
    QString startHacking(
            QString charset,
            QString salt,
            QString hash,
            unsigned int nbChars,
            unsigned int nbThreads
    );

    /**
     * \brief incrementPercentComputed fonction qui indique que le pourcentage
     * de test effectué pour casser le hash md5.
     * \param percentComputed double pourcentage de tests effectués pour
     * reverser le hash md5
     */
    void incrementPercentComputed(double percentComputed);

    /**
     * \brief fillThreadManager fonction qui remplit les attributs de la classe
     * \param charset QString tous les caractères possibles composant le mot de passe
     * \param salt QString sel qui permet de modifier dynamiquement le hash
     * \param hash QString hash à comparer
     * \param passLength taille du mot de passe
    */
    void fillThreadManager(QString charset, QString salt, QString hash, unsigned passLength);

    /**
     * \brief clean fonction qui nettoie les attributs de la classe. Elle est appelée à la fin de la recherche.
    */
    void clean();

    /**
     * \brief prepareThreads fonction qui prépare les threads pour la recherche du hash md5.
     * \param nbrOfThreads nombre de threads qui doivent être préparés
     * \param charset QString tous les caractères possibles composant le mot de passe
     * \param hash QString hash à comparer 
     * \param salt QString sel qui permet de modifier dynamiquement le hash
     * \param passLength taille du mot de passe
    */
    void prepareThreads(unsigned int nbrOfThreads, QString charset);


    /**
     * \brief startThreads fonction qui démarre les threads pour la recherche du hash md5. Elle doit être appelée après la fonction prepareThreads.
     * Elle appelle également waitThreads pour attendre que les threads se terminent et deleteThreads pour supprimer les threads.
    */
    void startThreads();

private:

    /**
     * \brief waitThreads fonction qui attend que les threads se terminent. Elle doit être appelée après la fonction startThreads.
    */
    void waitThreads();

    /**
     * \brief deleteThreads fonction qui supprime les threads. Elle doit être appelée après la fonction waitThreads.
    */
    void deleteThreads();

signals:
    /**
     * \brief sig_incrementPercentComputed signal qui indique que le pourcentage
     * de test effectué pour casser le hash md5.
     * \param percentComputed double pourcentage de tests effectués pour
     * reverser le hash md5
     */
    void sig_incrementPercentComputed(double percentComputed);
};
inline static QVector<QString> getCharsetToCompute(const QString& charset, size_t nbThreads);


#endif // THREADMANAGER_H
