<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_SIGNVERIFYMESSAGEDIALOG_H
#define WIFICOIN_QT_SIGNVERIFYMESSAGEDIALOG_H

#include <QDialog>

class PlatformStyle;
class WalletModel;

namespace Ui {
    class SignVerifyMessageDialog;
}
=======
#ifndef SIGNVERIFYMESSAGEDIALOG_H
#define SIGNVERIFYMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
    class SignVerifyMessageDialog;
}
class WalletModel;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

class SignVerifyMessageDialog : public QDialog
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit SignVerifyMessageDialog(const PlatformStyle *platformStyle, QWidget *parent);
=======
    explicit SignVerifyMessageDialog(QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~SignVerifyMessageDialog();

    void setModel(WalletModel *model);
    void setAddress_SM(const QString &address);
    void setAddress_VM(const QString &address);

    void showTab_SM(bool fShow);
    void showTab_VM(bool fShow);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::SignVerifyMessageDialog *ui;
    WalletModel *model;
<<<<<<< HEAD
    const PlatformStyle *platformStyle;

private Q_SLOTS:
=======

private slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /* sign message */
    void on_addressBookButton_SM_clicked();
    void on_pasteButton_SM_clicked();
    void on_signMessageButton_SM_clicked();
    void on_copySignatureButton_SM_clicked();
    void on_clearButton_SM_clicked();
    /* verify message */
    void on_addressBookButton_VM_clicked();
    void on_verifyMessageButton_VM_clicked();
    void on_clearButton_VM_clicked();
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_SIGNVERIFYMESSAGEDIALOG_H
=======
#endif // SIGNVERIFYMESSAGEDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
