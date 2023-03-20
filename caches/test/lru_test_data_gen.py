import os.path
from functools import lru_cache
import pathlib
import random

# M - max random cache size; N - max random number of values
MAX_M = 50
MAX_N = 500
MAX_VAL = 25
NUM_TESTS = 1000


def c_func(val):
    return val


def gen_random_list(max_len):
    res_list = []
    for _ in range(max_len):
        res_list.append(str(random.randint(1, MAX_VAL)))
    return res_list


if __name__ == "__main__":
    random.seed(0)
    cur_dir = pathlib.Path(__file__).parent.resolve() / "test_data"
    for data_path in [cur_dir / "test_data_lru.dat", cur_dir / "test_data_lru.ans"]:
        if os.path.exists(data_path):
            os.remove(data_path)

    for _ in range(NUM_TESTS):
        m = random.randint(1, MAX_M)
        n = random.randint(1, MAX_N)
        cache_func = lru_cache(m)(c_func)
        rand_vals = gen_random_list(n)
        for i in rand_vals:
            cache_func(i)
        with open(cur_dir / "test_data_lru.dat", "a") as f:
            f.write(f"{m} {n} {' '.join(rand_vals)}\n")
        with open(cur_dir / "test_data_lru.ans", "a") as f:
            f.write(f"{cache_func.cache_info().hits}\n")
