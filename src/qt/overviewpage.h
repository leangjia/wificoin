<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_OVERVIEWPAGE_H
#define WIFICOIN_QT_OVERVIEWPAGE_H

#include "amount.h"

#include <QWidget>
#include <memory>

class ClientModel;
class TransactionFilterProxy;
class TxViewDelegate;
class PlatformStyle;
class WalletModel;
=======
#ifndef OVERVIEWPAGE_H
#define OVERVIEWPAGE_H

#include <QWidget>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

namespace Ui {
    class OverviewPage;
}
<<<<<<< HEAD
=======
class ClientModel;
class WalletModel;
class TxViewDelegate;
class TransactionFilterProxy;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Overview ("home") page widget */
class OverviewPage : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit OverviewPage(const PlatformStyle *platformStyle, QWidget *parent = 0);
=======
    explicit OverviewPage(QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~OverviewPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);
    void showOutOfSyncWarning(bool fShow);

<<<<<<< HEAD
public Q_SLOTS:
    void setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance,
                    const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);

Q_SIGNALS:
    void transactionClicked(const QModelIndex &index);
    void outOfSyncWarningClicked();
=======
public slots:
    void setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance);

signals:
    void transactionClicked(const QModelIndex &index);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

private:
    Ui::OverviewPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
<<<<<<< HEAD
    CAmount currentBalance;
    CAmount currentUnconfirmedBalance;
    CAmount currentImmatureBalance;
    CAmount currentWatchOnlyBalance;
    CAmount currentWatchUnconfBalance;
    CAmount currentWatchImmatureBalance;

    TxViewDelegate *txdelegate;
    std::unique_ptr<TransactionFilterProxy> filter;

private Q_SLOTS:
    void updateDisplayUnit();
    void handleTransactionClicked(const QModelIndex &index);
    void updateAlerts(const QString &warnings);
    void updateWatchOnlyLabels(bool showWatchOnly);
    void handleOutOfSyncWarningClicks();
};

#endif // WIFICOIN_QT_OVERVIEWPAGE_H
=======
    qint64 currentBalance;
    qint64 currentUnconfirmedBalance;
    qint64 currentImmatureBalance;

    TxViewDelegate *txdelegate;
    TransactionFilterProxy *filter;

private slots:
    void updateDisplayUnit();
    void handleTransactionClicked(const QModelIndex &index);
    void updateAlerts(const QString &warnings);
};

#endif // OVERVIEWPAGE_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
