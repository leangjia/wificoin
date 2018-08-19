<<<<<<< HEAD
// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "base58.h"

#include "data/base58_encode_decode.json.h"
#include "data/base58_keys_invalid.json.h"
#include "data/base58_keys_valid.json.h"

#include "key.h"
#include "script/script.h"
#include "uint256.h"
#include "util.h"
#include "utilstrencodings.h"
#include "test/test_wificoin.h"

#include <boost/test/unit_test.hpp>

#include <univalue.h>

extern UniValue read_json(const std::string& jsondata);

BOOST_FIXTURE_TEST_SUITE(base58_tests, BasicTestingSetup)
=======
#include <boost/test/unit_test.hpp>
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

#include "base58.h"
#include "util.h"

using namespace json_spirit;
extern Array read_json(const std::string& filename);

BOOST_AUTO_TEST_SUITE(base58_tests)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

// Goal: test low-level base58 encoding functionality
BOOST_AUTO_TEST_CASE(base58_EncodeBase58)
{
<<<<<<< HEAD
    UniValue tests = read_json(std::string(json_tests::base58_encode_decode, json_tests::base58_encode_decode + sizeof(json_tests::base58_encode_decode)));
    for (unsigned int idx = 0; idx < tests.size(); idx++) {
        UniValue test = tests[idx];
        std::string strTest = test.write();
=======
    Array tests = read_json("base58_encode_decode.json");

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::vector<unsigned char> sourcedata = ParseHex(test[0].get_str());
        std::string base58string = test[1].get_str();
        BOOST_CHECK_MESSAGE(
<<<<<<< HEAD
                    EncodeBase58(sourcedata.data(), sourcedata.data() + sourcedata.size()) == base58string,
=======
                    EncodeBase58(&sourcedata[0], &sourcedata[sourcedata.size()]) == base58string,
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    strTest);
    }
}

// Goal: test low-level base58 decoding functionality
BOOST_AUTO_TEST_CASE(base58_DecodeBase58)
{
<<<<<<< HEAD
    UniValue tests = read_json(std::string(json_tests::base58_encode_decode, json_tests::base58_encode_decode + sizeof(json_tests::base58_encode_decode)));
    std::vector<unsigned char> result;

    for (unsigned int idx = 0; idx < tests.size(); idx++) {
        UniValue test = tests[idx];
        std::string strTest = test.write();
=======
    Array tests = read_json("base58_encode_decode.json");
    std::vector<unsigned char> result;

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::vector<unsigned char> expected = ParseHex(test[0].get_str());
        std::string base58string = test[1].get_str();
        BOOST_CHECK_MESSAGE(DecodeBase58(base58string, result), strTest);
        BOOST_CHECK_MESSAGE(result.size() == expected.size() && std::equal(result.begin(), result.end(), expected.begin()), strTest);
    }

    BOOST_CHECK(!DecodeBase58("invalid", result));
<<<<<<< HEAD

    // check that DecodeBase58 skips whitespace, but still fails with unexpected non-whitespace at the end.
    BOOST_CHECK(!DecodeBase58(" \t\n\v\f\r skip \r\f\v\n\t a", result));
    BOOST_CHECK( DecodeBase58(" \t\n\v\f\r skip \r\f\v\n\t ", result));
    std::vector<unsigned char> expected = ParseHex("971a55");
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

// Visitor to check address type
class TestAddrTypeVisitor : public boost::static_visitor<bool>
{
private:
    std::string exp_addrType;
public:
<<<<<<< HEAD
    TestAddrTypeVisitor(const std::string &_exp_addrType) : exp_addrType(_exp_addrType) { }
=======
    TestAddrTypeVisitor(const std::string &exp_addrType) : exp_addrType(exp_addrType) { }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    bool operator()(const CKeyID &id) const
    {
        return (exp_addrType == "pubkey");
    }
    bool operator()(const CScriptID &id) const
    {
        return (exp_addrType == "script");
    }
    bool operator()(const CNoDestination &no) const
    {
        return (exp_addrType == "none");
    }
};

// Visitor to check address payload
class TestPayloadVisitor : public boost::static_visitor<bool>
{
private:
    std::vector<unsigned char> exp_payload;
public:
<<<<<<< HEAD
    TestPayloadVisitor(std::vector<unsigned char> &_exp_payload) : exp_payload(_exp_payload) { }
=======
    TestPayloadVisitor(std::vector<unsigned char> &exp_payload) : exp_payload(exp_payload) { }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    bool operator()(const CKeyID &id) const
    {
        uint160 exp_key(exp_payload);
        return exp_key == id;
    }
    bool operator()(const CScriptID &id) const
    {
        uint160 exp_key(exp_payload);
        return exp_key == id;
    }
    bool operator()(const CNoDestination &no) const
    {
        return exp_payload.size() == 0;
    }
};

// Goal: check that parsed keys match test payload
BOOST_AUTO_TEST_CASE(base58_keys_valid_parse)
{
<<<<<<< HEAD
    UniValue tests = read_json(std::string(json_tests::base58_keys_valid, json_tests::base58_keys_valid + sizeof(json_tests::base58_keys_valid)));
    CWiFicoinSecret secret;
    CWiFicoinAddress addr;
    SelectParams(CBaseChainParams::MAIN);

    for (unsigned int idx = 0; idx < tests.size(); idx++) {
        UniValue test = tests[idx];
        std::string strTest = test.write();
=======
    Array tests = read_json("base58_keys_valid.json");
    std::vector<unsigned char> result;
    CBitcoinSecret secret;
    CBitcoinAddress addr;
    // Save global state
    bool fTestNet_stored = fTestNet;

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if (test.size() < 3) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::string exp_base58string = test[0].get_str();
        std::vector<unsigned char> exp_payload = ParseHex(test[1].get_str());
<<<<<<< HEAD
        const UniValue &metadata = test[2].get_obj();
        bool isPrivkey = find_value(metadata, "isPrivkey").get_bool();
        bool isTestnet = find_value(metadata, "isTestnet").get_bool();
        if (isTestnet)
            SelectParams(CBaseChainParams::TESTNET);
        else
            SelectParams(CBaseChainParams::MAIN);
=======
        const Object &metadata = test[2].get_obj();
        bool isPrivkey = find_value(metadata, "isPrivkey").get_bool();
        bool isTestnet = find_value(metadata, "isTestnet").get_bool();
        fTestNet = isTestnet; // Override testnet flag
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if(isPrivkey)
        {
            bool isCompressed = find_value(metadata, "isCompressed").get_bool();
            // Must be valid private key
<<<<<<< HEAD
            // Note: CWiFicoinSecret::SetString tests isValid, whereas CWiFicoinAddress does not!
            BOOST_CHECK_MESSAGE(secret.SetString(exp_base58string), "!SetString:"+ strTest);
            BOOST_CHECK_MESSAGE(secret.IsValid(), "!IsValid:" + strTest);
            CKey privkey = secret.GetKey();
            BOOST_CHECK_MESSAGE(privkey.IsCompressed() == isCompressed, "compressed mismatch:" + strTest);
=======
            // Note: CBitcoinSecret::SetString tests isValid, whereas CBitcoinAddress does not!
            BOOST_CHECK_MESSAGE(secret.SetString(exp_base58string), "!SetString:"+ strTest);
            BOOST_CHECK_MESSAGE(secret.IsValid(), "!IsValid:" + strTest);
            bool fCompressedOut = false;
            CSecret privkey = secret.GetSecret(fCompressedOut);
            BOOST_CHECK_MESSAGE(fCompressedOut == isCompressed, "compressed mismatch:" + strTest);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            BOOST_CHECK_MESSAGE(privkey.size() == exp_payload.size() && std::equal(privkey.begin(), privkey.end(), exp_payload.begin()), "key mismatch:" + strTest);

            // Private key must be invalid public key
            addr.SetString(exp_base58string);
            BOOST_CHECK_MESSAGE(!addr.IsValid(), "IsValid privkey as pubkey:" + strTest);
        }
        else
        {
            std::string exp_addrType = find_value(metadata, "addrType").get_str(); // "script" or "pubkey"
            // Must be valid public key
            BOOST_CHECK_MESSAGE(addr.SetString(exp_base58string), "SetString:" + strTest);
            BOOST_CHECK_MESSAGE(addr.IsValid(), "!IsValid:" + strTest);
            BOOST_CHECK_MESSAGE(addr.IsScript() == (exp_addrType == "script"), "isScript mismatch" + strTest);
            CTxDestination dest = addr.Get();
            BOOST_CHECK_MESSAGE(boost::apply_visitor(TestAddrTypeVisitor(exp_addrType), dest), "addrType mismatch" + strTest);

            // Public key must be invalid private key
            secret.SetString(exp_base58string);
            BOOST_CHECK_MESSAGE(!secret.IsValid(), "IsValid pubkey as privkey:" + strTest);
        }
    }
<<<<<<< HEAD
=======
    // Restore global state
    fTestNet = fTestNet_stored;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

// Goal: check that generated keys match test vectors
BOOST_AUTO_TEST_CASE(base58_keys_valid_gen)
{
<<<<<<< HEAD
    UniValue tests = read_json(std::string(json_tests::base58_keys_valid, json_tests::base58_keys_valid + sizeof(json_tests::base58_keys_valid)));

    for (unsigned int idx = 0; idx < tests.size(); idx++) {
        UniValue test = tests[idx];
        std::string strTest = test.write();
=======
    Array tests = read_json("base58_keys_valid.json");
    std::vector<unsigned char> result;
    // Save global state
    bool fTestNet_stored = fTestNet;

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if (test.size() < 3) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::string exp_base58string = test[0].get_str();
        std::vector<unsigned char> exp_payload = ParseHex(test[1].get_str());
<<<<<<< HEAD
        const UniValue &metadata = test[2].get_obj();
        bool isPrivkey = find_value(metadata, "isPrivkey").get_bool();
        bool isTestnet = find_value(metadata, "isTestnet").get_bool();
        if (isTestnet)
            SelectParams(CBaseChainParams::TESTNET);
        else
            SelectParams(CBaseChainParams::MAIN);
        if(isPrivkey)
        {
            bool isCompressed = find_value(metadata, "isCompressed").get_bool();
            CKey key;
            key.Set(exp_payload.begin(), exp_payload.end(), isCompressed);
            assert(key.IsValid());
            CWiFicoinSecret secret;
            secret.SetKey(key);
=======
        const Object &metadata = test[2].get_obj();
        bool isPrivkey = find_value(metadata, "isPrivkey").get_bool();
        bool isTestnet = find_value(metadata, "isTestnet").get_bool();
        fTestNet = isTestnet; // Override testnet flag
        if(isPrivkey)
        {
            bool isCompressed = find_value(metadata, "isCompressed").get_bool();
            CBitcoinSecret secret;
            secret.SetSecret(CSecret(exp_payload.begin(), exp_payload.end()), isCompressed);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            BOOST_CHECK_MESSAGE(secret.ToString() == exp_base58string, "result mismatch: " + strTest);
        }
        else
        {
            std::string exp_addrType = find_value(metadata, "addrType").get_str();
            CTxDestination dest;
            if(exp_addrType == "pubkey")
            {
                dest = CKeyID(uint160(exp_payload));
            }
            else if(exp_addrType == "script")
            {
                dest = CScriptID(uint160(exp_payload));
            }
            else if(exp_addrType == "none")
            {
                dest = CNoDestination();
            }
            else
            {
                BOOST_ERROR("Bad addrtype: " << strTest);
                continue;
            }
<<<<<<< HEAD
            CWiFicoinAddress addrOut;
            BOOST_CHECK_MESSAGE(addrOut.Set(dest), "encode dest: " + strTest);
=======
            CBitcoinAddress addrOut;
            BOOST_CHECK_MESSAGE(boost::apply_visitor(CBitcoinAddressVisitor(&addrOut), dest), "encode dest: " + strTest);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            BOOST_CHECK_MESSAGE(addrOut.ToString() == exp_base58string, "mismatch: " + strTest);
        }
    }

    // Visiting a CNoDestination must fail
<<<<<<< HEAD
    CWiFicoinAddress dummyAddr;
    CTxDestination nodest = CNoDestination();
    BOOST_CHECK(!dummyAddr.Set(nodest));

    SelectParams(CBaseChainParams::MAIN);
=======
    CBitcoinAddress dummyAddr;
    CTxDestination nodest = CNoDestination();
    BOOST_CHECK(!boost::apply_visitor(CBitcoinAddressVisitor(&dummyAddr), nodest));

    // Restore global state
    fTestNet = fTestNet_stored;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

// Goal: check that base58 parsing code is robust against a variety of corrupted data
BOOST_AUTO_TEST_CASE(base58_keys_invalid)
{
<<<<<<< HEAD
    UniValue tests = read_json(std::string(json_tests::base58_keys_invalid, json_tests::base58_keys_invalid + sizeof(json_tests::base58_keys_invalid))); // Negative testcases
    CWiFicoinSecret secret;
    CWiFicoinAddress addr;

    for (unsigned int idx = 0; idx < tests.size(); idx++) {
        UniValue test = tests[idx];
        std::string strTest = test.write();
=======
    Array tests = read_json("base58_keys_invalid.json"); // Negative testcases
    std::vector<unsigned char> result;
    CBitcoinSecret secret;
    CBitcoinAddress addr;

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if (test.size() < 1) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::string exp_base58string = test[0].get_str();

        // must be invalid as public and as private key
        addr.SetString(exp_base58string);
        BOOST_CHECK_MESSAGE(!addr.IsValid(), "IsValid pubkey:" + strTest);
        secret.SetString(exp_base58string);
        BOOST_CHECK_MESSAGE(!secret.IsValid(), "IsValid privkey:" + strTest);
    }
}


BOOST_AUTO_TEST_SUITE_END()

