<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_QVALIDATEDLINEEDIT_H
#define WIFICOIN_QT_QVALIDATEDLINEEDIT_H
=======
#ifndef QVALIDATEDLINEEDIT_H
#define QVALIDATEDLINEEDIT_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#include <QLineEdit>

/** Line edit that can be marked as "invalid" to show input validation feedback. When marked as invalid,
   it will get a red background until it is focused.
 */
class QValidatedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit QValidatedLineEdit(QWidget *parent);
    void clear();
    void setCheckValidator(const QValidator *v);
    bool isValid();

protected:
    void focusInEvent(QFocusEvent *evt);
    void focusOutEvent(QFocusEvent *evt);

private:
    bool valid;
    const QValidator *checkValidator;

public Q_SLOTS:
    void setValid(bool valid);
    void setEnabled(bool enabled);

Q_SIGNALS:
    void validationDidChange(QValidatedLineEdit *validatedLineEdit);
    
private Q_SLOTS:
    void markValid();
    void checkValidity();
};

#endif // WIFICOIN_QT_QVALIDATEDLINEEDIT_H
=======
    explicit QValidatedLineEdit(QWidget *parent = 0);
    void clear();

protected:
    void focusInEvent(QFocusEvent *evt);

private:
    bool valid;

public slots:
    void setValid(bool valid);

private slots:
    void markValid();
};

#endif // QVALIDATEDLINEEDIT_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
