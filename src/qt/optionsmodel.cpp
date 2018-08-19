<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/wificoin-config.h"
#endif

#include "optionsmodel.h"

#include "wificoinunits.h"
#include "guiutil.h"

#include "amount.h"
#include "init.h"
#include "validation.h" // For DEFAULT_SCRIPTCHECK_THREADS
#include "net.h"
#include "netbase.h"
#include "txdb.h" // for -dbcache defaults
#include "intro.h" 

#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
#include "wallet/walletdb.h"
#endif

#include <QNetworkProxy>
#include <QSettings>
#include <QStringList>

OptionsModel::OptionsModel(QObject *parent, bool resetSettings) :
    QAbstractListModel(parent)
{
    Init(resetSettings);
}

void OptionsModel::addOverriddenOption(const std::string &option)
{
    strOverriddenByCommandLine += QString::fromStdString(option) + "=" + QString::fromStdString(gArgs.GetArg(option, "")) + " ";
}

// Writes all missing QSettings with their default values
void OptionsModel::Init(bool resetSettings)
{
    if (resetSettings)
        Reset();

    checkAndMigrate();

    QSettings settings;

    // Ensure restart flag is unset on client startup
    setRestartRequired(false);

    // These are Qt-only settings:

    // Window
    if (!settings.contains("fHideTrayIcon"))
        settings.setValue("fHideTrayIcon", false);
    fHideTrayIcon = settings.value("fHideTrayIcon").toBool();
    Q_EMIT hideTrayIconChanged(fHideTrayIcon);
    
    if (!settings.contains("fMinimizeToTray"))
        settings.setValue("fMinimizeToTray", false);
    fMinimizeToTray = settings.value("fMinimizeToTray").toBool() && !fHideTrayIcon;

    if (!settings.contains("fMinimizeOnClose"))
        settings.setValue("fMinimizeOnClose", false);
    fMinimizeOnClose = settings.value("fMinimizeOnClose").toBool();

    // Display
    if (!settings.contains("nDisplayUnit"))
        settings.setValue("nDisplayUnit", WiFicoinUnits::WFC);
    nDisplayUnit = settings.value("nDisplayUnit").toInt();

    if (!settings.contains("strThirdPartyTxUrls"))
        settings.setValue("strThirdPartyTxUrls", "");
    strThirdPartyTxUrls = settings.value("strThirdPartyTxUrls", "").toString();

    if (!settings.contains("fCoinControlFeatures"))
        settings.setValue("fCoinControlFeatures", false);
    fCoinControlFeatures = settings.value("fCoinControlFeatures", false).toBool();

    // These are shared with the core or have a command-line parameter
    // and we want command-line parameters to overwrite the GUI settings.
    //
    // If setting doesn't exist create it with defaults.
    //
    // If gArgs.SoftSetArg() or gArgs.SoftSetBoolArg() return false we were overridden
    // by command-line and show this in the UI.

    // Main
    if (!settings.contains("nDatabaseCache"))
        settings.setValue("nDatabaseCache", (qint64)nDefaultDbCache);
    if (!gArgs.SoftSetArg("-dbcache", settings.value("nDatabaseCache").toString().toStdString()))
        addOverriddenOption("-dbcache");

    if (!settings.contains("nThreadsScriptVerif"))
        settings.setValue("nThreadsScriptVerif", DEFAULT_SCRIPTCHECK_THREADS);
    if (!gArgs.SoftSetArg("-par", settings.value("nThreadsScriptVerif").toString().toStdString()))
        addOverriddenOption("-par");

    if (!settings.contains("strDataDir"))
        settings.setValue("strDataDir", Intro::getDefaultDataDirectory());

    // Wallet
#ifdef ENABLE_WALLET
    if (!settings.contains("bSpendZeroConfChange"))
        settings.setValue("bSpendZeroConfChange", true);
    if (!gArgs.SoftSetBoolArg("-spendzeroconfchange", settings.value("bSpendZeroConfChange").toBool()))
        addOverriddenOption("-spendzeroconfchange");
#endif

    // Network
    if (!settings.contains("fUseUPnP"))
        settings.setValue("fUseUPnP", DEFAULT_UPNP);
    if (!gArgs.SoftSetBoolArg("-upnp", settings.value("fUseUPnP").toBool()))
        addOverriddenOption("-upnp");

    if (!settings.contains("fListen"))
        settings.setValue("fListen", DEFAULT_LISTEN);
    if (!gArgs.SoftSetBoolArg("-listen", settings.value("fListen").toBool()))
        addOverriddenOption("-listen");

    if (!settings.contains("fUseProxy"))
        settings.setValue("fUseProxy", false);
    if (!settings.contains("addrProxy"))
        settings.setValue("addrProxy", "127.0.0.1:9050");
    // Only try to set -proxy, if user has enabled fUseProxy
    if (settings.value("fUseProxy").toBool() && !gArgs.SoftSetArg("-proxy", settings.value("addrProxy").toString().toStdString()))
        addOverriddenOption("-proxy");
    else if(!settings.value("fUseProxy").toBool() && !gArgs.GetArg("-proxy", "").empty())
        addOverriddenOption("-proxy");

    if (!settings.contains("fUseSeparateProxyTor"))
        settings.setValue("fUseSeparateProxyTor", false);
    if (!settings.contains("addrSeparateProxyTor"))
        settings.setValue("addrSeparateProxyTor", "127.0.0.1:9050");
    // Only try to set -onion, if user has enabled fUseSeparateProxyTor
    if (settings.value("fUseSeparateProxyTor").toBool() && !gArgs.SoftSetArg("-onion", settings.value("addrSeparateProxyTor").toString().toStdString()))
        addOverriddenOption("-onion");
    else if(!settings.value("fUseSeparateProxyTor").toBool() && !gArgs.GetArg("-onion", "").empty())
        addOverriddenOption("-onion");

    // Display
    if (!settings.contains("language"))
        settings.setValue("language", "");
    if (!gArgs.SoftSetArg("-lang", settings.value("language").toString().toStdString()))
        addOverriddenOption("-lang");

    language = settings.value("language").toString();
}

/** Helper function to copy contents from one QSettings to another.
 * By using allKeys this also covers nested settings in a hierarchy.
 */
static void CopySettings(QSettings& dst, const QSettings& src)
{
    for (const QString& key : src.allKeys()) {
        dst.setValue(key, src.value(key));
    }
}

/** Back up a QSettings to an ini-formatted file. */
static void BackupSettings(const fs::path& filename, const QSettings& src)
{
    qWarning() << "Backing up GUI settings to" << GUIUtil::boostPathToQString(filename);
    QSettings dst(GUIUtil::boostPathToQString(filename), QSettings::IniFormat);
    dst.clear();
    CopySettings(dst, src);
}

void OptionsModel::Reset()
{
    QSettings settings;

    // Backup old settings to chain-specific datadir for troubleshooting
    BackupSettings(GetDataDir(true) / "guisettings.ini.bak", settings);

    // Save the strDataDir setting
    QString dataDir = Intro::getDefaultDataDirectory();
    dataDir = settings.value("strDataDir", dataDir).toString();

    // Remove all entries from our QSettings object
    settings.clear();

    // Set strDataDir
    settings.setValue("strDataDir", dataDir);

    // Set that this was reset
    settings.setValue("fReset", true);

    // default setting for OptionsModel::StartAtStartup - disabled
    if (GUIUtil::GetStartOnSystemStartup())
        GUIUtil::SetStartOnSystemStartup(false);
}

=======
#include "optionsmodel.h"

#include "bitcoinunits.h"
#include "init.h"
#include "walletdb.h"
#include "guiutil.h"

#include <QSettings>

OptionsModel::OptionsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    Init();
}

bool static ApplyProxySettings()
{
    QSettings settings;
    CService addrProxy(settings.value("addrProxy", "127.0.0.1:9050").toString().toStdString());
    int nSocksVersion(settings.value("nSocksVersion", 5).toInt());
    if (!settings.value("fUseProxy", false).toBool()) {
        addrProxy = CService();
        nSocksVersion = 0;
        return false;
    }
    if (nSocksVersion && !addrProxy.IsValid())
        return false;
    if (!IsLimited(NET_IPV4))
        SetProxy(NET_IPV4, addrProxy, nSocksVersion);
    if (nSocksVersion > 4) {
#ifdef USE_IPV6
        if (!IsLimited(NET_IPV6))
            SetProxy(NET_IPV6, addrProxy, nSocksVersion);
#endif
        SetNameProxy(addrProxy, nSocksVersion);
    }
    return true;
}

void OptionsModel::Init()
{
    QSettings settings;

    // These are Qt-only settings:
    nDisplayUnit = settings.value("nDisplayUnit", BitcoinUnits::BTC).toInt();
    bDisplayAddresses = settings.value("bDisplayAddresses", false).toBool();
    fMinimizeToTray = settings.value("fMinimizeToTray", false).toBool();
    fMinimizeOnClose = settings.value("fMinimizeOnClose", false).toBool();
    nTransactionFee = settings.value("nTransactionFee").toLongLong();
    language = settings.value("language", "").toString();

    // These are shared with core Bitcoin; we want
    // command-line options to override the GUI settings:
    if (settings.contains("fUseUPnP"))
        SoftSetBoolArg("-upnp", settings.value("fUseUPnP").toBool());
    if (settings.contains("addrProxy") && settings.value("fUseProxy").toBool())
        SoftSetArg("-proxy", settings.value("addrProxy").toString().toStdString());
    if (settings.contains("nSocksVersion") && settings.value("fUseProxy").toBool())
        SoftSetArg("-socks", settings.value("nSocksVersion").toString().toStdString());
    if (!language.isEmpty())
        SoftSetArg("-lang", language.toStdString());
}

void OptionsModel::Reset()
{
    QSettings settings;

    // Remove all entries in this QSettings object
    settings.clear();

    // default setting for OptionsModel::StartAtStartup - disabled
    if (GUIUtil::GetStartOnSystemStartup())
        GUIUtil::SetStartOnSystemStartup(false);

    // Re-Init to get default values
    Init();

    // Ensure Upgrade() is not running again by setting the bImportFinished flag
    settings.setValue("bImportFinished", true);
}

bool OptionsModel::Upgrade()
{
    QSettings settings;

    if (settings.contains("bImportFinished"))
        return false; // Already upgraded

    settings.setValue("bImportFinished", true);

    // Move settings from old wallet.dat (if any):
    CWalletDB walletdb("wallet.dat");

    QList<QString> intOptions;
    intOptions << "nDisplayUnit" << "nTransactionFee";
    foreach(QString key, intOptions)
    {
        int value = 0;
        if (walletdb.ReadSetting(key.toStdString(), value))
        {
            settings.setValue(key, value);
            walletdb.EraseSetting(key.toStdString());
        }
    }
    QList<QString> boolOptions;
    boolOptions << "bDisplayAddresses" << "fMinimizeToTray" << "fMinimizeOnClose" << "fUseProxy" << "fUseUPnP";
    foreach(QString key, boolOptions)
    {
        bool value = false;
        if (walletdb.ReadSetting(key.toStdString(), value))
        {
            settings.setValue(key, value);
            walletdb.EraseSetting(key.toStdString());
        }
    }
    try
    {
        CAddress addrProxyAddress;
        if (walletdb.ReadSetting("addrProxy", addrProxyAddress))
        {
            settings.setValue("addrProxy", addrProxyAddress.ToStringIPPort().c_str());
            walletdb.EraseSetting("addrProxy");
        }
    }
    catch (std::ios_base::failure &e)
    {
        // 0.6.0rc1 saved this as a CService, which causes failure when parsing as a CAddress
        CService addrProxy;
        if (walletdb.ReadSetting("addrProxy", addrProxy))
        {
            settings.setValue("addrProxy", addrProxy.ToStringIPPort().c_str());
            walletdb.EraseSetting("addrProxy");
        }
    }
    ApplyProxySettings();
    Init();

    return true;
}


>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
int OptionsModel::rowCount(const QModelIndex & parent) const
{
    return OptionIDRowCount;
}

<<<<<<< HEAD
// read QSettings values and return them
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
QVariant OptionsModel::data(const QModelIndex & index, int role) const
{
    if(role == Qt::EditRole)
    {
        QSettings settings;
        switch(index.row())
        {
        case StartAtStartup:
<<<<<<< HEAD
            return GUIUtil::GetStartOnSystemStartup();
        case HideTrayIcon:
            return fHideTrayIcon;
        case MinimizeToTray:
            return fMinimizeToTray;
        case MapPortUPnP:
#ifdef USE_UPNP
            return settings.value("fUseUPnP");
#else
            return false;
#endif
        case MinimizeOnClose:
            return fMinimizeOnClose;

        // default proxy
        case ProxyUse:
            return settings.value("fUseProxy", false);
        case ProxyIP: {
            // contains IP at index 0 and port at index 1
            QStringList strlIpPort = settings.value("addrProxy").toString().split(":", QString::SkipEmptyParts);
            return strlIpPort.at(0);
        }
        case ProxyPort: {
            // contains IP at index 0 and port at index 1
            QStringList strlIpPort = settings.value("addrProxy").toString().split(":", QString::SkipEmptyParts);
            return strlIpPort.at(1);
        }

        // separate Tor proxy
        case ProxyUseTor:
            return settings.value("fUseSeparateProxyTor", false);
        case ProxyIPTor: {
            // contains IP at index 0 and port at index 1
            QStringList strlIpPort = settings.value("addrSeparateProxyTor").toString().split(":", QString::SkipEmptyParts);
            return strlIpPort.at(0);
        }
        case ProxyPortTor: {
            // contains IP at index 0 and port at index 1
            QStringList strlIpPort = settings.value("addrSeparateProxyTor").toString().split(":", QString::SkipEmptyParts);
            return strlIpPort.at(1);
        }

#ifdef ENABLE_WALLET
        case SpendZeroConfChange:
            return settings.value("bSpendZeroConfChange");
#endif
        case DisplayUnit:
            return nDisplayUnit;
        case ThirdPartyTxUrls:
            return strThirdPartyTxUrls;
        case Language:
            return settings.value("language");
        case CoinControlFeatures:
            return fCoinControlFeatures;
        case DatabaseCache:
            return settings.value("nDatabaseCache");
        case ThreadsScriptVerif:
            return settings.value("nThreadsScriptVerif");
        case Listen:
            return settings.value("fListen");
=======
            return QVariant(GUIUtil::GetStartOnSystemStartup());
        case MinimizeToTray:
            return QVariant(fMinimizeToTray);
        case MapPortUPnP:
#ifdef USE_UPNP
            return settings.value("fUseUPnP", GetBoolArg("-upnp", true));
#else
            return QVariant(false);
#endif
        case MinimizeOnClose:
            return QVariant(fMinimizeOnClose);
        case ProxyUse: {
            proxyType proxy;
            return QVariant(GetProxy(NET_IPV4, proxy));
        }
        case ProxyIP: {
            proxyType proxy;
            if (GetProxy(NET_IPV4, proxy))
                return QVariant(QString::fromStdString(proxy.first.ToStringIP()));
            else
                return QVariant(QString::fromStdString("127.0.0.1"));
        }
        case ProxyPort: {
            proxyType proxy;
            if (GetProxy(NET_IPV4, proxy))
                return QVariant(proxy.first.GetPort());
            else
                return QVariant(9050);
        }
        case ProxySocksVersion: {
            proxyType proxy;
            if (GetProxy(NET_IPV4, proxy))
                return QVariant(proxy.second);
            else
                return QVariant(5);
        }
        case Fee:
            return QVariant(nTransactionFee);
        case DisplayUnit:
            return QVariant(nDisplayUnit);
        case DisplayAddresses:
            return QVariant(bDisplayAddresses);
        case Language:
            return settings.value("language", "");
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        default:
            return QVariant();
        }
    }
    return QVariant();
}

<<<<<<< HEAD
// write QSettings values
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
bool OptionsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    bool successful = true; /* set to false on parse error */
    if(role == Qt::EditRole)
    {
        QSettings settings;
        switch(index.row())
        {
        case StartAtStartup:
            successful = GUIUtil::SetStartOnSystemStartup(value.toBool());
            break;
<<<<<<< HEAD
        case HideTrayIcon:
            fHideTrayIcon = value.toBool();
            settings.setValue("fHideTrayIcon", fHideTrayIcon);
    		Q_EMIT hideTrayIconChanged(fHideTrayIcon);
            break;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        case MinimizeToTray:
            fMinimizeToTray = value.toBool();
            settings.setValue("fMinimizeToTray", fMinimizeToTray);
            break;
<<<<<<< HEAD
        case MapPortUPnP: // core option - can be changed on-the-fly
=======
        case MapPortUPnP:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            settings.setValue("fUseUPnP", value.toBool());
            MapPort(value.toBool());
            break;
        case MinimizeOnClose:
            fMinimizeOnClose = value.toBool();
            settings.setValue("fMinimizeOnClose", fMinimizeOnClose);
            break;
<<<<<<< HEAD

        // default proxy
        case ProxyUse:
            if (settings.value("fUseProxy") != value) {
                settings.setValue("fUseProxy", value.toBool());
                setRestartRequired(true);
            }
            break;
        case ProxyIP: {
            // contains current IP at index 0 and current port at index 1
            QStringList strlIpPort = settings.value("addrProxy").toString().split(":", QString::SkipEmptyParts);
            // if that key doesn't exist or has a changed IP
            if (!settings.contains("addrProxy") || strlIpPort.at(0) != value.toString()) {
                // construct new value from new IP and current port
                QString strNewValue = value.toString() + ":" + strlIpPort.at(1);
                settings.setValue("addrProxy", strNewValue);
                setRestartRequired(true);
            }
        }
        break;
        case ProxyPort: {
            // contains current IP at index 0 and current port at index 1
            QStringList strlIpPort = settings.value("addrProxy").toString().split(":", QString::SkipEmptyParts);
            // if that key doesn't exist or has a changed port
            if (!settings.contains("addrProxy") || strlIpPort.at(1) != value.toString()) {
                // construct new value from current IP and new port
                QString strNewValue = strlIpPort.at(0) + ":" + value.toString();
                settings.setValue("addrProxy", strNewValue);
                setRestartRequired(true);
            }
        }
        break;

        // separate Tor proxy
        case ProxyUseTor:
            if (settings.value("fUseSeparateProxyTor") != value) {
                settings.setValue("fUseSeparateProxyTor", value.toBool());
                setRestartRequired(true);
            }
            break;
        case ProxyIPTor: {
            // contains current IP at index 0 and current port at index 1
            QStringList strlIpPort = settings.value("addrSeparateProxyTor").toString().split(":", QString::SkipEmptyParts);
            // if that key doesn't exist or has a changed IP
            if (!settings.contains("addrSeparateProxyTor") || strlIpPort.at(0) != value.toString()) {
                // construct new value from new IP and current port
                QString strNewValue = value.toString() + ":" + strlIpPort.at(1);
                settings.setValue("addrSeparateProxyTor", strNewValue);
                setRestartRequired(true);
            }
        }
        break;
        case ProxyPortTor: {
            // contains current IP at index 0 and current port at index 1
            QStringList strlIpPort = settings.value("addrSeparateProxyTor").toString().split(":", QString::SkipEmptyParts);
            // if that key doesn't exist or has a changed port
            if (!settings.contains("addrSeparateProxyTor") || strlIpPort.at(1) != value.toString()) {
                // construct new value from current IP and new port
                QString strNewValue = strlIpPort.at(0) + ":" + value.toString();
                settings.setValue("addrSeparateProxyTor", strNewValue);
                setRestartRequired(true);
            }
        }
        break;

#ifdef ENABLE_WALLET
        case SpendZeroConfChange:
            if (settings.value("bSpendZeroConfChange") != value) {
                settings.setValue("bSpendZeroConfChange", value);
                setRestartRequired(true);
            }
            break;
#endif
        case DisplayUnit:
            setDisplayUnit(value);
            break;
        case ThirdPartyTxUrls:
            if (strThirdPartyTxUrls != value.toString()) {
                strThirdPartyTxUrls = value.toString();
                settings.setValue("strThirdPartyTxUrls", strThirdPartyTxUrls);
                setRestartRequired(true);
            }
            break;
        case Language:
            if (settings.value("language") != value) {
                settings.setValue("language", value);
                setRestartRequired(true);
            }
            break;
        case CoinControlFeatures:
            fCoinControlFeatures = value.toBool();
            settings.setValue("fCoinControlFeatures", fCoinControlFeatures);
            Q_EMIT coinControlFeaturesChanged(fCoinControlFeatures);
            break;
        case DatabaseCache:
            if (settings.value("nDatabaseCache") != value) {
                settings.setValue("nDatabaseCache", value);
                setRestartRequired(true);
            }
            break;
        case ThreadsScriptVerif:
            if (settings.value("nThreadsScriptVerif") != value) {
                settings.setValue("nThreadsScriptVerif", value);
                setRestartRequired(true);
            }
            break;
        case Listen:
            if (settings.value("fListen") != value) {
                settings.setValue("fListen", value);
                setRestartRequired(true);
            }
=======
        case ProxyUse:
            settings.setValue("fUseProxy", value.toBool());
            successful = ApplyProxySettings();
            break;
        case ProxyIP: {
            proxyType proxy;
            proxy.first = CService("127.0.0.1", 9050);
            GetProxy(NET_IPV4, proxy);

            CNetAddr addr(value.toString().toStdString());
            proxy.first.SetIP(addr);
            settings.setValue("addrProxy", proxy.first.ToStringIPPort().c_str());
            successful = ApplyProxySettings();
        }
        break;
        case ProxyPort: {
            proxyType proxy;
            proxy.first = CService("127.0.0.1", 9050);
            GetProxy(NET_IPV4, proxy);

            proxy.first.SetPort(value.toInt());
            settings.setValue("addrProxy", proxy.first.ToStringIPPort().c_str());
            successful = ApplyProxySettings();
        }
        break;
        case ProxySocksVersion: {
            proxyType proxy;
            proxy.second = 5;
            GetProxy(NET_IPV4, proxy);

            proxy.second = value.toInt();
            settings.setValue("nSocksVersion", proxy.second);
            successful = ApplyProxySettings();
        }
        break;
        case Fee:
            nTransactionFee = value.toLongLong();
            settings.setValue("nTransactionFee", nTransactionFee);
            break;
        case DisplayUnit:
            nDisplayUnit = value.toInt();
            settings.setValue("nDisplayUnit", nDisplayUnit);
            emit displayUnitChanged(nDisplayUnit);
            break;
        case DisplayAddresses:
            bDisplayAddresses = value.toBool();
            settings.setValue("bDisplayAddresses", bDisplayAddresses);
            break;
        case Language:
            settings.setValue("language", value);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            break;
        default:
            break;
        }
    }
<<<<<<< HEAD

    Q_EMIT dataChanged(index, index);
=======
    emit dataChanged(index, index);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    return successful;
}

<<<<<<< HEAD
/** Updates current unit in memory, settings and emits displayUnitChanged(newUnit) signal */
void OptionsModel::setDisplayUnit(const QVariant &value)
{
    if (!value.isNull())
    {
        QSettings settings;
        nDisplayUnit = value.toInt();
        settings.setValue("nDisplayUnit", nDisplayUnit);
        Q_EMIT displayUnitChanged(nDisplayUnit);
    }
}

bool OptionsModel::getProxySettings(QNetworkProxy& proxy) const
{
    // Directly query current base proxy, because
    // GUI settings can be overridden with -proxy.
    proxyType curProxy;
    if (GetProxy(NET_IPV4, curProxy)) {
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(QString::fromStdString(curProxy.proxy.ToStringIP()));
        proxy.setPort(curProxy.proxy.GetPort());

        return true;
    }
    else
        proxy.setType(QNetworkProxy::NoProxy);

    return false;
}

void OptionsModel::setRestartRequired(bool fRequired)
{
    QSettings settings;
    return settings.setValue("fRestartRequired", fRequired);
}

bool OptionsModel::isRestartRequired()
{
    QSettings settings;
    return settings.value("fRestartRequired", false).toBool();
}

void OptionsModel::checkAndMigrate()
{
    // Migration of default values
    // Check if the QSettings container was already loaded with this client version
    QSettings settings;
    static const char strSettingsVersionKey[] = "nSettingsVersion";
    int settingsVersion = settings.contains(strSettingsVersionKey) ? settings.value(strSettingsVersionKey).toInt() : 0;
    if (settingsVersion < CLIENT_VERSION)
    {
        // -dbcache was bumped from 100 to 300 in 0.13
        // see https://github.com/wificoin/wificoin/pull/8273
        // force people to upgrade to the new value if they are using 100MB
        if (settingsVersion < 130000 && settings.contains("nDatabaseCache") && settings.value("nDatabaseCache").toLongLong() == 100)
            settings.setValue("nDatabaseCache", (qint64)nDefaultDbCache);

        settings.setValue(strSettingsVersionKey, CLIENT_VERSION);
    }
=======
qint64 OptionsModel::getTransactionFee()
{
    return nTransactionFee;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
