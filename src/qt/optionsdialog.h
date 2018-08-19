<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_OPTIONSDIALOG_H
#define WIFICOIN_QT_OPTIONSDIALOG_H

#include <QDialog>
#include <QValidator>

class OptionsModel;
class QValidatedLineEdit;

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE
=======
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

namespace Ui {
class OptionsDialog;
}
<<<<<<< HEAD

/** Proxy address widget validator, checks for a valid proxy address.
 */
class ProxyAddressValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ProxyAddressValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};
=======
class OptionsModel;
class MonitoredDataMapper;
class QValidatedLineEdit;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

/** Preferences dialog. */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit OptionsDialog(QWidget *parent, bool enableWallet);
=======
    explicit OptionsDialog(QWidget *parent = 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~OptionsDialog();

    void setModel(OptionsModel *model);
    void setMapper();

<<<<<<< HEAD
private Q_SLOTS:
    /* set OK button state (enabled / disabled) */
    void setOkButtonState(bool fState);
    void on_resetButton_clicked();
    void on_openWiFicoinConfButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    
    void on_hideTrayIcon_stateChanged(int fState);

    void showRestartWarning(bool fPersistent = false);
    void clearStatusLabel();
    void updateProxyValidationState();
    /* query the networks, for which the default proxy is used */
    void updateDefaultProxyNets();

Q_SIGNALS:
    void proxyIpChecks(QValidatedLineEdit *pUiProxyIp, int nProxyPort);
=======
protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    /* enable only apply button */
    void enableApplyButton();
    /* disable only apply button */
    void disableApplyButton();
    /* enable apply button and OK button */
    void enableSaveButtons();
    /* disable apply button and OK button */
    void disableSaveButtons();
    /* set apply button and OK button state (enabled / disabled) */
    void setSaveButtonState(bool fState);
    void on_resetButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_applyButton_clicked();

    void showRestartWarning_Proxy();
    void showRestartWarning_Lang();
    void updateDisplayUnit();
    void handleProxyIpValid(QValidatedLineEdit *object, bool fState);

signals:
    void proxyIpValid(QValidatedLineEdit *object, bool fValid);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

private:
    Ui::OptionsDialog *ui;
    OptionsModel *model;
<<<<<<< HEAD
    QDataWidgetMapper *mapper;
};

#endif // WIFICOIN_QT_OPTIONSDIALOG_H
=======
    MonitoredDataMapper *mapper;
    bool fRestartWarningDisplayed_Proxy;
    bool fRestartWarningDisplayed_Lang;
    bool fProxyIpValid;
};

#endif // OPTIONSDIALOG_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
