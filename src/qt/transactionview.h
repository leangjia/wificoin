<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_TRANSACTIONVIEW_H
#define WIFICOIN_QT_TRANSACTIONVIEW_H

#include "guiutil.h"

#include <QWidget>
#include <QKeyEvent>

class PlatformStyle;
class TransactionFilterProxy;
class WalletModel;

QT_BEGIN_NAMESPACE
class QComboBox;
class QDateTimeEdit;
class QFrame;
class QLineEdit;
class QMenu;
class QModelIndex;
class QSignalMapper;
class QTableView;
=======
#ifndef TRANSACTIONVIEW_H
#define TRANSACTIONVIEW_H

#include <QWidget>

class WalletModel;
class TransactionFilterProxy;

QT_BEGIN_NAMESPACE
class QTableView;
class QComboBox;
class QLineEdit;
class QModelIndex;
class QMenu;
class QFrame;
class QDateTimeEdit;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
QT_END_NAMESPACE

/** Widget showing the transaction list for a wallet, including a filter row.
    Using the filter row, the user can view or export a subset of the transactions.
  */
class TransactionView : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit TransactionView(const PlatformStyle *platformStyle, QWidget *parent = 0);
=======
    explicit TransactionView(QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    void setModel(WalletModel *model);

    // Date ranges for filter
    enum DateEnum
    {
        All,
        Today,
        ThisWeek,
        ThisMonth,
        LastMonth,
        ThisYear,
        Range
    };

<<<<<<< HEAD
    enum ColumnWidths {
        STATUS_COLUMN_WIDTH = 30,
        WATCHONLY_COLUMN_WIDTH = 23,
        DATE_COLUMN_WIDTH = 120,
        TYPE_COLUMN_WIDTH = 113,
        AMOUNT_MINIMUM_COLUMN_WIDTH = 120,
        MINIMUM_COLUMN_WIDTH = 23
    };

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
private:
    WalletModel *model;
    TransactionFilterProxy *transactionProxyModel;
    QTableView *transactionView;

    QComboBox *dateWidget;
    QComboBox *typeWidget;
<<<<<<< HEAD
    QComboBox *watchOnlyWidget;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    QLineEdit *addressWidget;
    QLineEdit *amountWidget;

    QMenu *contextMenu;
<<<<<<< HEAD
    QSignalMapper *mapperThirdPartyTxUrls;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    QFrame *dateRangeWidget;
    QDateTimeEdit *dateFrom;
    QDateTimeEdit *dateTo;
<<<<<<< HEAD
    QAction *abandonAction;
    QAction *bumpFeeAction;

    QWidget *createDateRangeWidget();

    GUIUtil::TableViewLastColumnResizingFixer *columnResizingFixer;

    virtual void resizeEvent(QResizeEvent* event);

    bool eventFilter(QObject *obj, QEvent *event);

private Q_SLOTS:
=======

    QWidget *createDateRangeWidget();

private slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void contextualMenu(const QPoint &);
    void dateRangeChanged();
    void showDetails();
    void copyAddress();
    void editLabel();
    void copyLabel();
    void copyAmount();
    void copyTxID();
<<<<<<< HEAD
    void copyTxHex();
    void copyTxPlainText();
    void openThirdPartyTxUrl(QString url);
    void updateWatchOnlyColumn(bool fHaveWatchOnly);
    void abandonTx();
    void bumpFee();

Q_SIGNALS:
    void doubleClicked(const QModelIndex&);

    /**  Fired when a message should be reported to the user */
    void message(const QString &title, const QString &message, unsigned int style);

public Q_SLOTS:
    void chooseDate(int idx);
    void chooseType(int idx);
    void chooseWatchonly(int idx);
    void changedAmount();
    void changedPrefix();
=======

signals:
    void doubleClicked(const QModelIndex&);

public slots:
    void chooseDate(int idx);
    void chooseType(int idx);
    void changedPrefix(const QString &prefix);
    void changedAmount(const QString &amount);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void exportClicked();
    void focusTransaction(const QModelIndex&);

};

<<<<<<< HEAD
#endif // WIFICOIN_QT_TRANSACTIONVIEW_H
=======
#endif // TRANSACTIONVIEW_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
