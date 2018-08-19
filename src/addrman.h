// Copyright (c) 2012 Pieter Wuille
<<<<<<< HEAD
// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_ADDRMAN_H
#define WIFICOIN_ADDRMAN_H

#include "netaddress.h"
#include "protocol.h"
#include "random.h"
#include "sync.h"
#include "timedata.h"
#include "util.h"

#include <map>
#include <set>
#include <stdint.h>
#include <vector>

/**
 * Extended statistics about a CAddress
 */
class CAddrInfo : public CAddress
{


public:
    //! last try whatsoever by us (memory only)
    int64_t nLastTry;

    //! last counted attempt (memory only)
    int64_t nLastCountAttempt;

private:
    //! where knowledge about this address first came from
    CNetAddr source;

    //! last successful connection by us
    int64_t nLastSuccess;

    //! connection attempts since last successful attempt
    int nAttempts;

    //! reference count in new sets (memory only)
    int nRefCount;

    //! in tried set? (memory only)
    bool fInTried;

    //! position in vRandom
=======
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef _BITCOIN_ADDRMAN
#define _BITCOIN_ADDRMAN 1

#include "netbase.h"
#include "protocol.h"
#include "util.h"
#include "sync.h"


#include <map>
#include <vector>

#include <openssl/rand.h>


/** Extended statistics about a CAddress */
class CAddrInfo : public CAddress
{
private:
    // where knowledge about this address first came from
    CNetAddr source;

    // last successful connection by us
    int64 nLastSuccess;

    // last try whatsoever by us:
    // int64 CAddress::nLastTry

    // connection attempts since last successful attempt
    int nAttempts;

    // reference count in new sets (memory only)
    int nRefCount;

    // in tried set? (memory only)
    bool fInTried;

    // position in vRandom
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    int nRandomPos;

    friend class CAddrMan;

public:

<<<<<<< HEAD
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(*(CAddress*)this);
        READWRITE(source);
        READWRITE(nLastSuccess);
        READWRITE(nAttempts);
    }
=======
    IMPLEMENT_SERIALIZE(
        CAddress* pthis = (CAddress*)(this);
        READWRITE(*pthis);
        READWRITE(source);
        READWRITE(nLastSuccess);
        READWRITE(nAttempts);
    )
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

    void Init()
    {
        nLastSuccess = 0;
        nLastTry = 0;
<<<<<<< HEAD
        nLastCountAttempt = 0;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        nAttempts = 0;
        nRefCount = 0;
        fInTried = false;
        nRandomPos = -1;
    }

    CAddrInfo(const CAddress &addrIn, const CNetAddr &addrSource) : CAddress(addrIn), source(addrSource)
    {
        Init();
    }

    CAddrInfo() : CAddress(), source()
    {
        Init();
    }

<<<<<<< HEAD
    //! Calculate in which "tried" bucket this entry belongs
    int GetTriedBucket(const uint256 &nKey) const;

    //! Calculate in which "new" bucket this entry belongs, given a certain source
    int GetNewBucket(const uint256 &nKey, const CNetAddr& src) const;

    //! Calculate in which "new" bucket this entry belongs, using its default source
    int GetNewBucket(const uint256 &nKey) const
=======
    // Calculate in which "tried" bucket this entry belongs
    int GetTriedBucket(const std::vector<unsigned char> &nKey) const;

    // Calculate in which "new" bucket this entry belongs, given a certain source
    int GetNewBucket(const std::vector<unsigned char> &nKey, const CNetAddr& src) const;

    // Calculate in which "new" bucket this entry belongs, using its default source
    int GetNewBucket(const std::vector<unsigned char> &nKey) const
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        return GetNewBucket(nKey, source);
    }

<<<<<<< HEAD
    //! Calculate in which position of a bucket to store this entry.
    int GetBucketPosition(const uint256 &nKey, bool fNew, int nBucket) const;

    //! Determine whether the statistics about this entry are bad enough so that it can just be deleted
    bool IsTerrible(int64_t nNow = GetAdjustedTime()) const;

    //! Calculate the relative chance this entry should be given when selecting nodes to connect to
    double GetChance(int64_t nNow = GetAdjustedTime()) const;

};

/** Stochastic address manager
 *
 * Design goals:
 *  * Keep the address tables in-memory, and asynchronously dump the entire table to peers.dat.
 *  * Make sure no (localized) attacker can fill the entire table with his nodes/addresses.
 *
 * To that end:
 *  * Addresses are organized into buckets.
 *    * Addresses that have not yet been tried go into 1024 "new" buckets.
 *      * Based on the address range (/16 for IPv4) of the source of information, 64 buckets are selected at random.
 *      * The actual bucket is chosen from one of these, based on the range in which the address itself is located.
 *      * One single address can occur in up to 8 different buckets to increase selection chances for addresses that
 *        are seen frequently. The chance for increasing this multiplicity decreases exponentially.
 *      * When adding a new address to a full bucket, a randomly chosen entry (with a bias favoring less recently seen
 *        ones) is removed from it first.
 *    * Addresses of nodes that are known to be accessible go into 256 "tried" buckets.
 *      * Each address range selects at random 8 of these buckets.
 *      * The actual bucket is chosen from one of these, based on the full address.
 *      * When adding a new good address to a full bucket, a randomly chosen entry (with a bias favoring less recently
 *        tried ones) is evicted from it, back to the "new" buckets.
 *    * Bucket selection is based on cryptographic hashing, using a randomly-generated 256-bit key, which should not
 *      be observable by adversaries.
 *    * Several indexes are kept for high performance. Defining DEBUG_ADDRMAN will introduce frequent (and expensive)
 *      consistency checks for the entire data structure.
 */

//! total number of buckets for tried addresses
#define ADDRMAN_TRIED_BUCKET_COUNT_LOG2 8

//! total number of buckets for new addresses
#define ADDRMAN_NEW_BUCKET_COUNT_LOG2 10

//! maximum allowed number of entries in buckets for new and tried addresses
#define ADDRMAN_BUCKET_SIZE_LOG2 6

//! over how many buckets entries with tried addresses from a single group (/16 for IPv4) are spread
#define ADDRMAN_TRIED_BUCKETS_PER_GROUP 8

//! over how many buckets entries with new addresses originating from a single group are spread
#define ADDRMAN_NEW_BUCKETS_PER_SOURCE_GROUP 64

//! in how many buckets for entries with new addresses a single address may occur
#define ADDRMAN_NEW_BUCKETS_PER_ADDRESS 8

//! how old addresses can maximally be
#define ADDRMAN_HORIZON_DAYS 30

//! after how many failed attempts we give up on a new node
#define ADDRMAN_RETRIES 3

//! how many successive failures are allowed ...
#define ADDRMAN_MAX_FAILURES 10

//! ... in at least this many days
#define ADDRMAN_MIN_FAIL_DAYS 7

//! the maximum percentage of nodes to return in a getaddr call
#define ADDRMAN_GETADDR_MAX_PCT 23

//! the maximum number of nodes to return in a getaddr call
#define ADDRMAN_GETADDR_MAX 2500

//! Convenience
#define ADDRMAN_TRIED_BUCKET_COUNT (1 << ADDRMAN_TRIED_BUCKET_COUNT_LOG2)
#define ADDRMAN_NEW_BUCKET_COUNT (1 << ADDRMAN_NEW_BUCKET_COUNT_LOG2)
#define ADDRMAN_BUCKET_SIZE (1 << ADDRMAN_BUCKET_SIZE_LOG2)

/** 
 * Stochastical (IP) address manager 
 */
class CAddrMan
{
private:
    //! critical section to protect the inner data structures
    mutable CCriticalSection cs;

    //! last used nId
    int nIdCount;

    //! table with information about all nIds
    std::map<int, CAddrInfo> mapInfo;

    //! find an nId based on its network address
    std::map<CNetAddr, int> mapAddr;

    //! randomly-ordered vector of all nIds
=======
    // Determine whether the statistics about this entry are bad enough so that it can just be deleted
    bool IsTerrible(int64 nNow = GetAdjustedTime()) const;

    // Calculate the relative chance this entry should be given when selecting nodes to connect to
    double GetChance(int64 nNow = GetAdjustedTime()) const;

};

// Stochastic address manager
//
// Design goals:
//  * Only keep a limited number of addresses around, so that addr.dat and memory requirements do not grow without bound.
//  * Keep the address tables in-memory, and asynchronously dump the entire to able in addr.dat.
//  * Make sure no (localized) attacker can fill the entire table with his nodes/addresses.
//
// To that end:
//  * Addresses are organized into buckets.
//    * Address that have not yet been tried go into 256 "new" buckets.
//      * Based on the address range (/16 for IPv4) of source of the information, 32 buckets are selected at random
//      * The actual bucket is chosen from one of these, based on the range the address itself is located.
//      * One single address can occur in up to 4 different buckets, to increase selection chances for addresses that
//        are seen frequently. The chance for increasing this multiplicity decreases exponentially.
//      * When adding a new address to a full bucket, a randomly chosen entry (with a bias favoring less recently seen
//        ones) is removed from it first.
//    * Addresses of nodes that are known to be accessible go into 64 "tried" buckets.
//      * Each address range selects at random 4 of these buckets.
//      * The actual bucket is chosen from one of these, based on the full address.
//      * When adding a new good address to a full bucket, a randomly chosen entry (with a bias favoring less recently
//        tried ones) is evicted from it, back to the "new" buckets.
//    * Bucket selection is based on cryptographic hashing, using a randomly-generated 256-bit key, which should not
//      be observable by adversaries.
//    * Several indexes are kept for high performance. Defining DEBUG_ADDRMAN will introduce frequent (and expensive)
//      consistency checks for the entire data structure.

// total number of buckets for tried addresses
#define ADDRMAN_TRIED_BUCKET_COUNT 64

// maximum allowed number of entries in buckets for tried addresses
#define ADDRMAN_TRIED_BUCKET_SIZE 64

// total number of buckets for new addresses
#define ADDRMAN_NEW_BUCKET_COUNT 256

// maximum allowed number of entries in buckets for new addresses
#define ADDRMAN_NEW_BUCKET_SIZE 64

// over how many buckets entries with tried addresses from a single group (/16 for IPv4) are spread
#define ADDRMAN_TRIED_BUCKETS_PER_GROUP 4

// over how many buckets entries with new addresses originating from a single group are spread
#define ADDRMAN_NEW_BUCKETS_PER_SOURCE_GROUP 32

// in how many buckets for entries with new addresses a single address may occur
#define ADDRMAN_NEW_BUCKETS_PER_ADDRESS 4

// how many entries in a bucket with tried addresses are inspected, when selecting one to replace
#define ADDRMAN_TRIED_ENTRIES_INSPECT_ON_EVICT 4

// how old addresses can maximally be
#define ADDRMAN_HORIZON_DAYS 30

// after how many failed attempts we give up on a new node
#define ADDRMAN_RETRIES 3

// how many successive failures are allowed ...
#define ADDRMAN_MAX_FAILURES 10

// ... in at least this many days
#define ADDRMAN_MIN_FAIL_DAYS 7

// the maximum percentage of nodes to return in a getaddr call
#define ADDRMAN_GETADDR_MAX_PCT 23

// the maximum number of nodes to return in a getaddr call
#define ADDRMAN_GETADDR_MAX 2500

/** Stochastical (IP) address manager */
class CAddrMan
{
private:
    // critical section to protect the inner data structures
    mutable CCriticalSection cs;

    // secret key to randomize bucket select with
    std::vector<unsigned char> nKey;

    // last used nId
    int nIdCount;

    // table with information about all nIds
    std::map<int, CAddrInfo> mapInfo;

    // find an nId based on its network address
    std::map<CNetAddr, int> mapAddr;

    // randomly-ordered vector of all nIds
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    std::vector<int> vRandom;

    // number of "tried" entries
    int nTried;

<<<<<<< HEAD
    //! list of "tried" buckets
    int vvTried[ADDRMAN_TRIED_BUCKET_COUNT][ADDRMAN_BUCKET_SIZE];

    //! number of (unique) "new" entries
    int nNew;

    //! list of "new" buckets
    int vvNew[ADDRMAN_NEW_BUCKET_COUNT][ADDRMAN_BUCKET_SIZE];

    //! last time Good was called (memory only)
    int64_t nLastGood;

protected:
    //! secret key to randomize bucket select with
    uint256 nKey;

    //! Source of random numbers for randomization in inner loops
    FastRandomContext insecure_rand;

    //! Find an entry.
    CAddrInfo* Find(const CNetAddr& addr, int *pnId = nullptr);

    //! find an entry, creating it if necessary.
    //! nTime and nServices of the found node are updated, if necessary.
    CAddrInfo* Create(const CAddress &addr, const CNetAddr &addrSource, int *pnId = nullptr);

    //! Swap two elements in vRandom.
    void SwapRandom(unsigned int nRandomPos1, unsigned int nRandomPos2);

    //! Move an entry from the "new" table(s) to the "tried" table
    void MakeTried(CAddrInfo& info, int nId);

    //! Delete an entry. It must not be in tried, and have refcount 0.
    void Delete(int nId);

    //! Clear a position in a "new" table. This is the only place where entries are actually deleted.
    void ClearNew(int nUBucket, int nUBucketPos);

    //! Mark an entry "good", possibly moving it from "new" to "tried".
    void Good_(const CService &addr, int64_t nTime);

    //! Add an entry to the "new" table.
    bool Add_(const CAddress &addr, const CNetAddr& source, int64_t nTimePenalty);

    //! Mark an entry as attempted to connect.
    void Attempt_(const CService &addr, bool fCountFailure, int64_t nTime);

    //! Select an address to connect to, if newOnly is set to true, only the new table is selected from.
    CAddrInfo Select_(bool newOnly);

    //! Wraps GetRandInt to allow tests to override RandomInt and make it determinismistic.
    virtual int RandomInt(int nMax);

#ifdef DEBUG_ADDRMAN
    //! Perform consistency check. Returns an error code or zero.
    int Check_();
#endif

    //! Select several addresses at once.
    void GetAddr_(std::vector<CAddress> &vAddr);

    //! Mark an entry as currently-connected-to.
    void Connected_(const CService &addr, int64_t nTime);

    //! Update an entry's service bits.
    void SetServices_(const CService &addr, ServiceFlags nServices);

public:
    /**
     * serialized format:
     * * version byte (currently 1)
     * * 0x20 + nKey (serialized as if it were a vector, for backward compatibility)
     * * nNew
     * * nTried
     * * number of "new" buckets XOR 2**30
     * * all nNew addrinfos in vvNew
     * * all nTried addrinfos in vvTried
     * * for each bucket:
     *   * number of elements
     *   * for each element: index
     *
     * 2**30 is xorred with the number of buckets to make addrman deserializer v0 detect it
     * as incompatible. This is necessary because it did not check the version number on
     * deserialization.
     *
     * Notice that vvTried, mapAddr and vVector are never encoded explicitly;
     * they are instead reconstructed from the other information.
     *
     * vvNew is serialized, but only used if ADDRMAN_UNKNOWN_BUCKET_COUNT didn't change,
     * otherwise it is reconstructed as well.
     *
     * This format is more complex, but significantly smaller (at most 1.5 MiB), and supports
     * changes to the ADDRMAN_ parameters without breaking the on-disk structure.
     *
     * We don't use ADD_SERIALIZE_METHODS since the serialization and deserialization code has
     * very little in common.
     */
    template<typename Stream>
    void Serialize(Stream &s) const
    {
        LOCK(cs);

        unsigned char nVersion = 1;
        s << nVersion;
        s << ((unsigned char)32);
        s << nKey;
        s << nNew;
        s << nTried;

        int nUBuckets = ADDRMAN_NEW_BUCKET_COUNT ^ (1 << 30);
        s << nUBuckets;
        std::map<int, int> mapUnkIds;
        int nIds = 0;
        for (std::map<int, CAddrInfo>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); it++) {
            mapUnkIds[(*it).first] = nIds;
            const CAddrInfo &info = (*it).second;
            if (info.nRefCount) {
                assert(nIds != nNew); // this means nNew was wrong, oh ow
                s << info;
                nIds++;
            }
        }
        nIds = 0;
        for (std::map<int, CAddrInfo>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); it++) {
            const CAddrInfo &info = (*it).second;
            if (info.fInTried) {
                assert(nIds != nTried); // this means nTried was wrong, oh ow
                s << info;
                nIds++;
            }
        }
        for (int bucket = 0; bucket < ADDRMAN_NEW_BUCKET_COUNT; bucket++) {
            int nSize = 0;
            for (int i = 0; i < ADDRMAN_BUCKET_SIZE; i++) {
                if (vvNew[bucket][i] != -1)
                    nSize++;
            }
            s << nSize;
            for (int i = 0; i < ADDRMAN_BUCKET_SIZE; i++) {
                if (vvNew[bucket][i] != -1) {
                    int nIndex = mapUnkIds[vvNew[bucket][i]];
                    s << nIndex;
                }
            }
        }
    }

    template<typename Stream>
    void Unserialize(Stream& s)
    {
        LOCK(cs);

        Clear();

        unsigned char nVersion;
        s >> nVersion;
        unsigned char nKeySize;
        s >> nKeySize;
        if (nKeySize != 32) throw std::ios_base::failure("Incorrect keysize in addrman deserialization");
        s >> nKey;
        s >> nNew;
        s >> nTried;
        int nUBuckets = 0;
        s >> nUBuckets;
        if (nVersion != 0) {
            nUBuckets ^= (1 << 30);
        }

        if (nNew > ADDRMAN_NEW_BUCKET_COUNT * ADDRMAN_BUCKET_SIZE) {
            throw std::ios_base::failure("Corrupt CAddrMan serialization, nNew exceeds limit.");
        }

        if (nTried > ADDRMAN_TRIED_BUCKET_COUNT * ADDRMAN_BUCKET_SIZE) {
            throw std::ios_base::failure("Corrupt CAddrMan serialization, nTried exceeds limit.");
        }

        // Deserialize entries from the new table.
        for (int n = 0; n < nNew; n++) {
            CAddrInfo &info = mapInfo[n];
            s >> info;
            mapAddr[info] = n;
            info.nRandomPos = vRandom.size();
            vRandom.push_back(n);
            if (nVersion != 1 || nUBuckets != ADDRMAN_NEW_BUCKET_COUNT) {
                // In case the new table data cannot be used (nVersion unknown, or bucket count wrong),
                // immediately try to give them a reference based on their primary source address.
                int nUBucket = info.GetNewBucket(nKey);
                int nUBucketPos = info.GetBucketPosition(nKey, true, nUBucket);
                if (vvNew[nUBucket][nUBucketPos] == -1) {
                    vvNew[nUBucket][nUBucketPos] = n;
                    info.nRefCount++;
                }
            }
        }
        nIdCount = nNew;

        // Deserialize entries from the tried table.
        int nLost = 0;
        for (int n = 0; n < nTried; n++) {
            CAddrInfo info;
            s >> info;
            int nKBucket = info.GetTriedBucket(nKey);
            int nKBucketPos = info.GetBucketPosition(nKey, false, nKBucket);
            if (vvTried[nKBucket][nKBucketPos] == -1) {
                info.nRandomPos = vRandom.size();
                info.fInTried = true;
                vRandom.push_back(nIdCount);
                mapInfo[nIdCount] = info;
                mapAddr[info] = nIdCount;
                vvTried[nKBucket][nKBucketPos] = nIdCount;
                nIdCount++;
            } else {
                nLost++;
            }
        }
        nTried -= nLost;

        // Deserialize positions in the new table (if possible).
        for (int bucket = 0; bucket < nUBuckets; bucket++) {
            int nSize = 0;
            s >> nSize;
            for (int n = 0; n < nSize; n++) {
                int nIndex = 0;
                s >> nIndex;
                if (nIndex >= 0 && nIndex < nNew) {
                    CAddrInfo &info = mapInfo[nIndex];
                    int nUBucketPos = info.GetBucketPosition(nKey, true, bucket);
                    if (nVersion == 1 && nUBuckets == ADDRMAN_NEW_BUCKET_COUNT && vvNew[bucket][nUBucketPos] == -1 && info.nRefCount < ADDRMAN_NEW_BUCKETS_PER_ADDRESS) {
                        info.nRefCount++;
                        vvNew[bucket][nUBucketPos] = nIndex;
=======
    // list of "tried" buckets
    std::vector<std::vector<int> > vvTried;

    // number of (unique) "new" entries
    int nNew;

    // list of "new" buckets
    std::vector<std::set<int> > vvNew;

protected:

    // Find an entry.
    CAddrInfo* Find(const CNetAddr& addr, int *pnId = NULL);

    // find an entry, creating it if necessary.
    // nTime and nServices of found node is updated, if necessary.
    CAddrInfo* Create(const CAddress &addr, const CNetAddr &addrSource, int *pnId = NULL);

    // Swap two elements in vRandom.
    void SwapRandom(unsigned int nRandomPos1, unsigned int nRandomPos2);

    // Return position in given bucket to replace.
    int SelectTried(int nKBucket);

    // Remove an element from a "new" bucket.
    // This is the only place where actual deletes occur.
    // They are never deleted while in the "tried" table, only possibly evicted back to the "new" table.
    int ShrinkNew(int nUBucket);

    // Move an entry from the "new" table(s) to the "tried" table
    // @pre vvUnkown[nOrigin].count(nId) != 0
    void MakeTried(CAddrInfo& info, int nId, int nOrigin);

    // Mark an entry "good", possibly moving it from "new" to "tried".
    void Good_(const CService &addr, int64 nTime);

    // Add an entry to the "new" table.
    bool Add_(const CAddress &addr, const CNetAddr& source, int64 nTimePenalty);

    // Mark an entry as attempted to connect.
    void Attempt_(const CService &addr, int64 nTime);

    // Select an address to connect to.
    // nUnkBias determines how much to favor new addresses over tried ones (min=0, max=100)
    CAddress Select_(int nUnkBias);

#ifdef DEBUG_ADDRMAN
    // Perform consistency check. Returns an error code or zero.
    int Check_();
#endif

    // Select several addresses at once.
    void GetAddr_(std::vector<CAddress> &vAddr);

    // Mark an entry as currently-connected-to.
    void Connected_(const CService &addr, int64 nTime);

public:

    IMPLEMENT_SERIALIZE
    (({
        // serialized format:
        // * version byte (currently 0)
        // * nKey
        // * nNew
        // * nTried
        // * number of "new" buckets
        // * all nNew addrinfos in vvNew
        // * all nTried addrinfos in vvTried
        // * for each bucket:
        //   * number of elements
        //   * for each element: index
        //
        // Notice that vvTried, mapAddr and vVector are never encoded explicitly;
        // they are instead reconstructed from the other information.
        //
        // vvNew is serialized, but only used if ADDRMAN_UNKOWN_BUCKET_COUNT didn't change,
        // otherwise it is reconstructed as well.
        //
        // This format is more complex, but significantly smaller (at most 1.5 MiB), and supports
        // changes to the ADDRMAN_ parameters without breaking the on-disk structure.
        {
            LOCK(cs);
            unsigned char nVersion = 0;
            READWRITE(nVersion);
            READWRITE(nKey);
            READWRITE(nNew);
            READWRITE(nTried);

            CAddrMan *am = const_cast<CAddrMan*>(this);
            if (fWrite)
            {
                int nUBuckets = ADDRMAN_NEW_BUCKET_COUNT;
                READWRITE(nUBuckets);
                std::map<int, int> mapUnkIds;
                int nIds = 0;
                for (std::map<int, CAddrInfo>::iterator it = am->mapInfo.begin(); it != am->mapInfo.end(); it++)
                {
                    if (nIds == nNew) break; // this means nNew was wrong, oh ow
                    mapUnkIds[(*it).first] = nIds;
                    CAddrInfo &info = (*it).second;
                    if (info.nRefCount)
                    {
                        READWRITE(info);
                        nIds++;
                    }
                }
                nIds = 0;
                for (std::map<int, CAddrInfo>::iterator it = am->mapInfo.begin(); it != am->mapInfo.end(); it++)
                {
                    if (nIds == nTried) break; // this means nTried was wrong, oh ow
                    CAddrInfo &info = (*it).second;
                    if (info.fInTried)
                    {
                        READWRITE(info);
                        nIds++;
                    }
                }
                for (std::vector<std::set<int> >::iterator it = am->vvNew.begin(); it != am->vvNew.end(); it++)
                {
                    const std::set<int> &vNew = (*it);
                    int nSize = vNew.size();
                    READWRITE(nSize);
                    for (std::set<int>::iterator it2 = vNew.begin(); it2 != vNew.end(); it2++)
                    {
                        int nIndex = mapUnkIds[*it2];
                        READWRITE(nIndex);
                    }
                }
            } else {
                int nUBuckets = 0;
                READWRITE(nUBuckets);
                am->nIdCount = 0;
                am->mapInfo.clear();
                am->mapAddr.clear();
                am->vRandom.clear();
                am->vvTried = std::vector<std::vector<int> >(ADDRMAN_TRIED_BUCKET_COUNT, std::vector<int>(0));
                am->vvNew = std::vector<std::set<int> >(ADDRMAN_NEW_BUCKET_COUNT, std::set<int>());
                for (int n = 0; n < am->nNew; n++)
                {
                    CAddrInfo &info = am->mapInfo[n];
                    READWRITE(info);
                    am->mapAddr[info] = n;
                    info.nRandomPos = vRandom.size();
                    am->vRandom.push_back(n);
                    if (nUBuckets != ADDRMAN_NEW_BUCKET_COUNT)
                    {
                        am->vvNew[info.GetNewBucket(am->nKey)].insert(n);
                        info.nRefCount++;
                    }
                }
                am->nIdCount = am->nNew;
                int nLost = 0;
                for (int n = 0; n < am->nTried; n++)
                {
                    CAddrInfo info;
                    READWRITE(info);
                    std::vector<int> &vTried = am->vvTried[info.GetTriedBucket(am->nKey)];
                    if (vTried.size() < ADDRMAN_TRIED_BUCKET_SIZE)
                    {
                        info.nRandomPos = vRandom.size();
                        info.fInTried = true;
                        am->vRandom.push_back(am->nIdCount);
                        am->mapInfo[am->nIdCount] = info;
                        am->mapAddr[info] = am->nIdCount;
                        vTried.push_back(am->nIdCount);
                        am->nIdCount++;
                    } else {
                        nLost++;
                    }
                }
                am->nTried -= nLost;
                for (int b = 0; b < nUBuckets; b++)
                {
                    std::set<int> &vNew = am->vvNew[b];
                    int nSize = 0;
                    READWRITE(nSize);
                    for (int n = 0; n < nSize; n++)
                    {
                        int nIndex = 0;
                        READWRITE(nIndex);
                        CAddrInfo &info = am->mapInfo[nIndex];
                        if (nUBuckets == ADDRMAN_NEW_BUCKET_COUNT && info.nRefCount < ADDRMAN_NEW_BUCKETS_PER_ADDRESS)
                        {
                            info.nRefCount++;
                            vNew.insert(nIndex);
                        }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                    }
                }
            }
        }
<<<<<<< HEAD

        // Prune new entries with refcount 0 (as a result of collisions).
        int nLostUnk = 0;
        for (std::map<int, CAddrInfo>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); ) {
            if (it->second.fInTried == false && it->second.nRefCount == 0) {
                std::map<int, CAddrInfo>::const_iterator itCopy = it++;
                Delete(itCopy->first);
                nLostUnk++;
            } else {
                it++;
            }
        }
        if (nLost + nLostUnk > 0) {
            LogPrint(BCLog::ADDRMAN, "addrman lost %i new and %i tried addresses due to collisions\n", nLostUnk, nLost);
        }

        Check();
    }

    void Clear()
    {
        std::vector<int>().swap(vRandom);
        nKey = GetRandHash();
        for (size_t bucket = 0; bucket < ADDRMAN_NEW_BUCKET_COUNT; bucket++) {
            for (size_t entry = 0; entry < ADDRMAN_BUCKET_SIZE; entry++) {
                vvNew[bucket][entry] = -1;
            }
        }
        for (size_t bucket = 0; bucket < ADDRMAN_TRIED_BUCKET_COUNT; bucket++) {
            for (size_t entry = 0; entry < ADDRMAN_BUCKET_SIZE; entry++) {
                vvTried[bucket][entry] = -1;
            }
        }

        nIdCount = 0;
        nTried = 0;
        nNew = 0;
        nLastGood = 1; //Initially at 1 so that "never" is strictly worse.
        mapInfo.clear();
        mapAddr.clear();
    }

    CAddrMan()
    {
        Clear();
    }

    ~CAddrMan()
    {
        nKey.SetNull();
    }

    //! Return the number of (unique) addresses in all tables.
    size_t size() const
    {
        LOCK(cs); // TODO: Cache this in an atomic to avoid this overhead
        return vRandom.size();
    }

    //! Consistency check
=======
    });)

    CAddrMan() : vRandom(0), vvTried(ADDRMAN_TRIED_BUCKET_COUNT, std::vector<int>(0)), vvNew(ADDRMAN_NEW_BUCKET_COUNT, std::set<int>())
    {
         nKey.resize(32);
         RAND_bytes(&nKey[0], 32);

         nIdCount = 0;
         nTried = 0;
         nNew = 0;
    }

    // Return the number of (unique) addresses in all tables.
    int size()
    {
        return vRandom.size();
    }

    // Consistency check
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    void Check()
    {
#ifdef DEBUG_ADDRMAN
        {
            LOCK(cs);
            int err;
            if ((err=Check_()))
<<<<<<< HEAD
                LogPrintf("ADDRMAN CONSISTENCY CHECK FAILED!!! err=%i\n", err);
=======
                printf("ADDRMAN CONSISTENCY CHECK FAILED!!! err=%i\n", err);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        }
#endif
    }

<<<<<<< HEAD
    //! Add a single address.
    bool Add(const CAddress &addr, const CNetAddr& source, int64_t nTimePenalty = 0)
    {
        LOCK(cs);
        bool fRet = false;
        Check();
        fRet |= Add_(addr, source, nTimePenalty);
        Check();
        if (fRet) {
            LogPrint(BCLog::ADDRMAN, "Added %s from %s: %i tried, %i new\n", addr.ToStringIPPort(), source.ToString(), nTried, nNew);
        }
        return fRet;
    }

    //! Add multiple addresses.
    bool Add(const std::vector<CAddress> &vAddr, const CNetAddr& source, int64_t nTimePenalty = 0)
    {
        LOCK(cs);
        int nAdd = 0;
        Check();
        for (std::vector<CAddress>::const_iterator it = vAddr.begin(); it != vAddr.end(); it++)
            nAdd += Add_(*it, source, nTimePenalty) ? 1 : 0;
        Check();
        if (nAdd) {
            LogPrint(BCLog::ADDRMAN, "Added %i addresses from %s: %i tried, %i new\n", nAdd, source.ToString(), nTried, nNew);
        }
        return nAdd > 0;
    }

    //! Mark an entry as accessible.
    void Good(const CService &addr, int64_t nTime = GetAdjustedTime())
    {
        LOCK(cs);
        Check();
        Good_(addr, nTime);
        Check();
    }

    //! Mark an entry as connection attempted to.
    void Attempt(const CService &addr, bool fCountFailure, int64_t nTime = GetAdjustedTime())
    {
        LOCK(cs);
        Check();
        Attempt_(addr, fCountFailure, nTime);
        Check();
    }

    /**
     * Choose an address to connect to.
     */
    CAddrInfo Select(bool newOnly = false)
    {
        CAddrInfo addrRet;
        {
            LOCK(cs);
            Check();
            addrRet = Select_(newOnly);
=======
    // Add a single address.
    bool Add(const CAddress &addr, const CNetAddr& source, int64 nTimePenalty = 0)
    {
        bool fRet = false;
        {
            LOCK(cs);
            Check();
            fRet |= Add_(addr, source, nTimePenalty);
            Check();
        }
        if (fRet)
            printf("Added %s from %s: %i tried, %i new\n", addr.ToStringIPPort().c_str(), source.ToString().c_str(), nTried, nNew);
        return fRet;
    }

    // Add multiple addresses.
    bool Add(const std::vector<CAddress> &vAddr, const CNetAddr& source, int64 nTimePenalty = 0)
    {
        int nAdd = 0;
        {
            LOCK(cs);
            Check();
            for (std::vector<CAddress>::const_iterator it = vAddr.begin(); it != vAddr.end(); it++)
                nAdd += Add_(*it, source, nTimePenalty) ? 1 : 0;
            Check();
        }
        if (nAdd)
            printf("Added %i addresses from %s: %i tried, %i new\n", nAdd, source.ToString().c_str(), nTried, nNew);
        return nAdd > 0;
    }

    // Mark an entry as accessible.
    void Good(const CService &addr, int64 nTime = GetAdjustedTime())
    {
        {
            LOCK(cs);
            Check();
            Good_(addr, nTime);
            Check();
        }
    }

    // Mark an entry as connection attempted to.
    void Attempt(const CService &addr, int64 nTime = GetAdjustedTime())
    {
        {
            LOCK(cs);
            Check();
            Attempt_(addr, nTime);
            Check();
        }
    }

    // Choose an address to connect to.
    // nUnkBias determines how much "new" entries are favored over "tried" ones (0-100).
    CAddress Select(int nUnkBias = 50)
    {
        CAddress addrRet;
        {
            LOCK(cs);
            Check();
            addrRet = Select_(nUnkBias);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
            Check();
        }
        return addrRet;
    }

<<<<<<< HEAD
    //! Return a bunch of addresses, selected at random.
=======
    // Return a bunch of addresses, selected at random.
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    std::vector<CAddress> GetAddr()
    {
        Check();
        std::vector<CAddress> vAddr;
        {
            LOCK(cs);
            GetAddr_(vAddr);
        }
        Check();
        return vAddr;
    }

<<<<<<< HEAD
    //! Mark an entry as currently-connected-to.
    void Connected(const CService &addr, int64_t nTime = GetAdjustedTime())
    {
        LOCK(cs);
        Check();
        Connected_(addr, nTime);
        Check();
    }

    void SetServices(const CService &addr, ServiceFlags nServices)
    {
        LOCK(cs);
        Check();
        SetServices_(addr, nServices);
        Check();
    }

};

#endif // WIFICOIN_ADDRMAN_H
=======
    // Mark an entry as currently-connected-to.
    void Connected(const CService &addr, int64 nTime = GetAdjustedTime())
    {
        {
            LOCK(cs);
            Check();
            Connected_(addr, nTime);
            Check();
        }
    }
};

#endif
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
