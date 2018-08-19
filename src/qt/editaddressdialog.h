<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_EDITADDRESSDIALOG_H
#define WIFICOIN_QT_EDITADDRESSDIALOG_H

#include <QDialog>

class AddressTableModel;

namespace Ui {
    class EditAddressDialog;
}
=======
#ifndef EDITADDRESSDIALOG_H
#define EDITADDRESSDIALOG_H

#include <QDialog>

namespace Ui {
    class EditAddressDialog;
}
class AddressTableModel;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

/** Dialog for editing an address and associated information.
 */
class EditAddressDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        NewReceivingAddress,
        NewSendingAddress,
        EditReceivingAddress,
        EditSendingAddress
    };

<<<<<<< HEAD
    explicit EditAddressDialog(Mode mode, QWidget *parent);
=======
    explicit EditAddressDialog(Mode mode, QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~EditAddressDialog();

    void setModel(AddressTableModel *model);
    void loadRow(int row);

    QString getAddress() const;
    void setAddress(const QString &address);

<<<<<<< HEAD
public Q_SLOTS:
=======
public slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void accept();

private:
    bool saveCurrentRow();

    Ui::EditAddressDialog *ui;
    QDataWidgetMapper *mapper;
    Mode mode;
    AddressTableModel *model;

    QString address;
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_EDITADDRESSDIALOG_H
=======
#endif // EDITADDRESSDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
