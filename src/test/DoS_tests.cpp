<<<<<<< HEAD
// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// Unit tests for denial-of-service detection/prevention code

#include "chainparams.h"
#include "keystore.h"
#include "net.h"
#include "net_processing.h"
#include "pow.h"
#include "script/sign.h"
#include "serialize.h"
#include "util.h"
#include "validation.h"

#include "test/test_wificoin.h"

#include <stdint.h>

#include <boost/test/unit_test.hpp>

// Tests these internal-to-net_processing.cpp methods:
extern bool AddOrphanTx(const CTransactionRef& tx, NodeId peer);
extern void EraseOrphansFor(NodeId peer);
extern unsigned int LimitOrphanTxSize(unsigned int nMaxOrphans);
struct COrphanTx {
    CTransactionRef tx;
    NodeId fromPeer;
    int64_t nTimeExpire;
};
extern std::map<uint256, COrphanTx> mapOrphanTransactions;
=======
//
// Unit tests for denial-of-service detection/prevention code
//
#include <algorithm>

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "main.h"
#include "wallet.h"
#include "net.h"
#include "util.h"

#include <stdint.h>

// Tests this internal-to-main.cpp method:
extern bool AddOrphanTx(const CTransaction& tx);
extern unsigned int LimitOrphanTxSize(unsigned int nMaxOrphans);
extern std::map<uint256, CTransaction> mapOrphanTransactions;
extern std::map<uint256, std::set<uint256> > mapOrphanTransactionsByPrev;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

CService ip(uint32_t i)
{
    struct in_addr s;
    s.s_addr = i;
<<<<<<< HEAD
    return CService(CNetAddr(s), Params().GetDefaultPort());
}

static NodeId id = 0;

void UpdateLastBlockAnnounceTime(NodeId node, int64_t time_in_seconds);

BOOST_FIXTURE_TEST_SUITE(DoS_tests, TestingSetup)

// Test eviction of an outbound peer whose chain never advances
// Mock a node connection, and use mocktime to simulate a peer
// which never sends any headers messages.  PeerLogic should
// decide to evict that outbound peer, after the appropriate timeouts.
// Note that we protect 4 outbound nodes from being subject to
// this logic; this test takes advantage of that protection only
// being applied to nodes which send headers with sufficient
// work.
BOOST_AUTO_TEST_CASE(outbound_slow_chain_eviction)
{
    std::atomic<bool> interruptDummy(false);

    // Mock an outbound peer
    CAddress addr1(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode1(id++, ServiceFlags(NODE_NETWORK|NODE_WITNESS), 0, INVALID_SOCKET, addr1, 0, 0, CAddress(), "", /*fInboundIn=*/ false);
    dummyNode1.SetSendVersion(PROTOCOL_VERSION);

    peerLogic->InitializeNode(&dummyNode1);
    dummyNode1.nVersion = 1;
    dummyNode1.fSuccessfullyConnected = true;

    // This test requires that we have a chain with non-zero work.
    BOOST_CHECK(chainActive.Tip() != nullptr);
    BOOST_CHECK(chainActive.Tip()->nChainWork > 0);

    // Test starts here
    peerLogic->SendMessages(&dummyNode1, interruptDummy); // should result in getheaders
    BOOST_CHECK(dummyNode1.vSendMsg.size() > 0);
    dummyNode1.vSendMsg.clear();

    int64_t nStartTime = GetTime();
    // Wait 21 minutes
    SetMockTime(nStartTime+21*60);
    peerLogic->SendMessages(&dummyNode1, interruptDummy); // should result in getheaders
    BOOST_CHECK(dummyNode1.vSendMsg.size() > 0);
    // Wait 3 more minutes
    SetMockTime(nStartTime+24*60);
    peerLogic->SendMessages(&dummyNode1, interruptDummy); // should result in disconnect
    BOOST_CHECK(dummyNode1.fDisconnect == true);
    SetMockTime(0);

    bool dummy;
    peerLogic->FinalizeNode(dummyNode1.GetId(), dummy);
}

void AddRandomOutboundPeer(std::vector<CNode *> &vNodes, PeerLogicValidation &peerLogic)
{
    CAddress addr(ip(GetRandInt(0xffffffff)), NODE_NONE);
    vNodes.emplace_back(new CNode(id++, ServiceFlags(NODE_NETWORK|NODE_WITNESS), 0, INVALID_SOCKET, addr, 0, 0, CAddress(), "", /*fInboundIn=*/ false));
    CNode &node = *vNodes.back();
    node.SetSendVersion(PROTOCOL_VERSION);

    peerLogic.InitializeNode(&node);
    node.nVersion = 1;
    node.fSuccessfullyConnected = true;

    CConnmanTest::AddNode(node);
}

BOOST_AUTO_TEST_CASE(stale_tip_peer_management)
{
    const Consensus::Params& consensusParams = Params().GetConsensus();
    constexpr int nMaxOutbound = 8;
    CConnman::Options options;
    options.nMaxConnections = 125;
    options.nMaxOutbound = nMaxOutbound;
    options.nMaxFeeler = 1;

    connman->Init(options);
    std::vector<CNode *> vNodes;

    // Mock some outbound peers
    for (int i=0; i<nMaxOutbound; ++i) {
        AddRandomOutboundPeer(vNodes, *peerLogic);
    }

    peerLogic->CheckForStaleTipAndEvictPeers(consensusParams);

    // No nodes should be marked for disconnection while we have no extra peers
    for (const CNode *node : vNodes) {
        BOOST_CHECK(node->fDisconnect == false);
    }

    SetMockTime(GetTime() + 3*consensusParams.nPowTargetSpacing + 1);

    // Now tip should definitely be stale, and we should look for an extra
    // outbound peer
    peerLogic->CheckForStaleTipAndEvictPeers(consensusParams);
    BOOST_CHECK(connman->GetTryNewOutboundPeer());

    // Still no peers should be marked for disconnection
    for (const CNode *node : vNodes) {
        BOOST_CHECK(node->fDisconnect == false);
    }

    // If we add one more peer, something should get marked for eviction
    // on the next check (since we're mocking the time to be in the future, the
    // required time connected check should be satisfied).
    AddRandomOutboundPeer(vNodes, *peerLogic);

    peerLogic->CheckForStaleTipAndEvictPeers(consensusParams);
    for (int i=0; i<nMaxOutbound; ++i) {
        BOOST_CHECK(vNodes[i]->fDisconnect == false);
    }
    // Last added node should get marked for eviction
    BOOST_CHECK(vNodes.back()->fDisconnect == true);

    vNodes.back()->fDisconnect = false;

    // Update the last announced block time for the last
    // peer, and check that the next newest node gets evicted.
    UpdateLastBlockAnnounceTime(vNodes.back()->GetId(), GetTime());

    peerLogic->CheckForStaleTipAndEvictPeers(consensusParams);
    for (int i=0; i<nMaxOutbound-1; ++i) {
        BOOST_CHECK(vNodes[i]->fDisconnect == false);
    }
    BOOST_CHECK(vNodes[nMaxOutbound-1]->fDisconnect == true);
    BOOST_CHECK(vNodes.back()->fDisconnect == false);

    bool dummy;
    for (const CNode *node : vNodes) {
        peerLogic->FinalizeNode(node->GetId(), dummy);
    }

    CConnmanTest::ClearNodes();
}

BOOST_AUTO_TEST_CASE(DoS_banning)
{
    std::atomic<bool> interruptDummy(false);

    connman->ClearBanned();
    CAddress addr1(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode1(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr1, 0, 0, CAddress(), "", true);
    dummyNode1.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode1);
    dummyNode1.nVersion = 1;
    dummyNode1.fSuccessfullyConnected = true;
    Misbehaving(dummyNode1.GetId(), 100); // Should get banned
    peerLogic->SendMessages(&dummyNode1, interruptDummy);
    BOOST_CHECK(connman->IsBanned(addr1));
    BOOST_CHECK(!connman->IsBanned(ip(0xa0b0c001|0x0000ff00))); // Different IP, not banned

    CAddress addr2(ip(0xa0b0c002), NODE_NONE);
    CNode dummyNode2(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr2, 1, 1, CAddress(), "", true);
    dummyNode2.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode2);
    dummyNode2.nVersion = 1;
    dummyNode2.fSuccessfullyConnected = true;
    Misbehaving(dummyNode2.GetId(), 50);
    peerLogic->SendMessages(&dummyNode2, interruptDummy);
    BOOST_CHECK(!connman->IsBanned(addr2)); // 2 not banned yet...
    BOOST_CHECK(connman->IsBanned(addr1));  // ... but 1 still should be
    Misbehaving(dummyNode2.GetId(), 50);
    peerLogic->SendMessages(&dummyNode2, interruptDummy);
    BOOST_CHECK(connman->IsBanned(addr2));

    bool dummy;
    peerLogic->FinalizeNode(dummyNode1.GetId(), dummy);
    peerLogic->FinalizeNode(dummyNode2.GetId(), dummy);
}

BOOST_AUTO_TEST_CASE(DoS_banscore)
{
    std::atomic<bool> interruptDummy(false);

    connman->ClearBanned();
    gArgs.ForceSetArg("-banscore", "111"); // because 11 is my favorite number
    CAddress addr1(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode1(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr1, 3, 1, CAddress(), "", true);
    dummyNode1.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode1);
    dummyNode1.nVersion = 1;
    dummyNode1.fSuccessfullyConnected = true;
    Misbehaving(dummyNode1.GetId(), 100);
    peerLogic->SendMessages(&dummyNode1, interruptDummy);
    BOOST_CHECK(!connman->IsBanned(addr1));
    Misbehaving(dummyNode1.GetId(), 10);
    peerLogic->SendMessages(&dummyNode1, interruptDummy);
    BOOST_CHECK(!connman->IsBanned(addr1));
    Misbehaving(dummyNode1.GetId(), 1);
    peerLogic->SendMessages(&dummyNode1, interruptDummy);
    BOOST_CHECK(connman->IsBanned(addr1));
    gArgs.ForceSetArg("-banscore", std::to_string(DEFAULT_BANSCORE_THRESHOLD));

    bool dummy;
    peerLogic->FinalizeNode(dummyNode1.GetId(), dummy);
}

BOOST_AUTO_TEST_CASE(DoS_bantime)
{
    std::atomic<bool> interruptDummy(false);

    connman->ClearBanned();
    int64_t nStartTime = GetTime();
    SetMockTime(nStartTime); // Overrides future calls to GetTime()

    CAddress addr(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr, 4, 4, CAddress(), "", true);
    dummyNode.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode);
    dummyNode.nVersion = 1;
    dummyNode.fSuccessfullyConnected = true;

    Misbehaving(dummyNode.GetId(), 100);
    peerLogic->SendMessages(&dummyNode, interruptDummy);
    BOOST_CHECK(connman->IsBanned(addr));

    SetMockTime(nStartTime+60*60);
    BOOST_CHECK(connman->IsBanned(addr));

    SetMockTime(nStartTime+60*60*24+1);
    BOOST_CHECK(!connman->IsBanned(addr));

    bool dummy;
    peerLogic->FinalizeNode(dummyNode.GetId(), dummy);
}

CTransactionRef RandomOrphan()
{
    std::map<uint256, COrphanTx>::iterator it;
    it = mapOrphanTransactions.lower_bound(InsecureRand256());
    if (it == mapOrphanTransactions.end())
        it = mapOrphanTransactions.begin();
    return it->second.tx;
=======
    return CService(CNetAddr(s), GetDefaultPort());
}

BOOST_AUTO_TEST_SUITE(DoS_tests)

BOOST_AUTO_TEST_CASE(DoS_banning)
{
    CNode::ClearBanned();
    CAddress addr1(ip(0xa0b0c001));
    CNode dummyNode1(INVALID_SOCKET, addr1, "", true);
    dummyNode1.Misbehaving(100); // Should get banned
    BOOST_CHECK(CNode::IsBanned(addr1));
    BOOST_CHECK(!CNode::IsBanned(ip(0xa0b0c001|0x0000ff00))); // Different IP, not banned

    CAddress addr2(ip(0xa0b0c002));
    CNode dummyNode2(INVALID_SOCKET, addr2, "", true);
    dummyNode2.Misbehaving(50);
    BOOST_CHECK(!CNode::IsBanned(addr2)); // 2 not banned yet...
    BOOST_CHECK(CNode::IsBanned(addr1));  // ... but 1 still should be
    dummyNode2.Misbehaving(50);
    BOOST_CHECK(CNode::IsBanned(addr2));
}

BOOST_AUTO_TEST_CASE(DoS_banscore)
{
    CNode::ClearBanned();
    mapArgs["-banscore"] = "111"; // because 11 is my favorite number
    CAddress addr1(ip(0xa0b0c001));
    CNode dummyNode1(INVALID_SOCKET, addr1, "", true);
    dummyNode1.Misbehaving(100);
    BOOST_CHECK(!CNode::IsBanned(addr1));
    dummyNode1.Misbehaving(10);
    BOOST_CHECK(!CNode::IsBanned(addr1));
    dummyNode1.Misbehaving(1);
    BOOST_CHECK(CNode::IsBanned(addr1));
    mapArgs.erase("-banscore");
}

BOOST_AUTO_TEST_CASE(DoS_bantime)
{
    CNode::ClearBanned();
    int64 nStartTime = GetTime();
    SetMockTime(nStartTime); // Overrides future calls to GetTime()

    CAddress addr(ip(0xa0b0c001));
    CNode dummyNode(INVALID_SOCKET, addr, "", true);

    dummyNode.Misbehaving(100);
    BOOST_CHECK(CNode::IsBanned(addr));

    SetMockTime(nStartTime+60*60);
    BOOST_CHECK(CNode::IsBanned(addr));

    SetMockTime(nStartTime+60*60*24+1);
    BOOST_CHECK(!CNode::IsBanned(addr));
}

static bool CheckNBits(unsigned int nbits1, int64 time1, unsigned int nbits2, int64 time2)\
{
    if (time1 > time2)
        return CheckNBits(nbits2, time2, nbits1, time1);
    int64 deltaTime = time2-time1;

    CBigNum required;
    required.SetCompact(ComputeMinWork(nbits1, deltaTime));
    CBigNum have;
    have.SetCompact(nbits2);
    return (have <= required);
}

BOOST_AUTO_TEST_CASE(DoS_checknbits)
{
    using namespace boost::assign; // for 'map_list_of()'

    // Timestamps,nBits from the bitcoin block chain.
    // These are the block-chain checkpoint blocks
    typedef std::map<int64, unsigned int> BlockData;
    BlockData chainData =
        map_list_of(1239852051,486604799)(1262749024,486594666)
        (1279305360,469854461)(1280200847,469830746)(1281678674,469809688)
        (1296207707,453179945)(1302624061,453036989)(1309640330,437004818)
        (1313172719,436789733);

    // Make sure CheckNBits considers every combination of block-chain-lock-in-points
    // "sane":
    BOOST_FOREACH(const BlockData::value_type& i, chainData)
    {
        BOOST_FOREACH(const BlockData::value_type& j, chainData)
        {
            BOOST_CHECK(CheckNBits(i.second, i.first, j.second, j.first));
        }
    }

    // Test a couple of insane combinations:
    BlockData::value_type firstcheck = *(chainData.begin());
    BlockData::value_type lastcheck = *(chainData.rbegin());

    // First checkpoint difficulty at or a while after the last checkpoint time should fail when
    // compared to last checkpoint
    BOOST_CHECK(!CheckNBits(firstcheck.second, lastcheck.first+60*10, lastcheck.second, lastcheck.first));
    BOOST_CHECK(!CheckNBits(firstcheck.second, lastcheck.first+60*60*24*14, lastcheck.second, lastcheck.first));

    // ... but OK if enough time passed for difficulty to adjust downward:
    BOOST_CHECK(CheckNBits(firstcheck.second, lastcheck.first+60*60*24*365*4, lastcheck.second, lastcheck.first));
}

CTransaction RandomOrphan()
{
    std::map<uint256, CTransaction>::iterator it;
    it = mapOrphanTransactions.lower_bound(GetRandHash());
    if (it == mapOrphanTransactions.end())
        it = mapOrphanTransactions.begin();
    return it->second;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_CASE(DoS_mapOrphans)
{
    CKey key;
    key.MakeNewKey(true);
    CBasicKeyStore keystore;
    keystore.AddKey(key);

    // 50 orphan transactions:
    for (int i = 0; i < 50; i++)
    {
<<<<<<< HEAD
        CMutableTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = InsecureRand256();
        tx.vin[0].scriptSig << OP_1;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());

        AddOrphanTx(MakeTransactionRef(tx), i);
=======
        CTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = GetRandHash();
        tx.vin[0].scriptSig << OP_1;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey.SetDestination(key.GetPubKey().GetID());

        AddOrphanTx(tx);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }

    // ... and 50 that depend on other orphans:
    for (int i = 0; i < 50; i++)
    {
<<<<<<< HEAD
        CTransactionRef txPrev = RandomOrphan();

        CMutableTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = txPrev->GetHash();
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());
        SignSignature(keystore, *txPrev, tx, 0, SIGHASH_ALL);

        AddOrphanTx(MakeTransactionRef(tx), i);
=======
        CTransaction txPrev = RandomOrphan();

        CTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = txPrev.GetHash();
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey.SetDestination(key.GetPubKey().GetID());
        SignSignature(keystore, txPrev, tx, 0);

        AddOrphanTx(tx);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }

    // This really-big orphan should be ignored:
    for (int i = 0; i < 10; i++)
    {
<<<<<<< HEAD
        CTransactionRef txPrev = RandomOrphan();

        CMutableTransaction tx;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());
        tx.vin.resize(2777);
        for (unsigned int j = 0; j < tx.vin.size(); j++)
        {
            tx.vin[j].prevout.n = j;
            tx.vin[j].prevout.hash = txPrev->GetHash();
        }
        SignSignature(keystore, *txPrev, tx, 0, SIGHASH_ALL);
=======
        CTransaction txPrev = RandomOrphan();

        CTransaction tx;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey.SetDestination(key.GetPubKey().GetID());
        tx.vin.resize(500);
        for (unsigned int j = 0; j < tx.vin.size(); j++)
        {
            tx.vin[j].prevout.n = j;
            tx.vin[j].prevout.hash = txPrev.GetHash();
        }
        SignSignature(keystore, txPrev, tx, 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        // Re-use same signature for other inputs
        // (they don't have to be valid for this test)
        for (unsigned int j = 1; j < tx.vin.size(); j++)
            tx.vin[j].scriptSig = tx.vin[0].scriptSig;

<<<<<<< HEAD
        BOOST_CHECK(!AddOrphanTx(MakeTransactionRef(tx), i));
    }

    // Test EraseOrphansFor:
    for (NodeId i = 0; i < 3; i++)
    {
        size_t sizeBefore = mapOrphanTransactions.size();
        EraseOrphansFor(i);
        BOOST_CHECK(mapOrphanTransactions.size() < sizeBefore);
=======
        BOOST_CHECK(!AddOrphanTx(tx));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }

    // Test LimitOrphanTxSize() function:
    LimitOrphanTxSize(40);
    BOOST_CHECK(mapOrphanTransactions.size() <= 40);
    LimitOrphanTxSize(10);
    BOOST_CHECK(mapOrphanTransactions.size() <= 10);
    LimitOrphanTxSize(0);
    BOOST_CHECK(mapOrphanTransactions.empty());
<<<<<<< HEAD
=======
    BOOST_CHECK(mapOrphanTransactionsByPrev.empty());
}

BOOST_AUTO_TEST_CASE(DoS_checkSig)
{
    // Test signature caching code (see key.cpp Verify() methods)

    CKey key;
    key.MakeNewKey(true);
    CBasicKeyStore keystore;
    keystore.AddKey(key);
    unsigned int flags = SCRIPT_VERIFY_P2SH | SCRIPT_VERIFY_STRICTENC;

    // 100 orphan transactions:
    static const int NPREV=100;
    CTransaction orphans[NPREV];
    for (int i = 0; i < NPREV; i++)
    {
        CTransaction& tx = orphans[i];
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
        tx.vin[0].prevout.hash = GetRandHash();
        tx.vin[0].scriptSig << OP_1;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey.SetDestination(key.GetPubKey().GetID());

        AddOrphanTx(tx);
    }

    // Create a transaction that depends on orphans:
    CTransaction tx;
    tx.vout.resize(1);
    tx.vout[0].nValue = 1*CENT;
    tx.vout[0].scriptPubKey.SetDestination(key.GetPubKey().GetID());
    tx.vin.resize(NPREV);
    for (unsigned int j = 0; j < tx.vin.size(); j++)
    {
        tx.vin[j].prevout.n = 0;
        tx.vin[j].prevout.hash = orphans[j].GetHash();
    }
    // Creating signatures primes the cache:
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    for (unsigned int j = 0; j < tx.vin.size(); j++)
        BOOST_CHECK(SignSignature(keystore, orphans[j], tx, j));
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration msdiff = mst2 - mst1;
    long nOneValidate = msdiff.total_milliseconds();
    if (fDebug) printf("DoS_Checksig sign: %ld\n", nOneValidate);

    // ... now validating repeatedly should be quick:
    // 2.8GHz machine, -g build: Sign takes ~760ms,
    // uncached Verify takes ~250ms, cached Verify takes ~50ms
    // (for 100 single-signature inputs)
    mst1 = boost::posix_time::microsec_clock::local_time();
    for (unsigned int i = 0; i < 5; i++)
        for (unsigned int j = 0; j < tx.vin.size(); j++)
            BOOST_CHECK(VerifySignature(CCoins(orphans[j], MEMPOOL_HEIGHT), tx, j, flags, SIGHASH_ALL));
    mst2 = boost::posix_time::microsec_clock::local_time();
    msdiff = mst2 - mst1;
    long nManyValidate = msdiff.total_milliseconds();
    if (fDebug) printf("DoS_Checksig five: %ld\n", nManyValidate);

    BOOST_CHECK_MESSAGE(nManyValidate < nOneValidate, "Signature cache timing failed");

    // Empty a signature, validation should fail:
    CScript save = tx.vin[0].scriptSig;
    tx.vin[0].scriptSig = CScript();
    BOOST_CHECK(!VerifySignature(CCoins(orphans[0], MEMPOOL_HEIGHT), tx, 0, flags, SIGHASH_ALL));
    tx.vin[0].scriptSig = save;

    // Swap signatures, validation should fail:
    std::swap(tx.vin[0].scriptSig, tx.vin[1].scriptSig);
    BOOST_CHECK(!VerifySignature(CCoins(orphans[0], MEMPOOL_HEIGHT), tx, 0, flags, SIGHASH_ALL));
    BOOST_CHECK(!VerifySignature(CCoins(orphans[1], MEMPOOL_HEIGHT), tx, 1, flags, SIGHASH_ALL));
    std::swap(tx.vin[0].scriptSig, tx.vin[1].scriptSig);

    // Exercise -maxsigcachesize code:
    mapArgs["-maxsigcachesize"] = "10";
    // Generate a new, different signature for vin[0] to trigger cache clear:
    CScript oldSig = tx.vin[0].scriptSig;
    BOOST_CHECK(SignSignature(keystore, orphans[0], tx, 0));
    BOOST_CHECK(tx.vin[0].scriptSig != oldSig);
    for (unsigned int j = 0; j < tx.vin.size(); j++)
        BOOST_CHECK(VerifySignature(CCoins(orphans[j], MEMPOOL_HEIGHT), tx, j, flags, SIGHASH_ALL));
    mapArgs.erase("-maxsigcachesize");

    LimitOrphanTxSize(0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_SUITE_END()
