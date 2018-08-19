<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qvalidatedlineedit.h"

#include "wificoinaddressvalidator.h"
#include "guiconstants.h"

QValidatedLineEdit::QValidatedLineEdit(QWidget *parent) :
    QLineEdit(parent),
    valid(true),
    checkValidator(0)
=======
#include "qvalidatedlineedit.h"

#include "guiconstants.h"

QValidatedLineEdit::QValidatedLineEdit(QWidget *parent) :
    QLineEdit(parent), valid(true)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(markValid()));
}

<<<<<<< HEAD
void QValidatedLineEdit::setValid(bool _valid)
{
    if(_valid == this->valid)
=======
void QValidatedLineEdit::setValid(bool valid)
{
    if(valid == this->valid)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        return;
    }

<<<<<<< HEAD
    if(_valid)
=======
    if(valid)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        setStyleSheet("");
    }
    else
    {
        setStyleSheet(STYLE_INVALID);
    }
<<<<<<< HEAD
    this->valid = _valid;
=======
    this->valid = valid;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void QValidatedLineEdit::focusInEvent(QFocusEvent *evt)
{
    // Clear invalid flag on focus
    setValid(true);
<<<<<<< HEAD

    QLineEdit::focusInEvent(evt);
}

void QValidatedLineEdit::focusOutEvent(QFocusEvent *evt)
{
    checkValidity();

    QLineEdit::focusOutEvent(evt);
}

void QValidatedLineEdit::markValid()
{
    // As long as a user is typing ensure we display state as valid
=======
    QLineEdit::focusInEvent(evt);
}

void QValidatedLineEdit::markValid()
{
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    setValid(true);
}

void QValidatedLineEdit::clear()
{
    setValid(true);
    QLineEdit::clear();
}
<<<<<<< HEAD

void QValidatedLineEdit::setEnabled(bool enabled)
{
    if (!enabled)
    {
        // A disabled QValidatedLineEdit should be marked valid
        setValid(true);
    }
    else
    {
        // Recheck validity when QValidatedLineEdit gets enabled
        checkValidity();
    }

    QLineEdit::setEnabled(enabled);
}

void QValidatedLineEdit::checkValidity()
{
    if (text().isEmpty())
    {
        setValid(true);
    }
    else if (hasAcceptableInput())
    {
        setValid(true);

        // Check contents on focus out
        if (checkValidator)
        {
            QString address = text();
            int pos = 0;
            if (checkValidator->validate(address, pos) == QValidator::Acceptable)
                setValid(true);
            else
                setValid(false);
        }
    }
    else
        setValid(false);

    Q_EMIT validationDidChange(this);
}

void QValidatedLineEdit::setCheckValidator(const QValidator *v)
{
    checkValidator = v;
}

bool QValidatedLineEdit::isValid()
{
    // use checkValidator in case the QValidatedLineEdit is disabled
    if (checkValidator)
    {
        QString address = text();
        int pos = 0;
        if (checkValidator->validate(address, pos) == QValidator::Acceptable)
            return true;
    }

    return valid;
}
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
