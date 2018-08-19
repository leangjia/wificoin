<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_SENDCOINSENTRY_H
#define WIFICOIN_QT_SENDCOINSENTRY_H

#include "walletmodel.h"

#include <QStackedWidget>

class WalletModel;
class PlatformStyle;
=======
#ifndef SENDCOINSENTRY_H
#define SENDCOINSENTRY_H

#include <QFrame>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

namespace Ui {
    class SendCoinsEntry;
}
<<<<<<< HEAD

/**
 * A single entry in the dialog for sending wificoins.
 * Stacked widget, with different UIs for payment requests
 * with a strong payee identity.
 */
class SendCoinsEntry : public QStackedWidget
=======
class WalletModel;
class SendCoinsRecipient;

/** A single entry in the dialog for sending bitcoins. */
class SendCoinsEntry : public QFrame
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit SendCoinsEntry(const PlatformStyle *platformStyle, QWidget *parent = 0);
=======
    explicit SendCoinsEntry(QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~SendCoinsEntry();

    void setModel(WalletModel *model);
    bool validate();
    SendCoinsRecipient getValue();

    /** Return whether the entry is still empty and unedited */
    bool isClear();

    void setValue(const SendCoinsRecipient &value);
    void setAddress(const QString &address);

<<<<<<< HEAD
    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases
     *  (issue https://bugreports.qt-project.org/browse/QTBUG-10907).
=======
    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases (issue https://bugreports.qt-project.org/browse/QTBUG-10907).
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
     */
    QWidget *setupTabChain(QWidget *prev);

    void setFocus();

<<<<<<< HEAD
public Q_SLOTS:
    void clear();

Q_SIGNALS:
    void removeEntry(SendCoinsEntry *entry);
    void payAmountChanged();
    void subtractFeeFromAmountChanged();

private Q_SLOTS:
    void deleteClicked();
=======
public slots:
    void setRemoveEnabled(bool enabled);
    void clear();

signals:
    void removeEntry(SendCoinsEntry *entry);

private slots:
    void on_deleteButton_clicked();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void on_payTo_textChanged(const QString &address);
    void on_addressBookButton_clicked();
    void on_pasteButton_clicked();
    void updateDisplayUnit();

private:
<<<<<<< HEAD
    SendCoinsRecipient recipient;
    Ui::SendCoinsEntry *ui;
    WalletModel *model;
    const PlatformStyle *platformStyle;

    bool updateLabel(const QString &address);
};

#endif // WIFICOIN_QT_SENDCOINSENTRY_H
=======
    Ui::SendCoinsEntry *ui;
    WalletModel *model;
};

#endif // SENDCOINSENTRY_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
