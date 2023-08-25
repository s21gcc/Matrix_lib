#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : rows_(0), cols_(0), matrix_(nullptr) {}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ > 0 && cols_ > 0) {
    AllocatingMemory();
  } else {
    throw std::logic_error("Wrong arguments");
  }
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  if (rows_ > 0 && cols_ > 0) {
    AllocatingMemory();
    CopyMatrixData(other);
  } else {
    matrix_ = nullptr;
  }
}

S21Matrix::S21Matrix(S21Matrix &&other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

S21Matrix::~S21Matrix() { DeleteMatrix(); }

int S21Matrix::get_rows() const { return rows_; }

int S21Matrix::get_cols() const { return cols_; }

void S21Matrix::set_cols(int cols) {
  if (cols <= 0) throw std::logic_error("Wrong argument");
  if (cols_ != cols) {
    S21Matrix temp_matrix(rows_, cols);
    int min_cols = std::min(cols_, cols);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < min_cols; ++j) {
        temp_matrix.matrix_[i][j] = matrix_[i][j];
      }
    }
    *this = std::move(temp_matrix);
  }
}

void S21Matrix::set_rows(int rows) {
  if (rows <= 0) throw std::logic_error("Wrong argument");
  if (rows_ != rows) {
    S21Matrix temp_matrix(rows, cols_);
    int min_rows = std::min(rows, rows_);
    for (int i = 0; i < min_rows; ++i) {
      for (int j = 0; j < cols_; ++j) {
        temp_matrix.matrix_[i][j] = matrix_[i][j];
      }
    }
    *this = std::move(temp_matrix);
  }
}

bool S21Matrix::EqMatrix(const S21Matrix &other) {
  bool result = true;
  if ((rows_ != other.rows_) || (cols_ != other.cols_)) {
    result = false;
  } else {
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        if (fabs(matrix_[i][k] - other.matrix_[i][k]) > FLT_EPSILON) {
          result = false;
        }
      }
    }
  }
  return result;
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("different matrix dimensions");
  } else {
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        matrix_[i][k] += other.matrix_[i][k];
        if (std::isnan(matrix_[i][k]) || std::isinf(matrix_[i][k]))
          throw std::overflow_error("overflow of double");
      }
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("different matrix dimensions");
  } else {
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        matrix_[i][k] -= other.matrix_[i][k];
        if (std::isnan(matrix_[i][k]) || std::isinf(matrix_[i][k]))
          throw std::overflow_error("overflow of double");
      }
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      matrix_[i][k] *= num;
      if (std::isnan(matrix_[i][k]) || std::isinf(matrix_[i][k]))
        throw std::overflow_error("overflow of double");
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) {
    throw std::logic_error(
        "the number of columns of the first matrix is not equal to the number "
        "of rows of the second matrix");
  } else {
    S21Matrix temp(rows_, other.cols_);
    for (int i = 0; i < temp.rows_; ++i) {
      for (int j = 0; j < temp.cols_; ++j) {
        for (int k = 0; k < other.rows_; ++k) {
          temp.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
          if (std::isnan(temp.matrix_[i][j]) || std::isinf(temp.matrix_[i][j]))
            throw std::overflow_error("overflow of double");
        }
      }
    }
    *this = std::move(temp);
  }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix result_matrix(cols_, rows_);
  for (int i = 0; i < result_matrix.rows_; ++i) {
    for (int j = 0; j < result_matrix.cols_; ++j) {
      result_matrix.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result_matrix;
}

S21Matrix S21Matrix::CalcComplements() {
  if (rows_ != cols_) {
    throw std::logic_error("Matrix is not square");
  }
  if (rows_ == 1) {
    return *this;
  }
  S21Matrix result_matrix(rows_, cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      double determinant = 0;
      double complement = 0;
      S21Matrix minor = this->FillMinor(i, j);
      determinant = minor.Determinant();
      complement = determinant * pow(-1, i + j);
      result_matrix.matrix_[i][j] = complement;
    }
  }
  return result_matrix;
}

double S21Matrix::Determinant() {
  double result = 0;
  if (rows_ != cols_) {
    throw std::logic_error("Matrix is not square");
  } else if (rows_ == 1) {
    result = matrix_[0][0];
  } else if (rows_ == 2) {
    result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    for (int i = 0; i < cols_; ++i) {
      double determinant = 0;
      S21Matrix minor = this->FillMinor(0, i);
      determinant = minor.Determinant();
      result = result + (matrix_[0][i] * determinant * pow(-1, i));
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() {
  double determinant = 0;
  if (rows_ != cols_) {
    throw std::logic_error("Matrix is not square");
  }
  determinant = this->Determinant();
  if (determinant == 0) {
    throw std::logic_error("Determinant is zero");
  }
  S21Matrix result_matrix(*this);
  if (rows_ == 1) {
    result_matrix.matrix_[0][0] = 1 / determinant;
  } else {
    result_matrix = result_matrix.CalcComplements();
    result_matrix = result_matrix.Transpose();
    result_matrix.MulNumber(1 / determinant);
  }
  return result_matrix;
}

bool S21Matrix::operator==(const S21Matrix &other) { return EqMatrix(other); }

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this != &other) {
    S21Matrix temp(other);

    std::swap(rows_, temp.rows_);
    std::swap(cols_, temp.cols_);
    std::swap(matrix_, temp.matrix_);
  }
  return *this;
}

S21Matrix &S21Matrix::operator=(S21Matrix &&other) noexcept {
  if (this != &other) {
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    std::swap(matrix_, other.matrix_);

    other.DeleteMatrix();

    other.rows_ = 0;
    other.cols_ = 0;
    other.matrix_ = nullptr;
  }
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix result_matrix(*this);
  result_matrix.SumMatrix(other);
  return result_matrix;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix result_matrix(*this);
  result_matrix.SubMatrix(other);
  return result_matrix;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  S21Matrix result_matrix(*this);
  result_matrix.MulMatrix(other);
  return result_matrix;
}

S21Matrix S21Matrix::operator*(double x) {
  S21Matrix result_matrix(*this);
  result_matrix.MulNumber(x);
  return result_matrix;
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(double x) {
  MulNumber(x);
  return *this;
}

double &S21Matrix::operator()(int i, int j) {
  if (i >= rows_ || j >= cols_ || i < 0 || j < 0) {
    throw std::logic_error("index is outside the matrix or invalid");
  }
  return matrix_[i][j];
}

double S21Matrix::operator()(int i, int j) const {
  if (i >= rows_ || j >= cols_ || i < 0 || j < 0) {
    throw std::logic_error("index is outside the matrix or invalid");
  }
  return matrix_[i][j];
}

void S21Matrix::CopyMatrixData(const S21Matrix &other) {
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      matrix_[i][k] = other.matrix_[i][k];
    }
  }
}

void S21Matrix::AllocatingMemory() {
  matrix_ = new double *[rows_]();
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_]();
  }
}

void S21Matrix::DeleteMatrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
    rows_ = 0;
    cols_ = 0;
  }
}

S21Matrix S21Matrix::FillMinor(int delete_row, int delete_column) {
  int minor_row = 0;
  int minor_column = 0;
  S21Matrix minor(rows_ - 1, cols_ - 1);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (i != delete_row && j != delete_column) {
        minor.matrix_[minor_row][minor_column] = matrix_[i][j];
        if (minor_column + 1 >= minor.cols_) {
          ++minor_row;
          minor_column = 0;
        } else {
          ++minor_column;
        }
      }
    }
  }
  return minor;
}