import random
import pathlib
import os


def min_stat(num, l):
    sort_l = list(sorted(l))
    assert len(sort_l) > num > 0
    return sort_l[num - 1]


def cnt_less(val, l):
    sort_l = list(sorted(l))
    cnt = 0
    for k in sort_l:
        if val > k:
            cnt += 1
        else:
            break
    return cnt


Test_num = 1000
K_num = 4096
K_max_val = 1000
M_num = 100
N_num = 100

if __name__ == "__main__":
    cur_dir = pathlib.Path(__file__).parent.resolve() / "test_data"
    for data_path in [
        cur_dir / "test_data.dat",
        cur_dir / "test_data.ans",
    ]:
        if os.path.exists(data_path):
            os.remove(data_path)

    random.seed(0)
    for _ in range(Test_num):
        key_l = []
        m = []
        ans_m = []
        n = []
        ans_n = []

        for _ in range(K_num):
            key_l.append(random.randint(-K_max_val, K_max_val))
        key_l = list(set(key_l))
        random.shuffle(key_l)

        s_key_l = list(sorted(key_l))

        for _ in range(M_num):
            stat_num = random.randint(1, len(key_l) - 1)
            m.append(stat_num)
            ans_m.append(min_stat(stat_num, key_l))

        for _ in range(N_num):
            val = random.randint(-K_max_val, K_max_val)
            n.append(val)
            ans_n.append(cnt_less(val, key_l))

        with open(cur_dir / "test_data.dat", "a") as f:
            k_str = "k " + " k ".join(list(map(str, key_l)))
            m_str = "m " + " m ".join(list(map(str, m)))
            n_str = "n " + " n ".join(list(map(str, n)))

            f.write(f"{k_str} {m_str} {n_str}\n")

        with open(cur_dir / "test_data.ans", "a") as f:
            ans_1 = " ".join(list(map(str, ans_m)))
            ans_2 = " ".join(list(map(str, ans_n)))

            f.write(f"{ans_1} {ans_2}\n")