<<<<<<< HEAD
// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "util.h"

#include "support/allocators/secure.h"
#include "test/test_wificoin.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(allocator_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(arena_tests)
{
    // Fake memory base address for testing
    // without actually using memory.
    void *synth_base = reinterpret_cast<void*>(0x08000000);
    const size_t synth_size = 1024*1024;
    Arena b(synth_base, synth_size, 16);
    void *chunk = b.alloc(1000);
#ifdef ARENA_DEBUG
    b.walk();
#endif
    BOOST_CHECK(chunk != nullptr);
    BOOST_CHECK(b.stats().used == 1008); // Aligned to 16
    BOOST_CHECK(b.stats().total == synth_size); // Nothing has disappeared?
    b.free(chunk);
#ifdef ARENA_DEBUG
    b.walk();
#endif
    BOOST_CHECK(b.stats().used == 0);
    BOOST_CHECK(b.stats().free == synth_size);
    try { // Test exception on double-free
        b.free(chunk);
        BOOST_CHECK(0);
    } catch(std::runtime_error &)
    {
    }

    void *a0 = b.alloc(128);
    void *a1 = b.alloc(256);
    void *a2 = b.alloc(512);
    BOOST_CHECK(b.stats().used == 896);
    BOOST_CHECK(b.stats().total == synth_size);
#ifdef ARENA_DEBUG
    b.walk();
#endif
    b.free(a0);
#ifdef ARENA_DEBUG
    b.walk();
#endif
    BOOST_CHECK(b.stats().used == 768);
    b.free(a1);
    BOOST_CHECK(b.stats().used == 512);
    void *a3 = b.alloc(128);
#ifdef ARENA_DEBUG
    b.walk();
#endif
    BOOST_CHECK(b.stats().used == 640);
    b.free(a2);
    BOOST_CHECK(b.stats().used == 128);
    b.free(a3);
    BOOST_CHECK(b.stats().used == 0);
    BOOST_CHECK_EQUAL(b.stats().chunks_used, 0);
    BOOST_CHECK(b.stats().total == synth_size);
    BOOST_CHECK(b.stats().free == synth_size);
    BOOST_CHECK_EQUAL(b.stats().chunks_free, 1);

    std::vector<void*> addr;
    BOOST_CHECK(b.alloc(0) == nullptr); // allocating 0 always returns nullptr
#ifdef ARENA_DEBUG
    b.walk();
#endif
    // Sweeping allocate all memory
    for (int x=0; x<1024; ++x)
        addr.push_back(b.alloc(1024));
    BOOST_CHECK(b.stats().free == 0);
    BOOST_CHECK(b.alloc(1024) == nullptr); // memory is full, this must return nullptr
    BOOST_CHECK(b.alloc(0) == nullptr);
    for (int x=0; x<1024; ++x)
        b.free(addr[x]);
    addr.clear();
    BOOST_CHECK(b.stats().total == synth_size);
    BOOST_CHECK(b.stats().free == synth_size);

    // Now in the other direction...
    for (int x=0; x<1024; ++x)
        addr.push_back(b.alloc(1024));
    for (int x=0; x<1024; ++x)
        b.free(addr[1023-x]);
    addr.clear();

    // Now allocate in smaller unequal chunks, then deallocate haphazardly
    // Not all the chunks will succeed allocating, but freeing nullptr is
    // allowed so that is no problem.
    for (int x=0; x<2048; ++x)
        addr.push_back(b.alloc(x+1));
    for (int x=0; x<2048; ++x)
        b.free(addr[((x*23)%2048)^242]);
    addr.clear();

    // Go entirely wild: free and alloc interleaved,
    // generate targets and sizes using pseudo-randomness.
    for (int x=0; x<2048; ++x)
        addr.push_back(0);
    uint32_t s = 0x12345678;
    for (int x=0; x<5000; ++x) {
        int idx = s & (addr.size()-1);
        if (s & 0x80000000) {
            b.free(addr[idx]);
            addr[idx] = 0;
        } else if(!addr[idx]) {
            addr[idx] = b.alloc((s >> 16) & 2047);
        }
        bool lsb = s & 1;
        s >>= 1;
        if (lsb)
            s ^= 0xf00f00f0; // LFSR period 0xf7ffffe0
    }
    for (void *ptr: addr)
        b.free(ptr);
    addr.clear();

    BOOST_CHECK(b.stats().total == synth_size);
    BOOST_CHECK(b.stats().free == synth_size);
}

/** Mock LockedPageAllocator for testing */
class TestLockedPageAllocator: public LockedPageAllocator
{
public:
    TestLockedPageAllocator(int count_in, int lockedcount_in): count(count_in), lockedcount(lockedcount_in) {}
    void* AllocateLocked(size_t len, bool *lockingSuccess) override
    {
        *lockingSuccess = false;
        if (count > 0) {
            --count;

            if (lockedcount > 0) {
                --lockedcount;
                *lockingSuccess = true;
            }

            return reinterpret_cast<void*>(0x08000000 + (count<<24)); // Fake address, do not actually use this memory
        }
        return 0;
    }
    void FreeLocked(void* addr, size_t len) override
    {
    }
    size_t GetLimit() override
    {
        return std::numeric_limits<size_t>::max();
    }
private:
    int count;
    int lockedcount;
};

BOOST_AUTO_TEST_CASE(lockedpool_tests_mock)
{
    // Test over three virtual arenas, of which one will succeed being locked
    std::unique_ptr<LockedPageAllocator> x(new TestLockedPageAllocator(3, 1));
    LockedPool pool(std::move(x));
    BOOST_CHECK(pool.stats().total == 0);
    BOOST_CHECK(pool.stats().locked == 0);

    // Ensure unreasonable requests are refused without allocating anything
    void *invalid_toosmall = pool.alloc(0);
    BOOST_CHECK(invalid_toosmall == nullptr);
    BOOST_CHECK(pool.stats().used == 0);
    BOOST_CHECK(pool.stats().free == 0);
    void *invalid_toobig = pool.alloc(LockedPool::ARENA_SIZE+1);
    BOOST_CHECK(invalid_toobig == nullptr);
    BOOST_CHECK(pool.stats().used == 0);
    BOOST_CHECK(pool.stats().free == 0);

    void *a0 = pool.alloc(LockedPool::ARENA_SIZE / 2);
    BOOST_CHECK(a0);
    BOOST_CHECK(pool.stats().locked == LockedPool::ARENA_SIZE);
    void *a1 = pool.alloc(LockedPool::ARENA_SIZE / 2);
    BOOST_CHECK(a1);
    void *a2 = pool.alloc(LockedPool::ARENA_SIZE / 2);
    BOOST_CHECK(a2);
    void *a3 = pool.alloc(LockedPool::ARENA_SIZE / 2);
    BOOST_CHECK(a3);
    void *a4 = pool.alloc(LockedPool::ARENA_SIZE / 2);
    BOOST_CHECK(a4);
    void *a5 = pool.alloc(LockedPool::ARENA_SIZE / 2);
    BOOST_CHECK(a5);
    // We've passed a count of three arenas, so this allocation should fail
    void *a6 = pool.alloc(16);
    BOOST_CHECK(!a6);

    pool.free(a0);
    pool.free(a2);
    pool.free(a4);
    pool.free(a1);
    pool.free(a3);
    pool.free(a5);
    BOOST_CHECK(pool.stats().total == 3*LockedPool::ARENA_SIZE);
    BOOST_CHECK(pool.stats().locked == LockedPool::ARENA_SIZE);
    BOOST_CHECK(pool.stats().used == 0);
}

// These tests used the live LockedPoolManager object, this is also used
// by other tests so the conditions are somewhat less controllable and thus the
// tests are somewhat more error-prone.
BOOST_AUTO_TEST_CASE(lockedpool_tests_live)
{
    LockedPoolManager &pool = LockedPoolManager::Instance();
    LockedPool::Stats initial = pool.stats();

    void *a0 = pool.alloc(16);
    BOOST_CHECK(a0);
    // Test reading and writing the allocated memory
    *((uint32_t*)a0) = 0x1234;
    BOOST_CHECK(*((uint32_t*)a0) == 0x1234);

    pool.free(a0);
    try { // Test exception on double-free
        pool.free(a0);
        BOOST_CHECK(0);
    } catch(std::runtime_error &)
    {
    }
    // If more than one new arena was allocated for the above tests, something is wrong
    BOOST_CHECK(pool.stats().total <= (initial.total + LockedPool::ARENA_SIZE));
    // Usage must be back to where it started
    BOOST_CHECK(pool.stats().used == initial.used);
=======
#include <boost/test/unit_test.hpp>

#include "init.h"
#include "main.h"
#include "util.h"

BOOST_AUTO_TEST_SUITE(allocator_tests)

// Dummy memory page locker for platform independent tests
static const void *last_lock_addr, *last_unlock_addr;
static size_t last_lock_len, last_unlock_len;
class TestLocker
{
public:
    bool Lock(const void *addr, size_t len)
    {
        last_lock_addr = addr;
        last_lock_len = len;
        return true;
    }
    bool Unlock(const void *addr, size_t len)
    {
        last_unlock_addr = addr;
        last_unlock_len = len;
        return true;
    }
};

BOOST_AUTO_TEST_CASE(test_LockedPageManagerBase)
{
    const size_t test_page_size = 4096;
    LockedPageManagerBase<TestLocker> lpm(test_page_size);
    size_t addr;
    last_lock_addr = last_unlock_addr = 0;
    last_lock_len = last_unlock_len = 0;

    /* Try large number of small objects */
    addr = 0;
    for(int i=0; i<1000; ++i)
    {
        lpm.LockRange(reinterpret_cast<void*>(addr), 33);
        addr += 33;
    }
    /* Try small number of page-sized objects, straddling two pages */
    addr = test_page_size*100 + 53;
    for(int i=0; i<100; ++i)
    {
        lpm.LockRange(reinterpret_cast<void*>(addr), test_page_size);
        addr += test_page_size;
    }
    /* Try small number of page-sized objects aligned to exactly one page */
    addr = test_page_size*300;
    for(int i=0; i<100; ++i)
    {
        lpm.LockRange(reinterpret_cast<void*>(addr), test_page_size);
        addr += test_page_size;
    }
    /* one very large object, straddling pages */
    lpm.LockRange(reinterpret_cast<void*>(test_page_size*600+1), test_page_size*500);
    BOOST_CHECK(last_lock_addr == reinterpret_cast<void*>(test_page_size*(600+500)));
    /* one very large object, page aligned */
    lpm.LockRange(reinterpret_cast<void*>(test_page_size*1200), test_page_size*500-1);
    BOOST_CHECK(last_lock_addr == reinterpret_cast<void*>(test_page_size*(1200+500-1)));

    BOOST_CHECK(lpm.GetLockedPageCount() == (
        (1000*33+test_page_size-1)/test_page_size + // small objects
        101 + 100 +  // page-sized objects
        501 + 500)); // large objects
    BOOST_CHECK((last_lock_len & (test_page_size-1)) == 0); // always lock entire pages
    BOOST_CHECK(last_unlock_len == 0); // nothing unlocked yet

    /* And unlock again */
    addr = 0;
    for(int i=0; i<1000; ++i)
    {
        lpm.UnlockRange(reinterpret_cast<void*>(addr), 33);
        addr += 33;
    }
    addr = test_page_size*100 + 53;
    for(int i=0; i<100; ++i)
    {
        lpm.UnlockRange(reinterpret_cast<void*>(addr), test_page_size);
        addr += test_page_size;
    }
    addr = test_page_size*300;
    for(int i=0; i<100; ++i)
    {
        lpm.UnlockRange(reinterpret_cast<void*>(addr), test_page_size);
        addr += test_page_size;
    }
    lpm.UnlockRange(reinterpret_cast<void*>(test_page_size*600+1), test_page_size*500);
    lpm.UnlockRange(reinterpret_cast<void*>(test_page_size*1200), test_page_size*500-1);

    /* Check that everything is released */
    BOOST_CHECK(lpm.GetLockedPageCount() == 0);

    /* A few and unlocks of size zero (should have no effect) */
    addr = 0;
    for(int i=0; i<1000; ++i)
    {
        lpm.LockRange(reinterpret_cast<void*>(addr), 0);
        addr += 1;
    }
    BOOST_CHECK(lpm.GetLockedPageCount() == 0);
    addr = 0;
    for(int i=0; i<1000; ++i)
    {
        lpm.UnlockRange(reinterpret_cast<void*>(addr), 0);
        addr += 1;
    }
    BOOST_CHECK(lpm.GetLockedPageCount() == 0);
    BOOST_CHECK((last_unlock_len & (test_page_size-1)) == 0); // always unlock entire pages
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
}

BOOST_AUTO_TEST_SUITE_END()
