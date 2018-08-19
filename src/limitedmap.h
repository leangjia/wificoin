<<<<<<< HEAD
// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WIFICOIN_LIMITEDMAP_H
#define WIFICOIN_LIMITEDMAP_H

#include <assert.h>
#include <map>

/** STL-like map container that only keeps the N elements with the highest value. */
template <typename K, typename V>
class limitedmap
=======
// Copyright (c) 2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_LIMITEDMAP_H
#define BITCOIN_LIMITEDMAP_H

#include <map>
#include <deque>

/** STL-like map container that only keeps the N elements with the highest value. */
template <typename K, typename V> class limitedmap
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
{
public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef typename std::map<K, V>::const_iterator const_iterator;
    typedef typename std::map<K, V>::size_type size_type;

protected:
    std::map<K, V> map;
    typedef typename std::map<K, V>::iterator iterator;
    std::multimap<V, iterator> rmap;
    typedef typename std::multimap<V, iterator>::iterator rmap_iterator;
    size_type nMaxSize;

public:
<<<<<<< HEAD
    limitedmap(size_type nMaxSizeIn)
    {
        assert(nMaxSizeIn > 0);
        nMaxSize = nMaxSizeIn;
    }
=======
    limitedmap(size_type nMaxSizeIn = 0) { nMaxSize = nMaxSizeIn; }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    const_iterator begin() const { return map.begin(); }
    const_iterator end() const { return map.end(); }
    size_type size() const { return map.size(); }
    bool empty() const { return map.empty(); }
    const_iterator find(const key_type& k) const { return map.find(k); }
    size_type count(const key_type& k) const { return map.count(k); }
    void insert(const value_type& x)
    {
        std::pair<iterator, bool> ret = map.insert(x);
<<<<<<< HEAD
        if (ret.second) {
            if (map.size() > nMaxSize) {
=======
        if (ret.second)
        {
            if (nMaxSize && map.size() == nMaxSize)
            {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                map.erase(rmap.begin()->second);
                rmap.erase(rmap.begin());
            }
            rmap.insert(make_pair(x.second, ret.first));
        }
<<<<<<< HEAD
=======
        return;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
    void erase(const key_type& k)
    {
        iterator itTarget = map.find(k);
        if (itTarget == map.end())
            return;
        std::pair<rmap_iterator, rmap_iterator> itPair = rmap.equal_range(itTarget->second);
        for (rmap_iterator it = itPair.first; it != itPair.second; ++it)
<<<<<<< HEAD
            if (it->second == itTarget) {
=======
            if (it->second == itTarget)
            {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                rmap.erase(it);
                map.erase(itTarget);
                return;
            }
        // Shouldn't ever get here
<<<<<<< HEAD
        assert(0);
    }
    void update(const_iterator itIn, const mapped_type& v)
    {
        // Using map::erase() with empty range instead of map::find() to get a non-const iterator,
        // since it is a constant time operation in C++11. For more details, see
        // https://stackoverflow.com/questions/765148/how-to-remove-constness-of-const-iterator
        iterator itTarget = map.erase(itIn, itIn);
        
=======
        assert(0); //TODO remove me
        map.erase(itTarget);
    }
    void update(const_iterator itIn, const mapped_type& v)
    {
        //TODO: When we switch to C++11, use map.erase(itIn, itIn) to get the non-const iterator
        iterator itTarget = map.find(itIn->first);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        if (itTarget == map.end())
            return;
        std::pair<rmap_iterator, rmap_iterator> itPair = rmap.equal_range(itTarget->second);
        for (rmap_iterator it = itPair.first; it != itPair.second; ++it)
<<<<<<< HEAD
            if (it->second == itTarget) {
=======
            if (it->second == itTarget)
            {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
                rmap.erase(it);
                itTarget->second = v;
                rmap.insert(make_pair(v, itTarget));
                return;
            }
        // Shouldn't ever get here
<<<<<<< HEAD
        assert(0);
=======
        assert(0); //TODO remove me
        itTarget->second = v;
        rmap.insert(make_pair(v, itTarget));
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
    }
    size_type max_size() const { return nMaxSize; }
    size_type max_size(size_type s)
    {
<<<<<<< HEAD
        assert(s > 0);
        while (map.size() > s) {
            map.erase(rmap.begin()->second);
            rmap.erase(rmap.begin());
        }
=======
        if (s)
            while (map.size() > s)
            {
                map.erase(rmap.begin()->second);
                rmap.erase(rmap.begin());
            }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
        nMaxSize = s;
        return nMaxSize;
    }
};

<<<<<<< HEAD
#endif // WIFICOIN_LIMITEDMAP_H
=======
#endif
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
