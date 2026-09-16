#pragma once
#include <vector>
#include <assert.h>
#include <iostream>


template<typename T, int n> struct vec {
    T data[n] = {0};
    T& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
    T  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
};
template<typename T, int n> std::ostream& operator<<(std::ostream& out, const vec<T, n>& v) {
    for (int i=0; i<n; i++) out << v[i] << " ";
    return out;
}
template<typename T> struct vec<T, 3> {
    T x = 0, y = 0, z = 0;
    T& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    T  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
};

typedef vec<double, 3> double_vec3;
typedef vec<int, 3> int_vec3;