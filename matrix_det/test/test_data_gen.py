import numpy as np
import random
import pathlib
import os


Test_num = 500
Dim = 10
Int_min = -16
Int_max = 16

if __name__ == "__main__":
    cur_dir = pathlib.Path(__file__).parent.resolve() / "test_data"
    for data_path in [
        cur_dir / "test_data.dat",
        cur_dir / "test_data.ans",
    ]:
        if os.path.exists(data_path):
            os.remove(data_path)

    random.seed(0)
    np.random.seed(0)
    for _ in range(Test_num):
        cur_dim = random.randint(1, Dim)
        is_int = True
        if random.random() > 0.5:
            is_int = False

        if is_int:
            matr = np.random.randint(Int_min, Int_max, (cur_dim, cur_dim))
        else:
            matr = np.random.rand(cur_dim, cur_dim)

        det = np.linalg.det(matr)
        if det > 2147483648 or det < -2147483648:
            continue

        matr = matr.reshape(-1) * 1000000
        matr = np.floor(matr)
        matr /= 1000000

        if is_int:
            matr = matr.astype("int")
            det = int(det)

        matr_str = f" ".join(list(map(str, matr)))

        wr_t = "i" if is_int else "f"

        with open(cur_dir / "test_data.dat", "a") as f:
            f.write(f"{wr_t} {cur_dim} {matr_str}\n")

        with open(cur_dir / "test_data.ans", "a") as f:
            f.write(f"{wr_t} {det:.5f}\n")