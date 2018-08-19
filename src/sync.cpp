<<<<<<< HEAD
// Copyright (c) 2011-2016 The WiFicoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "sync.h"

#include "util.h"
#include "utilstrencodings.h"

#include <stdio.h>

#include <boost/thread.hpp>
=======
// Copyright (c) 2011-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "sync.h"
#include "util.h"

#include <boost/foreach.hpp>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

#ifdef DEBUG_LOCKCONTENTION
void PrintLockContention(const char* pszName, const char* pszFile, int nLine)
{
<<<<<<< HEAD
    LogPrintf("LOCKCONTENTION: %s\n", pszName);
    LogPrintf("Locker: %s:%d\n", pszFile, nLine);
=======
    printf("LOCKCONTENTION: %s\n", pszName);
    printf("Locker: %s:%d\n", pszFile, nLine);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}
#endif /* DEBUG_LOCKCONTENTION */

#ifdef DEBUG_LOCKORDER
//
// Early deadlock detection.
// Problem being solved:
//    Thread 1 locks  A, then B, then C
//    Thread 2 locks  D, then C, then A
//     --> may result in deadlock between the two threads, depending on when they run.
// Solution implemented here:
// Keep track of pairs of locks: (A before B), (A before C), etc.
// Complain if any thread tries to lock in a different order.
//

<<<<<<< HEAD
struct CLockLocation {
    CLockLocation(const char* pszName, const char* pszFile, int nLine, bool fTryIn)
=======
struct CLockLocation
{
    CLockLocation(const char* pszName, const char* pszFile, int nLine)
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    {
        mutexName = pszName;
        sourceFile = pszFile;
        sourceLine = nLine;
<<<<<<< HEAD
        fTry = fTryIn;
=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }

    std::string ToString() const
    {
<<<<<<< HEAD
        return mutexName + "  " + sourceFile + ":" + itostr(sourceLine) + (fTry ? " (TRY)" : "");
    }

    bool fTry;
=======
        return mutexName+"  "+sourceFile+":"+itostr(sourceLine);
    }

>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
private:
    std::string mutexName;
    std::string sourceFile;
    int sourceLine;
};

<<<<<<< HEAD
typedef std::vector<std::pair<void*, CLockLocation> > LockStack;
typedef std::map<std::pair<void*, void*>, LockStack> LockOrders;
typedef std::set<std::pair<void*, void*> > InvLockOrders;

struct LockData {
    // Very ugly hack: as the global constructs and destructors run single
    // threaded, we use this boolean to know whether LockData still exists,
    // as DeleteLock can get called by global CCriticalSection destructors
    // after LockData disappears.
    bool available;
    LockData() : available(true) {}
    ~LockData() { available = false; }

    LockOrders lockorders;
    InvLockOrders invlockorders;
    boost::mutex dd_mutex;
} static lockdata;

boost::thread_specific_ptr<LockStack> lockstack;

static void potential_deadlock_detected(const std::pair<void*, void*>& mismatch, const LockStack& s1, const LockStack& s2)
{
    LogPrintf("POTENTIAL DEADLOCK DETECTED\n");
    LogPrintf("Previous lock order was:\n");
    for (const std::pair<void*, CLockLocation> & i : s2) {
        if (i.first == mismatch.first) {
            LogPrintf(" (1)");
        }
        if (i.first == mismatch.second) {
            LogPrintf(" (2)");
        }
        LogPrintf(" %s\n", i.second.ToString());
    }
    LogPrintf("Current lock order is:\n");
    for (const std::pair<void*, CLockLocation> & i : s1) {
        if (i.first == mismatch.first) {
            LogPrintf(" (1)");
        }
        if (i.first == mismatch.second) {
            LogPrintf(" (2)");
        }
        LogPrintf(" %s\n", i.second.ToString());
    }
    assert(false);
=======
typedef std::vector< std::pair<void*, CLockLocation> > LockStack;

static boost::mutex dd_mutex;
static std::map<std::pair<void*, void*>, LockStack> lockorders;
static boost::thread_specific_ptr<LockStack> lockstack;


static void potential_deadlock_detected(const std::pair<void*, void*>& mismatch, const LockStack& s1, const LockStack& s2)
{
    printf("POTENTIAL DEADLOCK DETECTED\n");
    printf("Previous lock order was:\n");
    BOOST_FOREACH(const PAIRTYPE(void*, CLockLocation)& i, s2)
    {
        if (i.first == mismatch.first) printf(" (1)");
        if (i.first == mismatch.second) printf(" (2)");
        printf(" %s\n", i.second.ToString().c_str());
    }
    printf("Current lock order is:\n");
    BOOST_FOREACH(const PAIRTYPE(void*, CLockLocation)& i, s1)
    {
        if (i.first == mismatch.first) printf(" (1)");
        if (i.first == mismatch.second) printf(" (2)");
        printf(" %s\n", i.second.ToString().c_str());
    }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

static void push_lock(void* c, const CLockLocation& locklocation, bool fTry)
{
<<<<<<< HEAD
    if (lockstack.get() == nullptr)
        lockstack.reset(new LockStack);

    boost::unique_lock<boost::mutex> lock(lockdata.dd_mutex);

    (*lockstack).push_back(std::make_pair(c, locklocation));

    for (const std::pair<void*, CLockLocation> & i : (*lockstack)) {
        if (i.first == c)
            break;

        std::pair<void*, void*> p1 = std::make_pair(i.first, c);
        if (lockdata.lockorders.count(p1))
            continue;
        lockdata.lockorders[p1] = (*lockstack);

        std::pair<void*, void*> p2 = std::make_pair(c, i.first);
        lockdata.invlockorders.insert(p2);
        if (lockdata.lockorders.count(p2))
            potential_deadlock_detected(p1, lockdata.lockorders[p2], lockdata.lockorders[p1]);
    }
=======
    if (lockstack.get() == NULL)
        lockstack.reset(new LockStack);

    if (fDebug) printf("Locking: %s\n", locklocation.ToString().c_str());
    dd_mutex.lock();

    (*lockstack).push_back(std::make_pair(c, locklocation));

    if (!fTry) {
        BOOST_FOREACH(const PAIRTYPE(void*, CLockLocation)& i, (*lockstack)) {
            if (i.first == c) break;

            std::pair<void*, void*> p1 = std::make_pair(i.first, c);
            if (lockorders.count(p1))
                continue;
            lockorders[p1] = (*lockstack);

            std::pair<void*, void*> p2 = std::make_pair(c, i.first);
            if (lockorders.count(p2))
            {
                potential_deadlock_detected(p1, lockorders[p2], lockorders[p1]);
                break;
            }
        }
    }
    dd_mutex.unlock();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

static void pop_lock()
{
<<<<<<< HEAD
    (*lockstack).pop_back();
=======
    if (fDebug)
    {
        const CLockLocation& locklocation = (*lockstack).rbegin()->second;
        printf("Unlocked: %s\n", locklocation.ToString().c_str());
    }
    dd_mutex.lock();
    (*lockstack).pop_back();
    dd_mutex.unlock();
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void EnterCritical(const char* pszName, const char* pszFile, int nLine, void* cs, bool fTry)
{
<<<<<<< HEAD
    push_lock(cs, CLockLocation(pszName, pszFile, nLine, fTry), fTry);
=======
    push_lock(cs, CLockLocation(pszName, pszFile, nLine), fTry);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

void LeaveCritical()
{
    pop_lock();
}

<<<<<<< HEAD
std::string LocksHeld()
{
    std::string result;
    for (const std::pair<void*, CLockLocation> & i : *lockstack)
        result += i.second.ToString() + std::string("\n");
    return result;
}

void AssertLockHeldInternal(const char* pszName, const char* pszFile, int nLine, void* cs)
{
    for (const std::pair<void*, CLockLocation> & i : *lockstack)
        if (i.first == cs)
            return;
    fprintf(stderr, "Assertion failed: lock %s not held in %s:%i; locks held:\n%s", pszName, pszFile, nLine, LocksHeld().c_str());
    abort();
}

void DeleteLock(void* cs)
{
    if (!lockdata.available) {
        // We're already shutting down.
        return;
    }
    boost::unique_lock<boost::mutex> lock(lockdata.dd_mutex);
    std::pair<void*, void*> item = std::make_pair(cs, (void*)0);
    LockOrders::iterator it = lockdata.lockorders.lower_bound(item);
    while (it != lockdata.lockorders.end() && it->first.first == cs) {
        std::pair<void*, void*> invitem = std::make_pair(it->first.second, it->first.first);
        lockdata.invlockorders.erase(invitem);
        lockdata.lockorders.erase(it++);
    }
    InvLockOrders::iterator invit = lockdata.invlockorders.lower_bound(item);
    while (invit != lockdata.invlockorders.end() && invit->first == cs) {
        std::pair<void*, void*> invinvitem = std::make_pair(invit->second, invit->first);
        lockdata.lockorders.erase(invinvitem);
        lockdata.invlockorders.erase(invit++);
    }
}

=======
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
#endif /* DEBUG_LOCKORDER */
