<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_QVALUECOMBOBOX_H
#define WIFICOIN_QT_QVALUECOMBOBOX_H
=======
#ifndef QVALUECOMBOBOX_H
#define QVALUECOMBOBOX_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <QComboBox>
#include <QVariant>

/* QComboBox that can be used with QDataWidgetMapper to select ordinal values from a model. */
class QValueComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
    explicit QValueComboBox(QWidget *parent = 0);

    QVariant value() const;
    void setValue(const QVariant &value);

    /** Specify model role to use as ordinal value (defaults to Qt::UserRole) */
    void setRole(int role);

<<<<<<< HEAD
Q_SIGNALS:
=======
signals:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void valueChanged();

private:
    int role;

<<<<<<< HEAD
private Q_SLOTS:
    void handleSelectionChanged(int idx);
};

#endif // WIFICOIN_QT_QVALUECOMBOBOX_H
=======
private slots:
    void handleSelectionChanged(int idx);
};

#endif // QVALUECOMBOBOX_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
