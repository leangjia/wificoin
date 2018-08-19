<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_SENDCOINSDIALOG_H
#define WIFICOIN_QT_SENDCOINSDIALOG_H

#include "walletmodel.h"

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>

class ClientModel;
class PlatformStyle;
class SendCoinsEntry;
class SendCoinsRecipient;
=======
#ifndef SENDCOINSDIALOG_H
#define SENDCOINSDIALOG_H

#include <QDialog>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

namespace Ui {
    class SendCoinsDialog;
}
<<<<<<< HEAD
=======
class WalletModel;
class SendCoinsEntry;
class SendCoinsRecipient;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

<<<<<<< HEAD
/** Dialog for sending wificoins */
=======
/** Dialog for sending bitcoins */
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
class SendCoinsDialog : public QDialog
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit SendCoinsDialog(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~SendCoinsDialog();

    void setClientModel(ClientModel *clientModel);
=======
    explicit SendCoinsDialog(QWidget *parent = 0);
    ~SendCoinsDialog();

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void setModel(WalletModel *model);

    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases (issue https://bugreports.qt-project.org/browse/QTBUG-10907).
     */
    QWidget *setupTabChain(QWidget *prev);

    void setAddress(const QString &address);
    void pasteEntry(const SendCoinsRecipient &rv);
<<<<<<< HEAD
    bool handlePaymentRequest(const SendCoinsRecipient &recipient);

public Q_SLOTS:
=======
    bool handleURI(const QString &uri);

public slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void clear();
    void reject();
    void accept();
    SendCoinsEntry *addEntry();
<<<<<<< HEAD
    void updateTabsAndLabels();
    void setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance,
                    const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);

private:
    Ui::SendCoinsDialog *ui;
    ClientModel *clientModel;
    WalletModel *model;
    bool fNewRecipientAllowed;
    bool fFeeMinimized;
    const PlatformStyle *platformStyle;

    // Process WalletModel::SendCoinsReturn and generate a pair consisting
    // of a message and message flags for use in Q_EMIT message().
    // Additional parameter msgArg can be used via .arg(msgArg).
    void processSendCoinsReturn(const WalletModel::SendCoinsReturn &sendCoinsReturn, const QString &msgArg = QString());
    void minimizeFeeSection(bool fMinimize);
    void updateFeeMinimizedLabel();
    // Update the passed in CCoinControl with state from the GUI
    void updateCoinControlState(CCoinControl& ctrl);

private Q_SLOTS:
    void on_sendButton_clicked();
    void on_buttonChooseFee_clicked();
    void on_buttonMinimizeFee_clicked();
    void removeEntry(SendCoinsEntry* entry);
    void updateDisplayUnit();
    void coinControlFeatureChanged(bool);
    void coinControlButtonClicked();
    void coinControlChangeChecked(int);
    void coinControlChangeEdited(const QString &);
    void coinControlUpdateLabels();
    void coinControlClipboardQuantity();
    void coinControlClipboardAmount();
    void coinControlClipboardFee();
    void coinControlClipboardAfterFee();
    void coinControlClipboardBytes();
    void coinControlClipboardLowOutput();
    void coinControlClipboardChange();
    void setMinimumFee();
    void updateFeeSectionControls();
    void updateMinFeeLabel();
    void updateSmartFeeLabel();

Q_SIGNALS:
    // Fired when a message should be reported to the user
    void message(const QString &title, const QString &message, unsigned int style);
};


#define SEND_CONFIRM_DELAY   3

class SendConfirmationDialog : public QMessageBox
{
    Q_OBJECT

public:
    SendConfirmationDialog(const QString &title, const QString &text, int secDelay = SEND_CONFIRM_DELAY, QWidget *parent = 0);
    int exec();

private Q_SLOTS:
    void countDown();
    void updateYesButton();

private:
    QAbstractButton *yesButton;
    QTimer countDownTimer;
    int secDelay;
};

#endif // WIFICOIN_QT_SENDCOINSDIALOG_H
=======
    void updateRemoveEnabled();
    void setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance);

private:
    Ui::SendCoinsDialog *ui;
    WalletModel *model;
    bool fNewRecipientAllowed;

private slots:
    void on_sendButton_clicked();
    void removeEntry(SendCoinsEntry* entry);
    void updateDisplayUnit();
};

#endif // SENDCOINSDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
