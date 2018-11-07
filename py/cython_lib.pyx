# -*- coding: utf-8 -*-

from libc.math cimport sqrt

cpdef double f1_cy(int n):
    cdef double s = 0
    cdef int i = 0
    for i in range(n + 1):
        # s += i ** 0.5
        s += sqrt(i)
    return s

cdef extern from "native.h":
    struct V3:
        int x, y, z

    cpdef void hello_cpp()
    cpdef double f1_cpp(int n)
