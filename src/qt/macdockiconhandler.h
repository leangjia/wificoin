<<<<<<< HEAD
// Copyright (c) 2011-2015 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_QT_MACDOCKICONHANDLER_H
#define WIFICOIN_QT_MACDOCKICONHANDLER_H

#include <QMainWindow>
#include <QObject>

QT_BEGIN_NAMESPACE
class QIcon;
class QMenu;
class QWidget;
QT_END_NAMESPACE

=======
#ifndef MACDOCKICONHANDLER_H
#define MACDOCKICONHANDLER_H

#include <QObject>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMenu;
class QIcon;
class QWidget;
QT_END_NAMESPACE

#ifdef __OBJC__
@class DockIconClickEventHandler;
#else
class DockIconClickEventHandler;
#endif

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
/** Macintosh-specific dock icon handler.
 */
class MacDockIconHandler : public QObject
{
    Q_OBJECT

public:
    ~MacDockIconHandler();

    QMenu *dockMenu();
    void setIcon(const QIcon &icon);
    void setMainWindow(QMainWindow *window);
    static MacDockIconHandler *instance();
<<<<<<< HEAD
    static void cleanup();
    void handleDockIconClickEvent();

Q_SIGNALS:
=======

    void handleDockIconClickEvent();

signals:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void dockIconClicked();

private:
    MacDockIconHandler();

<<<<<<< HEAD
=======
    DockIconClickEventHandler *m_dockIconClickEventHandler;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    QWidget *m_dummyWidget;
    QMenu *m_dockMenu;
    QMainWindow *mainWindow;
};

<<<<<<< HEAD
#endif // WIFICOIN_QT_MACDOCKICONHANDLER_H
=======
#endif // MACDOCKICONCLICKHANDLER_H
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
