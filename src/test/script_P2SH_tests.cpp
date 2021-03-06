<<<<<<< HEAD
// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "consensus/tx_verify.h"
#include "core_io.h"
#include "key.h"
#include "keystore.h"
#include "validation.h"
#include "policy/policy.h"
#include "script/script.h"
#include "script/script_error.h"
#include "script/sign.h"
#include "script/ismine.h"
#include "test/test_wificoin.h"

#include <vector>

#include <boost/test/unit_test.hpp>
=======
#include <boost/assert.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "../main.h"
#include "../script.h"
#include "../wallet.h"

using namespace std;

// Test routines internal to script.cpp:
extern uint256 SignatureHash(CScript scriptCode, const CTransaction& txTo, unsigned int nIn, int nHashType);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

// Helpers:
static std::vector<unsigned char>
Serialize(const CScript& s)
{
<<<<<<< HEAD
    std::vector<unsigned char> sSerialized(s.begin(), s.end());
=======
    std::vector<unsigned char> sSerialized(s);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    return sSerialized;
}

static bool
<<<<<<< HEAD
Verify(const CScript& scriptSig, const CScript& scriptPubKey, bool fStrict, ScriptError& err)
{
    // Create dummy to/from transactions:
    CMutableTransaction txFrom;
    txFrom.vout.resize(1);
    txFrom.vout[0].scriptPubKey = scriptPubKey;

    CMutableTransaction txTo;
=======
Verify(const CScript& scriptSig, const CScript& scriptPubKey, bool fStrict)
{
    // Create dummy to/from transactions:
    CTransaction txFrom;
    txFrom.vout.resize(1);
    txFrom.vout[0].scriptPubKey = scriptPubKey;

    CTransaction txTo;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    txTo.vin.resize(1);
    txTo.vout.resize(1);
    txTo.vin[0].prevout.n = 0;
    txTo.vin[0].prevout.hash = txFrom.GetHash();
    txTo.vin[0].scriptSig = scriptSig;
    txTo.vout[0].nValue = 1;

<<<<<<< HEAD
    return VerifyScript(scriptSig, scriptPubKey, nullptr, fStrict ? SCRIPT_VERIFY_P2SH : SCRIPT_VERIFY_NONE, MutableTransactionSignatureChecker(&txTo, 0, txFrom.vout[0].nValue), &err);
}


BOOST_FIXTURE_TEST_SUITE(script_P2SH_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(sign)
{
    LOCK(cs_main);
=======
    return VerifyScript(scriptSig, scriptPubKey, txTo, 0, fStrict ? SCRIPT_VERIFY_P2SH : SCRIPT_VERIFY_NONE, 0);
}


BOOST_AUTO_TEST_SUITE(script_P2SH_tests)

BOOST_AUTO_TEST_CASE(sign)
{
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    // Pay-to-script-hash looks like this:
    // scriptSig:    <sig> <sig...> <serialized_script>
    // scriptPubKey: HASH160 <hash> EQUAL

    // Test SignSignature() (and therefore the version of Solver() that signs transactions)
    CBasicKeyStore keystore;
    CKey key[4];
    for (int i = 0; i < 4; i++)
    {
        key[i].MakeNewKey(true);
        keystore.AddKey(key[i]);
    }

    // 8 Scripts: checking all combinations of
    // different keys, straight/P2SH, pubkey/pubkeyhash
    CScript standardScripts[4];
<<<<<<< HEAD
    standardScripts[0] << ToByteVector(key[0].GetPubKey()) << OP_CHECKSIG;
    standardScripts[1] = GetScriptForDestination(key[1].GetPubKey().GetID());
    standardScripts[2] << ToByteVector(key[1].GetPubKey()) << OP_CHECKSIG;
    standardScripts[3] = GetScriptForDestination(key[2].GetPubKey().GetID());
=======
    standardScripts[0] << key[0].GetPubKey() << OP_CHECKSIG;
    standardScripts[1].SetDestination(key[1].GetPubKey().GetID());
    standardScripts[2] << key[1].GetPubKey() << OP_CHECKSIG;
    standardScripts[3].SetDestination(key[2].GetPubKey().GetID());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    CScript evalScripts[4];
    for (int i = 0; i < 4; i++)
    {
        keystore.AddCScript(standardScripts[i]);
<<<<<<< HEAD
        evalScripts[i] = GetScriptForDestination(CScriptID(standardScripts[i]));
    }

    CMutableTransaction txFrom;  // Funding transaction:
    std::string reason;
=======
        evalScripts[i].SetDestination(standardScripts[i].GetID());
    }

    CTransaction txFrom;  // Funding transaction:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    txFrom.vout.resize(8);
    for (int i = 0; i < 4; i++)
    {
        txFrom.vout[i].scriptPubKey = evalScripts[i];
        txFrom.vout[i].nValue = COIN;
        txFrom.vout[i+4].scriptPubKey = standardScripts[i];
        txFrom.vout[i+4].nValue = COIN;
    }
<<<<<<< HEAD
    BOOST_CHECK(IsStandardTx(txFrom, reason));

    CMutableTransaction txTo[8]; // Spending transactions
=======
    BOOST_CHECK(txFrom.IsStandard());

    CTransaction txTo[8]; // Spending transactions
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    for (int i = 0; i < 8; i++)
    {
        txTo[i].vin.resize(1);
        txTo[i].vout.resize(1);
        txTo[i].vin[0].prevout.n = i;
        txTo[i].vin[0].prevout.hash = txFrom.GetHash();
        txTo[i].vout[0].nValue = 1;
        BOOST_CHECK_MESSAGE(IsMine(keystore, txFrom.vout[i].scriptPubKey), strprintf("IsMine %d", i));
    }
    for (int i = 0; i < 8; i++)
    {
<<<<<<< HEAD
        BOOST_CHECK_MESSAGE(SignSignature(keystore, txFrom, txTo[i], 0, SIGHASH_ALL), strprintf("SignSignature %d", i));
    }
    // All of the above should be OK, and the txTos have valid signatures
    // Check to make sure signature verification fails if we use the wrong ScriptSig:
    for (int i = 0; i < 8; i++) {
        PrecomputedTransactionData txdata(txTo[i]);
=======
        BOOST_CHECK_MESSAGE(SignSignature(keystore, txFrom, txTo[i], 0), strprintf("SignSignature %d", i));
    }
    // All of the above should be OK, and the txTos have valid signatures
    // Check to make sure signature verification fails if we use the wrong ScriptSig:
    for (int i = 0; i < 8; i++)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        for (int j = 0; j < 8; j++)
        {
            CScript sigSave = txTo[i].vin[0].scriptSig;
            txTo[i].vin[0].scriptSig = txTo[j].vin[0].scriptSig;
<<<<<<< HEAD
            const CTxOut& output = txFrom.vout[txTo[i].vin[0].prevout.n];
            bool sigOK = CScriptCheck(output.scriptPubKey, output.nValue, txTo[i], 0, SCRIPT_VERIFY_P2SH | SCRIPT_VERIFY_STRICTENC, false, &txdata)();
=======
            bool sigOK = VerifySignature(CCoins(txFrom, 0), txTo[i], 0, SCRIPT_VERIFY_P2SH | SCRIPT_VERIFY_STRICTENC, 0);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            if (i == j)
                BOOST_CHECK_MESSAGE(sigOK, strprintf("VerifySignature %d %d", i, j));
            else
                BOOST_CHECK_MESSAGE(!sigOK, strprintf("VerifySignature %d %d", i, j));
            txTo[i].vin[0].scriptSig = sigSave;
        }
<<<<<<< HEAD
    }
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_CASE(norecurse)
{
<<<<<<< HEAD
    ScriptError err;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    // Make sure only the outer pay-to-script-hash does the
    // extra-validation thing:
    CScript invalidAsScript;
    invalidAsScript << OP_INVALIDOPCODE << OP_INVALIDOPCODE;

<<<<<<< HEAD
    CScript p2sh = GetScriptForDestination(CScriptID(invalidAsScript));
=======
    CScript p2sh;
    p2sh.SetDestination(invalidAsScript.GetID());
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    CScript scriptSig;
    scriptSig << Serialize(invalidAsScript);

    // Should not verify, because it will try to execute OP_INVALIDOPCODE
<<<<<<< HEAD
    BOOST_CHECK(!Verify(scriptSig, p2sh, true, err));
    BOOST_CHECK_MESSAGE(err == SCRIPT_ERR_BAD_OPCODE, ScriptErrorString(err));

    // Try to recur, and verification should succeed because
    // the inner HASH160 <> EQUAL should only check the hash:
    CScript p2sh2 = GetScriptForDestination(CScriptID(p2sh));
    CScript scriptSig2;
    scriptSig2 << Serialize(invalidAsScript) << Serialize(p2sh);

    BOOST_CHECK(Verify(scriptSig2, p2sh2, true, err));
    BOOST_CHECK_MESSAGE(err == SCRIPT_ERR_OK, ScriptErrorString(err));
=======
    BOOST_CHECK(!Verify(scriptSig, p2sh, true));

    // Try to recur, and verification should succeed because
    // the inner HASH160 <> EQUAL should only check the hash:
    CScript p2sh2;
    p2sh2.SetDestination(p2sh.GetID());
    CScript scriptSig2;
    scriptSig2 << Serialize(invalidAsScript) << Serialize(p2sh);

    BOOST_CHECK(Verify(scriptSig2, p2sh2, true));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_CASE(set)
{
<<<<<<< HEAD
    LOCK(cs_main);
    // Test the CScript::Set* methods
    CBasicKeyStore keystore;
    CKey key[4];
    std::vector<CPubKey> keys;
=======
    // Test the CScript::Set* methods
    CBasicKeyStore keystore;
    CKey key[4];
    std::vector<CKey> keys;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    for (int i = 0; i < 4; i++)
    {
        key[i].MakeNewKey(true);
        keystore.AddKey(key[i]);
<<<<<<< HEAD
        keys.push_back(key[i].GetPubKey());
    }

    CScript inner[4];
    inner[0] = GetScriptForDestination(key[0].GetPubKey().GetID());
    inner[1] = GetScriptForMultisig(2, std::vector<CPubKey>(keys.begin(), keys.begin()+2));
    inner[2] = GetScriptForMultisig(1, std::vector<CPubKey>(keys.begin(), keys.begin()+2));
    inner[3] = GetScriptForMultisig(2, std::vector<CPubKey>(keys.begin(), keys.begin()+3));
=======
        keys.push_back(key[i]);
    }

    CScript inner[4];
    inner[0].SetDestination(key[0].GetPubKey().GetID());
    inner[1].SetMultisig(2, std::vector<CKey>(keys.begin(), keys.begin()+2));
    inner[2].SetMultisig(1, std::vector<CKey>(keys.begin(), keys.begin()+2));
    inner[3].SetMultisig(2, std::vector<CKey>(keys.begin(), keys.begin()+3));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    CScript outer[4];
    for (int i = 0; i < 4; i++)
    {
<<<<<<< HEAD
        outer[i] = GetScriptForDestination(CScriptID(inner[i]));
        keystore.AddCScript(inner[i]);
    }

    CMutableTransaction txFrom;  // Funding transaction:
    std::string reason;
=======
        outer[i].SetDestination(inner[i].GetID());
        keystore.AddCScript(inner[i]);
    }

    CTransaction txFrom;  // Funding transaction:
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    txFrom.vout.resize(4);
    for (int i = 0; i < 4; i++)
    {
        txFrom.vout[i].scriptPubKey = outer[i];
        txFrom.vout[i].nValue = CENT;
    }
<<<<<<< HEAD
    BOOST_CHECK(IsStandardTx(txFrom, reason));

    CMutableTransaction txTo[4]; // Spending transactions
=======
    BOOST_CHECK(txFrom.IsStandard());

    CTransaction txTo[4]; // Spending transactions
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    for (int i = 0; i < 4; i++)
    {
        txTo[i].vin.resize(1);
        txTo[i].vout.resize(1);
        txTo[i].vin[0].prevout.n = i;
        txTo[i].vin[0].prevout.hash = txFrom.GetHash();
        txTo[i].vout[0].nValue = 1*CENT;
        txTo[i].vout[0].scriptPubKey = inner[i];
        BOOST_CHECK_MESSAGE(IsMine(keystore, txFrom.vout[i].scriptPubKey), strprintf("IsMine %d", i));
    }
    for (int i = 0; i < 4; i++)
    {
<<<<<<< HEAD
        BOOST_CHECK_MESSAGE(SignSignature(keystore, txFrom, txTo[i], 0, SIGHASH_ALL), strprintf("SignSignature %d", i));
        BOOST_CHECK_MESSAGE(IsStandardTx(txTo[i], reason), strprintf("txTo[%d].IsStandard", i));
=======
        BOOST_CHECK_MESSAGE(SignSignature(keystore, txFrom, txTo[i], 0), strprintf("SignSignature %d", i));
        BOOST_CHECK_MESSAGE(txTo[i].IsStandard(), strprintf("txTo[%d].IsStandard", i));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
}

BOOST_AUTO_TEST_CASE(is)
{
    // Test CScript::IsPayToScriptHash()
    uint160 dummy;
    CScript p2sh;
<<<<<<< HEAD
    p2sh << OP_HASH160 << ToByteVector(dummy) << OP_EQUAL;
=======
    p2sh << OP_HASH160 << dummy << OP_EQUAL;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    BOOST_CHECK(p2sh.IsPayToScriptHash());

    // Not considered pay-to-script-hash if using one of the OP_PUSHDATA opcodes:
    static const unsigned char direct[] =    { OP_HASH160, 20, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, OP_EQUAL };
    BOOST_CHECK(CScript(direct, direct+sizeof(direct)).IsPayToScriptHash());
    static const unsigned char pushdata1[] = { OP_HASH160, OP_PUSHDATA1, 20, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, OP_EQUAL };
    BOOST_CHECK(!CScript(pushdata1, pushdata1+sizeof(pushdata1)).IsPayToScriptHash());
    static const unsigned char pushdata2[] = { OP_HASH160, OP_PUSHDATA2, 20,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, OP_EQUAL };
    BOOST_CHECK(!CScript(pushdata2, pushdata2+sizeof(pushdata2)).IsPayToScriptHash());
    static const unsigned char pushdata4[] = { OP_HASH160, OP_PUSHDATA4, 20,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, OP_EQUAL };
    BOOST_CHECK(!CScript(pushdata4, pushdata4+sizeof(pushdata4)).IsPayToScriptHash());

    CScript not_p2sh;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());

<<<<<<< HEAD
    not_p2sh.clear(); not_p2sh << OP_HASH160 << ToByteVector(dummy) << ToByteVector(dummy) << OP_EQUAL;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());

    not_p2sh.clear(); not_p2sh << OP_NOP << ToByteVector(dummy) << OP_EQUAL;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());

    not_p2sh.clear(); not_p2sh << OP_HASH160 << ToByteVector(dummy) << OP_CHECKSIG;
=======
    not_p2sh.clear(); not_p2sh << OP_HASH160 << dummy << dummy << OP_EQUAL;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());

    not_p2sh.clear(); not_p2sh << OP_NOP << dummy << OP_EQUAL;
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());

    not_p2sh.clear(); not_p2sh << OP_HASH160 << dummy << OP_CHECKSIG;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    BOOST_CHECK(!not_p2sh.IsPayToScriptHash());
}

BOOST_AUTO_TEST_CASE(switchover)
{
    // Test switch over code
    CScript notValid;
<<<<<<< HEAD
    ScriptError err;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    notValid << OP_11 << OP_12 << OP_EQUALVERIFY;
    CScript scriptSig;
    scriptSig << Serialize(notValid);

<<<<<<< HEAD
    CScript fund = GetScriptForDestination(CScriptID(notValid));


    // Validation should succeed under old rules (hash is correct):
    BOOST_CHECK(Verify(scriptSig, fund, false, err));
    BOOST_CHECK_MESSAGE(err == SCRIPT_ERR_OK, ScriptErrorString(err));
    // Fail under new:
    BOOST_CHECK(!Verify(scriptSig, fund, true, err));
    BOOST_CHECK_MESSAGE(err == SCRIPT_ERR_EQUALVERIFY, ScriptErrorString(err));
=======
    CScript fund;
    fund.SetDestination(notValid.GetID());


    // Validation should succeed under old rules (hash is correct):
    BOOST_CHECK(Verify(scriptSig, fund, false));
    // Fail under new:
    BOOST_CHECK(!Verify(scriptSig, fund, true));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_CASE(AreInputsStandard)
{
<<<<<<< HEAD
    LOCK(cs_main);
    CCoinsView coinsDummy;
    CCoinsViewCache coins(&coinsDummy);
    CBasicKeyStore keystore;
    CKey key[6];
    std::vector<CPubKey> keys;
    for (int i = 0; i < 6; i++)
    {
        key[i].MakeNewKey(true);
        keystore.AddKey(key[i]);
    }
    for (int i = 0; i < 3; i++)
        keys.push_back(key[i].GetPubKey());

    CMutableTransaction txFrom;
    txFrom.vout.resize(7);

    // First three are standard:
    CScript pay1 = GetScriptForDestination(key[0].GetPubKey().GetID());
    keystore.AddCScript(pay1);
    CScript pay1of3 = GetScriptForMultisig(1, keys);

    txFrom.vout[0].scriptPubKey = GetScriptForDestination(CScriptID(pay1)); // P2SH (OP_CHECKSIG)
    txFrom.vout[0].nValue = 1000;
    txFrom.vout[1].scriptPubKey = pay1; // ordinary OP_CHECKSIG
    txFrom.vout[1].nValue = 2000;
    txFrom.vout[2].scriptPubKey = pay1of3; // ordinary OP_CHECKMULTISIG
    txFrom.vout[2].nValue = 3000;

    // vout[3] is complicated 1-of-3 AND 2-of-3
    // ... that is OK if wrapped in P2SH:
    CScript oneAndTwo;
    oneAndTwo << OP_1 << ToByteVector(key[0].GetPubKey()) << ToByteVector(key[1].GetPubKey()) << ToByteVector(key[2].GetPubKey());
    oneAndTwo << OP_3 << OP_CHECKMULTISIGVERIFY;
    oneAndTwo << OP_2 << ToByteVector(key[3].GetPubKey()) << ToByteVector(key[4].GetPubKey()) << ToByteVector(key[5].GetPubKey());
    oneAndTwo << OP_3 << OP_CHECKMULTISIG;
    keystore.AddCScript(oneAndTwo);
    txFrom.vout[3].scriptPubKey = GetScriptForDestination(CScriptID(oneAndTwo));
    txFrom.vout[3].nValue = 4000;

    // vout[4] is max sigops:
    CScript fifteenSigops; fifteenSigops << OP_1;
    for (unsigned i = 0; i < MAX_P2SH_SIGOPS; i++)
        fifteenSigops << ToByteVector(key[i%3].GetPubKey());
    fifteenSigops << OP_15 << OP_CHECKMULTISIG;
    keystore.AddCScript(fifteenSigops);
    txFrom.vout[4].scriptPubKey = GetScriptForDestination(CScriptID(fifteenSigops));
    txFrom.vout[4].nValue = 5000;

    // vout[5/6] are non-standard because they exceed MAX_P2SH_SIGOPS
    CScript sixteenSigops; sixteenSigops << OP_16 << OP_CHECKMULTISIG;
    keystore.AddCScript(sixteenSigops);
    txFrom.vout[5].scriptPubKey = GetScriptForDestination(CScriptID(fifteenSigops));
    txFrom.vout[5].nValue = 5000;
    CScript twentySigops; twentySigops << OP_CHECKMULTISIG;
    keystore.AddCScript(twentySigops);
    txFrom.vout[6].scriptPubKey = GetScriptForDestination(CScriptID(twentySigops));
    txFrom.vout[6].nValue = 6000;

    AddCoins(coins, txFrom, 0);

    CMutableTransaction txTo;
    txTo.vout.resize(1);
    txTo.vout[0].scriptPubKey = GetScriptForDestination(key[1].GetPubKey().GetID());

    txTo.vin.resize(5);
    for (int i = 0; i < 5; i++)
    {
        txTo.vin[i].prevout.n = i;
        txTo.vin[i].prevout.hash = txFrom.GetHash();
    }
    BOOST_CHECK(SignSignature(keystore, txFrom, txTo, 0, SIGHASH_ALL));
    BOOST_CHECK(SignSignature(keystore, txFrom, txTo, 1, SIGHASH_ALL));
    BOOST_CHECK(SignSignature(keystore, txFrom, txTo, 2, SIGHASH_ALL));
    // SignSignature doesn't know how to sign these. We're
    // not testing validating signatures, so just create
    // dummy signatures that DO include the correct P2SH scripts:
    txTo.vin[3].scriptSig << OP_11 << OP_11 << std::vector<unsigned char>(oneAndTwo.begin(), oneAndTwo.end());
    txTo.vin[4].scriptSig << std::vector<unsigned char>(fifteenSigops.begin(), fifteenSigops.end());

    BOOST_CHECK(::AreInputsStandard(txTo, coins));
    // 22 P2SH sigops for all inputs (1 for vin[0], 6 for vin[3], 15 for vin[4]
    BOOST_CHECK_EQUAL(GetP2SHSigOpCount(txTo, coins), 22U);

    CMutableTransaction txToNonStd1;
    txToNonStd1.vout.resize(1);
    txToNonStd1.vout[0].scriptPubKey = GetScriptForDestination(key[1].GetPubKey().GetID());
    txToNonStd1.vout[0].nValue = 1000;
    txToNonStd1.vin.resize(1);
    txToNonStd1.vin[0].prevout.n = 5;
    txToNonStd1.vin[0].prevout.hash = txFrom.GetHash();
    txToNonStd1.vin[0].scriptSig << std::vector<unsigned char>(sixteenSigops.begin(), sixteenSigops.end());

    BOOST_CHECK(!::AreInputsStandard(txToNonStd1, coins));
    BOOST_CHECK_EQUAL(GetP2SHSigOpCount(txToNonStd1, coins), 16U);

    CMutableTransaction txToNonStd2;
    txToNonStd2.vout.resize(1);
    txToNonStd2.vout[0].scriptPubKey = GetScriptForDestination(key[1].GetPubKey().GetID());
    txToNonStd2.vout[0].nValue = 1000;
    txToNonStd2.vin.resize(1);
    txToNonStd2.vin[0].prevout.n = 6;
    txToNonStd2.vin[0].prevout.hash = txFrom.GetHash();
    txToNonStd2.vin[0].scriptSig << std::vector<unsigned char>(twentySigops.begin(), twentySigops.end());

    BOOST_CHECK(!::AreInputsStandard(txToNonStd2, coins));
    BOOST_CHECK_EQUAL(GetP2SHSigOpCount(txToNonStd2, coins), 20U);
=======
    CCoinsView coinsDummy;
    CCoinsViewCache coins(coinsDummy);
    CBasicKeyStore keystore;
    CKey key[3];
    vector<CKey> keys;
    for (int i = 0; i < 3; i++)
    {
        key[i].MakeNewKey(true);
        keystore.AddKey(key[i]);
        keys.push_back(key[i]);
    }

    CTransaction txFrom;
    txFrom.vout.resize(6);

    // First three are standard:
    CScript pay1; pay1.SetDestination(key[0].GetPubKey().GetID());
    keystore.AddCScript(pay1);
    CScript payScriptHash1; payScriptHash1.SetDestination(pay1.GetID());
    CScript pay1of3; pay1of3.SetMultisig(1, keys);

    txFrom.vout[0].scriptPubKey = payScriptHash1;
    txFrom.vout[0].nValue = 1000;
    txFrom.vout[1].scriptPubKey = pay1;
    txFrom.vout[1].nValue = 2000;
    txFrom.vout[2].scriptPubKey = pay1of3;
    txFrom.vout[2].nValue = 3000;

    // Last three non-standard:
    CScript empty;
    keystore.AddCScript(empty);
    txFrom.vout[3].scriptPubKey = empty;
    txFrom.vout[3].nValue = 4000;
    // Can't use SetPayToScriptHash, it checks for the empty Script. So:
    txFrom.vout[4].scriptPubKey << OP_HASH160 << Hash160(empty) << OP_EQUAL;
    txFrom.vout[4].nValue = 5000;
    CScript oneOfEleven;
    oneOfEleven << OP_1;
    for (int i = 0; i < 11; i++)
        oneOfEleven << key[0].GetPubKey();
    oneOfEleven << OP_11 << OP_CHECKMULTISIG;
    txFrom.vout[5].scriptPubKey.SetDestination(oneOfEleven.GetID());
    txFrom.vout[5].nValue = 6000;

    coins.SetCoins(txFrom.GetHash(), CCoins(txFrom, 0));

    CTransaction txTo;
    txTo.vout.resize(1);
    txTo.vout[0].scriptPubKey.SetDestination(key[1].GetPubKey().GetID());

    txTo.vin.resize(3);
    txTo.vin[0].prevout.n = 0;
    txTo.vin[0].prevout.hash = txFrom.GetHash();
    BOOST_CHECK(SignSignature(keystore, txFrom, txTo, 0));
    txTo.vin[1].prevout.n = 1;
    txTo.vin[1].prevout.hash = txFrom.GetHash();
    BOOST_CHECK(SignSignature(keystore, txFrom, txTo, 1));
    txTo.vin[2].prevout.n = 2;
    txTo.vin[2].prevout.hash = txFrom.GetHash();
    BOOST_CHECK(SignSignature(keystore, txFrom, txTo, 2));

    BOOST_CHECK(txTo.AreInputsStandard(coins));
    BOOST_CHECK_EQUAL(txTo.GetP2SHSigOpCount(coins), 1U);

    // Make sure adding crap to the scriptSigs makes them non-standard:
    for (int i = 0; i < 3; i++)
    {
        CScript t = txTo.vin[i].scriptSig;
        txTo.vin[i].scriptSig = (CScript() << 11) + t;
        BOOST_CHECK(!txTo.AreInputsStandard(coins));
        txTo.vin[i].scriptSig = t;
    }

    CTransaction txToNonStd;
    txToNonStd.vout.resize(1);
    txToNonStd.vout[0].scriptPubKey.SetDestination(key[1].GetPubKey().GetID());
    txToNonStd.vout[0].nValue = 1000;
    txToNonStd.vin.resize(2);
    txToNonStd.vin[0].prevout.n = 4;
    txToNonStd.vin[0].prevout.hash = txFrom.GetHash();
    txToNonStd.vin[0].scriptSig << Serialize(empty);
    txToNonStd.vin[1].prevout.n = 5;
    txToNonStd.vin[1].prevout.hash = txFrom.GetHash();
    txToNonStd.vin[1].scriptSig << OP_0 << Serialize(oneOfEleven);

    BOOST_CHECK(!txToNonStd.AreInputsStandard(coins));
    BOOST_CHECK_EQUAL(txToNonStd.GetP2SHSigOpCount(coins), 11U);

    txToNonStd.vin[0].scriptSig.clear();
    BOOST_CHECK(!txToNonStd.AreInputsStandard(coins));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_SUITE_END()
