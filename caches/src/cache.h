#pragma once
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <queue>

namespace caches
{
    template <typename T, typename KeyT = int>
    struct lru_cache
    {
        lru_cache(size_t sz) : sz_(sz) {}

        bool full() const { return (sz_ == cache_.size()); }

        template <typename F>
        bool lookup_update(KeyT key, F slow_get_page)
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                // page not found
                if (full())
                {
                    auto last = cache_.end();
                    last--;
                    hash_.erase(last->first);
                    cache_.pop_back();
                }
                std::pair<KeyT, T> new_page(key, slow_get_page(key));
                cache_.push_front(new_page);
                hash_[key] = cache_.begin();
                return false;
            }

            auto eltit = hit->second;
            if (eltit != cache_.begin())
            {
                cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
            }
            return true;
        }

        size_t sz_;
        using CacheT = typename std::list<std::pair<KeyT, T>>;
        using ListIt = typename CacheT::iterator;
        CacheT cache_;
        std::unordered_map<KeyT, ListIt> hash_;
    };

    template <typename T, typename KeyT = int>
    struct two_q_simple_cache
    {
        /*
            2q simplified caching.
            Idea from this site: https://arpitbhayani.me/blogs/2q-cache
        */

        two_q_simple_cache(size_t sz) : sz_(sz) {}

        template <typename F>
        bool lookup_update(KeyT key, F slow_get_page)
        {
            // 1. check if cached in LRU
            auto lru_hit = lru_hash_.find(key);
            if (lru_hit != lru_hash_.end())
            {
                auto eltit = lru_hit->second;
                if (eltit != lru_cache_.begin())
                {
                    lru_cache_.splice(lru_cache_.begin(), lru_cache_, eltit, std::next(eltit));
                }
                return true;
            }

            // 2. check if cached in In Q
            auto q_hit = q_hash_.find(key);
            if (q_hit != q_hash_.end())
            {
                // if lru_cache_ is full - delete last page
                if (sz_ == lru_cache_.size())
                {
                    auto last = lru_cache_.end();
                    last--;
                    lru_hash_.erase(last->first);
                    lru_cache_.pop_back();
                }
                // add page to LRU from Input queue
                lru_cache_.push_front(*(q_hit->second));
                lru_hash_[key] = lru_cache_.begin();

                // delete page from Input queue
                q_.erase(q_hit->second);
                q_hash_.erase(q_hit->first);
                return true;
            }

            // 3. if no cached
            // if q_ cache is full - delete last page
            if (sz_ == q_.size())
            {
                auto last = q_.end();
                last--;
                q_hash_.erase(last->first);
                q_.pop_back();
            }
            std::pair<KeyT, T> new_page(key, slow_get_page(key));
            q_.push_front(new_page);
            q_hash_[key] = q_.begin();
            return false;
        }

        size_t sz_;

        using ListT = typename std::list<std::pair<KeyT, T>>;
        using ListIt = typename ListT::iterator;
        using HashT = typename std::unordered_map<KeyT, ListIt>;

        // LRU cache
        ListT lru_cache_;
        HashT lru_hash_;

        // Input queue has also size sz_
        ListT q_;
        HashT q_hash_;
    };
}
