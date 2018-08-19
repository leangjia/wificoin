<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#include "editaddressdialog.h"
#include "ui_editaddressdialog.h"

#include "addresstablemodel.h"
#include "guiutil.h"

#include <QDataWidgetMapper>
#include <QMessageBox>

<<<<<<< HEAD
EditAddressDialog::EditAddressDialog(Mode _mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAddressDialog),
    mapper(0),
    mode(_mode),
    model(0)
=======
EditAddressDialog::EditAddressDialog(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAddressDialog), mapper(0), mode(mode), model(0)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    ui->setupUi(this);

    GUIUtil::setupAddressWidget(ui->addressEdit, this);

    switch(mode)
    {
    case NewReceivingAddress:
        setWindowTitle(tr("New receiving address"));
        ui->addressEdit->setEnabled(false);
        break;
    case NewSendingAddress:
        setWindowTitle(tr("New sending address"));
        break;
    case EditReceivingAddress:
        setWindowTitle(tr("Edit receiving address"));
        ui->addressEdit->setEnabled(false);
        break;
    case EditSendingAddress:
        setWindowTitle(tr("Edit sending address"));
        break;
    }

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

EditAddressDialog::~EditAddressDialog()
{
    delete ui;
}

<<<<<<< HEAD
void EditAddressDialog::setModel(AddressTableModel *_model)
{
    this->model = _model;
    if(!_model)
        return;

    mapper->setModel(_model);
=======
void EditAddressDialog::setModel(AddressTableModel *model)
{
    this->model = model;
    if(!model)
        return;

    mapper->setModel(model);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    mapper->addMapping(ui->labelEdit, AddressTableModel::Label);
    mapper->addMapping(ui->addressEdit, AddressTableModel::Address);
}

void EditAddressDialog::loadRow(int row)
{
    mapper->setCurrentIndex(row);
}

bool EditAddressDialog::saveCurrentRow()
{
    if(!model)
        return false;

    switch(mode)
    {
    case NewReceivingAddress:
    case NewSendingAddress:
        address = model->addRow(
                mode == NewSendingAddress ? AddressTableModel::Send : AddressTableModel::Receive,
                ui->labelEdit->text(),
                ui->addressEdit->text());
        break;
    case EditReceivingAddress:
    case EditSendingAddress:
        if(mapper->submit())
        {
            address = ui->addressEdit->text();
        }
        break;
    }
    return !address.isEmpty();
}

void EditAddressDialog::accept()
{
    if(!model)
        return;

    if(!saveCurrentRow())
    {
        switch(model->getEditStatus())
        {
        case AddressTableModel::OK:
            // Failed with unknown reason. Just reject.
            break;
        case AddressTableModel::NO_CHANGES:
            // No changes were made during edit operation. Just reject.
            break;
        case AddressTableModel::INVALID_ADDRESS:
            QMessageBox::warning(this, windowTitle(),
<<<<<<< HEAD
                tr("The entered address \"%1\" is not a valid WiFicoin address.").arg(ui->addressEdit->text()),
=======
                tr("The entered address \"%1\" is not a valid Bitcoin address.").arg(ui->addressEdit->text()),
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::DUPLICATE_ADDRESS:
            QMessageBox::warning(this, windowTitle(),
                tr("The entered address \"%1\" is already in the address book.").arg(ui->addressEdit->text()),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::WALLET_UNLOCK_FAILURE:
            QMessageBox::critical(this, windowTitle(),
                tr("Could not unlock wallet."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::KEY_GENERATION_FAILURE:
            QMessageBox::critical(this, windowTitle(),
                tr("New key generation failed."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;

        }
        return;
    }
    QDialog::accept();
}

QString EditAddressDialog::getAddress() const
{
    return address;
}

<<<<<<< HEAD
void EditAddressDialog::setAddress(const QString &_address)
{
    this->address = _address;
    ui->addressEdit->setText(_address);
=======
void EditAddressDialog::setAddress(const QString &address)
{
    this->address = address;
    ui->addressEdit->setText(address);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
