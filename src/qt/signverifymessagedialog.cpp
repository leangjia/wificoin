<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#include "signverifymessagedialog.h"
#include "ui_signverifymessagedialog.h"

#include "addressbookpage.h"
<<<<<<< HEAD
#include "guiutil.h"
#include "platformstyle.h"
#include "walletmodel.h"

#include "base58.h"
#include "init.h"
#include "validation.h" // For strMessageMagic
#include "wallet/wallet.h"
=======
#include "base58.h"
#include "guiutil.h"
#include "init.h"
#include "main.h"
#include "optionsmodel.h"
#include "walletmodel.h"
#include "wallet.h"

#include <QClipboard>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <string>
#include <vector>

<<<<<<< HEAD
#include <QClipboard>

SignVerifyMessageDialog::SignVerifyMessageDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignVerifyMessageDialog),
    model(0),
    platformStyle(_platformStyle)
{
    ui->setupUi(this);

    ui->addressBookButton_SM->setIcon(platformStyle->SingleColorIcon(":/icons/address-book"));
    ui->pasteButton_SM->setIcon(platformStyle->SingleColorIcon(":/icons/editpaste"));
    ui->copySignatureButton_SM->setIcon(platformStyle->SingleColorIcon(":/icons/editcopy"));
    ui->signMessageButton_SM->setIcon(platformStyle->SingleColorIcon(":/icons/edit"));
    ui->clearButton_SM->setIcon(platformStyle->SingleColorIcon(":/icons/remove"));
    ui->addressBookButton_VM->setIcon(platformStyle->SingleColorIcon(":/icons/address-book"));
    ui->verifyMessageButton_VM->setIcon(platformStyle->SingleColorIcon(":/icons/transaction_0"));
    ui->clearButton_VM->setIcon(platformStyle->SingleColorIcon(":/icons/remove"));

#if QT_VERSION >= 0x040700
    ui->signatureOut_SM->setPlaceholderText(tr("Click \"Sign Message\" to generate signature"));
=======
SignVerifyMessageDialog::SignVerifyMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignVerifyMessageDialog),
    model(0)
{
    ui->setupUi(this);

#if (QT_VERSION >= 0x040700)
    /* Do not move this to the XML file, Qt before 4.7 will choke on it */
    ui->addressIn_SM->setPlaceholderText(tr("Enter a Bitcoin address (e.g. 1NS17iag9jJgTHD1VXjvLCEnZuQ3rJDE9L)"));
    ui->signatureOut_SM->setPlaceholderText(tr("Click \"Sign Message\" to generate signature"));

    ui->addressIn_VM->setPlaceholderText(tr("Enter a Bitcoin address (e.g. 1NS17iag9jJgTHD1VXjvLCEnZuQ3rJDE9L)"));
    ui->signatureIn_VM->setPlaceholderText(tr("Enter Bitcoin signature"));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#endif

    GUIUtil::setupAddressWidget(ui->addressIn_SM, this);
    GUIUtil::setupAddressWidget(ui->addressIn_VM, this);

    ui->addressIn_SM->installEventFilter(this);
    ui->messageIn_SM->installEventFilter(this);
    ui->signatureOut_SM->installEventFilter(this);
    ui->addressIn_VM->installEventFilter(this);
    ui->messageIn_VM->installEventFilter(this);
    ui->signatureIn_VM->installEventFilter(this);

<<<<<<< HEAD
    ui->signatureOut_SM->setFont(GUIUtil::fixedPitchFont());
    ui->signatureIn_VM->setFont(GUIUtil::fixedPitchFont());
=======
    ui->signatureOut_SM->setFont(GUIUtil::bitcoinAddressFont());
    ui->signatureIn_VM->setFont(GUIUtil::bitcoinAddressFont());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

SignVerifyMessageDialog::~SignVerifyMessageDialog()
{
    delete ui;
}

<<<<<<< HEAD
void SignVerifyMessageDialog::setModel(WalletModel *_model)
{
    this->model = _model;
=======
void SignVerifyMessageDialog::setModel(WalletModel *model)
{
    this->model = model;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void SignVerifyMessageDialog::setAddress_SM(const QString &address)
{
    ui->addressIn_SM->setText(address);
    ui->messageIn_SM->setFocus();
}

void SignVerifyMessageDialog::setAddress_VM(const QString &address)
{
    ui->addressIn_VM->setText(address);
    ui->messageIn_VM->setFocus();
}

void SignVerifyMessageDialog::showTab_SM(bool fShow)
{
    ui->tabWidget->setCurrentIndex(0);
<<<<<<< HEAD
=======

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    if (fShow)
        this->show();
}

void SignVerifyMessageDialog::showTab_VM(bool fShow)
{
    ui->tabWidget->setCurrentIndex(1);
    if (fShow)
        this->show();
}

void SignVerifyMessageDialog::on_addressBookButton_SM_clicked()
{
    if (model && model->getAddressTableModel())
    {
<<<<<<< HEAD
        AddressBookPage dlg(platformStyle, AddressBookPage::ForSelection, AddressBookPage::ReceivingTab, this);
=======
        AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::ReceivingTab, this);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        dlg.setModel(model->getAddressTableModel());
        if (dlg.exec())
        {
            setAddress_SM(dlg.getReturnValue());
        }
    }
}

void SignVerifyMessageDialog::on_pasteButton_SM_clicked()
{
    setAddress_SM(QApplication::clipboard()->text());
}

void SignVerifyMessageDialog::on_signMessageButton_SM_clicked()
{
<<<<<<< HEAD
    if (!model)
        return;

    /* Clear old signature to ensure users don't get confused on error with an old signature displayed */
    ui->signatureOut_SM->clear();

    CWiFicoinAddress addr(ui->addressIn_SM->text().toStdString());
    if (!addr.IsValid())
    {
=======
    /* Clear old signature to ensure users don't get confused on error with an old signature displayed */
    ui->signatureOut_SM->clear();

    CBitcoinAddress addr(ui->addressIn_SM->text().toStdString());
    if (!addr.IsValid())
    {
        ui->addressIn_SM->setValid(false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    CKeyID keyID;
    if (!addr.GetKeyID(keyID))
    {
        ui->addressIn_SM->setValid(false);
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    WalletModel::UnlockContext ctx(model->requestUnlock());
    if (!ctx.isValid())
    {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("Wallet unlock was cancelled."));
        return;
    }

    CKey key;
<<<<<<< HEAD
    if (!model->getPrivKey(keyID, key))
=======
    if (!pwalletMain->GetKey(keyID, key))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("Private key for the entered address is not available."));
        return;
    }

<<<<<<< HEAD
    CHashWriter ss(SER_GETHASH, 0);
=======
    CDataStream ss(SER_GETHASH, 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ss << strMessageMagic;
    ss << ui->messageIn_SM->document()->toPlainText().toStdString();

    std::vector<unsigned char> vchSig;
<<<<<<< HEAD
    if (!key.SignCompact(ss.GetHash(), vchSig))
=======
    if (!key.SignCompact(Hash(ss.begin(), ss.end()), vchSig))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(QString("<nobr>") + tr("Message signing failed.") + QString("</nobr>"));
        return;
    }

    ui->statusLabel_SM->setStyleSheet("QLabel { color: green; }");
    ui->statusLabel_SM->setText(QString("<nobr>") + tr("Message signed.") + QString("</nobr>"));

    ui->signatureOut_SM->setText(QString::fromStdString(EncodeBase64(&vchSig[0], vchSig.size())));
}

void SignVerifyMessageDialog::on_copySignatureButton_SM_clicked()
{
<<<<<<< HEAD
    GUIUtil::setClipboard(ui->signatureOut_SM->text());
=======
    QApplication::clipboard()->setText(ui->signatureOut_SM->text());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void SignVerifyMessageDialog::on_clearButton_SM_clicked()
{
    ui->addressIn_SM->clear();
    ui->messageIn_SM->clear();
    ui->signatureOut_SM->clear();
    ui->statusLabel_SM->clear();

    ui->addressIn_SM->setFocus();
}

void SignVerifyMessageDialog::on_addressBookButton_VM_clicked()
{
    if (model && model->getAddressTableModel())
    {
<<<<<<< HEAD
        AddressBookPage dlg(platformStyle, AddressBookPage::ForSelection, AddressBookPage::SendingTab, this);
=======
        AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::SendingTab, this);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        dlg.setModel(model->getAddressTableModel());
        if (dlg.exec())
        {
            setAddress_VM(dlg.getReturnValue());
        }
    }
}

void SignVerifyMessageDialog::on_verifyMessageButton_VM_clicked()
{
<<<<<<< HEAD
    CWiFicoinAddress addr(ui->addressIn_VM->text().toStdString());
    if (!addr.IsValid())
    {
=======
    CBitcoinAddress addr(ui->addressIn_VM->text().toStdString());
    if (!addr.IsValid())
    {
        ui->addressIn_VM->setValid(false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    CKeyID keyID;
    if (!addr.GetKeyID(keyID))
    {
        ui->addressIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    bool fInvalid = false;
    std::vector<unsigned char> vchSig = DecodeBase64(ui->signatureIn_VM->text().toStdString().c_str(), &fInvalid);

    if (fInvalid)
    {
        ui->signatureIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The signature could not be decoded.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

<<<<<<< HEAD
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_VM->document()->toPlainText().toStdString();

    CPubKey pubkey;
    if (!pubkey.RecoverCompact(ss.GetHash(), vchSig))
=======
    CDataStream ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_VM->document()->toPlainText().toStdString();

    CKey key;
    if (!key.SetCompactSignature(Hash(ss.begin(), ss.end()), vchSig))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        ui->signatureIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The signature did not match the message digest.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

<<<<<<< HEAD
    if (!(CWiFicoinAddress(pubkey.GetID()) == addr))
=======
    if (!(CBitcoinAddress(key.GetPubKey().GetID()) == addr))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verification failed.") + QString("</nobr>"));
        return;
    }

    ui->statusLabel_VM->setStyleSheet("QLabel { color: green; }");
    ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verified.") + QString("</nobr>"));
}

void SignVerifyMessageDialog::on_clearButton_VM_clicked()
{
    ui->addressIn_VM->clear();
    ui->signatureIn_VM->clear();
    ui->messageIn_VM->clear();
    ui->statusLabel_VM->clear();

    ui->addressIn_VM->setFocus();
}

bool SignVerifyMessageDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::FocusIn)
    {
        if (ui->tabWidget->currentIndex() == 0)
        {
            /* Clear status message on focus change */
            ui->statusLabel_SM->clear();

            /* Select generated signature */
            if (object == ui->signatureOut_SM)
            {
                ui->signatureOut_SM->selectAll();
                return true;
            }
        }
        else if (ui->tabWidget->currentIndex() == 1)
        {
            /* Clear status message on focus change */
            ui->statusLabel_VM->clear();
        }
    }
    return QDialog::eventFilter(object, event);
}
