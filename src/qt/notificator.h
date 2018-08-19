<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_NOTIFICATOR_H
#define WIFICOIN_QT_NOTIFICATOR_H

#if defined(HAVE_CONFIG_H)
#include "config/wificoin-config.h"
#endif

#include <QIcon>
#include <QObject>

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;

=======
#ifndef NOTIFICATOR_H
#define NOTIFICATOR_H

#include <QObject>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#ifdef USE_DBUS
class QDBusInterface;
#endif
QT_END_NAMESPACE

/** Cross-platform desktop notification client. */
class Notificator: public QObject
{
    Q_OBJECT

public:
    /** Create a new notificator.
       @note Ownership of trayIcon is not transferred to this object.
    */
<<<<<<< HEAD
    Notificator(const QString &programName, QSystemTrayIcon *trayIcon, QWidget *parent);
=======
    Notificator(const QString &programName=QString(), QSystemTrayIcon *trayIcon=0, QWidget *parent=0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    ~Notificator();

    // Message class
    enum Class
    {
        Information,    /**< Informational message */
        Warning,        /**< Notify user of potential problem */
        Critical        /**< An error occurred */
    };

<<<<<<< HEAD
public Q_SLOTS:
=======
public slots:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    /** Show notification message.
       @param[in] cls    general message class
       @param[in] title  title shown with message
       @param[in] text   message content
       @param[in] icon   optional icon to show with message
       @param[in] millisTimeout notification timeout in milliseconds (defaults to 10 seconds)
       @note Platform implementations are free to ignore any of the provided fields except for \a text.
     */
    void notify(Class cls, const QString &title, const QString &text,
                const QIcon &icon = QIcon(), int millisTimeout = 10000);

private:
    QWidget *parent;
    enum Mode {
<<<<<<< HEAD
        None,                       /**< Ignore informational notifications, and show a modal pop-up dialog for Critical notifications. */
        Freedesktop,                /**< Use DBus org.freedesktop.Notifications */
        QSystemTray,                /**< Use QSystemTray::showMessage */
        Growl12,                    /**< Use the Growl 1.2 notification system (Mac only) */
        Growl13,                    /**< Use the Growl 1.3 notification system (Mac only) */
        UserNotificationCenter      /**< Use the 10.8+ User Notification Center (Mac only) */
=======
        None,           /**< Ignore informational notifications, and show a modal pop-up dialog for Critical notifications. */
        Freedesktop,    /**< Use DBus org.freedesktop.Notifications */
        QSystemTray,    /**< Use QSystemTray::showMessage */
        Growl12,        /**< Use the Growl 1.2 notification system (Mac only) */
        Growl13         /**< Use the Growl 1.3 notification system (Mac only) */
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    };
    QString programName;
    Mode mode;
    QSystemTrayIcon *trayIcon;
#ifdef USE_DBUS
    QDBusInterface *interface;

    void notifyDBus(Class cls, const QString &title, const QString &text, const QIcon &icon, int millisTimeout);
#endif
    void notifySystray(Class cls, const QString &title, const QString &text, const QIcon &icon, int millisTimeout);
#ifdef Q_OS_MAC
    void notifyGrowl(Class cls, const QString &title, const QString &text, const QIcon &icon);
<<<<<<< HEAD
    void notifyMacUserNotificationCenter(Class cls, const QString &title, const QString &text, const QIcon &icon);
#endif
};

#endif // WIFICOIN_QT_NOTIFICATOR_H
=======
#endif
};

#endif // NOTIFICATOR_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
