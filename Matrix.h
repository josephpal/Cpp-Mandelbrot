#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>

template <class T>
struct RGB
{
    T r, g, b;
};

template <class T>
class Matrix {
  public:
    Matrix(const size_t rows, const size_t cols) : _rows(rows), _cols(cols) {
        _matrix = new T *[rows];
        for (size_t i = 0; i < rows; ++i)
        {
            _matrix[i] = new T[cols];
        }
    }

    Matrix(const Matrix &m) : _rows(m._rows), _cols(m._cols) {
        _matrix = new T *[m._rows];

        for (size_t i = 0; i < m._rows; ++i) {
            _matrix[i] = new T[m._cols];

            for (size_t j = 0; j < m._cols; ++j) {
                _matrix[i][j] = m._matrix[i][j];
            }
        }
    }

    ~Matrix() {
        for (size_t i = 0; i < _rows; ++i) {
            delete[] _matrix[i];
        }
        delete[] _matrix;
    }

    T *operator[](const size_t nIndex) {
        return _matrix[nIndex];
    }

    size_t width() const { return _cols; }

    size_t height() const { return _rows; }

  protected:
    size_t _rows, _cols;
    T **_matrix;
};

#endif /* MATRIX_H_ */
