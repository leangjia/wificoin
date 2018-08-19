<<<<<<< HEAD
// Copyright (c) 2011-2014 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_TRANSACTIONFILTERPROXY_H
#define WIFICOIN_QT_TRANSACTIONFILTERPROXY_H

#include "amount.h"

#include <QDateTime>
#include <QSortFilterProxyModel>
=======
#ifndef TRANSACTIONFILTERPROXY_H
#define TRANSACTIONFILTERPROXY_H

#include <QSortFilterProxyModel>
#include <QDateTime>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

/** Filter the transaction list according to pre-specified rules. */
class TransactionFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TransactionFilterProxy(QObject *parent = 0);

    /** Earliest date that can be represented (far in the past) */
    static const QDateTime MIN_DATE;
    /** Last date that can be represented (far in the future) */
    static const QDateTime MAX_DATE;
    /** Type filter bit field (all types) */
    static const quint32 ALL_TYPES = 0xFFFFFFFF;

    static quint32 TYPE(int type) { return 1<<type; }

<<<<<<< HEAD
    enum WatchOnlyFilter
    {
        WatchOnlyFilter_All,
        WatchOnlyFilter_Yes,
        WatchOnlyFilter_No
    };

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void setDateRange(const QDateTime &from, const QDateTime &to);
    void setAddressPrefix(const QString &addrPrefix);
    /**
      @note Type filter takes a bit field created with TYPE() or ALL_TYPES
     */
    void setTypeFilter(quint32 modes);
<<<<<<< HEAD
    void setMinAmount(const CAmount& minimum);
    void setWatchOnlyFilter(WatchOnlyFilter filter);
=======
    void setMinAmount(qint64 minimum);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    /** Set maximum number of rows returned, -1 if unlimited. */
    void setLimit(int limit);

<<<<<<< HEAD
    /** Set whether to show conflicted transactions. */
    void setShowInactive(bool showInactive);

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

private:
    QDateTime dateFrom;
    QDateTime dateTo;
    QString addrPrefix;
    quint32 typeFilter;
<<<<<<< HEAD
    WatchOnlyFilter watchOnlyFilter;
    CAmount minAmount;
    int limitRows;
    bool showInactive;
};

#endif // WIFICOIN_QT_TRANSACTIONFILTERPROXY_H
=======
    qint64 minAmount;
    int limitRows;
};

#endif // TRANSACTIONFILTERPROXY_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
