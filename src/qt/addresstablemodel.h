<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_ADDRESSTABLEMODEL_H
#define WIFICOIN_QT_ADDRESSTABLEMODEL_H
=======
#ifndef ADDRESSTABLEMODEL_H
#define ADDRESSTABLEMODEL_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <QAbstractTableModel>
#include <QStringList>

class AddressTablePriv;
<<<<<<< HEAD
class WalletModel;

class CWallet;
=======
class CWallet;
class WalletModel;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

/**
   Qt model of the address book in the core. This allows views to access and modify the address book.
 */
class AddressTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AddressTableModel(CWallet *wallet, WalletModel *parent = 0);
    ~AddressTableModel();

    enum ColumnIndex {
        Label = 0,   /**< User specified label */
<<<<<<< HEAD
        Address = 1  /**< WiFicoin address */
=======
        Address = 1  /**< Bitcoin address */
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    };

    enum RoleIndex {
        TypeRole = Qt::UserRole /**< Type of address (#Send or #Receive) */
    };

    /** Return status of edit/insert operation */
    enum EditStatus {
        OK,                     /**< Everything ok */
        NO_CHANGES,             /**< No changes were made during edit operation */
        INVALID_ADDRESS,        /**< Unparseable address */
        DUPLICATE_ADDRESS,      /**< Address already in address book */
        WALLET_UNLOCK_FAILURE,  /**< Wallet could not be unlocked to create new receiving address */
        KEY_GENERATION_FAILURE  /**< Generating a new public key for a receiving address failed */
    };

    static const QString Send;      /**< Specifies send address */
    static const QString Receive;   /**< Specifies receive address */

    /** @name Methods overridden from QAbstractTableModel
        @{*/
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex &index) const;
    /*@}*/

    /* Add an address to the model.
       Returns the added address on success, and an empty string otherwise.
     */
    QString addRow(const QString &type, const QString &label, const QString &address);

    /* Look up label for address in address book, if not found return empty string.
     */
    QString labelForAddress(const QString &address) const;

    /* Look up row index of an address in the model.
       Return -1 if not found.
     */
    int lookupAddress(const QString &address) const;

    EditStatus getEditStatus() const { return editStatus; }

private:
    WalletModel *walletModel;
    CWallet *wallet;
    AddressTablePriv *priv;
    QStringList columns;
    EditStatus editStatus;

    /** Notify listeners that data changed. */
    void emitDataChanged(int index);

<<<<<<< HEAD
public Q_SLOTS:
    /* Update address list from core.
     */
    void updateEntry(const QString &address, const QString &label, bool isMine, const QString &purpose, int status);
=======
signals:
    void defaultAddressChanged(const QString &address);

public slots:
    /* Update address list from core.
     */
    void updateEntry(const QString &address, const QString &label, bool isMine, int status);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    friend class AddressTablePriv;
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_ADDRESSTABLEMODEL_H
=======
#endif // ADDRESSTABLEMODEL_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
