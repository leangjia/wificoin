<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#include "addresstablemodel.h"

#include "guiutil.h"
#include "walletmodel.h"

<<<<<<< HEAD
#include "base58.h"
#include "wallet/wallet.h"


#include <QFont>
#include <QDebug>
=======
#include "wallet.h"
#include "base58.h"

#include <QFont>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

const QString AddressTableModel::Send = "S";
const QString AddressTableModel::Receive = "R";

struct AddressTableEntry
{
    enum Type {
        Sending,
<<<<<<< HEAD
        Receiving,
        Hidden /* QSortFilterProxyModel will filter these out */
=======
        Receiving
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    };

    Type type;
    QString label;
    QString address;

    AddressTableEntry() {}
<<<<<<< HEAD
    AddressTableEntry(Type _type, const QString &_label, const QString &_address):
        type(_type), label(_label), address(_address) {}
=======
    AddressTableEntry(Type type, const QString &label, const QString &address):
        type(type), label(label), address(address) {}
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
};

struct AddressTableEntryLessThan
{
    bool operator()(const AddressTableEntry &a, const AddressTableEntry &b) const
    {
        return a.address < b.address;
    }
    bool operator()(const AddressTableEntry &a, const QString &b) const
    {
        return a.address < b;
    }
    bool operator()(const QString &a, const AddressTableEntry &b) const
    {
        return a < b.address;
    }
};

<<<<<<< HEAD
/* Determine address type from address purpose */
static AddressTableEntry::Type translateTransactionType(const QString &strPurpose, bool isMine)
{
    AddressTableEntry::Type addressType = AddressTableEntry::Hidden;
    // "refund" addresses aren't shown, and change addresses aren't in mapAddressBook at all.
    if (strPurpose == "send")
        addressType = AddressTableEntry::Sending;
    else if (strPurpose == "receive")
        addressType = AddressTableEntry::Receiving;
    else if (strPurpose == "unknown" || strPurpose == "") // if purpose not set, guess
        addressType = (isMine ? AddressTableEntry::Receiving : AddressTableEntry::Sending);
    return addressType;
}

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
// Private implementation
class AddressTablePriv
{
public:
    CWallet *wallet;
    QList<AddressTableEntry> cachedAddressTable;
    AddressTableModel *parent;

<<<<<<< HEAD
    AddressTablePriv(CWallet *_wallet, AddressTableModel *_parent):
        wallet(_wallet), parent(_parent) {}
=======
    AddressTablePriv(CWallet *wallet, AddressTableModel *parent):
        wallet(wallet), parent(parent) {}
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    void refreshAddressTable()
    {
        cachedAddressTable.clear();
        {
            LOCK(wallet->cs_wallet);
<<<<<<< HEAD
            for (const std::pair<CTxDestination, CAddressBookData>& item : wallet->mapAddressBook)
            {
                const CWiFicoinAddress& address = item.first;
                bool fMine = IsMine(*wallet, address.Get());
                AddressTableEntry::Type addressType = translateTransactionType(
                        QString::fromStdString(item.second.purpose), fMine);
                const std::string& strName = item.second.name;
                cachedAddressTable.append(AddressTableEntry(addressType,
=======
            BOOST_FOREACH(const PAIRTYPE(CTxDestination, std::string)& item, wallet->mapAddressBook)
            {
                const CBitcoinAddress& address = item.first;
                const std::string& strName = item.second;
                bool fMine = IsMine(*wallet, address.Get());
                cachedAddressTable.append(AddressTableEntry(fMine ? AddressTableEntry::Receiving : AddressTableEntry::Sending,
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                                  QString::fromStdString(strName),
                                  QString::fromStdString(address.ToString())));
            }
        }
        // qLowerBound() and qUpperBound() require our cachedAddressTable list to be sorted in asc order
<<<<<<< HEAD
        // Even though the map is already sorted this re-sorting step is needed because the originating map
        // is sorted by binary address, not by base58() address.
        qSort(cachedAddressTable.begin(), cachedAddressTable.end(), AddressTableEntryLessThan());
    }

    void updateEntry(const QString &address, const QString &label, bool isMine, const QString &purpose, int status)
=======
        qSort(cachedAddressTable.begin(), cachedAddressTable.end(), AddressTableEntryLessThan());
    }

    void updateEntry(const QString &address, const QString &label, bool isMine, int status)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        // Find address / label in model
        QList<AddressTableEntry>::iterator lower = qLowerBound(
            cachedAddressTable.begin(), cachedAddressTable.end(), address, AddressTableEntryLessThan());
        QList<AddressTableEntry>::iterator upper = qUpperBound(
            cachedAddressTable.begin(), cachedAddressTable.end(), address, AddressTableEntryLessThan());
        int lowerIndex = (lower - cachedAddressTable.begin());
        int upperIndex = (upper - cachedAddressTable.begin());
        bool inModel = (lower != upper);
<<<<<<< HEAD
        AddressTableEntry::Type newEntryType = translateTransactionType(purpose, isMine);
=======
        AddressTableEntry::Type newEntryType = isMine ? AddressTableEntry::Receiving : AddressTableEntry::Sending;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        switch(status)
        {
        case CT_NEW:
            if(inModel)
            {
<<<<<<< HEAD
                qWarning() << "AddressTablePriv::updateEntry: Warning: Got CT_NEW, but entry is already in model";
=======
                OutputDebugStringF("Warning: AddressTablePriv::updateEntry: Got CT_NOW, but entry is already in model\n");
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                break;
            }
            parent->beginInsertRows(QModelIndex(), lowerIndex, lowerIndex);
            cachedAddressTable.insert(lowerIndex, AddressTableEntry(newEntryType, label, address));
            parent->endInsertRows();
            break;
        case CT_UPDATED:
            if(!inModel)
            {
<<<<<<< HEAD
                qWarning() << "AddressTablePriv::updateEntry: Warning: Got CT_UPDATED, but entry is not in model";
=======
                OutputDebugStringF("Warning: AddressTablePriv::updateEntry: Got CT_UPDATED, but entry is not in model\n");
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                break;
            }
            lower->type = newEntryType;
            lower->label = label;
            parent->emitDataChanged(lowerIndex);
            break;
        case CT_DELETED:
            if(!inModel)
            {
<<<<<<< HEAD
                qWarning() << "AddressTablePriv::updateEntry: Warning: Got CT_DELETED, but entry is not in model";
=======
                OutputDebugStringF("Warning: AddressTablePriv::updateEntry: Got CT_DELETED, but entry is not in model\n");
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                break;
            }
            parent->beginRemoveRows(QModelIndex(), lowerIndex, upperIndex-1);
            cachedAddressTable.erase(lower, upper);
            parent->endRemoveRows();
            break;
        }
    }

    int size()
    {
        return cachedAddressTable.size();
    }

    AddressTableEntry *index(int idx)
    {
        if(idx >= 0 && idx < cachedAddressTable.size())
        {
            return &cachedAddressTable[idx];
        }
        else
        {
            return 0;
        }
    }
};

<<<<<<< HEAD
AddressTableModel::AddressTableModel(CWallet *_wallet, WalletModel *parent) :
    QAbstractTableModel(parent),walletModel(parent),wallet(_wallet),priv(0)
=======
AddressTableModel::AddressTableModel(CWallet *wallet, WalletModel *parent) :
    QAbstractTableModel(parent),walletModel(parent),wallet(wallet),priv(0)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    columns << tr("Label") << tr("Address");
    priv = new AddressTablePriv(wallet, this);
    priv->refreshAddressTable();
}

AddressTableModel::~AddressTableModel()
{
    delete priv;
}

int AddressTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return priv->size();
}

int AddressTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columns.length();
}

QVariant AddressTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    AddressTableEntry *rec = static_cast<AddressTableEntry*>(index.internalPointer());

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case Label:
            if(rec->label.isEmpty() && role == Qt::DisplayRole)
            {
                return tr("(no label)");
            }
            else
            {
                return rec->label;
            }
        case Address:
            return rec->address;
        }
    }
    else if (role == Qt::FontRole)
    {
        QFont font;
        if(index.column() == Address)
        {
<<<<<<< HEAD
            font = GUIUtil::fixedPitchFont();
=======
            font = GUIUtil::bitcoinAddressFont();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
        return font;
    }
    else if (role == TypeRole)
    {
        switch(rec->type)
        {
        case AddressTableEntry::Sending:
            return Send;
        case AddressTableEntry::Receiving:
            return Receive;
        default: break;
        }
    }
    return QVariant();
}

bool AddressTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    AddressTableEntry *rec = static_cast<AddressTableEntry*>(index.internalPointer());
<<<<<<< HEAD
    std::string strPurpose = (rec->type == AddressTableEntry::Sending ? "send" : "receive");
=======

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    editStatus = OK;

    if(role == Qt::EditRole)
    {
<<<<<<< HEAD
        LOCK(wallet->cs_wallet); /* For SetAddressBook / DelAddressBook */
        CTxDestination curAddress = CWiFicoinAddress(rec->address.toStdString()).Get();
        if(index.column() == Label)
        {
=======
        switch(index.column())
        {
        case Label:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            // Do nothing, if old label == new label
            if(rec->label == value.toString())
            {
                editStatus = NO_CHANGES;
                return false;
            }
<<<<<<< HEAD
            wallet->SetAddressBook(curAddress, value.toString().toStdString(), strPurpose);
        } else if(index.column() == Address) {
            CTxDestination newAddress = CWiFicoinAddress(value.toString().toStdString()).Get();
            // Refuse to set invalid address, set error status and return false
            if(boost::get<CNoDestination>(&newAddress))
            {
                editStatus = INVALID_ADDRESS;
                return false;
            }
            // Do nothing, if old address == new address
            else if(newAddress == curAddress)
            {
                editStatus = NO_CHANGES;
=======
            wallet->SetAddressBookName(CBitcoinAddress(rec->address.toStdString()).Get(), value.toString().toStdString());
            break;
        case Address:
            // Do nothing, if old address == new address
            if(CBitcoinAddress(rec->address.toStdString()) == CBitcoinAddress(value.toString().toStdString()))
            {
                editStatus = NO_CHANGES;
                return false;
            }
            // Refuse to set invalid address, set error status and return false
            else if(!walletModel->validateAddress(value.toString()))
            {
                editStatus = INVALID_ADDRESS;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                return false;
            }
            // Check for duplicate addresses to prevent accidental deletion of addresses, if you try
            // to paste an existing address over another address (with a different label)
<<<<<<< HEAD
            else if(wallet->mapAddressBook.count(newAddress))
=======
            else if(wallet->mapAddressBook.count(CBitcoinAddress(value.toString().toStdString()).Get()))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            {
                editStatus = DUPLICATE_ADDRESS;
                return false;
            }
            // Double-check that we're not overwriting a receiving address
            else if(rec->type == AddressTableEntry::Sending)
            {
<<<<<<< HEAD
                // Remove old entry
                wallet->DelAddressBook(curAddress);
                // Add new entry with new address
                wallet->SetAddressBook(newAddress, rec->label.toStdString(), strPurpose);
            }
=======
                {
                    LOCK(wallet->cs_wallet);
                    // Remove old entry
                    wallet->DelAddressBookName(CBitcoinAddress(rec->address.toStdString()).Get());
                    // Add new entry with new address
                    wallet->SetAddressBookName(CBitcoinAddress(value.toString().toStdString()).Get(), rec->label.toStdString());
                }
            }
            break;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
        return true;
    }
    return false;
}

QVariant AddressTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
<<<<<<< HEAD
        if(role == Qt::DisplayRole && section < columns.size())
=======
        if(role == Qt::DisplayRole)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        {
            return columns[section];
        }
    }
    return QVariant();
}

Qt::ItemFlags AddressTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;
    AddressTableEntry *rec = static_cast<AddressTableEntry*>(index.internalPointer());

    Qt::ItemFlags retval = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    // Can edit address and label for sending addresses,
    // and only label for receiving addresses.
    if(rec->type == AddressTableEntry::Sending ||
      (rec->type == AddressTableEntry::Receiving && index.column()==Label))
    {
        retval |= Qt::ItemIsEditable;
    }
    return retval;
}

QModelIndex AddressTableModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    AddressTableEntry *data = priv->index(row);
    if(data)
    {
        return createIndex(row, column, priv->index(row));
    }
    else
    {
        return QModelIndex();
    }
}

<<<<<<< HEAD
void AddressTableModel::updateEntry(const QString &address,
        const QString &label, bool isMine, const QString &purpose, int status)
{
    // Update address book model from WiFicoin core
    priv->updateEntry(address, label, isMine, purpose, status);
=======
void AddressTableModel::updateEntry(const QString &address, const QString &label, bool isMine, int status)
{
    // Update address book model from Bitcoin core
    priv->updateEntry(address, label, isMine, status);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

QString AddressTableModel::addRow(const QString &type, const QString &label, const QString &address)
{
    std::string strLabel = label.toStdString();
    std::string strAddress = address.toStdString();

    editStatus = OK;

    if(type == Send)
    {
        if(!walletModel->validateAddress(address))
        {
            editStatus = INVALID_ADDRESS;
            return QString();
        }
        // Check for duplicate addresses
        {
            LOCK(wallet->cs_wallet);
<<<<<<< HEAD
            if(wallet->mapAddressBook.count(CWiFicoinAddress(strAddress).Get()))
=======
            if(wallet->mapAddressBook.count(CBitcoinAddress(strAddress).Get()))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            {
                editStatus = DUPLICATE_ADDRESS;
                return QString();
            }
        }
    }
    else if(type == Receive)
    {
        // Generate a new address to associate with given label
<<<<<<< HEAD
        CPubKey newKey;
        if(!wallet->GetKeyFromPool(newKey))
        {
            WalletModel::UnlockContext ctx(walletModel->requestUnlock());
            if(!ctx.isValid())
            {
                // Unlock wallet failed or was cancelled
                editStatus = WALLET_UNLOCK_FAILURE;
                return QString();
            }
            if(!wallet->GetKeyFromPool(newKey))
            {
                editStatus = KEY_GENERATION_FAILURE;
                return QString();
            }
        }
        strAddress = CWiFicoinAddress(newKey.GetID()).ToString();
=======
        WalletModel::UnlockContext ctx(walletModel->requestUnlock());
        if(!ctx.isValid())
        {
            // Unlock wallet failed or was cancelled
            editStatus = WALLET_UNLOCK_FAILURE;
            return QString();
        }
        CPubKey newKey;
        if(!wallet->GetKeyFromPool(newKey, true))
        {
            editStatus = KEY_GENERATION_FAILURE;
            return QString();
        }
        strAddress = CBitcoinAddress(newKey.GetID()).ToString();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
    else
    {
        return QString();
    }

    // Add entry
    {
        LOCK(wallet->cs_wallet);
<<<<<<< HEAD
        wallet->SetAddressBook(CWiFicoinAddress(strAddress).Get(), strLabel,
                               (type == Send ? "send" : "receive"));
=======
        wallet->SetAddressBookName(CBitcoinAddress(strAddress).Get(), strLabel);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
    return QString::fromStdString(strAddress);
}

bool AddressTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    AddressTableEntry *rec = priv->index(row);
    if(count != 1 || !rec || rec->type == AddressTableEntry::Receiving)
    {
        // Can only remove one row at a time, and cannot remove rows not in model.
        // Also refuse to remove receiving addresses.
        return false;
    }
    {
        LOCK(wallet->cs_wallet);
<<<<<<< HEAD
        wallet->DelAddressBook(CWiFicoinAddress(rec->address.toStdString()).Get());
=======
        wallet->DelAddressBookName(CBitcoinAddress(rec->address.toStdString()).Get());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
    return true;
}

/* Look up label for address in address book, if not found return empty string.
 */
QString AddressTableModel::labelForAddress(const QString &address) const
{
    {
        LOCK(wallet->cs_wallet);
<<<<<<< HEAD
        CWiFicoinAddress address_parsed(address.toStdString());
        std::map<CTxDestination, CAddressBookData>::iterator mi = wallet->mapAddressBook.find(address_parsed.Get());
        if (mi != wallet->mapAddressBook.end())
        {
            return QString::fromStdString(mi->second.name);
=======
        CBitcoinAddress address_parsed(address.toStdString());
        std::map<CTxDestination, std::string>::iterator mi = wallet->mapAddressBook.find(address_parsed.Get());
        if (mi != wallet->mapAddressBook.end())
        {
            return QString::fromStdString(mi->second);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
    }
    return QString();
}

int AddressTableModel::lookupAddress(const QString &address) const
{
    QModelIndexList lst = match(index(0, Address, QModelIndex()),
                                Qt::EditRole, address, 1, Qt::MatchExactly);
    if(lst.isEmpty())
    {
        return -1;
    }
    else
    {
        return lst.at(0).row();
    }
}

void AddressTableModel::emitDataChanged(int idx)
{
<<<<<<< HEAD
    Q_EMIT dataChanged(index(idx, 0, QModelIndex()), index(idx, columns.length()-1, QModelIndex()));
=======
    emit dataChanged(index(idx, 0, QModelIndex()), index(idx, columns.length()-1, QModelIndex()));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
