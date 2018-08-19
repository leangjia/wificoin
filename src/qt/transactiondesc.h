<<<<<<< HEAD
// Copyright (c) 2011-2014 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_TRANSACTIONDESC_H
#define WIFICOIN_QT_TRANSACTIONDESC_H

#include <QObject>
#include <QString>

class TransactionRecord;
=======
#ifndef TRANSACTIONDESC_H
#define TRANSACTIONDESC_H

#include <QString>
#include <QObject>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

class CWallet;
class CWalletTx;

/** Provide a human-readable extended HTML description of a transaction.
 */
class TransactionDesc: public QObject
{
    Q_OBJECT

public:
<<<<<<< HEAD
    static QString toHTML(CWallet *wallet, CWalletTx &wtx, TransactionRecord *rec, int unit);
=======
    static QString toHTML(CWallet *wallet, CWalletTx &wtx);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

private:
    TransactionDesc() {}

    static QString FormatTxStatus(const CWalletTx& wtx);
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_TRANSACTIONDESC_H
=======
#endif // TRANSACTIONDESC_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
