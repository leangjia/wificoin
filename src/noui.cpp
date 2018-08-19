// Copyright (c) 2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "noui.h"

#include "ui_interface.h"
#include "util.h"

#include <cstdio>
#include <stdint.h>
=======
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ui_interface.h"
#include "init.h"
#include "bitcoinrpc.h"

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#include <string>

static bool noui_ThreadSafeMessageBox(const std::string& message, const std::string& caption, unsigned int style)
{
<<<<<<< HEAD
    bool fSecure = style & CClientUIInterface::SECURE;
    style &= ~CClientUIInterface::SECURE;

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    std::string strCaption;
    // Check for usage of predefined caption
    switch (style) {
    case CClientUIInterface::MSG_ERROR:
        strCaption += _("Error");
        break;
    case CClientUIInterface::MSG_WARNING:
        strCaption += _("Warning");
        break;
    case CClientUIInterface::MSG_INFORMATION:
        strCaption += _("Information");
        break;
    default:
        strCaption += caption; // Use supplied caption (can be empty)
    }

<<<<<<< HEAD
    if (!fSecure)
        LogPrintf("%s: %s\n", strCaption, message);
=======
    printf("%s: %s\n", strCaption.c_str(), message.c_str());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    fprintf(stderr, "%s: %s\n", strCaption.c_str(), message.c_str());
    return false;
}

<<<<<<< HEAD
static bool noui_ThreadSafeQuestion(const std::string& /* ignored interactive message */, const std::string& message, const std::string& caption, unsigned int style)
{
    return noui_ThreadSafeMessageBox(message, caption, style);
}

static void noui_InitMessage(const std::string& message)
{
    LogPrintf("init message: %s\n", message);
=======
static bool noui_ThreadSafeAskFee(int64 /*nFeeRequired*/)
{
    return true;
}

static void noui_InitMessage(const std::string &message)
{
    printf("init message: %s\n", message.c_str());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void noui_connect()
{
<<<<<<< HEAD
    // Connect wificoind signal handlers
    uiInterface.ThreadSafeMessageBox.connect(noui_ThreadSafeMessageBox);
    uiInterface.ThreadSafeQuestion.connect(noui_ThreadSafeQuestion);
=======
    // Connect bitcoind signal handlers
    uiInterface.ThreadSafeMessageBox.connect(noui_ThreadSafeMessageBox);
    uiInterface.ThreadSafeAskFee.connect(noui_ThreadSafeAskFee);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    uiInterface.InitMessage.connect(noui_InitMessage);
}
