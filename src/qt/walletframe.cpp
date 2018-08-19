<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "walletframe.h"

#include "wificoingui.h"
#include "walletview.h"

#include <cstdio>

#include <QHBoxLayout>
#include <QLabel>

WalletFrame::WalletFrame(const PlatformStyle *_platformStyle, WiFicoinGUI *_gui) :
    QFrame(_gui),
    gui(_gui),
    platformStyle(_platformStyle)
=======
/*
 * Qt4 bitcoin GUI.
 *
 * W.J. van der Laan 2011-2012
 * The Bitcoin Developers 2011-2013
 */
#include "walletframe.h"
#include "bitcoingui.h"
#include "walletstack.h"

#include <QVBoxLayout>
#include <QMessageBox>

#include <stdio.h>

WalletFrame::WalletFrame(BitcoinGUI *_gui) :
    QFrame(_gui),
    gui(_gui),
    clientModel(0)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    // Leave HBox hook for adding a list view later
    QHBoxLayout *walletFrameLayout = new QHBoxLayout(this);
    setContentsMargins(0,0,0,0);
<<<<<<< HEAD
    walletStack = new QStackedWidget(this);
    walletFrameLayout->setContentsMargins(0,0,0,0);
    walletFrameLayout->addWidget(walletStack);

    QLabel *noWallet = new QLabel(tr("No wallet has been loaded."));
    noWallet->setAlignment(Qt::AlignCenter);
    walletStack->addWidget(noWallet);
=======
    walletStack = new WalletStack(this);
    walletStack->setBitcoinGUI(gui);
    walletFrameLayout->setContentsMargins(0,0,0,0);
    walletFrameLayout->addWidget(walletStack);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

WalletFrame::~WalletFrame()
{
}

<<<<<<< HEAD
void WalletFrame::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;
=======
void WalletFrame::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
    walletStack->setClientModel(clientModel);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

bool WalletFrame::addWallet(const QString& name, WalletModel *walletModel)
{
<<<<<<< HEAD
    if (!gui || !clientModel || !walletModel || mapWalletViews.count(name) > 0)
        return false;

    WalletView *walletView = new WalletView(platformStyle, this);
    walletView->setWiFicoinGUI(gui);
    walletView->setClientModel(clientModel);
    walletView->setWalletModel(walletModel);
    walletView->showOutOfSyncWarning(bOutOfSync);

     /* TODO we should goto the currently selected page once dynamically adding wallets is supported */
    walletView->gotoOverviewPage();
    walletStack->addWidget(walletView);
    mapWalletViews[name] = walletView;

    // Ensure a walletView is able to show the main window
    connect(walletView, SIGNAL(showNormalIfMinimized()), gui, SLOT(showNormalIfMinimized()));

    connect(walletView, SIGNAL(outOfSyncWarningClicked()), this, SLOT(outOfSyncWarningClicked()));

    return true;
=======
    return walletStack->addWallet(name, walletModel);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

bool WalletFrame::setCurrentWallet(const QString& name)
{
<<<<<<< HEAD
    if (mapWalletViews.count(name) == 0)
        return false;

    WalletView *walletView = mapWalletViews.value(name);
    walletStack->setCurrentWidget(walletView);
    walletView->updateEncryptionStatus();
    return true;
}

bool WalletFrame::removeWallet(const QString &name)
{
    if (mapWalletViews.count(name) == 0)
        return false;

    WalletView *walletView = mapWalletViews.take(name);
    walletStack->removeWidget(walletView);
=======
    // TODO: Check if valid name
    walletStack->setCurrentWallet(name);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    return true;
}

void WalletFrame::removeAllWallets()
{
<<<<<<< HEAD
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        walletStack->removeWidget(i.value());
    mapWalletViews.clear();
}

bool WalletFrame::handlePaymentRequest(const SendCoinsRecipient &recipient)
{
    WalletView *walletView = currentWalletView();
    if (!walletView)
        return false;

    return walletView->handlePaymentRequest(recipient);
=======
    walletStack->removeAllWallets();
}

bool WalletFrame::handleURI(const QString &uri)
{
    return walletStack->handleURI(uri);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::showOutOfSyncWarning(bool fShow)
{
<<<<<<< HEAD
    bOutOfSync = fShow;
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->showOutOfSyncWarning(fShow);
=======
    if (!walletStack) {
        QMessageBox box;
        box.setText("walletStack is null");
        box.exec();
        return;
    }
    walletStack->showOutOfSyncWarning(fShow);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::gotoOverviewPage()
{
<<<<<<< HEAD
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoOverviewPage();
=======
    walletStack->gotoOverviewPage();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::gotoHistoryPage()
{
<<<<<<< HEAD
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoHistoryPage();
=======
    walletStack->gotoHistoryPage();
}

void WalletFrame::gotoAddressBookPage()
{
    walletStack->gotoAddressBookPage();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::gotoReceiveCoinsPage()
{
<<<<<<< HEAD
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoReceiveCoinsPage();
=======
    walletStack->gotoReceiveCoinsPage();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::gotoSendCoinsPage(QString addr)
{
<<<<<<< HEAD
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoSendCoinsPage(addr);
=======
    walletStack->gotoSendCoinsPage(addr);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::gotoSignMessageTab(QString addr)
{
<<<<<<< HEAD
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->gotoSignMessageTab(addr);
=======
    walletStack->gotoSignMessageTab(addr);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::gotoVerifyMessageTab(QString addr)
{
<<<<<<< HEAD
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->gotoVerifyMessageTab(addr);
=======
    walletStack->gotoSignMessageTab(addr);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::encryptWallet(bool status)
{
<<<<<<< HEAD
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->encryptWallet(status);
=======
    walletStack->encryptWallet(status);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::backupWallet()
{
<<<<<<< HEAD
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->backupWallet();
=======
    walletStack->backupWallet();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::changePassphrase()
{
<<<<<<< HEAD
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->changePassphrase();
=======
    walletStack->changePassphrase();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void WalletFrame::unlockWallet()
{
<<<<<<< HEAD
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->unlockWallet();
}

void WalletFrame::usedSendingAddresses()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->usedSendingAddresses();
}

void WalletFrame::usedReceivingAddresses()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->usedReceivingAddresses();
}

WalletView *WalletFrame::currentWalletView()
{
    return qobject_cast<WalletView*>(walletStack->currentWidget());
}

void WalletFrame::outOfSyncWarningClicked()
{
    Q_EMIT requestedSyncWarningInfo();
=======
    walletStack->unlockWallet();
}

void WalletFrame::setEncryptionStatus()
{
    walletStack->setEncryptionStatus();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
