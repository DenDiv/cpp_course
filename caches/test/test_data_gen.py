import os.path
from functools import lru_cache
import pathlib
import random
from omegaconf import OmegaConf

# M - max random cache size; N - max random number of values
MAX_M = 50
MAX_N = 500
MAX_VAL = 25
NUM_TESTS = 1000


def c_func(val):
    return val


def two_q_simple_cache(cache_size):
    lru_cache = []
    lru_cache_size = cache_size
    in_q = []
    in_q_size = cache_size
    info = {"hits": 0}

    def cache_info():
        return OmegaConf.create(info)

    def inner(val):
        if val in lru_cache:
            lru_cache.remove(val)
            lru_cache.insert(0, val)
            info["hits"] += 1
            return lru_cache[0]
        elif val in in_q:
            in_q.remove(val)
            if len(lru_cache) == lru_cache_size:
                lru_cache.pop()
            lru_cache.insert(0, val)
            info["hits"] += 1
            return lru_cache[0]
        else:
            if len(in_q) == in_q_size:
                in_q.pop()
            in_q.insert(0, val)
            return in_q[0]

    inner.cache_info = cache_info
    return inner


def gen_random_list(max_len):
    res_list = []
    for _ in range(max_len):
        res_list.append(str(random.randint(1, MAX_VAL)))
    return res_list


if __name__ == "__main__":
    random.seed(0)
    cur_dir = pathlib.Path(__file__).parent.resolve() / "test_data"
    for data_path in [
        cur_dir / "test_data.dat",
        cur_dir / "test_data_lru.ans",
        cur_dir / "test_data_2q.ans",
        cur_dir / "test_data_best.ans",
    ]:
        if os.path.exists(data_path):
            os.remove(data_path)

    for _ in range(NUM_TESTS):
        m = random.randint(1, MAX_M)
        n = random.randint(1, MAX_N)
        cache_func_lru = lru_cache(m)(c_func)
        cache_func_2q = two_q_simple_cache(m)
        best_cache = {"values": set(), "hits": 0}
        rand_vals = gen_random_list(n)
        for i in rand_vals:
            cache_func_lru(i)
            cache_func_2q(i)
            if i in best_cache["values"]:
                best_cache["hits"] += 1
            best_cache["values"].add(i)

        with open(cur_dir / "test_data.dat", "a") as f:
            f.write(f"{m} {n} {' '.join(rand_vals)}\n")
        with open(cur_dir / "test_data_lru.ans", "a") as f:
            f.write(f"{cache_func_lru.cache_info().hits}\n")
        with open(cur_dir / "test_data_2q.ans", "a") as f:
            f.write(f"{cache_func_2q.cache_info().hits}\n")
        with open(cur_dir / "test_data_best.ans", "a") as f:
            f.write(f"{best_cache['hits']}\n")
