#include <QCryptographicHash>
#include <QVector>
#include "threadmanager.h"

/*
 * std::pow pour les long long unsigned int
 */
long long unsigned int intPow (
        long long unsigned int number,
        long long unsigned int index)
{
    long long unsigned int i;

    if (index == 0)
        return 1;

    long long unsigned int num = number;

    for (i = 1; i < index; i++)
        number *= num;

    return number;
}

ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{}


void ThreadManager::incrementPercentComputed(double percentComputed)
{
    emit sig_incrementPercentComputed(percentComputed);
}

/*
 * Les paramètres sont les suivants:
 *
 * - charset:   QString contenant tous les caractères possibles du mot de passe
 * - salt:      le sel à concaténer au début du mot de passe avant de le hasher
 * - hash:      le hash dont on doit retrouver la préimage
 * - nbChars:   le nombre de caractères du mot de passe à bruteforcer
 * - nbThreads: le nombre de threads à lancer
 *
 * Cette fonction doit retourner le mot de passe correspondant au hash, ou une
 * chaine vide si non trouvé.
 */
QString ThreadManager::startHacking(
        QString charset,
        QString salt,
        QString hash,
        unsigned int nbChars,
        unsigned int nbThreads
)
{
    this->fillThreadManager(charset, salt, hash, nbChars);
    this->prepareThreads(nbThreads, charset);
    this->startThreads();
    QString finalResult = this->result;
    this->clean();
    return finalResult;
}

void ThreadManager::fillThreadManager(QString charset, QString salt, QString hash, unsigned passLength){
    this->charset = charset;
    this->hash = hash;
    this->salt = salt;
    this->passLength = passLength;
    this->nbToCompute = intPow(charset.size(), passLength);
}

void ThreadManager::prepareThreads(unsigned int nbrOfThreads, QString charset){
    QVector<QString> firstCharsets = getCharsetToCompute(charset, nbrOfThreads);
    this->myThreads.resize(nbrOfThreads);
    for (unsigned i = 0; i < nbrOfThreads; i++) {
        this->myThreads[i] = MyThread(firstCharsets[i], this);
    }
}

void ThreadManager::clean(){
    myThreads.clear();
    charset = QString("");
    hash = QString("");
    salt = QString("");
    result = QString("");
    passLength = 0;
}


void ThreadManager::startThreads(){
    for (MyThread& myThread : this->myThreads) {
        myThread.pcoThread = new PcoThread(
            &MyThread::tryAllCombinations,
            &myThread
        );
    }
    this->waitThreads();
    this->deleteThreads();
}

void ThreadManager::waitThreads(){
    for (MyThread& thread : this->myThreads) {
        thread.pcoThread->join();
    }
}

void ThreadManager::deleteThreads(){
    for (MyThread& thread : this->myThreads) {
        delete thread.pcoThread;
    }
}

inline static QVector<QString> getCharsetToCompute(const QString& charset, size_t nbThreads) {
    if (nbThreads == 0) {
        return QVector<QString>();
    }
    QVector<QString> validCharVector(nbThreads);
    size_t lettersPerGroup = charset.size() / nbThreads;
    size_t remainingLetters = charset.size() % nbThreads;
    size_t index = 0;

    for (size_t i = 0; i < nbThreads; i++) {
        validCharVector[i] = charset.mid(index, lettersPerGroup);
        index += lettersPerGroup;

        if (remainingLetters > 0) {
            validCharVector[i].append(charset.at(index));
            index++;
            remainingLetters--;
        }
    }
    return validCharVector;

}
