<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_TRANSACTIONTABLEMODEL_H
#define WIFICOIN_QT_TRANSACTIONTABLEMODEL_H

#include "wificoinunits.h"
=======
#ifndef TRANSACTIONTABLEMODEL_H
#define TRANSACTIONTABLEMODEL_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <QAbstractTableModel>
#include <QStringList>

<<<<<<< HEAD
class PlatformStyle;
class TransactionRecord;
class TransactionTablePriv;
class WalletModel;

class CWallet;

=======
class CWallet;
class TransactionTablePriv;
class TransactionRecord;
class WalletModel;

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
/** UI model for the transaction table of a wallet.
 */
class TransactionTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit TransactionTableModel(const PlatformStyle *platformStyle, CWallet* wallet, WalletModel *parent = 0);
=======
    explicit TransactionTableModel(CWallet* wallet, WalletModel *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~TransactionTableModel();

    enum ColumnIndex {
        Status = 0,
<<<<<<< HEAD
        Watchonly = 1,
        Date = 2,
        Type = 3,
        ToAddress = 4,
        Amount = 5
=======
        Date = 1,
        Type = 2,
        ToAddress = 3,
        Amount = 4
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    };

    /** Roles to get specific information from a transaction row.
        These are independent of column.
    */
    enum RoleIndex {
        /** Type of transaction */
        TypeRole = Qt::UserRole,
        /** Date and time this transaction was created */
        DateRole,
<<<<<<< HEAD
        /** Watch-only boolean */
        WatchonlyRole,
        /** Watch-only icon */
        WatchonlyDecorationRole,
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        /** Long description (HTML format) */
        LongDescriptionRole,
        /** Address of transaction */
        AddressRole,
        /** Label of address related to transaction */
        LabelRole,
        /** Net amount of transaction */
        AmountRole,
        /** Unique identifier */
        TxIDRole,
<<<<<<< HEAD
        /** Transaction hash */
        TxHashRole,
        /** Transaction data, hex-encoded */
        TxHexRole,
        /** Whole transaction as plain text */
        TxPlainTextRole,
        /** Is transaction confirmed? */
        ConfirmedRole,
        /** Formatted amount, without brackets when unconfirmed */
        FormattedAmountRole,
        /** Transaction status (TransactionRecord::Status) */
        StatusRole,
        /** Unprocessed icon */
        RawDecorationRole,
=======
        /** Is transaction confirmed? */
        ConfirmedRole,
        /** Formatted amount, without brackets when unconfirmed */
        FormattedAmountRole
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    };

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
<<<<<<< HEAD
    bool processingQueuedTransactions() { return fProcessingQueuedTransactions; }
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

private:
    CWallet* wallet;
    WalletModel *walletModel;
    QStringList columns;
    TransactionTablePriv *priv;
<<<<<<< HEAD
    bool fProcessingQueuedTransactions;
    const PlatformStyle *platformStyle;

    void subscribeToCoreSignals();
    void unsubscribeFromCoreSignals();
=======
    int cachedNumBlocks;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    QString lookupAddress(const std::string &address, bool tooltip) const;
    QVariant addressColor(const TransactionRecord *wtx) const;
    QString formatTxStatus(const TransactionRecord *wtx) const;
    QString formatTxDate(const TransactionRecord *wtx) const;
    QString formatTxType(const TransactionRecord *wtx) const;
    QString formatTxToAddress(const TransactionRecord *wtx, bool tooltip) const;
<<<<<<< HEAD
    QString formatTxAmount(const TransactionRecord *wtx, bool showUnconfirmed=true, WiFicoinUnits::SeparatorStyle separators=WiFicoinUnits::separatorStandard) const;
    QString formatTooltip(const TransactionRecord *rec) const;
    QVariant txStatusDecoration(const TransactionRecord *wtx) const;
    QVariant txWatchonlyDecoration(const TransactionRecord *wtx) const;
    QVariant txAddressDecoration(const TransactionRecord *wtx) const;

public Q_SLOTS:
    /* New transaction, or transaction changed status */
    void updateTransaction(const QString &hash, int status, bool showTransaction);
    void updateConfirmations();
    void updateDisplayUnit();
    /** Updates the column title to "Amount (DisplayUnit)" and emits headerDataChanged() signal for table headers to react. */
    void updateAmountColumnTitle();
    /* Needed to update fProcessingQueuedTransactions through a QueuedConnection */
    void setProcessingQueuedTransactions(bool value) { fProcessingQueuedTransactions = value; }
=======
    QString formatTxAmount(const TransactionRecord *wtx, bool showUnconfirmed=true) const;
    QString formatTooltip(const TransactionRecord *rec) const;
    QVariant txStatusDecoration(const TransactionRecord *wtx) const;
    QVariant txAddressDecoration(const TransactionRecord *wtx) const;

public slots:
    void updateTransaction(const QString &hash, int status);
    void updateConfirmations();
    void updateDisplayUnit();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    friend class TransactionTablePriv;
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_TRANSACTIONTABLEMODEL_H
=======
#endif // TRANSACTIONTABLEMODEL_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
