# -*- coding: utf-8 -*-

from timeit import timeit
import os
import pyximport

pyximport.install(setup_args={"include_dirs": [os.getcwd()]})

from cython_lib import f1_cy, f1_cpp, hello_cpp


def f1_py(n):
    s = 0
    for i in range(n + 1):
        s += i ** 0.5
    return s


if __name__ == "__main__":
    # pure python
    print(f1_py(10000))
    print(timeit("f1_py(10000)", number=1000, globals=globals()))

    # cython
    print(f1_cy(10000))
    print(timeit("f1_cy(10000)", number=1000, globals=globals()))

    # cpp
    hello_cpp()
    print(f1_cpp(10000))
    print(timeit("f1_cpp(10000)", number=1000, globals=globals()))
