<<<<<<< HEAD
// Copyright (c) 2011-2014 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_TRANSACTIONDESCDIALOG_H
#define WIFICOIN_QT_TRANSACTIONDESCDIALOG_H
=======
#ifndef TRANSACTIONDESCDIALOG_H
#define TRANSACTIONDESCDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <QDialog>

namespace Ui {
    class TransactionDescDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Dialog showing transaction details. */
class TransactionDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDescDialog(const QModelIndex &idx, QWidget *parent = 0);
    ~TransactionDescDialog();

private:
    Ui::TransactionDescDialog *ui;
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_TRANSACTIONDESCDIALOG_H
=======
#endif // TRANSACTIONDESCDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
