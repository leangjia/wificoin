<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "transactionfilterproxy.h"

#include "transactiontablemodel.h"
#include "transactionrecord.h"

#include <cstdlib>

#include <QDateTime>

=======
#include "transactionfilterproxy.h"

#include "transactiontablemodel.h"

#include <QDateTime>

#include <cstdlib>

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
// Earliest date that can be represented (far in the past)
const QDateTime TransactionFilterProxy::MIN_DATE = QDateTime::fromTime_t(0);
// Last date that can be represented (far in the future)
const QDateTime TransactionFilterProxy::MAX_DATE = QDateTime::fromTime_t(0xFFFFFFFF);

TransactionFilterProxy::TransactionFilterProxy(QObject *parent) :
    QSortFilterProxyModel(parent),
    dateFrom(MIN_DATE),
    dateTo(MAX_DATE),
    addrPrefix(),
    typeFilter(ALL_TYPES),
<<<<<<< HEAD
    watchOnlyFilter(WatchOnlyFilter_All),
    minAmount(0),
    limitRows(-1),
    showInactive(true)
=======
    minAmount(0),
    limitRows(-1)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
}

bool TransactionFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    int type = index.data(TransactionTableModel::TypeRole).toInt();
    QDateTime datetime = index.data(TransactionTableModel::DateRole).toDateTime();
<<<<<<< HEAD
    bool involvesWatchAddress = index.data(TransactionTableModel::WatchonlyRole).toBool();
    QString address = index.data(TransactionTableModel::AddressRole).toString();
    QString label = index.data(TransactionTableModel::LabelRole).toString();
    qint64 amount = llabs(index.data(TransactionTableModel::AmountRole).toLongLong());
    int status = index.data(TransactionTableModel::StatusRole).toInt();

    if(!showInactive && status == TransactionStatus::Conflicted)
        return false;
    if(!(TYPE(type) & typeFilter))
        return false;
    if (involvesWatchAddress && watchOnlyFilter == WatchOnlyFilter_No)
        return false;
    if (!involvesWatchAddress && watchOnlyFilter == WatchOnlyFilter_Yes)
        return false;
=======
    QString address = index.data(TransactionTableModel::AddressRole).toString();
    QString label = index.data(TransactionTableModel::LabelRole).toString();
    qint64 amount = llabs(index.data(TransactionTableModel::AmountRole).toLongLong());

    if(!(TYPE(type) & typeFilter))
        return false;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    if(datetime < dateFrom || datetime > dateTo)
        return false;
    if (!address.contains(addrPrefix, Qt::CaseInsensitive) && !label.contains(addrPrefix, Qt::CaseInsensitive))
        return false;
    if(amount < minAmount)
        return false;

    return true;
}

void TransactionFilterProxy::setDateRange(const QDateTime &from, const QDateTime &to)
{
    this->dateFrom = from;
    this->dateTo = to;
    invalidateFilter();
}

<<<<<<< HEAD
void TransactionFilterProxy::setAddressPrefix(const QString &_addrPrefix)
{
    this->addrPrefix = _addrPrefix;
=======
void TransactionFilterProxy::setAddressPrefix(const QString &addrPrefix)
{
    this->addrPrefix = addrPrefix;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    invalidateFilter();
}

void TransactionFilterProxy::setTypeFilter(quint32 modes)
{
    this->typeFilter = modes;
    invalidateFilter();
}

<<<<<<< HEAD
void TransactionFilterProxy::setMinAmount(const CAmount& minimum)
=======
void TransactionFilterProxy::setMinAmount(qint64 minimum)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    this->minAmount = minimum;
    invalidateFilter();
}

<<<<<<< HEAD
void TransactionFilterProxy::setWatchOnlyFilter(WatchOnlyFilter filter)
{
    this->watchOnlyFilter = filter;
    invalidateFilter();
}

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
void TransactionFilterProxy::setLimit(int limit)
{
    this->limitRows = limit;
}

<<<<<<< HEAD
void TransactionFilterProxy::setShowInactive(bool _showInactive)
{
    this->showInactive = _showInactive;
    invalidateFilter();
}

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
int TransactionFilterProxy::rowCount(const QModelIndex &parent) const
{
    if(limitRows != -1)
    {
        return std::min(QSortFilterProxyModel::rowCount(parent), limitRows);
    }
    else
    {
        return QSortFilterProxyModel::rowCount(parent);
    }
}
