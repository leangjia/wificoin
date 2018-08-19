<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_WALLETVIEW_H
#define WIFICOIN_QT_WALLETVIEW_H

#include "amount.h"

#include <QStackedWidget>

class WiFicoinGUI;
class ClientModel;
class OverviewPage;
class PlatformStyle;
class ReceiveCoinsDialog;
class SendCoinsDialog;
class SendCoinsRecipient;
class TransactionView;
class WalletModel;
class AddressBookPage;

QT_BEGIN_NAMESPACE
class QModelIndex;
class QProgressDialog;
=======
/*
 * Qt4 bitcoin GUI.
 *
 * W.J. van der Laan 2011-2012
 * The Bitcoin Developers 2011-2013
 */
#ifndef WALLETVIEW_H
#define WALLETVIEW_H

#include <QStackedWidget>

class BitcoinGUI;
class ClientModel;
class WalletModel;
class TransactionView;
class OverviewPage;
class AddressBookPage;
class SendCoinsDialog;
class SignVerifyMessageDialog;
class RPCConsole;

QT_BEGIN_NAMESPACE
class QLabel;
class QModelIndex;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
QT_END_NAMESPACE

/*
  WalletView class. This class represents the view to a single wallet.
  It was added to support multiple wallet functionality. Each wallet gets its own WalletView instance.
  It communicates with both the client and the wallet models to give the user an up-to-date view of the
  current core state.
*/
class WalletView : public QStackedWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit WalletView(const PlatformStyle *platformStyle, QWidget *parent);
    ~WalletView();

    void setWiFicoinGUI(WiFicoinGUI *gui);
=======
    explicit WalletView(QWidget *parent, BitcoinGUI *_gui);
    ~WalletView();

    void setBitcoinGUI(BitcoinGUI *gui);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Set the client model.
        The client model represents the part of the core that communicates with the P2P network, and is wallet-agnostic.
    */
    void setClientModel(ClientModel *clientModel);
    /** Set the wallet model.
<<<<<<< HEAD
        The wallet model represents a wificoin wallet, and offers access to the list of transactions, address book and sending
=======
        The wallet model represents a bitcoin wallet, and offers access to the list of transactions, address book and sending
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        functionality.
    */
    void setWalletModel(WalletModel *walletModel);

<<<<<<< HEAD
    bool handlePaymentRequest(const SendCoinsRecipient& recipient);
=======
    bool handleURI(const QString &uri);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    void showOutOfSyncWarning(bool fShow);

private:
<<<<<<< HEAD
=======
    BitcoinGUI *gui;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ClientModel *clientModel;
    WalletModel *walletModel;

    OverviewPage *overviewPage;
    QWidget *transactionsPage;
<<<<<<< HEAD
    ReceiveCoinsDialog *receiveCoinsPage;
    SendCoinsDialog *sendCoinsPage;
    AddressBookPage *usedSendingAddressesPage;
    AddressBookPage *usedReceivingAddressesPage;

    TransactionView *transactionView;

    QProgressDialog *progressDialog;
    const PlatformStyle *platformStyle;

public Q_SLOTS:
=======
    AddressBookPage *addressBookPage;
    AddressBookPage *receiveCoinsPage;
    SendCoinsDialog *sendCoinsPage;
    SignVerifyMessageDialog *signVerifyMessageDialog;

    TransactionView *transactionView;

public slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Switch to overview (home) page */
    void gotoOverviewPage();
    /** Switch to history (transactions) page */
    void gotoHistoryPage();
<<<<<<< HEAD
=======
    /** Switch to address book page */
    void gotoAddressBookPage();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Switch to receive coins page */
    void gotoReceiveCoinsPage();
    /** Switch to send coins page */
    void gotoSendCoinsPage(QString addr = "");

    /** Show Sign/Verify Message dialog and switch to sign message tab */
    void gotoSignMessageTab(QString addr = "");
    /** Show Sign/Verify Message dialog and switch to verify message tab */
    void gotoVerifyMessageTab(QString addr = "");

    /** Show incoming transaction notification for new transactions.

        The new items are those between start and end inclusive, under the given parent item.
    */
<<<<<<< HEAD
    void processNewTransaction(const QModelIndex& parent, int start, int /*end*/);
=======
    void incomingTransaction(const QModelIndex& parent, int start, int /*end*/);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Encrypt the wallet */
    void encryptWallet(bool status);
    /** Backup the wallet */
    void backupWallet();
    /** Change encrypted wallet passphrase */
    void changePassphrase();
    /** Ask for passphrase to unlock wallet temporarily */
    void unlockWallet();

<<<<<<< HEAD
    /** Show used sending addresses */
    void usedSendingAddresses();
    /** Show used receiving addresses */
    void usedReceivingAddresses();

    /** Re-emit encryption status signal */
    void updateEncryptionStatus();

    /** Show progress dialog e.g. for rescan */
    void showProgress(const QString &title, int nProgress);

    /** User has requested more information about the out of sync state */
    void requestedSyncWarningInfo();

Q_SIGNALS:
    /** Signal that we want to show the main window */
    void showNormalIfMinimized();
    /**  Fired when a message should be reported to the user */
    void message(const QString &title, const QString &message, unsigned int style);
    /** Encryption status of wallet changed */
    void encryptionStatusChanged(int status);
    /** HD-Enabled status of wallet changed (only possible during startup) */
    void hdEnabledStatusChanged(int hdEnabled);
    /** Notify that a new transaction appeared */
    void incomingTransaction(const QString& date, int unit, const CAmount& amount, const QString& type, const QString& address, const QString& label);
    /** Notify that the out of sync warning icon has been pressed */
    void outOfSyncWarningClicked();
};

#endif // WIFICOIN_QT_WALLETVIEW_H
=======
    void setEncryptionStatus();

signals:
    /** Signal that we want to show the main window */
    void showNormalIfMinimized();
};

#endif // WALLETVIEW_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
