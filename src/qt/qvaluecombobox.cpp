<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#include "qvaluecombobox.h"

QValueComboBox::QValueComboBox(QWidget *parent) :
        QComboBox(parent), role(Qt::UserRole)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectionChanged(int)));
}

QVariant QValueComboBox::value() const
{
    return itemData(currentIndex(), role);
}

void QValueComboBox::setValue(const QVariant &value)
{
    setCurrentIndex(findData(value, role));
}

<<<<<<< HEAD
void QValueComboBox::setRole(int _role)
{
    this->role = _role;
=======
void QValueComboBox::setRole(int role)
{
    this->role = role;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void QValueComboBox::handleSelectionChanged(int idx)
{
<<<<<<< HEAD
    Q_EMIT valueChanged();
=======
    emit valueChanged();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
