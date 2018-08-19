<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "wificoinunits.h"
#include "clientmodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "transactionfilterproxy.h"
#include "transactiontablemodel.h"
#include "walletmodel.h"
=======
#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <QAbstractItemDelegate>
#include <QPainter>

<<<<<<< HEAD
#define DECORATION_SIZE 54
#define NUM_ITEMS 5
=======
#define DECORATION_SIZE 64
#define NUM_ITEMS 3
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
<<<<<<< HEAD
    TxViewDelegate(const PlatformStyle *_platformStyle, QObject *parent=nullptr):
        QAbstractItemDelegate(parent), unit(WiFicoinUnits::WFC),
        platformStyle(_platformStyle)
=======
    TxViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {

    }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        painter->save();

<<<<<<< HEAD
        QIcon icon = qvariant_cast<QIcon>(index.data(TransactionTableModel::RawDecorationRole));
=======
        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2*ypad)/2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top()+ypad, mainRect.width() - xspace, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top()+ypad+halfheight, mainRect.width() - xspace, halfheight);
<<<<<<< HEAD
        icon = platformStyle->SingleColorIcon(icon);
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();
        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = option.palette.color(QPalette::Text);
        if(value.canConvert<QBrush>())
        {
            QBrush brush = qvariant_cast<QBrush>(value);
            foreground = brush.color();
        }

        painter->setPen(foreground);
<<<<<<< HEAD
        QRect boundingRect;
        painter->drawText(addressRect, Qt::AlignLeft|Qt::AlignVCenter, address, &boundingRect);

        if (index.data(TransactionTableModel::WatchonlyRole).toBool())
        {
            QIcon iconWatchonly = qvariant_cast<QIcon>(index.data(TransactionTableModel::WatchonlyDecorationRole));
            QRect watchonlyRect(boundingRect.right() + 5, mainRect.top()+ypad+halfheight, 16, halfheight);
            iconWatchonly.paint(painter, watchonlyRect);
        }
=======
        painter->drawText(addressRect, Qt::AlignLeft|Qt::AlignVCenter, address);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        if(amount < 0)
        {
            foreground = COLOR_NEGATIVE;
        }
        else if(!confirmed)
        {
            foreground = COLOR_UNCONFIRMED;
        }
        else
        {
            foreground = option.palette.color(QPalette::Text);
        }
        painter->setPen(foreground);
<<<<<<< HEAD
        QString amountText = WiFicoinUnits::formatWithUnit(unit, amount, true, WiFicoinUnits::separatorAlways);
=======
        QString amountText = BitcoinUnits::formatWithUnit(unit, amount, true);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if(!confirmed)
        {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight|Qt::AlignVCenter, amountText);

        painter->setPen(option.palette.color(QPalette::Text));
        painter->drawText(amountRect, Qt::AlignLeft|Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;
<<<<<<< HEAD
    const PlatformStyle *platformStyle;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

};
#include "overviewpage.moc"

<<<<<<< HEAD
OverviewPage::OverviewPage(const PlatformStyle *platformStyle, QWidget *parent) :
=======
OverviewPage::OverviewPage(QWidget *parent) :
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    QWidget(parent),
    ui(new Ui::OverviewPage),
    clientModel(0),
    walletModel(0),
    currentBalance(-1),
    currentUnconfirmedBalance(-1),
    currentImmatureBalance(-1),
<<<<<<< HEAD
    currentWatchOnlyBalance(-1),
    currentWatchUnconfBalance(-1),
    currentWatchImmatureBalance(-1),
    txdelegate(new TxViewDelegate(platformStyle, this))
{
    ui->setupUi(this);

    // use a SingleColorIcon for the "out of sync warning" icon
    QIcon icon = platformStyle->SingleColorIcon(":/icons/warning");
    icon.addPixmap(icon.pixmap(QSize(64,64), QIcon::Normal), QIcon::Disabled); // also set the disabled icon because we are using a disabled QPushButton to work around missing HiDPI support of QLabel (https://bugreports.qt.io/browse/QTBUG-42503)
    ui->labelTransactionsStatus->setIcon(icon);
    ui->labelWalletStatus->setIcon(icon);

=======
    txdelegate(new TxViewDelegate()),
    filter(0)
{
    ui->setupUi(this);

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    // Recent transactions
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SLOT(handleTransactionClicked(QModelIndex)));

<<<<<<< HEAD
    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
    connect(ui->labelWalletStatus, SIGNAL(clicked()), this, SLOT(handleOutOfSyncWarningClicks()));
    connect(ui->labelTransactionsStatus, SIGNAL(clicked()), this, SLOT(handleOutOfSyncWarningClicks()));
=======
    // init "out of sync" warning labels
    ui->labelWalletStatus->setText("(" + tr("out of sync") + ")");
    ui->labelTransactionsStatus->setText("(" + tr("out of sync") + ")");

    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void OverviewPage::handleTransactionClicked(const QModelIndex &index)
{
    if(filter)
<<<<<<< HEAD
        Q_EMIT transactionClicked(filter->mapToSource(index));
}

void OverviewPage::handleOutOfSyncWarningClicks()
{
    Q_EMIT outOfSyncWarningClicked();
=======
        emit transactionClicked(filter->mapToSource(index));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

<<<<<<< HEAD
void OverviewPage::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance)
=======
void OverviewPage::setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    int unit = walletModel->getOptionsModel()->getDisplayUnit();
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;
<<<<<<< HEAD
    currentWatchOnlyBalance = watchOnlyBalance;
    currentWatchUnconfBalance = watchUnconfBalance;
    currentWatchImmatureBalance = watchImmatureBalance;
    ui->labelBalance->setText(WiFicoinUnits::formatWithUnit(unit, balance, false, WiFicoinUnits::separatorAlways));
    ui->labelUnconfirmed->setText(WiFicoinUnits::formatWithUnit(unit, unconfirmedBalance, false, WiFicoinUnits::separatorAlways));
    ui->labelImmature->setText(WiFicoinUnits::formatWithUnit(unit, immatureBalance, false, WiFicoinUnits::separatorAlways));
    ui->labelTotal->setText(WiFicoinUnits::formatWithUnit(unit, balance + unconfirmedBalance + immatureBalance, false, WiFicoinUnits::separatorAlways));
    ui->labelWatchAvailable->setText(WiFicoinUnits::formatWithUnit(unit, watchOnlyBalance, false, WiFicoinUnits::separatorAlways));
    ui->labelWatchPending->setText(WiFicoinUnits::formatWithUnit(unit, watchUnconfBalance, false, WiFicoinUnits::separatorAlways));
    ui->labelWatchImmature->setText(WiFicoinUnits::formatWithUnit(unit, watchImmatureBalance, false, WiFicoinUnits::separatorAlways));
    ui->labelWatchTotal->setText(WiFicoinUnits::formatWithUnit(unit, watchOnlyBalance + watchUnconfBalance + watchImmatureBalance, false, WiFicoinUnits::separatorAlways));
=======
    ui->labelBalance->setText(BitcoinUnits::formatWithUnit(unit, balance));
    ui->labelUnconfirmed->setText(BitcoinUnits::formatWithUnit(unit, unconfirmedBalance));
    ui->labelImmature->setText(BitcoinUnits::formatWithUnit(unit, immatureBalance));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    // only show immature (newly mined) balance if it's non-zero, so as not to complicate things
    // for the non-mining users
    bool showImmature = immatureBalance != 0;
<<<<<<< HEAD
    bool showWatchOnlyImmature = watchImmatureBalance != 0;

    // for symmetry reasons also show immature label when the watch-only one is shown
    ui->labelImmature->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelImmatureText->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelWatchImmature->setVisible(showWatchOnlyImmature); // show watch-only immature balance
}

// show/hide watch-only labels
void OverviewPage::updateWatchOnlyLabels(bool showWatchOnly)
{
    ui->labelSpendable->setVisible(showWatchOnly);      // show spendable label (only when watch-only is active)
    ui->labelWatchonly->setVisible(showWatchOnly);      // show watch-only label
    ui->lineWatchBalance->setVisible(showWatchOnly);    // show watch-only balance separator line
    ui->labelWatchAvailable->setVisible(showWatchOnly); // show watch-only available balance
    ui->labelWatchPending->setVisible(showWatchOnly);   // show watch-only pending balance
    ui->labelWatchTotal->setVisible(showWatchOnly);     // show watch-only total balance

    if (!showWatchOnly)
        ui->labelWatchImmature->hide();
=======
    ui->labelImmature->setVisible(showImmature);
    ui->labelImmatureText->setVisible(showImmature);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void OverviewPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        // Show warning if this is a prerelease version
        connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        updateAlerts(model->getStatusBarWarnings());
    }
}

void OverviewPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if(model && model->getOptionsModel())
    {
        // Set up transaction list
<<<<<<< HEAD
        filter.reset(new TransactionFilterProxy());
=======
        filter = new TransactionFilterProxy();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
<<<<<<< HEAD
        filter->setShowInactive(false);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter.get());
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance(),
                   model->getWatchBalance(), model->getWatchUnconfirmedBalance(), model->getWatchImmatureBalance());
        connect(model, SIGNAL(balanceChanged(CAmount,CAmount,CAmount,CAmount,CAmount,CAmount)), this, SLOT(setBalance(CAmount,CAmount,CAmount,CAmount,CAmount,CAmount)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

        updateWatchOnlyLabels(model->haveWatchOnly());
        connect(model, SIGNAL(notifyWatchonlyChanged(bool)), this, SLOT(updateWatchOnlyLabels(bool)));
    }

    // update the display unit, to not use the default ("WFC")
=======
        filter->sort(TransactionTableModel::Status, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
    }

    // update the display unit, to not use the default ("LGBT")
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    updateDisplayUnit();
}

void OverviewPage::updateDisplayUnit()
{
    if(walletModel && walletModel->getOptionsModel())
    {
        if(currentBalance != -1)
<<<<<<< HEAD
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance,
                       currentWatchOnlyBalance, currentWatchUnconfBalance, currentWatchImmatureBalance);
=======
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        // Update txdelegate->unit with the current unit
        txdelegate->unit = walletModel->getOptionsModel()->getDisplayUnit();

        ui->listTransactions->update();
    }
}

void OverviewPage::updateAlerts(const QString &warnings)
{
    this->ui->labelAlerts->setVisible(!warnings.isEmpty());
    this->ui->labelAlerts->setText(warnings);
}

void OverviewPage::showOutOfSyncWarning(bool fShow)
{
    ui->labelWalletStatus->setVisible(fShow);
    ui->labelTransactionsStatus->setVisible(fShow);
}
