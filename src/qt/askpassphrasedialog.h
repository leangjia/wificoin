<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_ASKPASSPHRASEDIALOG_H
#define WIFICOIN_QT_ASKPASSPHRASEDIALOG_H

#include <QDialog>

class WalletModel;

namespace Ui {
    class AskPassphraseDialog;
}
=======
#ifndef ASKPASSPHRASEDIALOG_H
#define ASKPASSPHRASEDIALOG_H

#include <QDialog>

namespace Ui {
    class AskPassphraseDialog;
}
class WalletModel;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

/** Multifunctional dialog to ask for passphrases. Used for encryption, unlocking, and changing the passphrase.
 */
class AskPassphraseDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        Encrypt,    /**< Ask passphrase twice and encrypt */
        Unlock,     /**< Ask passphrase and unlock */
        ChangePass, /**< Ask old passphrase + new passphrase twice */
        Decrypt     /**< Ask passphrase and decrypt wallet */
    };

<<<<<<< HEAD
    explicit AskPassphraseDialog(Mode mode, QWidget *parent);
=======
    explicit AskPassphraseDialog(Mode mode, QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~AskPassphraseDialog();

    void accept();

    void setModel(WalletModel *model);

private:
    Ui::AskPassphraseDialog *ui;
    Mode mode;
    WalletModel *model;
    bool fCapsLock;

<<<<<<< HEAD
private Q_SLOTS:
    void textChanged();
    void secureClearPassFields();

protected:
=======
private slots:
    void textChanged();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    bool event(QEvent *event);
    bool eventFilter(QObject *object, QEvent *event);
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_ASKPASSPHRASEDIALOG_H
=======
#endif // ASKPASSPHRASEDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
