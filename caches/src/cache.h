#pragma once
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>

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
}

