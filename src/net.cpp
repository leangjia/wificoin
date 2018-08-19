// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/wificoin-config.h"
#endif

#include "net.h"

#include "addrman.h"
#include "chainparams.h"
#include "clientversion.h"
#include "consensus/consensus.h"
#include "crypto/common.h"
#include "crypto/sha256.h"
#include "hash.h"
#include "primitives/transaction.h"
#include "netbase.h"
#include "scheduler.h"
#include "ui_interface.h"
#include "utilstrencodings.h"

#ifdef WIN32
#include <string.h>
#else
#include <fcntl.h>
#endif

#ifdef USE_UPNP
#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/miniwget.h>
=======
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "db.h"
#include "net.h"
#include "init.h"
#include "addrman.h"
#include "ui_interface.h"
#include "script.h"

#ifdef WIN32
#include <string.h>
#endif

#ifdef USE_UPNP
#include <miniupnpc/miniwget.h>
#include <miniupnpc/miniupnpc.h>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>
#endif

<<<<<<< HEAD

#include <math.h>

// Dump addresses to peers.dat and banlist.dat every 15 minutes (900s)
#define DUMP_ADDRESSES_INTERVAL 900

// We add a random period time (0 to 1 seconds) to feeler connections to prevent synchronization.
#define FEELER_SLEEP_WINDOW 1

#if !defined(HAVE_MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

// MSG_DONTWAIT is not available on some platforms, if it doesn't exist define it as 0
#if !defined(HAVE_MSG_DONTWAIT)
#define MSG_DONTWAIT 0
#endif

// Fix for ancient MinGW versions, that don't have defined these in ws2tcpip.h.
// Todo: Can be removed when our pull-tester is upgraded to a modern MinGW version.
#ifdef WIN32
#ifndef PROTECTION_LEVEL_UNRESTRICTED
#define PROTECTION_LEVEL_UNRESTRICTED 10
#endif
#ifndef IPV6_PROTECTION_LEVEL
#define IPV6_PROTECTION_LEVEL 23
#endif
#endif

/** Used to pass flags to the Bind() function */
enum BindFlags {
    BF_NONE         = 0,
    BF_EXPLICIT     = (1U << 0),
    BF_REPORT_ERROR = (1U << 1),
    BF_WHITELIST    = (1U << 2),
};

const static std::string NET_MESSAGE_COMMAND_OTHER = "*other*";

static const uint64_t RANDOMIZER_ID_NETGROUP = 0x6c0edd8036ef4036ULL; // SHA256("netgroup")[0:8]
static const uint64_t RANDOMIZER_ID_LOCALHOSTNONCE = 0xd93e69e2bbfa5735ULL; // SHA256("localhostnonce")[0:8]
=======
// Dump addresses to peers.dat every 15 minutes (900s)
#define DUMP_ADDRESSES_INTERVAL 900

using namespace std;
using namespace boost;

static const int MAX_OUTBOUND_CONNECTIONS = 8;

bool OpenNetworkConnection(const CAddress& addrConnect, CSemaphoreGrant *grantOutbound = NULL, const char *strDest = NULL, bool fOneShot = false);


struct LocalServiceInfo {
    int nScore;
    int nPort;
};

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
//
// Global state variables
//
bool fDiscover = true;
<<<<<<< HEAD
bool fListen = true;
bool fRelayTxes = true;
CCriticalSection cs_mapLocalHost;
std::map<CNetAddr, LocalServiceInfo> mapLocalHost;
static bool vfLimited[NET_MAX] = {};
std::string strSubVersion;

limitedmap<uint256, int64_t> mapAlreadyAskedFor(MAX_INV_SZ);

void CConnman::AddOneShot(const std::string& strDest)
=======
uint64 nLocalServices = NODE_NETWORK;
static CCriticalSection cs_mapLocalHost;
static map<CNetAddr, LocalServiceInfo> mapLocalHost;
static bool vfReachable[NET_MAX] = {};
static bool vfLimited[NET_MAX] = {};
static CNode* pnodeLocalHost = NULL;
static CNode* pnodeSync = NULL;
uint64 nLocalHostNonce = 0;
static std::vector<SOCKET> vhListenSocket;
CAddrMan addrman;
int nMaxConnections = 125;

vector<CNode*> vNodes;
CCriticalSection cs_vNodes;
map<CInv, CDataStream> mapRelay;
deque<pair<int64, CInv> > vRelayExpiration;
CCriticalSection cs_mapRelay;
limitedmap<CInv, int64> mapAlreadyAskedFor(MAX_INV_SZ);

static deque<string> vOneShots;
CCriticalSection cs_vOneShots;

set<CNetAddr> setservAddNodeAddresses;
CCriticalSection cs_setservAddNodeAddresses;

vector<std::string> vAddedNodes;
CCriticalSection cs_vAddedNodes;

static CSemaphore *semOutbound = NULL;

void AddOneShot(string strDest)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    LOCK(cs_vOneShots);
    vOneShots.push_back(strDest);
}

unsigned short GetListenPort()
{
<<<<<<< HEAD
    return (unsigned short)(gArgs.GetArg("-port", Params().GetDefaultPort()));
=======
    return (unsigned short)(GetArg("-port", GetDefaultPort()));
}

void CNode::PushGetBlocks(CBlockIndex* pindexBegin, uint256 hashEnd)
{
    // Filter out duplicate requests
    if (pindexBegin == pindexLastGetBlocksBegin && hashEnd == hashLastGetBlocksEnd)
        return;
    pindexLastGetBlocksBegin = pindexBegin;
    hashLastGetBlocksEnd = hashEnd;

    PushMessage("getblocks", CBlockLocator(pindexBegin), hashEnd);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

// find 'best' local address for a particular peer
bool GetLocal(CService& addr, const CNetAddr *paddrPeer)
{
<<<<<<< HEAD
    if (!fListen)
=======
    if (fNoListen)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        return false;

    int nBestScore = -1;
    int nBestReachability = -1;
    {
        LOCK(cs_mapLocalHost);
<<<<<<< HEAD
        for (std::map<CNetAddr, LocalServiceInfo>::iterator it = mapLocalHost.begin(); it != mapLocalHost.end(); it++)
=======
        for (map<CNetAddr, LocalServiceInfo>::iterator it = mapLocalHost.begin(); it != mapLocalHost.end(); it++)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        {
            int nScore = (*it).second.nScore;
            int nReachability = (*it).first.GetReachabilityFrom(paddrPeer);
            if (nReachability > nBestReachability || (nReachability == nBestReachability && nScore > nBestScore))
            {
                addr = CService((*it).first, (*it).second.nPort);
                nBestReachability = nReachability;
                nBestScore = nScore;
            }
        }
    }
    return nBestScore >= 0;
}

<<<<<<< HEAD
//! Convert the pnSeeds6 array into usable address objects.
static std::vector<CAddress> convertSeed6(const std::vector<SeedSpec6> &vSeedsIn)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    std::vector<CAddress> vSeedsOut;
    vSeedsOut.reserve(vSeedsIn.size());
    for (std::vector<SeedSpec6>::const_iterator i(vSeedsIn.begin()); i != vSeedsIn.end(); ++i)
    {
        struct in6_addr ip;
        memcpy(&ip, i->addr, sizeof(ip));
        CAddress addr(CService(ip, i->port), NODE_NETWORK);
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
    return vSeedsOut;
}

// get best local address for a particular peer as a CAddress
// Otherwise, return the unroutable 0.0.0.0 but filled in with
// the normal parameters, since the IP may be changed to a useful
// one by discovery.
CAddress GetLocalAddress(const CNetAddr *paddrPeer, ServiceFlags nLocalServices)
{
    CAddress ret(CService(CNetAddr(),GetListenPort()), nLocalServices);
    CService addr;
    if (GetLocal(addr, paddrPeer))
    {
        ret = CAddress(addr, nLocalServices);
    }
    ret.nTime = GetAdjustedTime();
    return ret;
}

int GetnScore(const CService& addr)
{
    LOCK(cs_mapLocalHost);
    if (mapLocalHost.count(addr) == LOCAL_NONE)
        return 0;
    return mapLocalHost[addr].nScore;
}

// Is our peer's addrLocal potentially useful as an external IP source?
bool IsPeerAddrLocalGood(CNode *pnode)
{
    CService addrLocal = pnode->GetAddrLocal();
    return fDiscover && pnode->addr.IsRoutable() && addrLocal.IsRoutable() &&
           !IsLimited(addrLocal.GetNetwork());
}

// pushes our own address to a peer
void AdvertiseLocal(CNode *pnode)
{
    if (fListen && pnode->fSuccessfullyConnected)
    {
        CAddress addrLocal = GetLocalAddress(&pnode->addr, pnode->GetLocalServices());
        // If discovery is enabled, sometimes give our peer the address it
        // tells us that it sees us as in case it has a better idea of our
        // address than we do.
        if (IsPeerAddrLocalGood(pnode) && (!addrLocal.IsRoutable() ||
             GetRand((GetnScore(addrLocal) > LOCAL_MANUAL) ? 8:2) == 0))
        {
            addrLocal.SetIP(pnode->GetAddrLocal());
        }
        if (addrLocal.IsRoutable())
        {
            LogPrint(BCLog::NET, "AdvertiseLocal: advertising address %s\n", addrLocal.ToString());
            FastRandomContext insecure_rand;
            pnode->PushAddress(addrLocal, insecure_rand);
=======
// get best local address for a particular peer as a CAddress
CAddress GetLocalAddress(const CNetAddr *paddrPeer)
{
    CAddress ret(CService("0.0.0.0",0),0);
    CService addr;
    if (GetLocal(addr, paddrPeer))
    {
        ret = CAddress(addr);
        ret.nServices = nLocalServices;
        ret.nTime = GetAdjustedTime();
    }
    return ret;
}

bool RecvLine(SOCKET hSocket, string& strLine)
{
    strLine = "";
    loop
    {
        char c;
        int nBytes = recv(hSocket, &c, 1, 0);
        if (nBytes > 0)
        {
            if (c == '\n')
                continue;
            if (c == '\r')
                return true;
            strLine += c;
            if (strLine.size() >= 9000)
                return true;
        }
        else if (nBytes <= 0)
        {
            boost::this_thread::interruption_point();
            if (nBytes < 0)
            {
                int nErr = WSAGetLastError();
                if (nErr == WSAEMSGSIZE)
                    continue;
                if (nErr == WSAEWOULDBLOCK || nErr == WSAEINTR || nErr == WSAEINPROGRESS)
                {
                    MilliSleep(10);
                    continue;
                }
            }
            if (!strLine.empty())
                return true;
            if (nBytes == 0)
            {
                // socket closed
                printf("socket closed\n");
                return false;
            }
            else
            {
                // socket error
                int nErr = WSAGetLastError();
                printf("recv failed: %d\n", nErr);
                return false;
            }
        }
    }
}

// used when scores of local addresses may have changed
// pushes better local address to peers
void static AdvertizeLocal()
{
    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
    {
        if (pnode->fSuccessfullyConnected)
        {
            CAddress addrLocal = GetLocalAddress(&pnode->addr);
            if (addrLocal.IsRoutable() && (CService)addrLocal != (CService)pnode->addrLocal)
            {
                pnode->PushAddress(addrLocal);
                pnode->addrLocal = addrLocal;
            }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
    }
}

<<<<<<< HEAD
=======
void SetReachable(enum Network net, bool fFlag)
{
    LOCK(cs_mapLocalHost);
    vfReachable[net] = fFlag;
    if (net == NET_IPV6 && fFlag)
        vfReachable[NET_IPV4] = true;
}

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
// learn a new local address
bool AddLocal(const CService& addr, int nScore)
{
    if (!addr.IsRoutable())
        return false;

    if (!fDiscover && nScore < LOCAL_MANUAL)
        return false;

    if (IsLimited(addr))
        return false;

<<<<<<< HEAD
    LogPrintf("AddLocal(%s,%i)\n", addr.ToString(), nScore);
=======
    printf("AddLocal(%s,%i)\n", addr.ToString().c_str(), nScore);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    {
        LOCK(cs_mapLocalHost);
        bool fAlready = mapLocalHost.count(addr) > 0;
        LocalServiceInfo &info = mapLocalHost[addr];
        if (!fAlready || nScore >= info.nScore) {
            info.nScore = nScore + (fAlready ? 1 : 0);
            info.nPort = addr.GetPort();
        }
<<<<<<< HEAD
    }

=======
        SetReachable(addr.GetNetwork());
    }

    AdvertizeLocal();

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    return true;
}

bool AddLocal(const CNetAddr &addr, int nScore)
{
    return AddLocal(CService(addr, GetListenPort()), nScore);
}

<<<<<<< HEAD
bool RemoveLocal(const CService& addr)
{
    LOCK(cs_mapLocalHost);
    LogPrintf("RemoveLocal(%s)\n", addr.ToString());
    mapLocalHost.erase(addr);
    return true;
}

/** Make a particular network entirely off-limits (no automatic connects to it) */
void SetLimited(enum Network net, bool fLimited)
{
    if (net == NET_UNROUTABLE || net == NET_INTERNAL)
=======
/** Make a particular network entirely off-limits (no automatic connects to it) */
void SetLimited(enum Network net, bool fLimited)
{
    if (net == NET_UNROUTABLE)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        return;
    LOCK(cs_mapLocalHost);
    vfLimited[net] = fLimited;
}

bool IsLimited(enum Network net)
{
    LOCK(cs_mapLocalHost);
    return vfLimited[net];
}

bool IsLimited(const CNetAddr &addr)
{
    return IsLimited(addr.GetNetwork());
}

/** vote for a local address */
bool SeenLocal(const CService& addr)
{
    {
        LOCK(cs_mapLocalHost);
        if (mapLocalHost.count(addr) == 0)
            return false;
        mapLocalHost[addr].nScore++;
    }
<<<<<<< HEAD
    return true;
}


=======

    AdvertizeLocal();

    return true;
}

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
/** check whether a given address is potentially local */
bool IsLocal(const CService& addr)
{
    LOCK(cs_mapLocalHost);
    return mapLocalHost.count(addr) > 0;
}

<<<<<<< HEAD
/** check whether a given network is one we can probably connect to */
bool IsReachable(enum Network net)
{
    LOCK(cs_mapLocalHost);
    return !vfLimited[net];
}

/** check whether a given address is in a network we can probably connect to */
bool IsReachable(const CNetAddr& addr)
{
    enum Network net = addr.GetNetwork();
    return IsReachable(net);
}


CNode* CConnman::FindNode(const CNetAddr& ip)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes)
        if ((CNetAddr)pnode->addr == ip)
            return (pnode);
    return nullptr;
}

CNode* CConnman::FindNode(const CSubNet& subNet)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes)
    if (subNet.Match((CNetAddr)pnode->addr))
        return (pnode);
    return nullptr;
}

CNode* CConnman::FindNode(const std::string& addrName)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (pnode->GetAddrName() == addrName) {
            return (pnode);
        }
    }
    return nullptr;
}

CNode* CConnman::FindNode(const CService& addr)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes)
        if ((CService)pnode->addr == addr)
            return (pnode);
    return nullptr;
}

bool CConnman::CheckIncomingNonce(uint64_t nonce)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (!pnode->fSuccessfullyConnected && !pnode->fInbound && pnode->GetLocalNonce() == nonce)
            return false;
    }
    return true;
}

/** Get the bind address for a socket as CAddress */
static CAddress GetBindAddress(SOCKET sock)
{
    CAddress addr_bind;
    struct sockaddr_storage sockaddr_bind;
    socklen_t sockaddr_bind_len = sizeof(sockaddr_bind);
    if (sock != INVALID_SOCKET) {
        if (!getsockname(sock, (struct sockaddr*)&sockaddr_bind, &sockaddr_bind_len)) {
            addr_bind.SetSockAddr((const struct sockaddr*)&sockaddr_bind);
        } else {
            LogPrint(BCLog::NET, "Warning: getsockname failed\n");
        }
    }
    return addr_bind;
}

CNode* CConnman::ConnectNode(CAddress addrConnect, const char *pszDest, bool fCountFailure)
{
    if (pszDest == nullptr) {
        if (IsLocal(addrConnect))
            return nullptr;
=======
/** check whether a given address is in a network we can probably connect to */
bool IsReachable(const CNetAddr& addr)
{
    LOCK(cs_mapLocalHost);
    enum Network net = addr.GetNetwork();
    return vfReachable[net] && !vfLimited[net];
}

bool GetMyExternalIP2(const CService& addrConnect, const char* pszGet, const char* pszKeyword, CNetAddr& ipRet)
{
    SOCKET hSocket;
    if (!ConnectSocket(addrConnect, hSocket))
        return error("GetMyExternalIP() : connection to %s failed", addrConnect.ToString().c_str());

    send(hSocket, pszGet, strlen(pszGet), MSG_NOSIGNAL);

    string strLine;
    while (RecvLine(hSocket, strLine))
    {
        if (strLine.empty()) // HTTP response is separated from headers by blank line
        {
            loop
            {
                if (!RecvLine(hSocket, strLine))
                {
                    closesocket(hSocket);
                    return false;
                }
                if (pszKeyword == NULL)
                    break;
                if (strLine.find(pszKeyword) != string::npos)
                {
                    strLine = strLine.substr(strLine.find(pszKeyword) + strlen(pszKeyword));
                    break;
                }
            }
            closesocket(hSocket);
            if (strLine.find("<") != string::npos)
                strLine = strLine.substr(0, strLine.find("<"));
            strLine = strLine.substr(strspn(strLine.c_str(), " \t\n\r"));
            while (strLine.size() > 0 && isspace(strLine[strLine.size()-1]))
                strLine.resize(strLine.size()-1);
            CService addr(strLine,0,true);
            printf("GetMyExternalIP() received [%s] %s\n", strLine.c_str(), addr.ToString().c_str());
            if (!addr.IsValid() || !addr.IsRoutable())
                return false;
            ipRet.SetIP(addr);
            return true;
        }
    }
    closesocket(hSocket);
    return error("GetMyExternalIP() : connection closed");
}

bool GetMyExternalIP(CNetAddr& ipRet)
{
    CService addrConnect;
    const char* pszGet;
    const char* pszKeyword;

    for (int nLookup = 0; nLookup <= 1; nLookup++)
    for (int nHost = 1; nHost <= 2; nHost++)
    {
        // We should be phasing out our use of sites like these. If we need
        // replacements, we should ask for volunteers to put this simple
        // php file on their web server that prints the client IP:
        //  <?php echo $_SERVER["REMOTE_ADDR"]; ?>
        if (nHost == 1)
        {
            addrConnect = CService("91.198.22.70", 80); // checkip.dyndns.org

            if (nLookup == 1)
            {
                CService addrIP("checkip.dyndns.org", 80, true);
                if (addrIP.IsValid())
                    addrConnect = addrIP;
            }

            pszGet = "GET / HTTP/1.1\r\n"
                     "Host: checkip.dyndns.org\r\n"
                     "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)\r\n"
                     "Connection: close\r\n"
                     "\r\n";

            pszKeyword = "Address:";
        }
        else if (nHost == 2)
        {
            addrConnect = CService("74.208.43.192", 80); // www.showmyip.com

            if (nLookup == 1)
            {
                CService addrIP("www.showmyip.com", 80, true);
                if (addrIP.IsValid())
                    addrConnect = addrIP;
            }

            pszGet = "GET /simple/ HTTP/1.1\r\n"
                     "Host: www.showmyip.com\r\n"
                     "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)\r\n"
                     "Connection: close\r\n"
                     "\r\n";

            pszKeyword = NULL; // Returns just IP address
        }

        if (GetMyExternalIP2(addrConnect, pszGet, pszKeyword, ipRet))
            return true;
    }

    return false;
}

void ThreadGetMyExternalIP(void* parg)
{
    // Make this thread recognisable as the external IP detection thread
    RenameThread("bitcoin-ext-ip");

    CNetAddr addrLocalHost;
    if (GetMyExternalIP(addrLocalHost))
    {
        printf("GetMyExternalIP() returned %s\n", addrLocalHost.ToStringIP().c_str());
        AddLocal(addrLocalHost, LOCAL_HTTP);
    }
}





void AddressCurrentlyConnected(const CService& addr)
{
    addrman.Connected(addr);
}







CNode* FindNode(const CNetAddr& ip)
{
    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
        if ((CNetAddr)pnode->addr == ip)
            return (pnode);
    return NULL;
}

CNode* FindNode(std::string addrName)
{
    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
        if (pnode->addrName == addrName)
            return (pnode);
    return NULL;
}

CNode* FindNode(const CService& addr)
{
    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
        if ((CService)pnode->addr == addr)
            return (pnode);
    return NULL;
}

CNode* ConnectNode(CAddress addrConnect, const char *pszDest)
{
    if (pszDest == NULL) {
        if (IsLocal(addrConnect))
            return NULL;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        // Look for an existing connection
        CNode* pnode = FindNode((CService)addrConnect);
        if (pnode)
        {
<<<<<<< HEAD
            LogPrintf("Failed to open new connection, already connected\n");
            return nullptr;
        }
    }

    /// debug print
    LogPrint(BCLog::NET, "trying connection %s lastseen=%.1fhrs\n",
        pszDest ? pszDest : addrConnect.ToString(),
        pszDest ? 0.0 : (double)(GetAdjustedTime() - addrConnect.nTime)/3600.0);

    // Connect
    SOCKET hSocket;
    bool proxyConnectionFailed = false;
    if (pszDest ? ConnectSocketByName(addrConnect, hSocket, pszDest, Params().GetDefaultPort(), nConnectTimeout, &proxyConnectionFailed) :
                  ConnectSocket(addrConnect, hSocket, nConnectTimeout, &proxyConnectionFailed))
    {
        if (!IsSelectableSocket(hSocket)) {
            LogPrintf("Cannot create connection: non-selectable socket created (fd >= FD_SETSIZE ?)\n");
            CloseSocket(hSocket);
            return nullptr;
        }

        if (pszDest && addrConnect.IsValid()) {
            // It is possible that we already have a connection to the IP/port pszDest resolved to.
            // In that case, drop the connection that was just created, and return the existing CNode instead.
            // Also store the name we used to connect in that CNode, so that future FindNode() calls to that
            // name catch this early.
            LOCK(cs_vNodes);
            CNode* pnode = FindNode((CService)addrConnect);
            if (pnode)
            {
                pnode->MaybeSetAddrName(std::string(pszDest));
                CloseSocket(hSocket);
                LogPrintf("Failed to open new connection, already connected\n");
                return nullptr;
            }
        }

        addrman.Attempt(addrConnect, fCountFailure);

        // Add node
        NodeId id = GetNewNodeId();
        uint64_t nonce = GetDeterministicRandomizer(RANDOMIZER_ID_LOCALHOSTNONCE).Write(id).Finalize();
        CAddress addr_bind = GetBindAddress(hSocket);
        CNode* pnode = new CNode(id, nLocalServices, GetBestHeight(), hSocket, addrConnect, CalculateKeyedNetGroup(addrConnect), nonce, addr_bind, pszDest ? pszDest : "", false);
        pnode->nServicesExpected = ServiceFlags(addrConnect.nServices & nRelevantServices);
        pnode->AddRef();

        return pnode;
    } else if (!proxyConnectionFailed) {
        // If connecting to the node failed, and failure is not caused by a problem connecting to
        // the proxy, mark this as an attempt.
        addrman.Attempt(addrConnect, fCountFailure);
    }

    return nullptr;
}

void CConnman::DumpBanlist()
{
    SweepBanned(); // clean unused entries (if bantime has expired)

    if (!BannedSetIsDirty())
        return;

    int64_t nStart = GetTimeMillis();

    CBanDB bandb;
    banmap_t banmap;
    GetBanned(banmap);
    if (bandb.Write(banmap)) {
        SetBannedSetDirty(false);
    }

    LogPrint(BCLog::NET, "Flushed %d banned node ips/subnets to banlist.dat  %dms\n",
        banmap.size(), GetTimeMillis() - nStart);
=======
            pnode->AddRef();
            return pnode;
        }
    }


    /// debug print
    printf("trying connection %s lastseen=%.1fhrs\n",
        pszDest ? pszDest : addrConnect.ToString().c_str(),
        pszDest ? 0 : (double)(GetAdjustedTime() - addrConnect.nTime)/3600.0);

    // Connect
    SOCKET hSocket;
    if (pszDest ? ConnectSocketByName(addrConnect, hSocket, pszDest, GetDefaultPort()) : ConnectSocket(addrConnect, hSocket))
    {
        addrman.Attempt(addrConnect);

        /// debug print
        printf("connected %s\n", pszDest ? pszDest : addrConnect.ToString().c_str());

        // Set to non-blocking
#ifdef WIN32
        u_long nOne = 1;
        if (ioctlsocket(hSocket, FIONBIO, &nOne) == SOCKET_ERROR)
            printf("ConnectSocket() : ioctlsocket non-blocking setting failed, error %d\n", WSAGetLastError());
#else
        if (fcntl(hSocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
            printf("ConnectSocket() : fcntl non-blocking setting failed, error %d\n", errno);
#endif

        // Add node
        CNode* pnode = new CNode(hSocket, addrConnect, pszDest ? pszDest : "", false);
        pnode->AddRef();

        {
            LOCK(cs_vNodes);
            vNodes.push_back(pnode);
        }

        pnode->nTimeConnected = GetTime();
        return pnode;
    }
    else
    {
        return NULL;
    }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void CNode::CloseSocketDisconnect()
{
    fDisconnect = true;
<<<<<<< HEAD
    LOCK(cs_hSocket);
    if (hSocket != INVALID_SOCKET)
    {
        LogPrint(BCLog::NET, "disconnecting peer=%d\n", id);
        CloseSocket(hSocket);
    }
}

void CConnman::ClearBanned()
{
    {
        LOCK(cs_setBanned);
        setBanned.clear();
        setBannedIsDirty = true;
    }
    DumpBanlist(); //store banlist to disk
    if(clientInterface)
        clientInterface->BannedListChanged();
}

bool CConnman::IsBanned(CNetAddr ip)
{
    LOCK(cs_setBanned);
    for (banmap_t::iterator it = setBanned.begin(); it != setBanned.end(); it++)
    {
        CSubNet subNet = (*it).first;
        CBanEntry banEntry = (*it).second;

        if (subNet.Match(ip) && GetTime() < banEntry.nBanUntil) {
            return true;
        }
    }
    return false;
}

bool CConnman::IsBanned(CSubNet subnet)
{
    LOCK(cs_setBanned);
    banmap_t::iterator i = setBanned.find(subnet);
    if (i != setBanned.end())
    {
        CBanEntry banEntry = (*i).second;
        if (GetTime() < banEntry.nBanUntil) {
            return true;
        }
    }
    return false;
}

void CConnman::Ban(const CNetAddr& addr, const BanReason &banReason, int64_t bantimeoffset, bool sinceUnixEpoch) {
    CSubNet subNet(addr);
    Ban(subNet, banReason, bantimeoffset, sinceUnixEpoch);
}

void CConnman::Ban(const CSubNet& subNet, const BanReason &banReason, int64_t bantimeoffset, bool sinceUnixEpoch) {
    CBanEntry banEntry(GetTime());
    banEntry.banReason = banReason;
    if (bantimeoffset <= 0)
    {
        bantimeoffset = gArgs.GetArg("-bantime", DEFAULT_MISBEHAVING_BANTIME);
        sinceUnixEpoch = false;
    }
    banEntry.nBanUntil = (sinceUnixEpoch ? 0 : GetTime() )+bantimeoffset;

    {
        LOCK(cs_setBanned);
        if (setBanned[subNet].nBanUntil < banEntry.nBanUntil) {
            setBanned[subNet] = banEntry;
            setBannedIsDirty = true;
        }
        else
            return;
    }
    if(clientInterface)
        clientInterface->BannedListChanged();
    {
        LOCK(cs_vNodes);
        for (CNode* pnode : vNodes) {
            if (subNet.Match((CNetAddr)pnode->addr))
                pnode->fDisconnect = true;
        }
    }
    if(banReason == BanReasonManuallyAdded)
        DumpBanlist(); //store banlist to disk immediately if user requested ban
}

bool CConnman::Unban(const CNetAddr &addr) {
    CSubNet subNet(addr);
    return Unban(subNet);
}

bool CConnman::Unban(const CSubNet &subNet) {
    {
        LOCK(cs_setBanned);
        if (!setBanned.erase(subNet))
            return false;
        setBannedIsDirty = true;
    }
    if(clientInterface)
        clientInterface->BannedListChanged();
    DumpBanlist(); //store banlist to disk immediately
    return true;
}

void CConnman::GetBanned(banmap_t &banMap)
{
    LOCK(cs_setBanned);
    // Sweep the banlist so expired bans are not returned
    SweepBanned();
    banMap = setBanned; //create a thread safe copy
}

void CConnman::SetBanned(const banmap_t &banMap)
{
    LOCK(cs_setBanned);
    setBanned = banMap;
    setBannedIsDirty = true;
}

void CConnman::SweepBanned()
{
    int64_t now = GetTime();

    LOCK(cs_setBanned);
    banmap_t::iterator it = setBanned.begin();
    while(it != setBanned.end())
    {
        CSubNet subNet = (*it).first;
        CBanEntry banEntry = (*it).second;
        if(now > banEntry.nBanUntil)
        {
            setBanned.erase(it++);
            setBannedIsDirty = true;
            LogPrint(BCLog::NET, "%s: Removed banned node ip/subnet from banlist.dat: %s\n", __func__, subNet.ToString());
        }
        else
            ++it;
    }
}

bool CConnman::BannedSetIsDirty()
{
    LOCK(cs_setBanned);
    return setBannedIsDirty;
}

void CConnman::SetBannedSetDirty(bool dirty)
{
    LOCK(cs_setBanned); //reuse setBanned lock for the isDirty flag
    setBannedIsDirty = dirty;
}


bool CConnman::IsWhitelistedRange(const CNetAddr &addr) {
    for (const CSubNet& subnet : vWhitelistedRange) {
        if (subnet.Match(addr))
            return true;
    }
    return false;
}

std::string CNode::GetAddrName() const {
    LOCK(cs_addrName);
    return addrName;
}

void CNode::MaybeSetAddrName(const std::string& addrNameIn) {
    LOCK(cs_addrName);
    if (addrName.empty()) {
        addrName = addrNameIn;
    }
}

CService CNode::GetAddrLocal() const {
    LOCK(cs_addrLocal);
    return addrLocal;
}

void CNode::SetAddrLocal(const CService& addrLocalIn) {
    LOCK(cs_addrLocal);
    if (addrLocal.IsValid()) {
        error("Addr local already set for node: %i. Refusing to change from %s to %s", id, addrLocal.ToString(), addrLocalIn.ToString());
    } else {
        addrLocal = addrLocalIn;
    }
=======
    if (hSocket != INVALID_SOCKET)
    {
        printf("disconnecting node %s\n", addrName.c_str());
        closesocket(hSocket);
        hSocket = INVALID_SOCKET;
    }

    // in case this fails, we'll empty the recv buffer when the CNode is deleted
    TRY_LOCK(cs_vRecvMsg, lockRecv);
    if (lockRecv)
        vRecvMsg.clear();

    // if this was the sync node, we'll need a new one
    if (this == pnodeSync)
        pnodeSync = NULL;
}

void CNode::Cleanup()
{
}


void CNode::PushVersion()
{
    /// when NTP implemented, change to just nTime = GetAdjustedTime()
    int64 nTime = (fInbound ? GetAdjustedTime() : GetTime());
    CAddress addrYou = (addr.IsRoutable() && !IsProxy(addr) ? addr : CAddress(CService("0.0.0.0",0)));
    CAddress addrMe = GetLocalAddress(&addr);
    RAND_bytes((unsigned char*)&nLocalHostNonce, sizeof(nLocalHostNonce));
    printf("send version message: version %d, blocks=%d, us=%s, them=%s, peer=%s\n", PROTOCOL_VERSION, nBestHeight, addrMe.ToString().c_str(), addrYou.ToString().c_str(), addr.ToString().c_str());
    PushMessage("version", PROTOCOL_VERSION, nLocalServices, nTime, addrYou, addrMe,
                nLocalHostNonce, FormatSubVersion(CLIENT_NAME, CLIENT_VERSION, std::vector<string>()), nBestHeight);
}





std::map<CNetAddr, int64> CNode::setBanned;
CCriticalSection CNode::cs_setBanned;

void CNode::ClearBanned()
{
    setBanned.clear();
}

bool CNode::IsBanned(CNetAddr ip)
{
    bool fResult = false;
    {
        LOCK(cs_setBanned);
        std::map<CNetAddr, int64>::iterator i = setBanned.find(ip);
        if (i != setBanned.end())
        {
            int64 t = (*i).second;
            if (GetTime() < t)
                fResult = true;
        }
    }
    return fResult;
}

bool CNode::Misbehaving(int howmuch)
{
    if (addr.IsLocal())
    {
        printf("Warning: Local node %s misbehaving (delta: %d)!\n", addrName.c_str(), howmuch);
        return false;
    }

    nMisbehavior += howmuch;
    if (nMisbehavior >= GetArg("-banscore", 100))
    {
        int64 banTime = GetTime()+GetArg("-bantime", 60*60*24);  // Default 24-hour ban
        printf("Misbehaving: %s (%d -> %d) DISCONNECTING\n", addr.ToString().c_str(), nMisbehavior-howmuch, nMisbehavior);
        {
            LOCK(cs_setBanned);
            if (setBanned[addr] < banTime)
                setBanned[addr] = banTime;
        }
        CloseSocketDisconnect();
        return true;
    } else
        printf("Misbehaving: %s (%d -> %d)\n", addr.ToString().c_str(), nMisbehavior-howmuch, nMisbehavior);
    return false;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

#undef X
#define X(name) stats.name = name
void CNode::copyStats(CNodeStats &stats)
{
<<<<<<< HEAD
    stats.nodeid = this->GetId();
    X(nServices);
    X(addr);
    X(addrBind);
    {
        LOCK(cs_filter);
        X(fRelayTxes);
    }
    X(nLastSend);
    X(nLastRecv);
    X(nTimeConnected);
    X(nTimeOffset);
    stats.addrName = GetAddrName();
    X(nVersion);
    {
        LOCK(cs_SubVer);
        X(cleanSubVer);
    }
    X(fInbound);
    X(m_manual_connection);
    X(nStartingHeight);
    {
        LOCK(cs_vSend);
        X(mapSendBytesPerMsgCmd);
        X(nSendBytes);
    }
    {
        LOCK(cs_vRecv);
        X(mapRecvBytesPerMsgCmd);
        X(nRecvBytes);
    }
    X(fWhitelisted);

    // It is common for nodes with good ping times to suddenly become lagged,
    // due to a new block arriving or other large transfer.
    // Merely reporting pingtime might fool the caller into thinking the node was still responsive,
    // since pingtime does not update until the ping is complete, which might take a while.
    // So, if a ping is taking an unusually long time in flight,
    // the caller can immediately detect that this is happening.
    int64_t nPingUsecWait = 0;
    if ((0 != nPingNonceSent) && (0 != nPingUsecStart)) {
        nPingUsecWait = GetTimeMicros() - nPingUsecStart;
    }

    // Raw ping time is in microseconds, but show it to user as whole seconds (WiFicoin users should be well used to small numbers with many decimal places by now :)
    stats.dPingTime = (((double)nPingUsecTime) / 1e6);
    stats.dMinPing  = (((double)nMinPingUsecTime) / 1e6);
    stats.dPingWait = (((double)nPingUsecWait) / 1e6);

    // Leave string empty if addrLocal invalid (not filled in yet)
    CService addrLocalUnlocked = GetAddrLocal();
    stats.addrLocal = addrLocalUnlocked.IsValid() ? addrLocalUnlocked.ToString() : "";
}
#undef X

bool CNode::ReceiveMsgBytes(const char *pch, unsigned int nBytes, bool& complete)
{
    complete = false;
    int64_t nTimeMicros = GetTimeMicros();
    LOCK(cs_vRecv);
    nLastRecv = nTimeMicros / 1000000;
    nRecvBytes += nBytes;
=======
    X(nServices);
    X(nLastSend);
    X(nLastRecv);
    X(nTimeConnected);
    X(addrName);
    X(nVersion);
    X(cleanSubVer);
    X(fInbound);
    X(nStartingHeight);
    X(nMisbehavior);
    X(nSendBytes);
    X(nRecvBytes);
    stats.fSyncNode = (this == pnodeSync);
}
#undef X

// requires LOCK(cs_vRecvMsg)
bool CNode::ReceiveMsgBytes(const char *pch, unsigned int nBytes)
{
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    while (nBytes > 0) {

        // get current incomplete message, or create a new one
        if (vRecvMsg.empty() ||
            vRecvMsg.back().complete())
<<<<<<< HEAD
            vRecvMsg.push_back(CNetMessage(Params().MessageStart(), SER_NETWORK, INIT_PROTO_VERSION));
=======
            vRecvMsg.push_back(CNetMessage(SER_NETWORK, nRecvVersion));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        CNetMessage& msg = vRecvMsg.back();

        // absorb network data
        int handled;
        if (!msg.in_data)
            handled = msg.readHeader(pch, nBytes);
        else
            handled = msg.readData(pch, nBytes);

        if (handled < 0)
<<<<<<< HEAD
            return false;

        if (msg.in_data && msg.hdr.nMessageSize > MAX_PROTOCOL_MESSAGE_LENGTH) {
            LogPrint(BCLog::NET, "Oversized message from peer=%i, disconnecting\n", GetId());
            return false;
        }

        pch += handled;
        nBytes -= handled;

        if (msg.complete()) {

            //store received bytes per message command
            //to prevent a memory DOS, only allow valid commands
            mapMsgCmdSize::iterator i = mapRecvBytesPerMsgCmd.find(msg.hdr.pchCommand);
            if (i == mapRecvBytesPerMsgCmd.end())
                i = mapRecvBytesPerMsgCmd.find(NET_MESSAGE_COMMAND_OTHER);
            assert(i != mapRecvBytesPerMsgCmd.end());
            i->second += msg.hdr.nMessageSize + CMessageHeader::HEADER_SIZE;

            msg.nTime = nTimeMicros;
            complete = true;
        }
=======
                return false;

        pch += handled;
        nBytes -= handled;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }

    return true;
}

<<<<<<< HEAD
void CNode::SetSendVersion(int nVersionIn)
{
    // Send version may only be changed in the version message, and
    // only one version message is allowed per session. We can therefore
    // treat this value as const and even atomic as long as it's only used
    // once a version message has been successfully processed. Any attempt to
    // set this twice is an error.
    if (nSendVersion != 0) {
        error("Send version already set for node: %i. Refusing to change from %i to %i", id, nSendVersion, nVersionIn);
    } else {
        nSendVersion = nVersionIn;
    }
}

int CNode::GetSendVersion() const
{
    // The send version should always be explicitly set to
    // INIT_PROTO_VERSION rather than using this value until SetSendVersion
    // has been called.
    if (nSendVersion == 0) {
        error("Requesting unset send version for node: %i. Using %i", id, INIT_PROTO_VERSION);
        return INIT_PROTO_VERSION;
    }
    return nSendVersion;
}


=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
int CNetMessage::readHeader(const char *pch, unsigned int nBytes)
{
    // copy data to temporary parsing buffer
    unsigned int nRemaining = 24 - nHdrPos;
    unsigned int nCopy = std::min(nRemaining, nBytes);

    memcpy(&hdrbuf[nHdrPos], pch, nCopy);
    nHdrPos += nCopy;

    // if header incomplete, exit
    if (nHdrPos < 24)
        return nCopy;

    // deserialize to CMessageHeader
    try {
        hdrbuf >> hdr;
    }
<<<<<<< HEAD
    catch (const std::exception&) {
=======
    catch (std::exception &e) {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        return -1;
    }

    // reject messages larger than MAX_SIZE
    if (hdr.nMessageSize > MAX_SIZE)
<<<<<<< HEAD
        return -1;

    // switch state to reading message data
    in_data = true;
=======
            return -1;

    // switch state to reading message data
    in_data = true;
    vRecv.resize(hdr.nMessageSize);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    return nCopy;
}

int CNetMessage::readData(const char *pch, unsigned int nBytes)
{
    unsigned int nRemaining = hdr.nMessageSize - nDataPos;
    unsigned int nCopy = std::min(nRemaining, nBytes);

<<<<<<< HEAD
    if (vRecv.size() < nDataPos + nCopy) {
        // Allocate up to 256 KiB ahead, but never more than the total message size.
        vRecv.resize(std::min(hdr.nMessageSize, nDataPos + nCopy + 256 * 1024));
    }

    hasher.Write((const unsigned char*)pch, nCopy);
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    memcpy(&vRecv[nDataPos], pch, nCopy);
    nDataPos += nCopy;

    return nCopy;
}

<<<<<<< HEAD
const uint256& CNetMessage::GetMessageHash() const
{
    assert(complete());
    if (data_hash.IsNull())
        hasher.Finalize(data_hash.begin());
    return data_hash;
}

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1








// requires LOCK(cs_vSend)
<<<<<<< HEAD
size_t CConnman::SocketSendData(CNode *pnode) const
{
    auto it = pnode->vSendMsg.begin();
    size_t nSentSize = 0;

    while (it != pnode->vSendMsg.end()) {
        const auto &data = *it;
        assert(data.size() > pnode->nSendOffset);
        int nBytes = 0;
        {
            LOCK(pnode->cs_hSocket);
            if (pnode->hSocket == INVALID_SOCKET)
                break;
            nBytes = send(pnode->hSocket, reinterpret_cast<const char*>(data.data()) + pnode->nSendOffset, data.size() - pnode->nSendOffset, MSG_NOSIGNAL | MSG_DONTWAIT);
        }
        if (nBytes > 0) {
            pnode->nLastSend = GetSystemTimeInSeconds();
            pnode->nSendBytes += nBytes;
            pnode->nSendOffset += nBytes;
            nSentSize += nBytes;
            if (pnode->nSendOffset == data.size()) {
                pnode->nSendOffset = 0;
                pnode->nSendSize -= data.size();
                pnode->fPauseSend = pnode->nSendSize > nSendBufferMaxSize;
=======
void SocketSendData(CNode *pnode)
{
    std::deque<CSerializeData>::iterator it = pnode->vSendMsg.begin();

    while (it != pnode->vSendMsg.end()) {
        const CSerializeData &data = *it;
        assert(data.size() > pnode->nSendOffset);
        int nBytes = send(pnode->hSocket, &data[pnode->nSendOffset], data.size() - pnode->nSendOffset, MSG_NOSIGNAL | MSG_DONTWAIT);
        if (nBytes > 0) {
            pnode->nLastSend = GetTime();
            pnode->nSendBytes += nBytes;
            pnode->nSendOffset += nBytes;
            if (pnode->nSendOffset == data.size()) {
                pnode->nSendOffset = 0;
                pnode->nSendSize -= data.size();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                it++;
            } else {
                // could not send full message; stop sending more
                break;
            }
        } else {
            if (nBytes < 0) {
                // error
                int nErr = WSAGetLastError();
                if (nErr != WSAEWOULDBLOCK && nErr != WSAEMSGSIZE && nErr != WSAEINTR && nErr != WSAEINPROGRESS)
                {
<<<<<<< HEAD
                    LogPrintf("socket send error %s\n", NetworkErrorString(nErr));
=======
                    printf("socket send error %d\n", nErr);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    pnode->CloseSocketDisconnect();
                }
            }
            // couldn't send anything at all
            break;
        }
    }

    if (it == pnode->vSendMsg.end()) {
        assert(pnode->nSendOffset == 0);
        assert(pnode->nSendSize == 0);
    }
    pnode->vSendMsg.erase(pnode->vSendMsg.begin(), it);
<<<<<<< HEAD
    return nSentSize;
}

struct NodeEvictionCandidate
{
    NodeId id;
    int64_t nTimeConnected;
    int64_t nMinPingUsecTime;
    int64_t nLastBlockTime;
    int64_t nLastTXTime;
    bool fRelevantServices;
    bool fRelayTxes;
    bool fBloomFilter;
    CAddress addr;
    uint64_t nKeyedNetGroup;
};

static bool ReverseCompareNodeMinPingTime(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b)
{
    return a.nMinPingUsecTime > b.nMinPingUsecTime;
}

static bool ReverseCompareNodeTimeConnected(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b)
{
    return a.nTimeConnected > b.nTimeConnected;
}

static bool CompareNetGroupKeyed(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b) {
    return a.nKeyedNetGroup < b.nKeyedNetGroup;
}

static bool CompareNodeBlockTime(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b)
{
    // There is a fall-through here because it is common for a node to have many peers which have not yet relayed a block.
    if (a.nLastBlockTime != b.nLastBlockTime) return a.nLastBlockTime < b.nLastBlockTime;
    if (a.fRelevantServices != b.fRelevantServices) return b.fRelevantServices;
    return a.nTimeConnected > b.nTimeConnected;
}

static bool CompareNodeTXTime(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b)
{
    // There is a fall-through here because it is common for a node to have more than a few peers that have not yet relayed txn.
    if (a.nLastTXTime != b.nLastTXTime) return a.nLastTXTime < b.nLastTXTime;
    if (a.fRelayTxes != b.fRelayTxes) return b.fRelayTxes;
    if (a.fBloomFilter != b.fBloomFilter) return a.fBloomFilter;
    return a.nTimeConnected > b.nTimeConnected;
}

/** Try to find a connection to evict when the node is full.
 *  Extreme care must be taken to avoid opening the node to attacker
 *   triggered network partitioning.
 *  The strategy used here is to protect a small number of peers
 *   for each of several distinct characteristics which are difficult
 *   to forge.  In order to partition a node the attacker must be
 *   simultaneously better at all of them than honest peers.
 */
bool CConnman::AttemptToEvictConnection()
{
    std::vector<NodeEvictionCandidate> vEvictionCandidates;
    {
        LOCK(cs_vNodes);

        for (CNode *node : vNodes) {
            if (node->fWhitelisted)
                continue;
            if (!node->fInbound)
                continue;
            if (node->fDisconnect)
                continue;
            NodeEvictionCandidate candidate = {node->GetId(), node->nTimeConnected, node->nMinPingUsecTime,
                                               node->nLastBlockTime, node->nLastTXTime,
                                               (node->nServices & nRelevantServices) == nRelevantServices,
                                               node->fRelayTxes, node->pfilter != nullptr, node->addr, node->nKeyedNetGroup};
            vEvictionCandidates.push_back(candidate);
        }
    }

    if (vEvictionCandidates.empty()) return false;

    // Protect connections with certain characteristics

    // Deterministically select 4 peers to protect by netgroup.
    // An attacker cannot predict which netgroups will be protected
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), CompareNetGroupKeyed);
    vEvictionCandidates.erase(vEvictionCandidates.end() - std::min(4, static_cast<int>(vEvictionCandidates.size())), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Protect the 8 nodes with the lowest minimum ping time.
    // An attacker cannot manipulate this metric without physically moving nodes closer to the target.
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), ReverseCompareNodeMinPingTime);
    vEvictionCandidates.erase(vEvictionCandidates.end() - std::min(8, static_cast<int>(vEvictionCandidates.size())), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Protect 4 nodes that most recently sent us transactions.
    // An attacker cannot manipulate this metric without performing useful work.
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), CompareNodeTXTime);
    vEvictionCandidates.erase(vEvictionCandidates.end() - std::min(4, static_cast<int>(vEvictionCandidates.size())), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Protect 4 nodes that most recently sent us blocks.
    // An attacker cannot manipulate this metric without performing useful work.
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), CompareNodeBlockTime);
    vEvictionCandidates.erase(vEvictionCandidates.end() - std::min(4, static_cast<int>(vEvictionCandidates.size())), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Protect the half of the remaining nodes which have been connected the longest.
    // This replicates the non-eviction implicit behavior, and precludes attacks that start later.
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), ReverseCompareNodeTimeConnected);
    vEvictionCandidates.erase(vEvictionCandidates.end() - static_cast<int>(vEvictionCandidates.size() / 2), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Identify the network group with the most connections and youngest member.
    // (vEvictionCandidates is already sorted by reverse connect time)
    uint64_t naMostConnections;
    unsigned int nMostConnections = 0;
    int64_t nMostConnectionsTime = 0;
    std::map<uint64_t, std::vector<NodeEvictionCandidate> > mapNetGroupNodes;
    for (const NodeEvictionCandidate &node : vEvictionCandidates) {
        mapNetGroupNodes[node.nKeyedNetGroup].push_back(node);
        int64_t grouptime = mapNetGroupNodes[node.nKeyedNetGroup][0].nTimeConnected;
        size_t groupsize = mapNetGroupNodes[node.nKeyedNetGroup].size();

        if (groupsize > nMostConnections || (groupsize == nMostConnections && grouptime > nMostConnectionsTime)) {
            nMostConnections = groupsize;
            nMostConnectionsTime = grouptime;
            naMostConnections = node.nKeyedNetGroup;
        }
    }

    // Reduce to the network group with the most connections
    vEvictionCandidates = std::move(mapNetGroupNodes[naMostConnections]);

    // Disconnect from the network group with the most connections
    NodeId evicted = vEvictionCandidates.front().id;
    LOCK(cs_vNodes);
    for(std::vector<CNode*>::const_iterator it(vNodes.begin()); it != vNodes.end(); ++it) {
        if ((*it)->GetId() == evicted) {
            (*it)->fDisconnect = true;
            return true;
        }
    }
    return false;
}

void CConnman::AcceptConnection(const ListenSocket& hListenSocket) {
    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
    SOCKET hSocket = accept(hListenSocket.socket, (struct sockaddr*)&sockaddr, &len);
    CAddress addr;
    int nInbound = 0;
    int nMaxInbound = nMaxConnections - (nMaxOutbound + nMaxFeeler);

    if (hSocket != INVALID_SOCKET) {
        if (!addr.SetSockAddr((const struct sockaddr*)&sockaddr)) {
            LogPrintf("Warning: Unknown socket family\n");
        }
    }

    bool whitelisted = hListenSocket.whitelisted || IsWhitelistedRange(addr);
    {
        LOCK(cs_vNodes);
        for (CNode* pnode : vNodes)
            if (pnode->fInbound)
                nInbound++;
    }

    if (hSocket == INVALID_SOCKET)
    {
        int nErr = WSAGetLastError();
        if (nErr != WSAEWOULDBLOCK)
            LogPrintf("socket error accept failed: %s\n", NetworkErrorString(nErr));
        return;
    }

    if (!fNetworkActive) {
        LogPrintf("connection from %s dropped: not accepting new connections\n", addr.ToString());
        CloseSocket(hSocket);
        return;
    }

    if (!IsSelectableSocket(hSocket))
    {
        LogPrintf("connection from %s dropped: non-selectable socket\n", addr.ToString());
        CloseSocket(hSocket);
        return;
    }

    // According to the internet TCP_NODELAY is not carried into accepted sockets
    // on all platforms.  Set it again here just to be sure.
    SetSocketNoDelay(hSocket);

    if (IsBanned(addr) && !whitelisted)
    {
        LogPrintf("connection from %s dropped (banned)\n", addr.ToString());
        CloseSocket(hSocket);
        return;
    }

    if (nInbound >= nMaxInbound)
    {
        if (!AttemptToEvictConnection()) {
            // No connection to evict, disconnect the new connection
            LogPrint(BCLog::NET, "failed to find an eviction candidate - connection dropped (full)\n");
            CloseSocket(hSocket);
            return;
        }
    }

    NodeId id = GetNewNodeId();
    uint64_t nonce = GetDeterministicRandomizer(RANDOMIZER_ID_LOCALHOSTNONCE).Write(id).Finalize();
    CAddress addr_bind = GetBindAddress(hSocket);

    CNode* pnode = new CNode(id, nLocalServices, GetBestHeight(), hSocket, addr, CalculateKeyedNetGroup(addr), nonce, addr_bind, "", true);
    pnode->AddRef();
    pnode->fWhitelisted = whitelisted;
    m_msgproc->InitializeNode(pnode);

    LogPrint(BCLog::NET, "connection from %s accepted\n", addr.ToString());

    {
        LOCK(cs_vNodes);
        vNodes.push_back(pnode);
    }
}

void CConnman::ThreadSocketHandler()
{
    unsigned int nPrevNodeCount = 0;
    while (!interruptNet)
=======
}

static list<CNode*> vNodesDisconnected;

void ThreadSocketHandler()
{
    unsigned int nPrevNodeCount = 0;
    loop
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        //
        // Disconnect nodes
        //
        {
            LOCK(cs_vNodes);
            // Disconnect unused nodes
<<<<<<< HEAD
            std::vector<CNode*> vNodesCopy = vNodes;
            for (CNode* pnode : vNodesCopy)
            {
                if (pnode->fDisconnect)
=======
            vector<CNode*> vNodesCopy = vNodes;
            BOOST_FOREACH(CNode* pnode, vNodesCopy)
            {
                if (pnode->fDisconnect ||
                    (pnode->GetRefCount() <= 0 && pnode->vRecvMsg.empty() && pnode->nSendSize == 0 && pnode->ssSend.empty()))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                {
                    // remove from vNodes
                    vNodes.erase(remove(vNodes.begin(), vNodes.end(), pnode), vNodes.end());

                    // release outbound grant (if any)
                    pnode->grantOutbound.Release();

                    // close socket and cleanup
                    pnode->CloseSocketDisconnect();
<<<<<<< HEAD

                    // hold in disconnected pool until all refs are released
                    pnode->Release();
                    vNodesDisconnected.push_back(pnode);
                }
            }
        }
        {
            // Delete disconnected nodes
            std::list<CNode*> vNodesDisconnectedCopy = vNodesDisconnected;
            for (CNode* pnode : vNodesDisconnectedCopy)
            {
                // wait until threads are done using it
                if (pnode->GetRefCount() <= 0) {
                    bool fDelete = false;
                    {
                        TRY_LOCK(pnode->cs_inventory, lockInv);
                        if (lockInv) {
                            TRY_LOCK(pnode->cs_vSend, lockSend);
                            if (lockSend) {
                                fDelete = true;
                            }
                        }
                    }
                    if (fDelete) {
                        vNodesDisconnected.remove(pnode);
                        DeleteNode(pnode);
=======
                    pnode->Cleanup();

                    // hold in disconnected pool until all refs are released
                    if (pnode->fNetworkNode || pnode->fInbound)
                        pnode->Release();
                    vNodesDisconnected.push_back(pnode);
                }
            }

            // Delete disconnected nodes
            list<CNode*> vNodesDisconnectedCopy = vNodesDisconnected;
            BOOST_FOREACH(CNode* pnode, vNodesDisconnectedCopy)
            {
                // wait until threads are done using it
                if (pnode->GetRefCount() <= 0)
                {
                    bool fDelete = false;
                    {
                        TRY_LOCK(pnode->cs_vSend, lockSend);
                        if (lockSend)
                        {
                            TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                            if (lockRecv)
                            {
                                TRY_LOCK(pnode->cs_inventory, lockInv);
                                if (lockInv)
                                    fDelete = true;
                            }
                        }
                    }
                    if (fDelete)
                    {
                        vNodesDisconnected.remove(pnode);
                        delete pnode;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    }
                }
            }
        }
<<<<<<< HEAD
        size_t vNodesSize;
        {
            LOCK(cs_vNodes);
            vNodesSize = vNodes.size();
        }
        if(vNodesSize != nPrevNodeCount) {
            nPrevNodeCount = vNodesSize;
            if(clientInterface)
                clientInterface->NotifyNumConnectionsChanged(nPrevNodeCount);
        }

=======
        if (vNodes.size() != nPrevNodeCount)
        {
            nPrevNodeCount = vNodes.size();
            uiInterface.NotifyNumConnectionsChanged(vNodes.size());
        }


>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        //
        // Find which sockets have data to receive
        //
        struct timeval timeout;
        timeout.tv_sec  = 0;
        timeout.tv_usec = 50000; // frequency to poll pnode->vSend

        fd_set fdsetRecv;
        fd_set fdsetSend;
        fd_set fdsetError;
        FD_ZERO(&fdsetRecv);
        FD_ZERO(&fdsetSend);
        FD_ZERO(&fdsetError);
        SOCKET hSocketMax = 0;
        bool have_fds = false;

<<<<<<< HEAD
        for (const ListenSocket& hListenSocket : vhListenSocket) {
            FD_SET(hListenSocket.socket, &fdsetRecv);
            hSocketMax = std::max(hSocketMax, hListenSocket.socket);
            have_fds = true;
        }

        {
            LOCK(cs_vNodes);
            for (CNode* pnode : vNodes)
            {
=======
        BOOST_FOREACH(SOCKET hListenSocket, vhListenSocket) {
            FD_SET(hListenSocket, &fdsetRecv);
            hSocketMax = max(hSocketMax, hListenSocket);
            have_fds = true;
        }
        {
            LOCK(cs_vNodes);
            BOOST_FOREACH(CNode* pnode, vNodes)
            {
                if (pnode->hSocket == INVALID_SOCKET)
                    continue;
                FD_SET(pnode->hSocket, &fdsetError);
                hSocketMax = max(hSocketMax, pnode->hSocket);
                have_fds = true;

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                // Implement the following logic:
                // * If there is data to send, select() for sending data. As this only
                //   happens when optimistic write failed, we choose to first drain the
                //   write buffer in this case before receiving more. This avoids
                //   needlessly queueing received data, if the remote peer is not themselves
                //   receiving data. This means properly utilizing TCP flow control signalling.
<<<<<<< HEAD
                // * Otherwise, if there is space left in the receive buffer, select() for
                //   receiving data.
                // * Hand off all complete messages to the processor, to be handled without
                //   blocking here.

                bool select_recv = !pnode->fPauseRecv;
                bool select_send;
                {
                    LOCK(pnode->cs_vSend);
                    select_send = !pnode->vSendMsg.empty();
                }

                LOCK(pnode->cs_hSocket);
                if (pnode->hSocket == INVALID_SOCKET)
                    continue;

                FD_SET(pnode->hSocket, &fdsetError);
                hSocketMax = std::max(hSocketMax, pnode->hSocket);
                have_fds = true;

                if (select_send) {
                    FD_SET(pnode->hSocket, &fdsetSend);
                    continue;
                }
                if (select_recv) {
                    FD_SET(pnode->hSocket, &fdsetRecv);
=======
                // * Otherwise, if there is no (complete) message in the receive buffer,
                //   or there is space left in the buffer, select() for receiving data.
                // * (if neither of the above applies, there is certainly one message
                //   in the receiver buffer ready to be processed).
                // Together, that means that at least one of the following is always possible,
                // so we don't deadlock:
                // * We send some data.
                // * We wait for data to be received (and disconnect after timeout).
                // * We process a message in the buffer (message handler thread).
                {
                    TRY_LOCK(pnode->cs_vSend, lockSend);
                    if (lockSend && !pnode->vSendMsg.empty()) {
                        FD_SET(pnode->hSocket, &fdsetSend);
                        continue;
                    }
                }
                {
                    TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                    if (lockRecv && (
                        pnode->vRecvMsg.empty() || !pnode->vRecvMsg.front().complete() ||
                        pnode->GetTotalRecvSize() <= ReceiveFloodSize()))
                        FD_SET(pnode->hSocket, &fdsetRecv);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                }
            }
        }

        int nSelect = select(have_fds ? hSocketMax + 1 : 0,
                             &fdsetRecv, &fdsetSend, &fdsetError, &timeout);
<<<<<<< HEAD
        if (interruptNet)
            return;
=======
        boost::this_thread::interruption_point();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

        if (nSelect == SOCKET_ERROR)
        {
            if (have_fds)
            {
                int nErr = WSAGetLastError();
<<<<<<< HEAD
                LogPrintf("socket select error %s\n", NetworkErrorString(nErr));
=======
                printf("socket select error %d\n", nErr);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                for (unsigned int i = 0; i <= hSocketMax; i++)
                    FD_SET(i, &fdsetRecv);
            }
            FD_ZERO(&fdsetSend);
            FD_ZERO(&fdsetError);
<<<<<<< HEAD
            if (!interruptNet.sleep_for(std::chrono::milliseconds(timeout.tv_usec/1000)))
                return;
        }

        //
        // Accept new connections
        //
        for (const ListenSocket& hListenSocket : vhListenSocket)
        {
            if (hListenSocket.socket != INVALID_SOCKET && FD_ISSET(hListenSocket.socket, &fdsetRecv))
            {
                AcceptConnection(hListenSocket);
            }
        }

        //
        // Service each socket
        //
        std::vector<CNode*> vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            for (CNode* pnode : vNodesCopy)
                pnode->AddRef();
        }
        for (CNode* pnode : vNodesCopy)
        {
            if (interruptNet)
                return;
=======
            MilliSleep(timeout.tv_usec/1000);
        }


        //
        // Accept new connections
        //
        BOOST_FOREACH(SOCKET hListenSocket, vhListenSocket)
        if (hListenSocket != INVALID_SOCKET && FD_ISSET(hListenSocket, &fdsetRecv))
        {
#ifdef USE_IPV6
            struct sockaddr_storage sockaddr;
#else
            struct sockaddr sockaddr;
#endif
            socklen_t len = sizeof(sockaddr);
            SOCKET hSocket = accept(hListenSocket, (struct sockaddr*)&sockaddr, &len);
            CAddress addr;
            int nInbound = 0;

            if (hSocket != INVALID_SOCKET)
                if (!addr.SetSockAddr((const struct sockaddr*)&sockaddr))
                    printf("Warning: Unknown socket family\n");

            {
                LOCK(cs_vNodes);
                BOOST_FOREACH(CNode* pnode, vNodes)
                    if (pnode->fInbound)
                        nInbound++;
            }

            if (hSocket == INVALID_SOCKET)
            {
                int nErr = WSAGetLastError();
                if (nErr != WSAEWOULDBLOCK)
                    printf("socket error accept failed: %d\n", nErr);
            }
            else if (nInbound >= nMaxConnections - MAX_OUTBOUND_CONNECTIONS)
            {
                {
                    LOCK(cs_setservAddNodeAddresses);
                    if (!setservAddNodeAddresses.count(addr))
                        closesocket(hSocket);
                }
            }
            else if (CNode::IsBanned(addr))
            {
                printf("connection from %s dropped (banned)\n", addr.ToString().c_str());
                closesocket(hSocket);
            }
            else
            {
                printf("accepted connection %s\n", addr.ToString().c_str());
                CNode* pnode = new CNode(hSocket, addr, "", true);
                pnode->AddRef();
                {
                    LOCK(cs_vNodes);
                    vNodes.push_back(pnode);
                }
            }
        }


        //
        // Service each socket
        //
        vector<CNode*> vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            BOOST_FOREACH(CNode* pnode, vNodesCopy)
                pnode->AddRef();
        }
        BOOST_FOREACH(CNode* pnode, vNodesCopy)
        {
            boost::this_thread::interruption_point();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

            //
            // Receive
            //
<<<<<<< HEAD
            bool recvSet = false;
            bool sendSet = false;
            bool errorSet = false;
            {
                LOCK(pnode->cs_hSocket);
                if (pnode->hSocket == INVALID_SOCKET)
                    continue;
                recvSet = FD_ISSET(pnode->hSocket, &fdsetRecv);
                sendSet = FD_ISSET(pnode->hSocket, &fdsetSend);
                errorSet = FD_ISSET(pnode->hSocket, &fdsetError);
            }
            if (recvSet || errorSet)
            {
                // typical socket buffer is 8K-64K
                char pchBuf[0x10000];
                int nBytes = 0;
                {
                    LOCK(pnode->cs_hSocket);
                    if (pnode->hSocket == INVALID_SOCKET)
                        continue;
                    nBytes = recv(pnode->hSocket, pchBuf, sizeof(pchBuf), MSG_DONTWAIT);
                }
                if (nBytes > 0)
                {
                    bool notify = false;
                    if (!pnode->ReceiveMsgBytes(pchBuf, nBytes, notify))
                        pnode->CloseSocketDisconnect();
                    RecordBytesRecv(nBytes);
                    if (notify) {
                        size_t nSizeAdded = 0;
                        auto it(pnode->vRecvMsg.begin());
                        for (; it != pnode->vRecvMsg.end(); ++it) {
                            if (!it->complete())
                                break;
                            nSizeAdded += it->vRecv.size() + CMessageHeader::HEADER_SIZE;
                        }
                        {
                            LOCK(pnode->cs_vProcessMsg);
                            pnode->vProcessMsg.splice(pnode->vProcessMsg.end(), pnode->vRecvMsg, pnode->vRecvMsg.begin(), it);
                            pnode->nProcessQueueSize += nSizeAdded;
                            pnode->fPauseRecv = pnode->nProcessQueueSize > nReceiveFloodSize;
                        }
                        WakeMessageHandler();
                    }
                }
                else if (nBytes == 0)
                {
                    // socket closed gracefully
                    if (!pnode->fDisconnect) {
                        LogPrint(BCLog::NET, "socket closed\n");
                    }
                    pnode->CloseSocketDisconnect();
                }
                else if (nBytes < 0)
                {
                    // error
                    int nErr = WSAGetLastError();
                    if (nErr != WSAEWOULDBLOCK && nErr != WSAEMSGSIZE && nErr != WSAEINTR && nErr != WSAEINPROGRESS)
                    {
                        if (!pnode->fDisconnect)
                            LogPrintf("socket recv error %s\n", NetworkErrorString(nErr));
                        pnode->CloseSocketDisconnect();
=======
            if (pnode->hSocket == INVALID_SOCKET)
                continue;
            if (FD_ISSET(pnode->hSocket, &fdsetRecv) || FD_ISSET(pnode->hSocket, &fdsetError))
            {
                TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                if (lockRecv)
                {
                    {
                        // typical socket buffer is 8K-64K
                        char pchBuf[0x10000];
                        int nBytes = recv(pnode->hSocket, pchBuf, sizeof(pchBuf), MSG_DONTWAIT);
                        if (nBytes > 0)
                        {
                            if (!pnode->ReceiveMsgBytes(pchBuf, nBytes))
                                pnode->CloseSocketDisconnect();
                            pnode->nLastRecv = GetTime();
                            pnode->nRecvBytes += nBytes;
                        }
                        else if (nBytes == 0)
                        {
                            // socket closed gracefully
                            if (!pnode->fDisconnect)
                                printf("socket closed\n");
                            pnode->CloseSocketDisconnect();
                        }
                        else if (nBytes < 0)
                        {
                            // error
                            int nErr = WSAGetLastError();
                            if (nErr != WSAEWOULDBLOCK && nErr != WSAEMSGSIZE && nErr != WSAEINTR && nErr != WSAEINPROGRESS)
                            {
                                if (!pnode->fDisconnect)
                                    printf("socket recv error %d\n", nErr);
                                pnode->CloseSocketDisconnect();
                            }
                        }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    }
                }
            }

            //
            // Send
            //
<<<<<<< HEAD
            if (sendSet)
            {
                LOCK(pnode->cs_vSend);
                size_t nBytes = SocketSendData(pnode);
                if (nBytes) {
                    RecordBytesSent(nBytes);
                }
=======
            if (pnode->hSocket == INVALID_SOCKET)
                continue;
            if (FD_ISSET(pnode->hSocket, &fdsetSend))
            {
                TRY_LOCK(pnode->cs_vSend, lockSend);
                if (lockSend)
                    SocketSendData(pnode);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            }

            //
            // Inactivity checking
            //
<<<<<<< HEAD
            int64_t nTime = GetSystemTimeInSeconds();
            if (nTime - pnode->nTimeConnected > 60)
            {
                if (pnode->nLastRecv == 0 || pnode->nLastSend == 0)
                {
                    LogPrint(BCLog::NET, "socket no message in first 60 seconds, %d %d from %d\n", pnode->nLastRecv != 0, pnode->nLastSend != 0, pnode->GetId());
                    pnode->fDisconnect = true;
                }
                else if (nTime - pnode->nLastSend > TIMEOUT_INTERVAL)
                {
                    LogPrintf("socket sending timeout: %is\n", nTime - pnode->nLastSend);
                    pnode->fDisconnect = true;
                }
                else if (nTime - pnode->nLastRecv > (pnode->nVersion > BIP0031_VERSION ? TIMEOUT_INTERVAL : 90*60))
                {
                    LogPrintf("socket receive timeout: %is\n", nTime - pnode->nLastRecv);
                    pnode->fDisconnect = true;
                }
                else if (pnode->nPingNonceSent && pnode->nPingUsecStart + TIMEOUT_INTERVAL * 1000000 < GetTimeMicros())
                {
                    LogPrintf("ping timeout: %fs\n", 0.000001 * (GetTimeMicros() - pnode->nPingUsecStart));
                    pnode->fDisconnect = true;
                }
                else if (!pnode->fSuccessfullyConnected)
                {
                    LogPrintf("version handshake timeout from %d\n", pnode->GetId());
=======
            if (pnode->vSendMsg.empty())
                pnode->nLastSendEmpty = GetTime();
            if (GetTime() - pnode->nTimeConnected > 60)
            {
                if (pnode->nLastRecv == 0 || pnode->nLastSend == 0)
                {
                    printf("socket no message in first 60 seconds, %d %d\n", pnode->nLastRecv != 0, pnode->nLastSend != 0);
                    pnode->fDisconnect = true;
                }
                else if (GetTime() - pnode->nLastSend > 90*60 && GetTime() - pnode->nLastSendEmpty > 90*60)
                {
                    printf("socket not sending\n");
                    pnode->fDisconnect = true;
                }
                else if (GetTime() - pnode->nLastRecv > 90*60)
                {
                    printf("socket inactivity timeout\n");
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    pnode->fDisconnect = true;
                }
            }
        }
        {
            LOCK(cs_vNodes);
<<<<<<< HEAD
            for (CNode* pnode : vNodesCopy)
                pnode->Release();
        }
    }
}

void CConnman::WakeMessageHandler()
{
    {
        std::lock_guard<std::mutex> lock(mutexMsgProc);
        fMsgProcWake = true;
    }
    condMsgProc.notify_one();
=======
            BOOST_FOREACH(CNode* pnode, vNodesCopy)
                pnode->Release();
        }

        MilliSleep(10);
    }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}






<<<<<<< HEAD
=======



>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#ifdef USE_UPNP
void ThreadMapPort()
{
    std::string port = strprintf("%u", GetListenPort());
    const char * multicastif = 0;
    const char * minissdpdpath = 0;
    struct UPNPDev * devlist = 0;
    char lanaddr[64];

#ifndef UPNPDISCOVER_SUCCESS
    /* miniupnpc 1.5 */
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0);
<<<<<<< HEAD
#elif MINIUPNPC_API_VERSION < 14
    /* miniupnpc 1.6 */
    int error = 0;
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0, 0, &error);
#else
    /* miniupnpc 1.9.20150730 */
    int error = 0;
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0, 0, 2, &error);
=======
#else
    /* miniupnpc 1.6 */
    int error = 0;
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0, 0, &error);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#endif

    struct UPNPUrls urls;
    struct IGDdatas data;
    int r;

    r = UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));
    if (r == 1)
    {
        if (fDiscover) {
            char externalIPAddress[40];
            r = UPNP_GetExternalIPAddress(urls.controlURL, data.first.servicetype, externalIPAddress);
            if(r != UPNPCOMMAND_SUCCESS)
<<<<<<< HEAD
                LogPrintf("UPnP: GetExternalIPAddress() returned %d\n", r);
=======
                printf("UPnP: GetExternalIPAddress() returned %d\n", r);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            else
            {
                if(externalIPAddress[0])
                {
<<<<<<< HEAD
                    CNetAddr resolved;
                    if(LookupHost(externalIPAddress, resolved, false)) {
                        LogPrintf("UPnP: ExternalIPAddress = %s\n", resolved.ToString().c_str());
                        AddLocal(resolved, LOCAL_UPNP);
                    }
                }
                else
                    LogPrintf("UPnP: GetExternalIPAddress failed.\n");
            }
        }

        std::string strDesc = "WiFicoin " + FormatFullVersion();

        try {
            while (true) {
=======
                    printf("UPnP: ExternalIPAddress = %s\n", externalIPAddress);
                    AddLocal(CNetAddr(externalIPAddress), LOCAL_UPNP);
                }
                else
                    printf("UPnP: GetExternalIPAddress failed.\n");
            }
        }

        string strDesc = "rainbowcoin " + FormatFullVersion();

        try {
            loop {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#ifndef UPNPDISCOVER_SUCCESS
                /* miniupnpc 1.5 */
                r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                                    port.c_str(), port.c_str(), lanaddr, strDesc.c_str(), "TCP", 0);
#else
                /* miniupnpc 1.6 */
                r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                                    port.c_str(), port.c_str(), lanaddr, strDesc.c_str(), "TCP", 0, "0");
#endif

                if(r!=UPNPCOMMAND_SUCCESS)
<<<<<<< HEAD
                    LogPrintf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
                        port, port, lanaddr, r, strupnperror(r));
                else
                    LogPrintf("UPnP Port Mapping successful.\n");
=======
                    printf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
                        port.c_str(), port.c_str(), lanaddr, r, strupnperror(r));
                else
                    printf("UPnP Port Mapping successful.\n");;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

                MilliSleep(20*60*1000); // Refresh every 20 minutes
            }
        }
<<<<<<< HEAD
        catch (const boost::thread_interrupted&)
        {
            r = UPNP_DeletePortMapping(urls.controlURL, data.first.servicetype, port.c_str(), "TCP", 0);
            LogPrintf("UPNP_DeletePortMapping() returned: %d\n", r);
=======
        catch (boost::thread_interrupted)
        {
            r = UPNP_DeletePortMapping(urls.controlURL, data.first.servicetype, port.c_str(), "TCP", 0);
            printf("UPNP_DeletePortMapping() returned : %d\n", r);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            freeUPNPDevlist(devlist); devlist = 0;
            FreeUPNPUrls(&urls);
            throw;
        }
    } else {
<<<<<<< HEAD
        LogPrintf("No valid UPnP IGDs found\n");
=======
        printf("No valid UPnP IGDs found\n");
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        freeUPNPDevlist(devlist); devlist = 0;
        if (r != 0)
            FreeUPNPUrls(&urls);
    }
}

void MapPort(bool fUseUPnP)
{
<<<<<<< HEAD
    static boost::thread* upnp_thread = nullptr;
=======
    static boost::thread* upnp_thread = NULL;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    if (fUseUPnP)
    {
        if (upnp_thread) {
            upnp_thread->interrupt();
            upnp_thread->join();
            delete upnp_thread;
        }
<<<<<<< HEAD
        upnp_thread = new boost::thread(boost::bind(&TraceThread<void (*)()>, "upnp", &ThreadMapPort));
=======
        upnp_thread = new boost::thread(boost::bind(&TraceThread<boost::function<void()> >, "upnp", &ThreadMapPort));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
    else if (upnp_thread) {
        upnp_thread->interrupt();
        upnp_thread->join();
        delete upnp_thread;
<<<<<<< HEAD
        upnp_thread = nullptr;
=======
        upnp_thread = NULL;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
}

#else
void MapPort(bool)
{
    // Intentionally left blank.
}
#endif






<<<<<<< HEAD
static std::string GetDNSHost(const CDNSSeedData& data, ServiceFlags* requiredServiceBits)
{
    //use default host for non-filter-capable seeds or if we use the default service bits (NODE_NETWORK)
    if (!data.supportsServiceBitsFiltering || *requiredServiceBits == NODE_NETWORK) {
        *requiredServiceBits = NODE_NETWORK;
        return data.host;
    }

    // See chainparams.cpp, most dnsseeds only support one or two possible servicebits hostnames
    return strprintf("x%x.%s", *requiredServiceBits, data.host);
}


void CConnman::ThreadDNSAddressSeed()
{
    // goal: only query DNS seeds if address need is acute
    // Avoiding DNS seeds when we don't need them improves user privacy by
    //  creating fewer identifying DNS requests, reduces trust by giving seeds
    //  less influence on the network topology, and reduces traffic to the seeds.
    if ((addrman.size() > 0) &&
        (!gArgs.GetBoolArg("-forcednsseed", DEFAULT_FORCEDNSSEED))) {
        if (!interruptNet.sleep_for(std::chrono::seconds(11)))
            return;

        LOCK(cs_vNodes);
        int nRelevant = 0;
        for (auto pnode : vNodes) {
            nRelevant += pnode->fSuccessfullyConnected && ((pnode->nServices & nRelevantServices) == nRelevantServices);
        }
        if (nRelevant >= 2) {
            LogPrintf("P2P peers available. Skipped DNS seeding.\n");
            return;
        }
    }

    const std::vector<CDNSSeedData> &vSeeds = Params().DNSSeeds();
    int found = 0;

    LogPrintf("Loading addresses from DNS seeds (could take a while)\n");

    for (const CDNSSeedData &seed : vSeeds) {
        if (interruptNet) {
            return;
        }
        if (HaveNameProxy()) {
            AddOneShot(seed.host);
        } else {
            std::vector<CNetAddr> vIPs;
            std::vector<CAddress> vAdd;
            ServiceFlags requiredServiceBits = nRelevantServices;
            std::string host = GetDNSHost(seed, &requiredServiceBits);
            CNetAddr resolveSource;
            if (!resolveSource.SetInternal(host)) {
                continue;
            }
            if (LookupHost(host.c_str(), vIPs, 0, true))
            {
                for (const CNetAddr& ip : vIPs)
                {
                    int nOneDay = 24*3600;
                    CAddress addr = CAddress(CService(ip, Params().GetDefaultPort()), requiredServiceBits);
=======



// DNS seeds
// Each pair gives a source name and a seed name.
// The first name is used as information source for addrman.
// The second name should resolve to a list of seed addresses.
static const char *strMainNetDNSSeed[][2] = {
    {NULL, NULL},
    {NULL, NULL}
};

static const char *strTestNetDNSSeed[][2] = {
    {NULL, NULL}
};

void ThreadDNSAddressSeed()
{
    static const char *(*strDNSSeed)[2] = fTestNet ? strTestNetDNSSeed : strMainNetDNSSeed;

    int found = 0;

    printf("Loading addresses from DNS seeds (could take a while)\n");

    for (unsigned int seed_idx = 0; strDNSSeed[seed_idx][0] != NULL; seed_idx++) {
        if (HaveNameProxy()) {
            AddOneShot(strDNSSeed[seed_idx][1]);
        } else {
            vector<CNetAddr> vaddr;
            vector<CAddress> vAdd;
            if (LookupHost(strDNSSeed[seed_idx][1], vaddr))
            {
                BOOST_FOREACH(CNetAddr& ip, vaddr)
                {
                    int nOneDay = 24*3600;
                    CAddress addr = CAddress(CService(ip, GetDefaultPort()));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    addr.nTime = GetTime() - 3*nOneDay - GetRand(4*nOneDay); // use a random age between 3 and 7 days old
                    vAdd.push_back(addr);
                    found++;
                }
<<<<<<< HEAD
                addrman.Add(vAdd, resolveSource);
            }
        }
    }

    LogPrintf("%d addresses found from DNS seeds\n", found);
}












void CConnman::DumpAddresses()
{
    int64_t nStart = GetTimeMillis();
=======
            }
            addrman.Add(vAdd, CNetAddr(strDNSSeed[seed_idx][0], true));
        }
    }

    printf("%d addresses found from DNS seeds\n", found);
}












unsigned int pnSeed[] =
{
    0xe473042e, 0xb177f2ad, 0xd63f3fb2, 0xf864f736, 0x44a23ac7, 0xcf6d9650, 0xd648042e, 0x0536f447,
    0x3c654ed0, 0x3e16a5bc, 0xa38e09b0, 0xdfae795b, 0xabfeca5b, 0x94ad7840, 0xf3b9f1c7, 0xbe70e0ad,
    0x3bbd09b0, 0x8d0c7dd5, 0x3b2a7332, 0x1a06175e, 0x581f175e, 0xca0d2dcc, 0x0fdbc658, 0xcf591ec7,
    0x295a12b2, 0xb4707bce, 0x68bb09b0, 0x4e735747, 0x89709553, 0x05a7814e, 0x5b8ec658, 0x402c5512,
    0xe80d0905, 0x17681a5e, 0xc02aa748, 0x9f811741, 0x5f321cb0, 0x23e1ee47, 0xaf7f170c, 0xaa240ab0,
    0xedea6257, 0x76106bc1, 0x2cf310cc, 0x08612acb, 0x9c682e4e, 0x8e963c6c, 0x443c795b, 0x22e246b8,
    0xfa1f2dcc, 0x90118140, 0x3821042e, 0x33c3fd2e, 0x10046d5b, 0x40d14b3e, 0x7fb8f8ce, 0x67696550,
    0xeeecbe58, 0x4f341745, 0x46b8fbd5, 0xc8463932, 0x6b73e862, 0x4c715932, 0x4a6785d5, 0xce3a64c2,
    0xde9604c7, 0x9b06884f, 0x18002a45, 0xea9bc345, 0xc4f1c658, 0xe475c1c7, 0xdd3e795b, 0x9722175e,
    0x34562f4e, 0x66c46e4e, 0x40bb1243, 0x7d9171d0, 0x17b8dbd5, 0x63cbfd2e, 0x1a08b8d8, 0x6175a73b,
    0x228d2660, 0x8627c658, 0x9c566644, 0x38cca5bc, 0x3089de5b, 0x92e25f5d, 0xa393f73f, 0xcc92dc3e,
    0x27487446, 0x62cbfd2e, 0x9d983b45, 0xf72a09b0, 0xf75f042e, 0x6434bb6a, 0xb29e77d8, 0x19be4fd9,
    0x76443243, 0x9dd72645, 0x694cef43, 0x89c2efd5, 0x5f1c5058, 0x46c6e45b, 0xe1391b40, 0x77ccefd5,
    0x472e5a6d, 0x85709553, 0xdd4f5d4c, 0x64ef5a46, 0x7f0ae502, 0xcf08d850, 0x3460042e, 0xeafa2d42,
    0x793c9044, 0x9d094746, 0x1ab9b153, 0xbfe9a5bc, 0x34771fb0, 0xb7722e32, 0x1168964b, 0x19b06ab8,
    0x19243b25, 0x13188045, 0xb4070905, 0x728ebb5d, 0x44f24ac8, 0xa317fead, 0x642f6a57, 0x3d951f32,
    0x3d312e4e, 0xfac4d048, 0xefc4dd50, 0x52b9f1c7, 0xc14d3cc3, 0x0219ea44, 0x3b79d058, 0xfa217242,
    0x39c80647, 0xfb697252, 0x1d495a42, 0x0aa81f4e, 0x58249ab8, 0xe6a8e6c3, 0x2bc4dad8, 0x85963c6c,
    0xa4ce09b0, 0x2005f536, 0x5cc2703e, 0x1992de43, 0x74e86b4c, 0xe7085653, 0xf5e15a51, 0xb4872b60,
    0x29e2b162, 0xa07ea053, 0x8229fd18, 0x4562ec4d, 0x8dec814e, 0x36cfa4cf, 0x96461032, 0x3c8770de,
    0xd10a1f5f, 0x95934641, 0x97cd65d0, 0x2e35324a, 0x2566ba1f, 0x1ca1a9d1, 0xb808b8d5, 0xf9a24a5d,
    0xafc8d431, 0xe4b8d9b2, 0x0f5321b2, 0x330bc658, 0x74b347ce, 0x972babd5, 0x044f7d4f, 0x06562f4e,
    0x8b8d3c6c, 0x3507c658, 0xe4174e4d, 0xf1c009b0, 0x52249ab8, 0x27211772, 0xf6a9ba59, 0x7a391b40,
    0x855dc6c0, 0x291f20b2, 0xe29bc345, 0x90963c6c, 0x0af70732, 0x4242a91f, 0x4c531d48, 0xa32df948,
    0x627e3044, 0x65be1f54, 0x1a0cbf83, 0x6a443532, 0x8d5f1955, 0xbafa8132, 0x3534bdd5, 0xca019dd9,
    0x8a0d9332, 0x5584e7d8, 0x7cd1f25e, 0xeabe3fb2, 0x2945d0d1, 0x46415718, 0x70d6042e, 0x99eb76d0,
    0x9ece09b0, 0xb3777418, 0x5e5e91d9, 0x237a3ab0, 0xf512b62e, 0x45dec347, 0x59b7f862, 0x4c443b25,
    0x3cc6484b, 0x9a8ec6d1, 0x021eea44, 0xc9483944, 0xfd567e32, 0xfd204bb2, 0xc5330bcc, 0x5202894e,
    0xf9e309b0, 0x4cc17557, 0xdb9064ae, 0xe19e77d8, 0x25857f60, 0xeb4a15ad, 0x1f47f554, 0xea4472d9,
    0xd20de593, 0xf5733b25, 0x11892b54, 0x5729d35f, 0xe6188cd1, 0x488b132e, 0x541c534a, 0xa8e854ae,
    0xa255a66c, 0x33688763, 0xc6629ac6, 0xc20a6265, 0xcd92a059, 0x72029d3b, 0x4c298f5e, 0x51452e4e,
    0xbb065058, 0x15fd2dcc, 0xf40c135e, 0x615a0bad, 0x0c6a6805, 0x4971a7ad, 0x17f2a5d5, 0xf8babf47,
    0xb61f50ad, 0x4e1451b1, 0xf72d9252, 0x5c2abe58, 0xbd987c61, 0x084ae5cf, 0x20781fb0, 0x38b0f160,
    0x18aac705, 0x14f86dc1, 0x5556f481, 0x0a36c144, 0xeb446e4c, 0x2c1c0d6c, 0xbd0ff860, 0x869f92db,
    0x36c94f4c, 0x05502444, 0x148fe55b, 0xd5301e59, 0xd57a8f45, 0x110dc04a, 0x8670fc36, 0xee733b25,
    0xca56f481, 0x2a5c3bae, 0x844b0905, 0x1e51fe53, 0x0241c244, 0x59c0614e, 0x94e70a55, 0x7312fead,
    0xb735be44, 0xa55d0905, 0x2f63962e, 0x14a4e15b, 0x63f8f05c, 0x62d0d262, 0x3cab41ad, 0x87f1b1cb,
    0x018da6b8, 0xb3967dd5, 0xcb56f481, 0x685ad718, 0x3b4aeeca, 0x8d106bc1, 0x51180905, 0x72660f48,
    0x1521a243, 0x5b56f481, 0x6390e560, 0xdd61464e, 0x58353b25, 0x553fc062, 0x27c45d59, 0xacc62e4e,
    0x0d5a1cd9, 0x7f65f442, 0xbdeef660, 0xf1bd1855, 0xf8473cae, 0x13b120b2, 0x442440d0, 0x53fd4352,
    0xa305fc57, 0x458be84d, 0x639ce1c3, 0xebaaee47, 0x95e2c247, 0xf056f481, 0x6256f481, 0x1d87c65e,
    0x0a453418, 0x5beb175e, 0xd64f1618, 0xc360795b, 0x2fbf5753, 0xa8c58e53, 0x651cec52, 0x9d37b043,
    0x124a9758, 0x5242e4a9, 0x89913c6c, 0x880efe2e, 0x2f2f2f0c, 0x72b26751, 0x2896e46d, 0x80f4166c,
    0x320d59ad, 0xc50151d0, 0x11a8aa43, 0xccf56057, 0x5fbad118, 0x4719b151, 0x2b5f4bc0, 0x4d7a4a50,
    0xad06e047, 0x62ef5a46, 0x5aebde58, 0xdf7aa66c, 0x851acb50, 0x66b9a559, 0x3e9bb153, 0xcc512f2e,
    0xc073b08e, 0xd519be58, 0xe981ea4d, 0x12fd50cb, 0x378739ad, 0x06683cae, 0xa22310b2, 0xc185c705,
    0x8741b545, 0xa26c8318, 0x22d5bc43, 0x39201ec0, 0x68581e3e, 0xdc9bcf62, 0xd508cc82, 0xb149675b,
    0x4c9609b0, 0x84feb84c, 0x08291e2e, 0xfd2253b2, 0x1fd269c1, 0xc9483932, 0x4d641fb0, 0x7d37c918,
    0xa9de20ad, 0x77e2d655, 0x6d421b59, 0xd7668f80, 0xced09b62, 0xa9e5a5bc, 0xa4074e18, 0x60fc5ecc,
    0x01300148, 0x68062444, 0xb4224847, 0xed3aa443, 0xb772fb43, 0x9f56f481, 0x220dfd18, 0x8e1c3d6c,
    0xc44f09b0, 0x7df2bb73, 0xe22fb844, 0xea534242, 0xb6a755d4, 0xa036654b, 0x138ece5b, 0xda65d3c3,
    0x955871bc, 0x792124b0, 0xfc82594c, 0x851d494b, 0x2c7aee47, 0x26af46b8, 0x1416252e, 0xa8abb944,
    0x36c49d25, 0x674f645d, 0x363646b8, 0x9e1a2942, 0x66d0c154, 0xc6c2a545, 0x3570f2ad, 0xe7d547c7,
    0x7d104932, 0x18cb9c18, 0x1dcfa4cf, 0xd156f481, 0x2a02b91f, 0x3eeb3fa8, 0xcac4175e, 0x34146d42,
    0x994c4d46, 0x5666f440, 0x85d6713e, 0x5ecb296c, 0x0ea0ae46, 0x87e69f42, 0xc58409b0, 0x1f3436ae,
    0x21dc6a57, 0x4ad1cd42, 0xfb8c1a4c, 0x52d3dab2, 0x3769894b, 0xb52f1c62, 0x3677916d, 0x82b3fe57,
    0x493d4ac6, 0x9f963c6c, 0x5d91ff60, 0x458e0dad, 0xa49d0947, 0x491a3e18, 0x4aadcd5b, 0x0e46494b,
    0x1d1610ad, 0x1a10af5d, 0x4956f481, 0x207a3eae, 0x77e73244, 0xfa3b8742, 0x3261fc36, 0xfcebf536,
    0x1662e836, 0xf655f636, 0xa2dbd0ad, 0x23036693, 0x30448432, 0xa2b03463, 0x30730344, 0x8e4a6882,
    0x0c50a1cb, 0xc8d8c06b, 0xc9cd6191, 0xf443db50, 0xa9553c50, 0x23145847, 0xc35da66c, 0x29c12a60,
    0x55c2b447, 0x7434f75c, 0x61660640, 0xde2a7018, 0xc639494c, 0x1c306fce, 0x19b89244, 0xd29a6462,
    0x462cd1b2, 0x29902f44, 0x2817fa53, 0x21a30905, 0x7777ae46, 0x288443a1, 0x7bee5148, 0xc2a8b043,
    0xf5c3d35f, 0x2311ef84, 0x57de08a4, 0x6b221bb2, 0xf2625846, 0x4b9e09b0, 0xa24f880e, 0x22b11447,
    0xb3a0c744, 0x919e77d8, 0xec8b64ae, 0xff5c8d45, 0x7b15b484, 0x32679a5f, 0xba80b62e, 0x05c25c61,
    0x60014746, 0x5e8fb04c, 0xe67c0905, 0x4329c658, 0xac8fe555, 0xf875e647, 0x67406386, 0x35ceea18,
    0xbb79484b, 0xd7b9fa62, 0x238209b0, 0x208a1d32, 0x9630995e, 0x039c1318, 0x6e48006c, 0x60582344,
    0xadbb0150, 0x853fd462, 0x03772e4e, 0x652ce960, 0x49b630ad, 0x9993af43, 0x3735b34b, 0x548a07d9,
    0x55a44aad, 0xa23d1bcc, 0xfdbb2f4e, 0x530b24a0, 0x0a44b451, 0x6827c657, 0x1f66494b, 0x4e680a47,
    0x77e7b747, 0xa5eb3fa8, 0x6649764a, 0xd4e76c4b, 0x2c691fb0, 0xf1292e44, 0xc6d6c774, 0x85d23775,
    0x28275f4d, 0x259ae46d, 0x02424e81, 0x5f16be58, 0xe707c658, 0x49eae5c7, 0xd5d147ad, 0x9a7abdc3,
    0xe8ac7fc7, 0x84ec3aae, 0xc24942d0, 0x294aa318, 0x08ac3d18, 0x8894042e, 0xb24609b0, 0x9bcaab58,
    0xc400f712, 0xd5c512b8, 0x2c02cc62, 0x25080fd8, 0xed74a847, 0x18a5ec5e, 0x9850ec6d, 0xf8909758,
    0x7f56f481, 0x4496f23c, 0xae27784f, 0xcb7cd93e, 0x06e32860, 0x50b9a84f, 0x3660434a, 0x09161f5f,
    0x900486bc, 0x08055459, 0xe7ec1017, 0x7e39494c, 0x4f443b25, 0x14751a8a, 0x717d03d4, 0xbd0e24d8,
    0x054b6f56, 0x854c496c, 0xd92a454a, 0xc39bd054, 0x6093614b, 0x9dbad754, 0x5bf0604a, 0x99f22305
};

void DumpAddresses()
{
    int64 nStart = GetTimeMillis();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    CAddrDB adb;
    adb.Write(addrman);

<<<<<<< HEAD
    LogPrint(BCLog::NET, "Flushed %d addresses to peers.dat  %dms\n",
           addrman.size(), GetTimeMillis() - nStart);
}

void CConnman::DumpData()
{
    DumpAddresses();
    DumpBanlist();
}

void CConnman::ProcessOneShot()
{
    std::string strDest;
=======
    printf("Flushed %d addresses to peers.dat  %"PRI64d"ms\n",
           addrman.size(), GetTimeMillis() - nStart);
}

void static ProcessOneShot()
{
    string strDest;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        LOCK(cs_vOneShots);
        if (vOneShots.empty())
            return;
        strDest = vOneShots.front();
        vOneShots.pop_front();
    }
    CAddress addr;
    CSemaphoreGrant grant(*semOutbound, true);
    if (grant) {
<<<<<<< HEAD
        if (!OpenNetworkConnection(addr, false, &grant, strDest.c_str(), true))
=======
        if (!OpenNetworkConnection(addr, &grant, strDest.c_str(), true))
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            AddOneShot(strDest);
    }
}

<<<<<<< HEAD
bool CConnman::GetTryNewOutboundPeer()
{
    return m_try_another_outbound_peer;
}

void CConnman::SetTryNewOutboundPeer(bool flag)
{
    m_try_another_outbound_peer = flag;
    LogPrint(BCLog::NET, "net: setting try another outbound peer=%s\n", flag ? "true" : "false");
}

// Return the number of peers we have over our outbound connection limit
// Exclude peers that are marked for disconnect, or are going to be
// disconnected soon (eg one-shots and feelers)
// Also exclude peers that haven't finished initial connection handshake yet
// (so that we don't decide we're over our desired connection limit, and then
// evict some peer that has finished the handshake)
int CConnman::GetExtraOutboundCount()
{
    int nOutbound = 0;
    {
        LOCK(cs_vNodes);
        for (CNode* pnode : vNodes) {
            if (!pnode->fInbound && !pnode->m_manual_connection && !pnode->fFeeler && !pnode->fDisconnect && !pnode->fOneShot && pnode->fSuccessfullyConnected) {
                ++nOutbound;
            }
        }
    }
    return std::max(nOutbound - nMaxOutbound, 0);
}

void CConnman::ThreadOpenConnections()
{
    // Connect to specific addresses
    if (gArgs.IsArgSet("-connect"))
    {
        for (int64_t nLoop = 0;; nLoop++)
        {
            ProcessOneShot();
            for (const std::string& strAddr : gArgs.GetArgs("-connect"))
            {
                CAddress addr(CService(), NODE_NONE);
                OpenNetworkConnection(addr, false, nullptr, strAddr.c_str());
                for (int i = 0; i < 10 && i < nLoop; i++)
                {
                    if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
                        return;
                }
            }
            if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
                return;
=======
void ThreadOpenConnections()
{
    // Connect to specific addresses
    if (mapArgs.count("-connect") && mapMultiArgs["-connect"].size() > 0)
    {
        for (int64 nLoop = 0;; nLoop++)
        {
            ProcessOneShot();
            BOOST_FOREACH(string strAddr, mapMultiArgs["-connect"])
            {
                CAddress addr;
                OpenNetworkConnection(addr, NULL, strAddr.c_str());
                for (int i = 0; i < 10 && i < nLoop; i++)
                {
                    MilliSleep(500);
                }
            }
            MilliSleep(500);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
    }

    // Initiate network connections
<<<<<<< HEAD
    int64_t nStart = GetTime();

    // Minimum time before next feeler connection (in microseconds).
    int64_t nNextFeeler = PoissonNextSend(nStart*1000*1000, FEELER_INTERVAL);
    while (!interruptNet)
    {
        ProcessOneShot();

        if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
            return;

        CSemaphoreGrant grant(*semOutbound);
        if (interruptNet)
            return;

        // Add seed nodes if DNS seeds are all down (an infrastructure attack?).
        if (addrman.size() == 0 && (GetTime() - nStart > 60)) {
            static bool done = false;
            if (!done) {
                LogPrintf("Adding fixed seed nodes as DNS doesn't seem to be available.\n");
                CNetAddr local;
                local.SetInternal("fixedseeds");
                addrman.Add(convertSeed6(Params().FixedSeeds()), local);
                done = true;
            }
=======
    int64 nStart = GetTime();
    loop
    {
        ProcessOneShot();

        MilliSleep(500);

        CSemaphoreGrant grant(*semOutbound);
        boost::this_thread::interruption_point();

        // Add seed nodes if IRC isn't working
        if (addrman.size()==0 && (GetTime() - nStart > 60) && !fTestNet)
        {
            std::vector<CAddress> vAdd;
            for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
            {
                // It'll only connect to one or two seed nodes because once it connects,
                // it'll get a pile of addresses with newer timestamps.
                // Seed nodes are given a random 'last seen time' of between one and two
                // weeks ago.
                const int64 nOneWeek = 7*24*60*60;
                struct in_addr ip;
                memcpy(&ip, &pnSeed[i], sizeof(ip));
                CAddress addr(CService(ip, GetDefaultPort()));
                addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
                vAdd.push_back(addr);
            }
            addrman.Add(vAdd, CNetAddr("127.0.0.1"));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }

        //
        // Choose an address to connect to based on most recently seen
        //
        CAddress addrConnect;

        // Only connect out to one peer per network group (/16 for IPv4).
        // Do this here so we don't have to critsect vNodes inside mapAddresses critsect.
        int nOutbound = 0;
<<<<<<< HEAD
        int nOutboundRelevant = 0;
        std::set<std::vector<unsigned char> > setConnected;
        {
            LOCK(cs_vNodes);
            for (CNode* pnode : vNodes) {
                if (!pnode->fInbound && !pnode->m_manual_connection) {

                    // Count the peers that have all relevant services
                    if (pnode->fSuccessfullyConnected && !pnode->fFeeler && ((pnode->nServices & nRelevantServices) == nRelevantServices)) {
                        nOutboundRelevant++;
                    }
                    // Netgroups for inbound and addnode peers are not excluded because our goal here
                    // is to not use multiple of our limited outbound slots on a single netgroup
                    // but inbound and addnode peers do not use our outbound slots.  Inbound peers
                    // also have the added issue that they're attacker controlled and could be used
                    // to prevent us from connecting to particular hosts if we used them here.
=======
        set<vector<unsigned char> > setConnected;
        {
            LOCK(cs_vNodes);
            BOOST_FOREACH(CNode* pnode, vNodes) {
                if (!pnode->fInbound) {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    setConnected.insert(pnode->addr.GetGroup());
                    nOutbound++;
                }
            }
        }

<<<<<<< HEAD
        // Feeler Connections
        //
        // Design goals:
        //  * Increase the number of connectable addresses in the tried table.
        //
        // Method:
        //  * Choose a random address from new and attempt to connect to it if we can connect
        //    successfully it is added to tried.
        //  * Start attempting feeler connections only after node finishes making outbound
        //    connections.
        //  * Only make a feeler connection once every few minutes.
        //
        bool fFeeler = false;

        if (nOutbound >= nMaxOutbound && !GetTryNewOutboundPeer()) {
            int64_t nTime = GetTimeMicros(); // The current time right now (in microseconds).
            if (nTime > nNextFeeler) {
                nNextFeeler = PoissonNextSend(nTime, FEELER_INTERVAL);
                fFeeler = true;
            } else {
                continue;
            }
        }

        int64_t nANow = GetAdjustedTime();
        int nTries = 0;
        while (!interruptNet)
        {
            CAddrInfo addr = addrman.Select(fFeeler);
=======
        int64 nANow = GetAdjustedTime();

        int nTries = 0;
        loop
        {
            // use an nUnkBias between 10 (no outgoing connections) and 90 (8 outgoing connections)
            CAddress addr = addrman.Select(10 + min(nOutbound,8)*10);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

            // if we selected an invalid address, restart
            if (!addr.IsValid() || setConnected.count(addr.GetGroup()) || IsLocal(addr))
                break;

            // If we didn't find an appropriate destination after trying 100 addresses fetched from addrman,
            // stop this loop, and let the outer loop run again (which sleeps, adds seed nodes, recalculates
            // already-connected network ranges, ...) before trying new addrman addresses.
            nTries++;
            if (nTries > 100)
                break;

            if (IsLimited(addr))
                continue;

<<<<<<< HEAD
            // only connect to full nodes
            if ((addr.nServices & REQUIRED_SERVICES) != REQUIRED_SERVICES)
                continue;

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            // only consider very recently tried nodes after 30 failed attempts
            if (nANow - addr.nLastTry < 600 && nTries < 30)
                continue;

<<<<<<< HEAD
            // only consider nodes missing relevant services after 40 failed attempts and only if less than half the outbound are up.
            ServiceFlags nRequiredServices = nRelevantServices;
            if (nTries >= 40 && nOutbound < (nMaxOutbound >> 1)) {
                nRequiredServices = REQUIRED_SERVICES;
            }

            if ((addr.nServices & nRequiredServices) != nRequiredServices) {
                continue;
            }

            // do not allow non-default ports, unless after 50 invalid addresses selected already
            if (addr.GetPort() != Params().GetDefaultPort() && nTries < 50)
                continue;

            addrConnect = addr;

            // regardless of the services assumed to be available, only require the minimum if half or more outbound have relevant services
            if (nOutboundRelevant >= (nMaxOutbound >> 1)) {
                addrConnect.nServices = REQUIRED_SERVICES;
            } else {
                addrConnect.nServices = nRequiredServices;
            }
            break;
        }

        if (addrConnect.IsValid()) {

            if (fFeeler) {
                // Add small amount of random noise before connection to avoid synchronization.
                int randsleep = GetRandInt(FEELER_SLEEP_WINDOW * 1000);
                if (!interruptNet.sleep_for(std::chrono::milliseconds(randsleep)))
                    return;
                LogPrint(BCLog::NET, "Making feeler connection to %s\n", addrConnect.ToString());
            }

            OpenNetworkConnection(addrConnect, (int)setConnected.size() >= std::min(nMaxConnections - 1, 2), &grant, nullptr, false, fFeeler);
        }
    }
}

std::vector<AddedNodeInfo> CConnman::GetAddedNodeInfo()
{
    std::vector<AddedNodeInfo> ret;

    std::list<std::string> lAddresses(0);
    {
        LOCK(cs_vAddedNodes);
        ret.reserve(vAddedNodes.size());
        for (const std::string& strAddNode : vAddedNodes)
            lAddresses.push_back(strAddNode);
    }


    // Build a map of all already connected addresses (by IP:port and by name) to inbound/outbound and resolved CService
    std::map<CService, bool> mapConnected;
    std::map<std::string, std::pair<bool, CService>> mapConnectedByName;
    {
        LOCK(cs_vNodes);
        for (const CNode* pnode : vNodes) {
            if (pnode->addr.IsValid()) {
                mapConnected[pnode->addr] = pnode->fInbound;
            }
            std::string addrName = pnode->GetAddrName();
            if (!addrName.empty()) {
                mapConnectedByName[std::move(addrName)] = std::make_pair(pnode->fInbound, static_cast<const CService&>(pnode->addr));
            }
        }
    }

    for (const std::string& strAddNode : lAddresses) {
        CService service(LookupNumeric(strAddNode.c_str(), Params().GetDefaultPort()));
        if (service.IsValid()) {
            // strAddNode is an IP:port
            auto it = mapConnected.find(service);
            if (it != mapConnected.end()) {
                ret.push_back(AddedNodeInfo{strAddNode, service, true, it->second});
            } else {
                ret.push_back(AddedNodeInfo{strAddNode, CService(), false, false});
            }
        } else {
            // strAddNode is a name
            auto it = mapConnectedByName.find(strAddNode);
            if (it != mapConnectedByName.end()) {
                ret.push_back(AddedNodeInfo{strAddNode, it->second.second, true, it->second.first});
            } else {
                ret.push_back(AddedNodeInfo{strAddNode, CService(), false, false});
            }
        }
    }

    return ret;
}

void CConnman::ThreadOpenAddedConnections()
{
    {
        LOCK(cs_vAddedNodes);
        vAddedNodes = gArgs.GetArgs("-addnode");
    }

    while (true)
    {
        CSemaphoreGrant grant(*semAddnode);
        std::vector<AddedNodeInfo> vInfo = GetAddedNodeInfo();
        bool tried = false;
        for (const AddedNodeInfo& info : vInfo) {
            if (!info.fConnected) {
                if (!grant.TryAcquire()) {
                    // If we've used up our semaphore and need a new one, lets not wait here since while we are waiting
                    // the addednodeinfo state might change.
                    break;
                }
                // If strAddedNode is an IP/port, decode it immediately, so
                // OpenNetworkConnection can detect existing connections to that IP/port.
                tried = true;
                CService service(LookupNumeric(info.strAddedNode.c_str(), Params().GetDefaultPort()));
                OpenNetworkConnection(CAddress(service, NODE_NONE), false, &grant, info.strAddedNode.c_str(), false, false, true);
                if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
                    return;
            }
        }
        // Retry every 60 seconds if a connection was attempted, otherwise two seconds
        if (!interruptNet.sleep_for(std::chrono::seconds(tried ? 60 : 2)))
            return;
=======
            // do not allow non-default ports, unless after 50 invalid addresses selected already
            if (addr.GetPort() != GetDefaultPort() && nTries < 50)
                continue;

            addrConnect = addr;
            break;
        }

        if (addrConnect.IsValid())
            OpenNetworkConnection(addrConnect, &grant);
    }
}

void ThreadOpenAddedConnections()
{
    {
        LOCK(cs_vAddedNodes);
        vAddedNodes = mapMultiArgs["-addnode"];
    }

    if (HaveNameProxy()) {
        while(true) {
            list<string> lAddresses(0);
            {
                LOCK(cs_vAddedNodes);
                BOOST_FOREACH(string& strAddNode, vAddedNodes)
                    lAddresses.push_back(strAddNode);
            }
            BOOST_FOREACH(string& strAddNode, lAddresses) {
                CAddress addr;
                CSemaphoreGrant grant(*semOutbound);
                OpenNetworkConnection(addr, &grant, strAddNode.c_str());
                MilliSleep(500);
            }
            MilliSleep(120000); // Retry every 2 minutes
        }
    }

    for (unsigned int i = 0; true; i++)
    {
        list<string> lAddresses(0);
        {
            LOCK(cs_vAddedNodes);
            BOOST_FOREACH(string& strAddNode, vAddedNodes)
                lAddresses.push_back(strAddNode);
        }

        list<vector<CService> > lservAddressesToAdd(0);
        BOOST_FOREACH(string& strAddNode, lAddresses)
        {
            vector<CService> vservNode(0);
            if(Lookup(strAddNode.c_str(), vservNode, GetDefaultPort(), fNameLookup, 0))
            {
                lservAddressesToAdd.push_back(vservNode);
                {
                    LOCK(cs_setservAddNodeAddresses);
                    BOOST_FOREACH(CService& serv, vservNode)
                        setservAddNodeAddresses.insert(serv);
                }
            }
        }
        // Attempt to connect to each IP for each addnode entry until at least one is successful per addnode entry
        // (keeping in mind that addnode entries can have many IPs if fNameLookup)
        {
            LOCK(cs_vNodes);
            BOOST_FOREACH(CNode* pnode, vNodes)
                for (list<vector<CService> >::iterator it = lservAddressesToAdd.begin(); it != lservAddressesToAdd.end(); it++)
                    BOOST_FOREACH(CService& addrNode, *(it))
                        if (pnode->addr == addrNode)
                        {
                            it = lservAddressesToAdd.erase(it);
                            it--;
                            break;
                        }
        }
        BOOST_FOREACH(vector<CService>& vserv, lservAddressesToAdd)
        {
            CSemaphoreGrant grant(*semOutbound);
            OpenNetworkConnection(CAddress(vserv[i % vserv.size()]), &grant);
            MilliSleep(500);
        }
        MilliSleep(120000); // Retry every 2 minutes
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
}

// if successful, this moves the passed grant to the constructed node
<<<<<<< HEAD
bool CConnman::OpenNetworkConnection(const CAddress& addrConnect, bool fCountFailure, CSemaphoreGrant *grantOutbound, const char *pszDest, bool fOneShot, bool fFeeler, bool manual_connection)
=======
bool OpenNetworkConnection(const CAddress& addrConnect, CSemaphoreGrant *grantOutbound, const char *strDest, bool fOneShot)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    //
    // Initiate outbound network connection
    //
<<<<<<< HEAD
    if (interruptNet) {
        return false;
    }
    if (!fNetworkActive) {
        return false;
    }
    if (!pszDest) {
        if (IsLocal(addrConnect) ||
            FindNode((CNetAddr)addrConnect) || IsBanned(addrConnect) ||
            FindNode(addrConnect.ToStringIPPort()))
            return false;
    } else if (FindNode(std::string(pszDest)))
        return false;

    CNode* pnode = ConnectNode(addrConnect, pszDest, fCountFailure);
=======
    boost::this_thread::interruption_point();
    if (!strDest)
        if (IsLocal(addrConnect) ||
            FindNode((CNetAddr)addrConnect) || CNode::IsBanned(addrConnect) ||
            FindNode(addrConnect.ToStringIPPort().c_str()))
            return false;
    if (strDest && FindNode(strDest))
        return false;

    CNode* pnode = ConnectNode(addrConnect, strDest);
    boost::this_thread::interruption_point();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    if (!pnode)
        return false;
    if (grantOutbound)
        grantOutbound->MoveTo(pnode->grantOutbound);
<<<<<<< HEAD
    if (fOneShot)
        pnode->fOneShot = true;
    if (fFeeler)
        pnode->fFeeler = true;
    if (manual_connection)
        pnode->m_manual_connection = true;

    m_msgproc->InitializeNode(pnode);
    {
        LOCK(cs_vNodes);
        vNodes.push_back(pnode);
    }
=======
    pnode->fNetworkNode = true;
    if (fOneShot)
        pnode->fOneShot = true;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    return true;
}

<<<<<<< HEAD
void CConnman::ThreadMessageHandler()
{
    while (!flagInterruptMsgProc)
    {
        std::vector<CNode*> vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            for (CNode* pnode : vNodesCopy) {
                pnode->AddRef();
            }
        }

        bool fMoreWork = false;

        for (CNode* pnode : vNodesCopy)
=======

// for now, use a very simple selection metric: the node from which we received
// most recently
double static NodeSyncScore(const CNode *pnode) {
    return -pnode->nLastRecv;
}

void static StartSync(const vector<CNode*> &vNodes) {
    CNode *pnodeNewSync = NULL;
    double dBestScore = 0;

    // fImporting and fReindex are accessed out of cs_main here, but only
    // as an optimization - they are checked again in SendMessages.
    if (fImporting || fReindex)
        return;

    // Iterate over all nodes
    BOOST_FOREACH(CNode* pnode, vNodes) {
        // check preconditions for allowing a sync
        if (!pnode->fClient && !pnode->fOneShot &&
            !pnode->fDisconnect && pnode->fSuccessfullyConnected &&
            (pnode->nStartingHeight > (nBestHeight - 144)) &&
            (pnode->nVersion < NOBLKS_VERSION_START || pnode->nVersion >= NOBLKS_VERSION_END)) {
            // if ok, compare node's score with the best so far
            double dScore = NodeSyncScore(pnode);
            if (pnodeNewSync == NULL || dScore > dBestScore) {
                pnodeNewSync = pnode;
                dBestScore = dScore;
            }
        }
    }
    // if a new sync candidate was found, start sync!
    if (pnodeNewSync) {
        pnodeNewSync->fStartSync = true;
        pnodeSync = pnodeNewSync;
    }
}

void ThreadMessageHandler()
{
    SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);
    while (true)
    {
        bool fHaveSyncNode = false;

        vector<CNode*> vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            BOOST_FOREACH(CNode* pnode, vNodesCopy) {
                pnode->AddRef();
                if (pnode == pnodeSync)
                    fHaveSyncNode = true;
            }
        }

        if (!fHaveSyncNode)
            StartSync(vNodesCopy);

        // Poll the connected nodes for messages
        CNode* pnodeTrickle = NULL;
        if (!vNodesCopy.empty())
            pnodeTrickle = vNodesCopy[GetRand(vNodesCopy.size())];

        bool fSleep = true;

        BOOST_FOREACH(CNode* pnode, vNodesCopy)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        {
            if (pnode->fDisconnect)
                continue;

            // Receive messages
<<<<<<< HEAD
            bool fMoreNodeWork = m_msgproc->ProcessMessages(pnode, flagInterruptMsgProc);
            fMoreWork |= (fMoreNodeWork && !pnode->fPauseSend);
            if (flagInterruptMsgProc)
                return;
            // Send messages
            {
                LOCK(pnode->cs_sendProcessing);
                m_msgproc->SendMessages(pnode, flagInterruptMsgProc);
            }

            if (flagInterruptMsgProc)
                return;
=======
            {
                TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                if (lockRecv)
                {
                    if (!ProcessMessages(pnode))
                        pnode->CloseSocketDisconnect();

                    if (pnode->nSendSize < SendBufferSize())
                    {
                        if (!pnode->vRecvGetData.empty() || (!pnode->vRecvMsg.empty() && pnode->vRecvMsg[0].complete()))
                        {
                            fSleep = false;
                        }
                    }
                }
            }
            boost::this_thread::interruption_point();

            // Send messages
            {
                TRY_LOCK(pnode->cs_vSend, lockSend);
                if (lockSend)
                    SendMessages(pnode, pnode == pnodeTrickle);
            }
            boost::this_thread::interruption_point();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }

        {
            LOCK(cs_vNodes);
<<<<<<< HEAD
            for (CNode* pnode : vNodesCopy)
                pnode->Release();
        }

        std::unique_lock<std::mutex> lock(mutexMsgProc);
        if (!fMoreWork) {
            condMsgProc.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::milliseconds(100), [this] { return fMsgProcWake; });
        }
        fMsgProcWake = false;
=======
            BOOST_FOREACH(CNode* pnode, vNodesCopy)
                pnode->Release();
        }

        if (fSleep)
            MilliSleep(100);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
}






<<<<<<< HEAD
bool CConnman::BindListenPort(const CService &addrBind, std::string& strError, bool fWhitelisted)
=======
bool BindListenPort(const CService &addrBind, string& strError)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    strError = "";
    int nOne = 1;

    // Create socket for listening for incoming connections
<<<<<<< HEAD
    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
    if (!addrBind.GetSockAddr((struct sockaddr*)&sockaddr, &len))
    {
        strError = strprintf("Error: Bind address family for %s not supported", addrBind.ToString());
        LogPrintf("%s\n", strError);
=======
#ifdef USE_IPV6
    struct sockaddr_storage sockaddr;
#else
    struct sockaddr sockaddr;
#endif
    socklen_t len = sizeof(sockaddr);
    if (!addrBind.GetSockAddr((struct sockaddr*)&sockaddr, &len))
    {
        strError = strprintf("Error: bind address family for %s not supported", addrBind.ToString().c_str());
        printf("%s\n", strError.c_str());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        return false;
    }

    SOCKET hListenSocket = socket(((struct sockaddr*)&sockaddr)->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (hListenSocket == INVALID_SOCKET)
    {
<<<<<<< HEAD
        strError = strprintf("Error: Couldn't open socket for incoming connections (socket returned error %s)", NetworkErrorString(WSAGetLastError()));
        LogPrintf("%s\n", strError);
        return false;
    }
    if (!IsSelectableSocket(hListenSocket))
    {
        strError = "Error: Couldn't create a listenable socket for incoming connections";
        LogPrintf("%s\n", strError);
        return false;
    }


#ifndef WIN32
=======
        strError = strprintf("Error: Couldn't open socket for incoming connections (socket returned error %d)", WSAGetLastError());
        printf("%s\n", strError.c_str());
        return false;
    }

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#ifdef SO_NOSIGPIPE
    // Different way of disabling SIGPIPE on BSD
    setsockopt(hListenSocket, SOL_SOCKET, SO_NOSIGPIPE, (void*)&nOne, sizeof(int));
#endif
<<<<<<< HEAD
    // Allow binding if the port is still in TIME_WAIT state after
    // the program was closed and restarted.
    setsockopt(hListenSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&nOne, sizeof(int));
    // Disable Nagle's algorithm
    setsockopt(hListenSocket, IPPROTO_TCP, TCP_NODELAY, (void*)&nOne, sizeof(int));
#else
    setsockopt(hListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOne, sizeof(int));
    setsockopt(hListenSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&nOne, sizeof(int));
#endif

    // Set to non-blocking, incoming connections will also inherit this
    if (!SetSocketNonBlocking(hListenSocket, true)) {
        CloseSocket(hListenSocket);
        strError = strprintf("BindListenPort: Setting listening socket to non-blocking failed, error %s\n", NetworkErrorString(WSAGetLastError()));
        LogPrintf("%s\n", strError);
        return false;
    }

=======

#ifndef WIN32
    // Allow binding if the port is still in TIME_WAIT state after
    // the program was closed and restarted.  Not an issue on windows.
    setsockopt(hListenSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&nOne, sizeof(int));
#endif


#ifdef WIN32
    // Set to non-blocking, incoming connections will also inherit this
    if (ioctlsocket(hListenSocket, FIONBIO, (u_long*)&nOne) == SOCKET_ERROR)
#else
    if (fcntl(hListenSocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
#endif
    {
        strError = strprintf("Error: Couldn't set properties on socket for incoming connections (error %d)", WSAGetLastError());
        printf("%s\n", strError.c_str());
        return false;
    }

#ifdef USE_IPV6
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    // some systems don't have IPV6_V6ONLY but are always v6only; others do have the option
    // and enable it by default or not. Try to enable it, if possible.
    if (addrBind.IsIPv6()) {
#ifdef IPV6_V6ONLY
#ifdef WIN32
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&nOne, sizeof(int));
#else
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&nOne, sizeof(int));
#endif
#endif
#ifdef WIN32
<<<<<<< HEAD
        int nProtLevel = PROTECTION_LEVEL_UNRESTRICTED;
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_PROTECTION_LEVEL, (const char*)&nProtLevel, sizeof(int));
#endif
    }
=======
        int nProtLevel = 10 /* PROTECTION_LEVEL_UNRESTRICTED */;
        int nParameterId = 23 /* IPV6_PROTECTION_LEVEl */;
        // this call is allowed to fail
        setsockopt(hListenSocket, IPPROTO_IPV6, nParameterId, (const char*)&nProtLevel, sizeof(int));
#endif
    }
#endif
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    if (::bind(hListenSocket, (struct sockaddr*)&sockaddr, len) == SOCKET_ERROR)
    {
        int nErr = WSAGetLastError();
        if (nErr == WSAEADDRINUSE)
<<<<<<< HEAD
            strError = strprintf(_("Unable to bind to %s on this computer. %s is probably already running."), addrBind.ToString(), _(PACKAGE_NAME));
        else
            strError = strprintf(_("Unable to bind to %s on this computer (bind returned error %s)"), addrBind.ToString(), NetworkErrorString(nErr));
        LogPrintf("%s\n", strError);
        CloseSocket(hListenSocket);
        return false;
    }
    LogPrintf("Bound to %s\n", addrBind.ToString());
=======
            strError = strprintf(_("Unable to bind to %s on this computer. rainbowcoin is probably already running."), addrBind.ToString().c_str());
        else
            strError = strprintf(_("Unable to bind to %s on this computer (bind returned error %d, %s)"), addrBind.ToString().c_str(), nErr, strerror(nErr));
        printf("%s\n", strError.c_str());
        return false;
    }
    printf("Bound to %s\n", addrBind.ToString().c_str());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    // Listen for incoming connections
    if (listen(hListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
<<<<<<< HEAD
        strError = strprintf(_("Error: Listening for incoming connections failed (listen returned error %s)"), NetworkErrorString(WSAGetLastError()));
        LogPrintf("%s\n", strError);
        CloseSocket(hListenSocket);
        return false;
    }

    vhListenSocket.push_back(ListenSocket(hListenSocket, fWhitelisted));

    if (addrBind.IsRoutable() && fDiscover && !fWhitelisted)
=======
        strError = strprintf("Error: Listening for incoming connections failed (listen returned error %d)", WSAGetLastError());
        printf("%s\n", strError.c_str());
        return false;
    }

    vhListenSocket.push_back(hListenSocket);

    if (addrBind.IsRoutable() && fDiscover)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        AddLocal(addrBind, LOCAL_BIND);

    return true;
}

<<<<<<< HEAD
void Discover(boost::thread_group& threadGroup)
=======
void static Discover()
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
    if (!fDiscover)
        return;

#ifdef WIN32
    // Get local host IP
<<<<<<< HEAD
    char pszHostName[256] = "";
    if (gethostname(pszHostName, sizeof(pszHostName)) != SOCKET_ERROR)
    {
        std::vector<CNetAddr> vaddr;
        if (LookupHost(pszHostName, vaddr, 0, true))
        {
            for (const CNetAddr &addr : vaddr)
            {
                if (AddLocal(addr, LOCAL_IF))
                    LogPrintf("%s: %s - %s\n", __func__, pszHostName, addr.ToString());
=======
    char pszHostName[1000] = "";
    if (gethostname(pszHostName, sizeof(pszHostName)) != SOCKET_ERROR)
    {
        vector<CNetAddr> vaddr;
        if (LookupHost(pszHostName, vaddr))
        {
            BOOST_FOREACH (const CNetAddr &addr, vaddr)
            {
                AddLocal(addr, LOCAL_IF);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            }
        }
    }
#else
    // Get local host ip
    struct ifaddrs* myaddrs;
    if (getifaddrs(&myaddrs) == 0)
    {
<<<<<<< HEAD
        for (struct ifaddrs* ifa = myaddrs; ifa != nullptr; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == nullptr) continue;
=======
        for (struct ifaddrs* ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) continue;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            if ((ifa->ifa_flags & IFF_UP) == 0) continue;
            if (strcmp(ifa->ifa_name, "lo") == 0) continue;
            if (strcmp(ifa->ifa_name, "lo0") == 0) continue;
            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                struct sockaddr_in* s4 = (struct sockaddr_in*)(ifa->ifa_addr);
                CNetAddr addr(s4->sin_addr);
                if (AddLocal(addr, LOCAL_IF))
<<<<<<< HEAD
                    LogPrintf("%s: IPv4 %s: %s\n", __func__, ifa->ifa_name, addr.ToString());
            }
=======
                    printf("IPv4 %s: %s\n", ifa->ifa_name, addr.ToString().c_str());
            }
#ifdef USE_IPV6
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            else if (ifa->ifa_addr->sa_family == AF_INET6)
            {
                struct sockaddr_in6* s6 = (struct sockaddr_in6*)(ifa->ifa_addr);
                CNetAddr addr(s6->sin6_addr);
                if (AddLocal(addr, LOCAL_IF))
<<<<<<< HEAD
                    LogPrintf("%s: IPv6 %s: %s\n", __func__, ifa->ifa_name, addr.ToString());
            }
=======
                    printf("IPv6 %s: %s\n", ifa->ifa_name, addr.ToString().c_str());
            }
#endif
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
        freeifaddrs(myaddrs);
    }
#endif
<<<<<<< HEAD
}

void CConnman::SetNetworkActive(bool active)
{
    LogPrint(BCLog::NET, "SetNetworkActive: %s\n", active);

    if (fNetworkActive == active) {
        return;
    }

    fNetworkActive = active;

    if (!fNetworkActive) {
        LOCK(cs_vNodes);
        // Close sockets to all nodes
        for (CNode* pnode : vNodes) {
            pnode->CloseSocketDisconnect();
        }
    }

    uiInterface.NotifyNetworkActiveChanged(fNetworkActive);
}

CConnman::CConnman(uint64_t nSeed0In, uint64_t nSeed1In) : nSeed0(nSeed0In), nSeed1(nSeed1In)
{
    fNetworkActive = true;
    setBannedIsDirty = false;
    fAddressesInitialized = false;
    nLastNodeId = 0;
    nSendBufferMaxSize = 0;
    nReceiveFloodSize = 0;
    semOutbound = nullptr;
    semAddnode = nullptr;
    flagInterruptMsgProc = false;
    SetTryNewOutboundPeer(false);

    Options connOptions;
    Init(connOptions);
}

NodeId CConnman::GetNewNodeId()
{
    return nLastNodeId.fetch_add(1, std::memory_order_relaxed);
}


bool CConnman::Bind(const CService &addr, unsigned int flags) {
    if (!(flags & BF_EXPLICIT) && IsLimited(addr))
        return false;
    std::string strError;
    if (!BindListenPort(addr, strError, (flags & BF_WHITELIST) != 0)) {
        if ((flags & BF_REPORT_ERROR) && clientInterface) {
            clientInterface->ThreadSafeMessageBox(strError, "", CClientUIInterface::MSG_ERROR);
        }
        return false;
    }
    return true;
}

bool CConnman::InitBinds(const std::vector<CService>& binds, const std::vector<CService>& whiteBinds) {
    bool fBound = false;
    for (const auto& addrBind : binds) {
        fBound |= Bind(addrBind, (BF_EXPLICIT | BF_REPORT_ERROR));
    }
    for (const auto& addrBind : whiteBinds) {
        fBound |= Bind(addrBind, (BF_EXPLICIT | BF_REPORT_ERROR | BF_WHITELIST));
    }
    if (binds.empty() && whiteBinds.empty()) {
        struct in_addr inaddr_any;
        inaddr_any.s_addr = INADDR_ANY;
        fBound |= Bind(CService(in6addr_any, GetListenPort()), BF_NONE);
        fBound |= Bind(CService(inaddr_any, GetListenPort()), !fBound ? BF_REPORT_ERROR : BF_NONE);
    }
    return fBound;
}

bool CConnman::Start(CScheduler& scheduler, const Options& connOptions)
{
    Init(connOptions);

    nTotalBytesRecv = 0;
    nTotalBytesSent = 0;
    nMaxOutboundTotalBytesSentInCycle = 0;
    nMaxOutboundCycleStartTime = 0;

    if (fListen && !InitBinds(connOptions.vBinds, connOptions.vWhiteBinds)) {
        if (clientInterface) {
            clientInterface->ThreadSafeMessageBox(
                _("Failed to listen on any port. Use -listen=0 if you want this."),
                "", CClientUIInterface::MSG_ERROR);
        }
        return false;
    }

    for (const auto& strDest : connOptions.vSeedNodes) {
        AddOneShot(strDest);
    }

    if (clientInterface) {
        clientInterface->InitMessage(_("Loading P2P addresses..."));
    }
    // Load addresses from peers.dat
    int64_t nStart = GetTimeMillis();
    {
        CAddrDB adb;
        if (adb.Read(addrman))
            LogPrintf("Loaded %i addresses from peers.dat  %dms\n", addrman.size(), GetTimeMillis() - nStart);
        else {
            addrman.Clear(); // Addrman can be in an inconsistent state after failure, reset it
            LogPrintf("Invalid or missing peers.dat; recreating\n");
            DumpAddresses();
        }
    }
    if (clientInterface)
        clientInterface->InitMessage(_("Loading banlist..."));
    // Load addresses from banlist.dat
    nStart = GetTimeMillis();
    CBanDB bandb;
    banmap_t banmap;
    if (bandb.Read(banmap)) {
        SetBanned(banmap); // thread save setter
        SetBannedSetDirty(false); // no need to write down, just read data
        SweepBanned(); // sweep out unused entries

        LogPrint(BCLog::NET, "Loaded %d banned node ips/subnets from banlist.dat  %dms\n",
            banmap.size(), GetTimeMillis() - nStart);
    } else {
        LogPrintf("Invalid or missing banlist.dat; recreating\n");
        SetBannedSetDirty(true); // force write
        DumpBanlist();
    }

    uiInterface.InitMessage(_("Starting network threads..."));

    fAddressesInitialized = true;

    if (semOutbound == nullptr) {
        // initialize semaphore
        semOutbound = new CSemaphore(std::min((nMaxOutbound + nMaxFeeler), nMaxConnections));
    }
    if (semAddnode == nullptr) {
        // initialize semaphore
        semAddnode = new CSemaphore(nMaxAddnode);
    }
=======

    // Don't use external IPv4 discovery, when -onlynet="IPv6"
    if (!IsLimited(NET_IPV4))
        NewThread(ThreadGetMyExternalIP, NULL);
}

void StartNode(boost::thread_group& threadGroup)
{
    if (semOutbound == NULL) {
        // initialize semaphore
        int nMaxOutbound = min(MAX_OUTBOUND_CONNECTIONS, nMaxConnections);
        semOutbound = new CSemaphore(nMaxOutbound);
    }

    if (pnodeLocalHost == NULL)
        pnodeLocalHost = new CNode(INVALID_SOCKET, CAddress(CService("127.0.0.1", 0), nLocalServices));

    Discover();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    //
    // Start threads
    //
<<<<<<< HEAD
    assert(m_msgproc);
    InterruptSocks5(false);
    interruptNet.reset();
    flagInterruptMsgProc = false;

    {
        std::unique_lock<std::mutex> lock(mutexMsgProc);
        fMsgProcWake = false;
    }

    // Send and receive from sockets, accept connections
    threadSocketHandler = std::thread(&TraceThread<std::function<void()> >, "net", std::function<void()>(std::bind(&CConnman::ThreadSocketHandler, this)));

    if (!gArgs.GetBoolArg("-dnsseed", true))
        LogPrintf("DNS seeding disabled\n");
    else
        threadDNSAddressSeed = std::thread(&TraceThread<std::function<void()> >, "dnsseed", std::function<void()>(std::bind(&CConnman::ThreadDNSAddressSeed, this)));

    // Initiate outbound connections from -addnode
    threadOpenAddedConnections = std::thread(&TraceThread<std::function<void()> >, "addcon", std::function<void()>(std::bind(&CConnman::ThreadOpenAddedConnections, this)));

    // Initiate outbound connections unless connect=0
    if (!gArgs.IsArgSet("-connect") || gArgs.GetArgs("-connect").size() != 1 || gArgs.GetArgs("-connect")[0] != "0")
        threadOpenConnections = std::thread(&TraceThread<std::function<void()> >, "opencon", std::function<void()>(std::bind(&CConnman::ThreadOpenConnections, this)));

    // Process messages
    threadMessageHandler = std::thread(&TraceThread<std::function<void()> >, "msghand", std::function<void()>(std::bind(&CConnman::ThreadMessageHandler, this)));

    // Dump network addresses
    scheduler.scheduleEvery(std::bind(&CConnman::DumpData, this), DUMP_ADDRESSES_INTERVAL * 1000);
=======

    if (!GetBoolArg("-dnsseed", true))
        printf("DNS seeding disabled\n");
    else
        threadGroup.create_thread(boost::bind(&TraceThread<boost::function<void()> >, "dnsseed", &ThreadDNSAddressSeed));

#ifdef USE_UPNP
    // Map ports with UPnP
    MapPort(GetBoolArg("-upnp", USE_UPNP));
#endif

    // Send and receive from sockets, accept connections
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "net", &ThreadSocketHandler));

    // Initiate outbound connections from -addnode
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "addcon", &ThreadOpenAddedConnections));

    // Initiate outbound connections
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "opencon", &ThreadOpenConnections));

    // Process messages
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "msghand", &ThreadMessageHandler));

    // Dump network addresses
    threadGroup.create_thread(boost::bind(&LoopForever<void (*)()>, "dumpaddr", &DumpAddresses, DUMP_ADDRESSES_INTERVAL * 1000));
}

bool StopNode()
{
    printf("StopNode()\n");
    GenerateBitcoins(false, NULL);
    MapPort(false);
    nTransactionsUpdated++;
    if (semOutbound)
        for (int i=0; i<MAX_OUTBOUND_CONNECTIONS; i++)
            semOutbound->post();
    MilliSleep(50);
    DumpAddresses();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    return true;
}

class CNetCleanup
{
public:
<<<<<<< HEAD
    CNetCleanup() {}

    ~CNetCleanup()
    {
=======
    CNetCleanup()
    {
    }
    ~CNetCleanup()
    {
        // Close sockets
        BOOST_FOREACH(CNode* pnode, vNodes)
            if (pnode->hSocket != INVALID_SOCKET)
                closesocket(pnode->hSocket);
        BOOST_FOREACH(SOCKET hListenSocket, vhListenSocket)
            if (hListenSocket != INVALID_SOCKET)
                if (closesocket(hListenSocket) == SOCKET_ERROR)
                    printf("closesocket(hListenSocket) failed with error %d\n", WSAGetLastError());

        // clean up some globals (to help leak detection)
        BOOST_FOREACH(CNode *pnode, vNodes)
            delete pnode;
        BOOST_FOREACH(CNode *pnode, vNodesDisconnected)
            delete pnode;
        vNodes.clear();
        vNodesDisconnected.clear();
        delete semOutbound;
        semOutbound = NULL;
        delete pnodeLocalHost;
        pnodeLocalHost = NULL;

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#ifdef WIN32
        // Shutdown Windows Sockets
        WSACleanup();
#endif
    }
}
instance_of_cnetcleanup;

<<<<<<< HEAD
void CConnman::Interrupt()
{
    {
        std::lock_guard<std::mutex> lock(mutexMsgProc);
        flagInterruptMsgProc = true;
    }
    condMsgProc.notify_all();

    interruptNet();
    InterruptSocks5(true);

    if (semOutbound) {
        for (int i=0; i<(nMaxOutbound + nMaxFeeler); i++) {
            semOutbound->post();
        }
    }

    if (semAddnode) {
        for (int i=0; i<nMaxAddnode; i++) {
            semAddnode->post();
        }
    }
}

void CConnman::Stop()
{
    if (threadMessageHandler.joinable())
        threadMessageHandler.join();
    if (threadOpenConnections.joinable())
        threadOpenConnections.join();
    if (threadOpenAddedConnections.joinable())
        threadOpenAddedConnections.join();
    if (threadDNSAddressSeed.joinable())
        threadDNSAddressSeed.join();
    if (threadSocketHandler.joinable())
        threadSocketHandler.join();

    if (fAddressesInitialized)
    {
        DumpData();
        fAddressesInitialized = false;
    }

    // Close sockets
    for (CNode* pnode : vNodes)
        pnode->CloseSocketDisconnect();
    for (ListenSocket& hListenSocket : vhListenSocket)
        if (hListenSocket.socket != INVALID_SOCKET)
            if (!CloseSocket(hListenSocket.socket))
                LogPrintf("CloseSocket(hListenSocket) failed with error %s\n", NetworkErrorString(WSAGetLastError()));

    // clean up some globals (to help leak detection)
    for (CNode *pnode : vNodes) {
        DeleteNode(pnode);
    }
    for (CNode *pnode : vNodesDisconnected) {
        DeleteNode(pnode);
    }
    vNodes.clear();
    vNodesDisconnected.clear();
    vhListenSocket.clear();
    delete semOutbound;
    semOutbound = nullptr;
    delete semAddnode;
    semAddnode = nullptr;
}

void CConnman::DeleteNode(CNode* pnode)
{
    assert(pnode);
    bool fUpdateConnectionTime = false;
    m_msgproc->FinalizeNode(pnode->GetId(), fUpdateConnectionTime);
    if(fUpdateConnectionTime) {
        addrman.Connected(pnode->addr);
    }
    delete pnode;
}

CConnman::~CConnman()
{
    Interrupt();
    Stop();
}

size_t CConnman::GetAddressCount() const
{
    return addrman.size();
}

void CConnman::SetServices(const CService &addr, ServiceFlags nServices)
{
    addrman.SetServices(addr, nServices);
}

void CConnman::MarkAddressGood(const CAddress& addr)
{
    addrman.Good(addr);
}

void CConnman::AddNewAddresses(const std::vector<CAddress>& vAddr, const CAddress& addrFrom, int64_t nTimePenalty)
{
    addrman.Add(vAddr, addrFrom, nTimePenalty);
}

std::vector<CAddress> CConnman::GetAddresses()
{
    return addrman.GetAddr();
}

bool CConnman::AddNode(const std::string& strNode)
{
    LOCK(cs_vAddedNodes);
    for(std::vector<std::string>::const_iterator it = vAddedNodes.begin(); it != vAddedNodes.end(); ++it) {
        if (strNode == *it)
            return false;
    }

    vAddedNodes.push_back(strNode);
    return true;
}

bool CConnman::RemoveAddedNode(const std::string& strNode)
{
    LOCK(cs_vAddedNodes);
    for(std::vector<std::string>::iterator it = vAddedNodes.begin(); it != vAddedNodes.end(); ++it) {
        if (strNode == *it) {
            vAddedNodes.erase(it);
            return true;
        }
    }
    return false;
}

size_t CConnman::GetNodeCount(NumConnections flags)
{
    LOCK(cs_vNodes);
    if (flags == CConnman::CONNECTIONS_ALL) // Shortcut if we want total
        return vNodes.size();

    int nNum = 0;
    for(std::vector<CNode*>::const_iterator it = vNodes.begin(); it != vNodes.end(); ++it)
        if (flags & ((*it)->fInbound ? CONNECTIONS_IN : CONNECTIONS_OUT))
            nNum++;

    return nNum;
}

void CConnman::GetNodeStats(std::vector<CNodeStats>& vstats)
{
    vstats.clear();
    LOCK(cs_vNodes);
    vstats.reserve(vNodes.size());
    for(std::vector<CNode*>::iterator it = vNodes.begin(); it != vNodes.end(); ++it) {
        CNode* pnode = *it;
        vstats.emplace_back();
        pnode->copyStats(vstats.back());
    }
}

bool CConnman::DisconnectNode(const std::string& strNode)
{
    LOCK(cs_vNodes);
    if (CNode* pnode = FindNode(strNode)) {
        pnode->fDisconnect = true;
        return true;
    }
    return false;
}
bool CConnman::DisconnectNode(NodeId id)
{
    LOCK(cs_vNodes);
    for(CNode* pnode : vNodes) {
        if (id == pnode->GetId()) {
            pnode->fDisconnect = true;
            return true;
        }
    }
    return false;
}

void CConnman::RecordBytesRecv(uint64_t bytes)
{
    LOCK(cs_totalBytesRecv);
    nTotalBytesRecv += bytes;
}

void CConnman::RecordBytesSent(uint64_t bytes)
{
    LOCK(cs_totalBytesSent);
    nTotalBytesSent += bytes;

    uint64_t now = GetTime();
    if (nMaxOutboundCycleStartTime + nMaxOutboundTimeframe < now)
    {
        // timeframe expired, reset cycle
        nMaxOutboundCycleStartTime = now;
        nMaxOutboundTotalBytesSentInCycle = 0;
    }

    // TODO, exclude whitebind peers
    nMaxOutboundTotalBytesSentInCycle += bytes;
}

void CConnman::SetMaxOutboundTarget(uint64_t limit)
{
    LOCK(cs_totalBytesSent);
    nMaxOutboundLimit = limit;
}

uint64_t CConnman::GetMaxOutboundTarget()
{
    LOCK(cs_totalBytesSent);
    return nMaxOutboundLimit;
}

uint64_t CConnman::GetMaxOutboundTimeframe()
{
    LOCK(cs_totalBytesSent);
    return nMaxOutboundTimeframe;
}

uint64_t CConnman::GetMaxOutboundTimeLeftInCycle()
{
    LOCK(cs_totalBytesSent);
    if (nMaxOutboundLimit == 0)
        return 0;

    if (nMaxOutboundCycleStartTime == 0)
        return nMaxOutboundTimeframe;

    uint64_t cycleEndTime = nMaxOutboundCycleStartTime + nMaxOutboundTimeframe;
    uint64_t now = GetTime();
    return (cycleEndTime < now) ? 0 : cycleEndTime - GetTime();
}

void CConnman::SetMaxOutboundTimeframe(uint64_t timeframe)
{
    LOCK(cs_totalBytesSent);
    if (nMaxOutboundTimeframe != timeframe)
    {
        // reset measure-cycle in case of changing
        // the timeframe
        nMaxOutboundCycleStartTime = GetTime();
    }
    nMaxOutboundTimeframe = timeframe;
}

bool CConnman::OutboundTargetReached(bool historicalBlockServingLimit)
{
    LOCK(cs_totalBytesSent);
    if (nMaxOutboundLimit == 0)
        return false;

    if (historicalBlockServingLimit)
    {
        // keep a large enough buffer to at least relay each block once
        uint64_t timeLeftInCycle = GetMaxOutboundTimeLeftInCycle();
        uint64_t buffer = timeLeftInCycle / 600 * MAX_BLOCK_SERIALIZED_SIZE;
        if (buffer >= nMaxOutboundLimit || nMaxOutboundTotalBytesSentInCycle >= nMaxOutboundLimit - buffer)
            return true;
    }
    else if (nMaxOutboundTotalBytesSentInCycle >= nMaxOutboundLimit)
        return true;

    return false;
}

uint64_t CConnman::GetOutboundTargetBytesLeft()
{
    LOCK(cs_totalBytesSent);
    if (nMaxOutboundLimit == 0)
        return 0;

    return (nMaxOutboundTotalBytesSentInCycle >= nMaxOutboundLimit) ? 0 : nMaxOutboundLimit - nMaxOutboundTotalBytesSentInCycle;
}

uint64_t CConnman::GetTotalBytesRecv()
{
    LOCK(cs_totalBytesRecv);
    return nTotalBytesRecv;
}

uint64_t CConnman::GetTotalBytesSent()
{
    LOCK(cs_totalBytesSent);
    return nTotalBytesSent;
}

ServiceFlags CConnman::GetLocalServices() const
{
    return nLocalServices;
}

void CConnman::SetBestHeight(int height)
{
    nBestHeight.store(height, std::memory_order_release);
}

int CConnman::GetBestHeight() const
{
    return nBestHeight.load(std::memory_order_acquire);
}

unsigned int CConnman::GetReceiveFloodSize() const { return nReceiveFloodSize; }

CNode::CNode(NodeId idIn, ServiceFlags nLocalServicesIn, int nMyStartingHeightIn, SOCKET hSocketIn, const CAddress& addrIn, uint64_t nKeyedNetGroupIn, uint64_t nLocalHostNonceIn, const CAddress &addrBindIn, const std::string& addrNameIn, bool fInboundIn) :
    nTimeConnected(GetSystemTimeInSeconds()),
    addr(addrIn),
    addrBind(addrBindIn),
    fInbound(fInboundIn),
    nKeyedNetGroup(nKeyedNetGroupIn),
    addrKnown(5000, 0.001),
    filterInventoryKnown(50000, 0.000001),
    id(idIn),
    nLocalHostNonce(nLocalHostNonceIn),
    nLocalServices(nLocalServicesIn),
    nMyStartingHeight(nMyStartingHeightIn),
    nSendVersion(0)
{
    nServices = NODE_NONE;
    nServicesExpected = NODE_NONE;
    hSocket = hSocketIn;
    nRecvVersion = INIT_PROTO_VERSION;
    nLastSend = 0;
    nLastRecv = 0;
    nSendBytes = 0;
    nRecvBytes = 0;
    nTimeOffset = 0;
    addrName = addrNameIn == "" ? addr.ToStringIPPort() : addrNameIn;
    nVersion = 0;
    strSubVer = "";
    fWhitelisted = false;
    fOneShot = false;
    m_manual_connection = false;
    fClient = false; // set by version message
    fFeeler = false;
    fSuccessfullyConnected = false;
    fDisconnect = false;
    nRefCount = 0;
    nSendSize = 0;
    nSendOffset = 0;
    hashContinue = uint256();
    nStartingHeight = -1;
    filterInventoryKnown.reset();
    fSendMempool = false;
    fGetAddr = false;
    nNextLocalAddrSend = 0;
    nNextAddrSend = 0;
    nNextInvSend = 0;
    fRelayTxes = false;
    fSentAddr = false;
    pfilter = new CBloomFilter();
    timeLastMempoolReq = 0;
    nLastBlockTime = 0;
    nLastTXTime = 0;
    nPingNonceSent = 0;
    nPingUsecStart = 0;
    nPingUsecTime = 0;
    fPingQueued = false;
    nMinPingUsecTime = std::numeric_limits<int64_t>::max();
    minFeeFilter = 0;
    lastSentFeeFilter = 0;
    nextSendTimeFeeFilter = 0;
    fPauseRecv = false;
    fPauseSend = false;
    nProcessQueueSize = 0;

    for (const std::string &msg : getAllNetMessageTypes())
        mapRecvBytesPerMsgCmd[msg] = 0;
    mapRecvBytesPerMsgCmd[NET_MESSAGE_COMMAND_OTHER] = 0;

    if (fLogIPs) {
        LogPrint(BCLog::NET, "Added connection to %s peer=%d\n", addrName, id);
    } else {
        LogPrint(BCLog::NET, "Added connection peer=%d\n", id);
    }
}

CNode::~CNode()
{
    CloseSocket(hSocket);

    if (pfilter)
        delete pfilter;
}

void CNode::AskFor(const CInv& inv)
{
    if (mapAskFor.size() > MAPASKFOR_MAX_SZ || setAskFor.size() > SETASKFOR_MAX_SZ)
        return;
    // a peer may not have multiple non-responded queue positions for a single inv item
    if (!setAskFor.insert(inv.hash).second)
        return;

    // We're using mapAskFor as a priority queue,
    // the key is the earliest time the request can be sent
    int64_t nRequestTime;
    limitedmap<uint256, int64_t>::const_iterator it = mapAlreadyAskedFor.find(inv.hash);
    if (it != mapAlreadyAskedFor.end())
        nRequestTime = it->second;
    else
        nRequestTime = 0;
    LogPrint(BCLog::NET, "askfor %s  %d (%s) peer=%d\n", inv.ToString(), nRequestTime, DateTimeStrFormat("%H:%M:%S", nRequestTime/1000000), id);

    // Make sure not to reuse time indexes to keep things in the same order
    int64_t nNow = GetTimeMicros() - 1000000;
    static int64_t nLastTime;
    ++nLastTime;
    nNow = std::max(nNow, nLastTime);
    nLastTime = nNow;

    // Each retry is 2 minutes after the last
    nRequestTime = std::max(nRequestTime + 2 * 60 * 1000000, nNow);
    if (it != mapAlreadyAskedFor.end())
        mapAlreadyAskedFor.update(it, nRequestTime);
    else
        mapAlreadyAskedFor.insert(std::make_pair(inv.hash, nRequestTime));
    mapAskFor.insert(std::make_pair(nRequestTime, inv));
}

bool CConnman::NodeFullyConnected(const CNode* pnode)
{
    return pnode && pnode->fSuccessfullyConnected && !pnode->fDisconnect;
}

void CConnman::PushMessage(CNode* pnode, CSerializedNetMsg&& msg)
{
    size_t nMessageSize = msg.data.size();
    size_t nTotalSize = nMessageSize + CMessageHeader::HEADER_SIZE;
    LogPrint(BCLog::NET, "sending %s (%d bytes) peer=%d\n",  SanitizeString(msg.command.c_str()), nMessageSize, pnode->GetId());

    std::vector<unsigned char> serializedHeader;
    serializedHeader.reserve(CMessageHeader::HEADER_SIZE);
    uint256 hash = Hash(msg.data.data(), msg.data.data() + nMessageSize);
    CMessageHeader hdr(Params().MessageStart(), msg.command.c_str(), nMessageSize);
    memcpy(hdr.pchChecksum, hash.begin(), CMessageHeader::CHECKSUM_SIZE);

    CVectorWriter{SER_NETWORK, INIT_PROTO_VERSION, serializedHeader, 0, hdr};

    size_t nBytesSent = 0;
    {
        LOCK(pnode->cs_vSend);
        bool optimisticSend(pnode->vSendMsg.empty());

        //log total amount of bytes per command
        pnode->mapSendBytesPerMsgCmd[msg.command] += nTotalSize;
        pnode->nSendSize += nTotalSize;

        if (pnode->nSendSize > nSendBufferMaxSize)
            pnode->fPauseSend = true;
        pnode->vSendMsg.push_back(std::move(serializedHeader));
        if (nMessageSize)
            pnode->vSendMsg.push_back(std::move(msg.data));

        // If write queue empty, attempt "optimistic write"
        if (optimisticSend == true)
            nBytesSent = SocketSendData(pnode);
    }
    if (nBytesSent)
        RecordBytesSent(nBytesSent);
}

bool CConnman::ForNode(NodeId id, std::function<bool(CNode* pnode)> func)
{
    CNode* found = nullptr;
    LOCK(cs_vNodes);
    for (auto&& pnode : vNodes) {
        if(pnode->GetId() == id) {
            found = pnode;
            break;
        }
    }
    return found != nullptr && NodeFullyConnected(found) && func(found);
}

int64_t PoissonNextSend(int64_t nNow, int average_interval_seconds) {
    return nNow + (int64_t)(log1p(GetRand(1ULL << 48) * -0.0000000000000035527136788 /* -1/2^48 */) * average_interval_seconds * -1000000.0 + 0.5);
}

CSipHasher CConnman::GetDeterministicRandomizer(uint64_t id) const
{
    return CSipHasher(nSeed0, nSeed1).Write(id);
}

uint64_t CConnman::CalculateKeyedNetGroup(const CAddress& ad) const
{
    std::vector<unsigned char> vchNetGroup(ad.GetGroup());

    return GetDeterministicRandomizer(RANDOMIZER_ID_NETGROUP).Write(vchNetGroup.data(), vchNetGroup.size()).Finalize();
=======






void RelayTransaction(const CTransaction& tx, const uint256& hash)
{
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss.reserve(10000);
    ss << tx;
    RelayTransaction(tx, hash, ss);
}

void RelayTransaction(const CTransaction& tx, const uint256& hash, const CDataStream& ss)
{
    CInv inv(MSG_TX, hash);
    {
        LOCK(cs_mapRelay);
        // Expire old relay messages
        while (!vRelayExpiration.empty() && vRelayExpiration.front().first < GetTime())
        {
            mapRelay.erase(vRelayExpiration.front().second);
            vRelayExpiration.pop_front();
        }

        // Save original serialized message so newer versions are preserved
        mapRelay.insert(std::make_pair(inv, ss));
        vRelayExpiration.push_back(std::make_pair(GetTime() + 15 * 60, inv));
    }
    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
    {
        if(!pnode->fRelayTxes)
            continue;
        LOCK(pnode->cs_filter);
        if (pnode->pfilter)
        {
            if (pnode->pfilter->IsRelevantAndUpdate(tx, hash))
                pnode->PushInventory(inv);
        } else
            pnode->PushInventory(inv);
    }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
