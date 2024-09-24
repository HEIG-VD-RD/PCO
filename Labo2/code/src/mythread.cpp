#include "mythread.h"
#include <QVector>
#include <QCryptographicHash>


inline static void generateNewPasswordToTry(QString& currentPasswordString, unsigned& lengthOfComb, QVector<unsigned> &indexes, const QString& charset) {
    unsigned i = 0;
    while (i < lengthOfComb) {
        indexes[i]++;
        if (indexes[i] < (unsigned) charset.size()) {
            break;
        }
        indexes[i] = 0;
        i++;
    }
    for (i = 0; i < lengthOfComb; i++) {
        currentPasswordString[i] = charset.at(indexes.at(i));
    }
}

void MyThread::tryAllCombinations() {
    unsigned lengthOfComb = manager->passLength - 1;
    long long unsigned int localTotalNbrToCompute = manager->nbToCompute / manager->charset.size();
    QCryptographicHash md5(QCryptographicHash::Md5);
    QString currentHash;
    QVector<unsigned> indexes(lengthOfComb, 0);
    QString currentSuffixPasswordString(lengthOfComb, manager->charset.at(0));
    QString currentPasswordString;

    for (auto& fc : this->firstCharset) {
        long long unsigned int localComputed = 0;

        while (localComputed < localTotalNbrToCompute && manager->result.isEmpty()) {
            if ((this->nbComputed % 1000) == 0) {
                this->manager->incrementPercentComputed((double)1000/manager->nbToCompute);
            }
            currentPasswordString = QString(fc).append(currentSuffixPasswordString);
            md5.reset();
            md5.addData(manager->salt.toLatin1());
            md5.addData(currentPasswordString.toLatin1());
            currentHash = md5.result().toHex();
            if (currentHash == manager->hash) {
                manager->result = currentPasswordString;
            }
            if(!manager->result.isEmpty()){
                return;
            }
            generateNewPasswordToTry(currentSuffixPasswordString, lengthOfComb, indexes, manager->charset);
            localComputed++;
            this->nbComputed++;
        }
    }
}
