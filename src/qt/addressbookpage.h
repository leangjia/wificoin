<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_ADDRESSBOOKPAGE_H
#define WIFICOIN_QT_ADDRESSBOOKPAGE_H

#include <QDialog>

class AddressTableModel;
class PlatformStyle;

namespace Ui {
    class AddressBookPage;
}

QT_BEGIN_NAMESPACE
class QItemSelection;
class QMenu;
class QModelIndex;
class QSortFilterProxyModel;
=======
#ifndef ADDRESSBOOKPAGE_H
#define ADDRESSBOOKPAGE_H

#include <QDialog>

namespace Ui {
    class AddressBookPage;
}
class AddressTableModel;
class OptionsModel;

QT_BEGIN_NAMESPACE
class QTableView;
class QItemSelection;
class QSortFilterProxyModel;
class QMenu;
class QModelIndex;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
QT_END_NAMESPACE

/** Widget that shows a list of sending or receiving addresses.
  */
class AddressBookPage : public QDialog
{
    Q_OBJECT

public:
    enum Tabs {
        SendingTab = 0,
        ReceivingTab = 1
    };

    enum Mode {
<<<<<<< HEAD
        ForSelection, /**< Open address book to pick address */
        ForEditing  /**< Open address book for editing */
    };

    explicit AddressBookPage(const PlatformStyle *platformStyle, Mode mode, Tabs tab, QWidget *parent);
    ~AddressBookPage();

    void setModel(AddressTableModel *model);
    const QString &getReturnValue() const { return returnValue; }

public Q_SLOTS:
=======
        ForSending, /**< Open address book to pick address for sending */
        ForEditing  /**< Open address book for editing */
    };

    explicit AddressBookPage(Mode mode, Tabs tab, QWidget *parent = 0);
    ~AddressBookPage();

    void setModel(AddressTableModel *model);
    void setOptionsModel(OptionsModel *optionsModel);
    const QString &getReturnValue() const { return returnValue; }

public slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void done(int retval);

private:
    Ui::AddressBookPage *ui;
    AddressTableModel *model;
<<<<<<< HEAD
=======
    OptionsModel *optionsModel;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    Mode mode;
    Tabs tab;
    QString returnValue;
    QSortFilterProxyModel *proxyModel;
    QMenu *contextMenu;
    QAction *deleteAction; // to be able to explicitly disable it
    QString newAddressToSelect;

<<<<<<< HEAD
private Q_SLOTS:
=======
private slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Delete currently selected address entry */
    void on_deleteAddress_clicked();
    /** Create a new address for receiving coins and / or add a new address book entry */
    void on_newAddress_clicked();
    /** Copy address of currently selected address entry to clipboard */
    void on_copyAddress_clicked();
<<<<<<< HEAD
=======
    /** Open the sign message tab in the Sign/Verify Message dialog with currently selected address */
    void on_signMessage_clicked();
    /** Open the verify message tab in the Sign/Verify Message dialog with currently selected address */
    void on_verifyMessage_clicked();
    /** Open send coins dialog for currently selected address (no button) */
    void onSendCoinsAction();
    /** Generate a QR Code from the currently selected address */
    void on_showQRCode_clicked();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Copy label of currently selected address entry to clipboard (no button) */
    void onCopyLabelAction();
    /** Edit currently selected address entry (no button) */
    void onEditAction();
    /** Export button clicked */
    void on_exportButton_clicked();

    /** Set button states based on selected tab and selection */
    void selectionChanged();
    /** Spawn contextual menu (right mouse menu) for address book entry */
    void contextualMenu(const QPoint &point);
    /** New entry/entries were added to address table */
    void selectNewAddress(const QModelIndex &parent, int begin, int /*end*/);

<<<<<<< HEAD
Q_SIGNALS:
    void sendCoins(QString addr);
};

#endif // WIFICOIN_QT_ADDRESSBOOKPAGE_H
=======
signals:
    void signMessage(QString addr);
    void verifyMessage(QString addr);
    void sendCoins(QString addr);
};

#endif // ADDRESSBOOKPAGE_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
