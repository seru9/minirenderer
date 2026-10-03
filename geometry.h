#pragma once
#include <vector>
#include <assert.h>
#include <cmath>
#include <iostream>


template<typename T, int n> struct vec {
    T data[n] = {0};
    T& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
    T  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
};
template<typename T> struct vec<T, 3> {
    T x = 0, y = 0, z = 0;
    T& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    T  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
};

typedef vec<double, 3> double_vec3;
typedef vec<int, 3> int_vec3;

template<typename T, int n> double operator*(const vec<T, n>& lhs, const vec<T, n>& rhs) {
    T ret = 0;
    for (int i = 0; i < n; ++i) {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

template<typename T, int n> vec<T, n> operator+(const vec<T, n>& lhs, const vec<T, n>& rhs) {
    vec<T, n> ret = lhs;
    for (int i = 0; i < n; ++i) {
        ret[i] += rhs[i];
    }
    return ret;
}

template<typename T, int n> vec<T, n> operator-(const vec<T, n>& lhs, const vec<T, n>& rhs) {
    vec<T, n> ret = lhs;
    for (int i = 0; i < n; ++i) {
        ret[i] -= rhs[i];
    }
    return ret;
}

template<typename T, int n> vec<T, n> operator*(const vec<T, n>& lhs, const double& rhs) {
    vec<T, n> ret = lhs;
    for (int i = 0; i < n; ++i) {
        ret[i] *= rhs;
    }
    return ret;
}

template<typename T, int n> vec<T, n> operator*(const double& lhs, const vec<T, n> &rhs) {
    return rhs * lhs;
}

template<typename T, int n> vec<T, n> operator/(const vec<T, n>& lhs, const double& rhs) {
    vec<T, n> ret = lhs;
    for (int i = 0; i < n; ++i) {
        ret[i] /= rhs;
    }
    return ret;
}

template<typename T, int n> std::ostream& operator<<(std::ostream& out, const vec<T, n>& v) {
    for (int i=0; i<n; i++) out << v[i] << " ";
    return out;
}

template<typename T> struct vec<T, 2> {
    T x = 0, y = 0;
    T& operator[](const int i)       { assert(i>=0 && i<2); return i ? y : x; }
    T  operator[](const int i) const { assert(i>=0 && i<2); return i ? y : x; }
};

template<typename T> struct vec<T, 4> {
    T x = 0, y = 0, z = 0, w = 0;
    T& operator[](const int i)       { assert(i>=0 && i<4); return i<2 ? (i ? y : x) : (2==i ? z : w); }
    T  operator[](const int i) const { assert(i>=0 && i<4); return i<2 ? (i ? y : x) : (2==i ? z : w); }
    vec<T, 2> xy()  const { return {x, y};    }
    vec<T, 3> xyz() const { return {x, y, z}; }
};

typedef vec<double, 2> double_vec2;
typedef vec<double, 3> double_vec3;
typedef vec<double, 2> vec2;
typedef vec<double, 3> vec3;
typedef vec<double, 4> vec4;

template<typename T, int n> T norm(const vec<T, n>& v) {
    return std::sqrt(v*v);
}

template<typename T, int n> vec<T, n> normalized(const vec<T, n>& v) {
    return v / norm(v);
}

inline double_vec3 cross(const double_vec3 &v1, const double_vec3 &v2) {
    return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}
inline int_vec3 cross(const int_vec3 &v1, const int_vec3 &v2) {
    return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}

template<typename T, int n> struct dt;

template<typename T, int nrows,int ncols> struct mat {
    vec<T, ncols> rows[nrows] = {{}};

          vec<T, ncols>& operator[] (const int idx)       { assert(idx>=0 && idx<nrows); return rows[idx]; }
    const vec<T, ncols>& operator[] (const int idx) const { assert(idx>=0 && idx<nrows); return rows[idx]; }

    T det() const {
        return dt<T, ncols>::det(*this);
    }

    T cofactor(const int row, const int col) const {
        mat<T, nrows-1,ncols-1> submatrix;
        for (int i = 0; i < nrows - 1; ++i) {
            for (int j = 0; j < ncols - 1; ++j) {
                submatrix[i][j] = rows[i + int(i >= row)][j + int(j >= col)];
            }
        }
        return submatrix.det() * ((row + col) % 2 ? -1 : 1);
    }

    mat<T, nrows,ncols> invert_transpose() const {
        mat<T, nrows,ncols> adjugate_transpose; // transpose to ease determinant computation, check the last line
        for (int i = 0; i < nrows; ++i) {
            for (int j = 0; j < ncols; ++j) {
                adjugate_transpose[i][j] = cofactor(i, j);
            }
        }
        return adjugate_transpose / (adjugate_transpose[0] * rows[0]);
    }

    mat<T, nrows,ncols> invert() const {
        return invert_transpose().transpose();
    }

    mat<T, ncols,nrows> transpose() const {
        mat<T, ncols,nrows> ret;
        for (int i = 0; i < ncols; ++i) {
            for (int j = 0; j < nrows; ++j) {
                ret[i][j] = rows[j][i];
            }
        }
        return ret;
    }
};

template<typename T, int nrows,int ncols> vec<T, ncols> operator*(const vec<T, nrows>& lhs, const mat<T, nrows,ncols>& rhs) {
    return (mat<T, 1,nrows>{{lhs}}*rhs)[0];
}

template<typename T, int nrows,int ncols> vec<T, nrows> operator*(const mat<T, nrows,ncols>& lhs, const vec<T, ncols>& rhs) {
    vec<T, nrows> ret;
    for (int i = 0; i < nrows; ++i) {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template<typename T, int R1,int C1,int C2> mat<T, R1,C2> operator*(const mat<T, R1,C1>& lhs, const mat<T, C1,C2>& rhs) {
    mat<T, R1,C2> result;
    for (int i = 0; i < R1; ++i) {
        for (int j = 0; j < C2; ++j) {
            for (int k = 0; k < C1; ++k) {
                result[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return result;
}

template<typename T, int nrows,int ncols> mat<T, nrows,ncols> operator*(const mat<T, nrows,ncols>& lhs, const double& val) {
    mat<T, nrows,ncols> result;
    for (int i = 0; i < nrows; ++i) {
        result[i] = lhs[i] * val;
    }
    return result;
}

template<typename T, int nrows,int ncols> mat<T, nrows,ncols> operator/(const mat<T, nrows,ncols>& lhs, const double& val) {
    mat<T, nrows,ncols> result;
    for (int i = 0; i < nrows; ++i) {
        result[i] = lhs[i] / val;
    }
    return result;
}

template<typename T, int nrows,int ncols> mat<T, nrows,ncols> operator+(const mat<T, nrows,ncols>& lhs, const mat<T, nrows,ncols>& rhs) {
    mat<T, nrows,ncols> result;
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}

template<typename T, int nrows,int ncols> mat<T, nrows,ncols> operator-(const mat<T, nrows,ncols>& lhs, const mat<T, nrows,ncols>& rhs) {
    mat<T, nrows,ncols> result;
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
}

template<typename T, int nrows,int ncols> std::ostream& operator<<(std::ostream& out, const mat<T, nrows,ncols>& m) {
    for (int i=0; i<nrows; i++) out << m[i] << std::endl;
    return out;
}

template<typename T, int n> struct dt { // template metaprogramming to compute the determinant recursively
    static T det(const mat<T, n,n>& src) {
        T ret = 0;
        for (int i = 0; i < n; ++i) {
            ret += src[0][i] * src.cofactor(0, i);
        }
        return ret;
    }
};

template<typename T> struct dt<T, 1> {   // template specialization to stop the recursion
    static T det(const mat<T,1,1>& src) {
        return src[0][0];
    }
};
