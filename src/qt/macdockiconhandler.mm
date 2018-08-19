<<<<<<< HEAD
// Copyright (c) 2011-2013 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "macdockiconhandler.h"

#include <QImageWriter>
#include <QMenu>
#include <QBuffer>
#include <QWidget>

#undef slots
#include <Cocoa/Cocoa.h>
#include <objc/objc.h>
#include <objc/message.h>

#if QT_VERSION < 0x050000
extern void qt_mac_set_dock_menu(QMenu *);
#endif

static MacDockIconHandler *s_instance = NULL;

bool dockClickHandler(id self,SEL _cmd,...) {
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    
    s_instance->handleDockIconClickEvent();
    
    // Return NO (false) to suppress the default OS X actions
    return false;
}

void setupDockClickHandler() {
    Class cls = objc_getClass("NSApplication");
    id appInst = objc_msgSend((id)cls, sel_registerName("sharedApplication"));
    
    if (appInst != NULL) {
        id delegate = objc_msgSend(appInst, sel_registerName("delegate"));
        Class delClass = (Class)objc_msgSend(delegate,  sel_registerName("class"));
        SEL shouldHandle = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
        if (class_getInstanceMethod(delClass, shouldHandle))
            class_replaceMethod(delClass, shouldHandle, (IMP)dockClickHandler, "B@:");
        else
            class_addMethod(delClass, shouldHandle, (IMP)dockClickHandler,"B@:");
    }
}

=======
#include "macdockiconhandler.h"

#include <QMenu>
#include <QWidget>

extern void qt_mac_set_dock_menu(QMenu*);

#undef slots
#include <Cocoa/Cocoa.h>

@interface DockIconClickEventHandler : NSObject
{
    MacDockIconHandler* dockIconHandler;
}

@end

@implementation DockIconClickEventHandler

- (id)initWithDockIconHandler:(MacDockIconHandler *)aDockIconHandler
{
    self = [super init];
    if (self) {
        dockIconHandler = aDockIconHandler;

        [[NSAppleEventManager sharedAppleEventManager]
            setEventHandler:self
                andSelector:@selector(handleDockClickEvent:withReplyEvent:)
              forEventClass:kCoreEventClass
                 andEventID:kAEReopenApplication];
    }
    return self;
}

- (void)handleDockClickEvent:(NSAppleEventDescriptor*)event withReplyEvent:(NSAppleEventDescriptor*)replyEvent
{
    Q_UNUSED(event)
    Q_UNUSED(replyEvent)

    if (dockIconHandler) {
        dockIconHandler->handleDockIconClickEvent();
    }
}

@end
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

MacDockIconHandler::MacDockIconHandler() : QObject()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
<<<<<<< HEAD

    setupDockClickHandler();
    this->m_dummyWidget = new QWidget();
    this->m_dockMenu = new QMenu(this->m_dummyWidget);
    this->setMainWindow(NULL);
#if QT_VERSION < 0x050000
    qt_mac_set_dock_menu(this->m_dockMenu);
#elif QT_VERSION >= 0x050200
    this->m_dockMenu->setAsDockMenu();
#endif
=======
    this->m_dockIconClickEventHandler = [[DockIconClickEventHandler alloc] initWithDockIconHandler:this];

    this->m_dummyWidget = new QWidget();
    this->m_dockMenu = new QMenu(this->m_dummyWidget);
    qt_mac_set_dock_menu(this->m_dockMenu);
    this->setMainWindow(NULL);

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    [pool release];
}

void MacDockIconHandler::setMainWindow(QMainWindow *window) {
    this->mainWindow = window;
}

MacDockIconHandler::~MacDockIconHandler()
{
<<<<<<< HEAD
=======
    [this->m_dockIconClickEventHandler release];
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    delete this->m_dummyWidget;
    this->setMainWindow(NULL);
}

QMenu *MacDockIconHandler::dockMenu()
{
    return this->m_dockMenu;
}

void MacDockIconHandler::setIcon(const QIcon &icon)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
<<<<<<< HEAD
    NSImage *image = nil;
    if (icon.isNull())
        image = [[NSImage imageNamed:@"NSApplicationIcon"] retain];
    else {
        // generate NSImage from QIcon and use this as dock icon.
        QSize size = icon.actualSize(QSize(128, 128));
        QPixmap pixmap = icon.pixmap(size);

        // Write image into a R/W buffer from raw pixmap, then save the image.
        QBuffer notificationBuffer;
        if (!pixmap.isNull() && notificationBuffer.open(QIODevice::ReadWrite)) {
            QImageWriter writer(&notificationBuffer, "PNG");
            if (writer.write(pixmap.toImage())) {
                NSData* macImgData = [NSData dataWithBytes:notificationBuffer.buffer().data()
                                             length:notificationBuffer.buffer().size()];
                image =  [[NSImage alloc] initWithData:macImgData];
            }
        }

        if(!image) {
            // if testnet image could not be created, load std. app icon
            image = [[NSImage imageNamed:@"NSApplicationIcon"] retain];
        }
=======
    NSImage *image;
    if (icon.isNull())
        image = [[NSImage imageNamed:@"NSApplicationIcon"] retain];
    else {
        QSize size = icon.actualSize(QSize(128, 128));
        QPixmap pixmap = icon.pixmap(size);
        CGImageRef cgImage = pixmap.toMacCGImageRef();
        image = [[NSImage alloc] initWithCGImage:cgImage size:NSZeroSize];
        CFRelease(cgImage);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }

    [NSApp setApplicationIconImage:image];
    [image release];
    [pool release];
}

MacDockIconHandler *MacDockIconHandler::instance()
{
<<<<<<< HEAD
=======
    static MacDockIconHandler *s_instance = NULL;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    if (!s_instance)
        s_instance = new MacDockIconHandler();
    return s_instance;
}

<<<<<<< HEAD
void MacDockIconHandler::cleanup()
{
    delete s_instance;
}

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
void MacDockIconHandler::handleDockIconClickEvent()
{
    if (this->mainWindow)
    {
        this->mainWindow->activateWindow();
        this->mainWindow->show();
    }

<<<<<<< HEAD
    Q_EMIT this->dockIconClicked();
=======
    emit this->dockIconClicked();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
