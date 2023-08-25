#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

class MatrixTest : public testing::Test {
 protected:
  S21Matrix *matrix_1x1;
  S21Matrix *matrix_2x2;
  S21Matrix *matrix_3x3;
  S21Matrix *matrix_2x3;

  void SetUp() {
    matrix_1x1 = new S21Matrix(1, 1);
    matrix_2x2 = new S21Matrix(2, 2);
    matrix_2x3 = new S21Matrix(2, 3);  //
    matrix_3x3 = new S21Matrix(3, 3);

    matrix_1x1->operator()(0, 0) = DBL_MAX;

    matrix_2x2->operator()(0, 0) = 1;
    matrix_2x2->operator()(0, 1) = 2;
    matrix_2x2->operator()(1, 0) = 3;
    matrix_2x2->operator()(1, 1) = 4;

    matrix_2x3->operator()(0, 0) = 5.21;
    matrix_2x3->operator()(0, 1) = 2.134;
    matrix_2x3->operator()(0, 2) = 1.532;
    matrix_2x3->operator()(1, 0) = 2145.12;
    matrix_2x3->operator()(1, 1) = 573;
    matrix_2x3->operator()(1, 2) = 0;

    matrix_3x3->operator()(0, 0) = 1;
    matrix_3x3->operator()(0, 1) = 2;
    matrix_3x3->operator()(0, 2) = 3;
    matrix_3x3->operator()(1, 0) = 0;
    matrix_3x3->operator()(1, 1) = 4;
    matrix_3x3->operator()(1, 2) = 2;
    matrix_3x3->operator()(2, 0) = 5;
    matrix_3x3->operator()(2, 1) = 2;
    matrix_3x3->operator()(2, 2) = 1;
  }

  void TearDown() {
    delete matrix_1x1;
    delete matrix_2x2;
    delete matrix_2x3;
    delete matrix_3x3;
  }
};

TEST_F(MatrixTest, ConstructorDefault) {
  S21Matrix test;
  EXPECT_EQ(test.get_cols(), 0);
  EXPECT_EQ(test.get_rows(), 0);
}

TEST_F(MatrixTest, ConstructorExeption1) {
  EXPECT_THROW(S21Matrix test(3, 0), std::logic_error);
}

TEST_F(MatrixTest, ConstructorExeption2) {
  EXPECT_THROW(S21Matrix test(-1, 2), std::logic_error);
}

TEST_F(MatrixTest, ConstructorParametrized) {
  S21Matrix test(2, 3);
  EXPECT_EQ(test.get_cols(), 3);
  EXPECT_EQ(test.get_rows(), 2);
}

TEST_F(MatrixTest, ConstructorCopy) {
  S21Matrix test(*matrix_2x2);
  EXPECT_EQ(test.get_cols(), 2);
  EXPECT_EQ(test.get_rows(), 2);
  EXPECT_EQ(matrix_2x2->get_rows(), 2);
}

TEST_F(MatrixTest, ConstructorMove) {
  S21Matrix test(std::move(*matrix_2x2));
  EXPECT_EQ(test.get_cols(), 2);
  EXPECT_EQ(test.get_rows(), 2);
  EXPECT_EQ(matrix_2x2->get_rows(), 0);
}

TEST_F(MatrixTest, SetCols1) {
  matrix_2x2->set_cols(3);
  EXPECT_EQ(matrix_2x2->get_cols(), 3);
  EXPECT_EQ(matrix_2x2->operator()(1, 2), 0);
}

TEST_F(MatrixTest, SetCols2) {
  matrix_2x2->set_cols(1);
  EXPECT_EQ(matrix_2x2->get_cols(), 1);
}

TEST_F(MatrixTest, SetColsExeption) {
  EXPECT_THROW(matrix_2x2->set_cols(-1), std::logic_error);
}

TEST_F(MatrixTest, SetRows1) {
  matrix_3x3->set_rows(3);
  EXPECT_EQ(matrix_3x3->get_rows(), 3);
  EXPECT_EQ(matrix_3x3->operator()(2, 2), 1);
}

TEST_F(MatrixTest, SetRows2) {
  matrix_3x3->set_rows(2);
  EXPECT_EQ(matrix_3x3->get_rows(), 2);
}

TEST_F(MatrixTest, SetRowsExeption) {
  EXPECT_THROW(matrix_3x3->set_rows(0), std::logic_error);
}

TEST_F(MatrixTest, EqMatrix) {
  S21Matrix test(2, 2);
  EXPECT_EQ(matrix_3x3->EqMatrix(*matrix_3x3), 1);
  EXPECT_EQ(matrix_3x3->EqMatrix(*matrix_2x3), 0);
  EXPECT_EQ(test.EqMatrix(*matrix_2x2), 0);
}

TEST_F(MatrixTest, EqMatrixOverload) {
  S21Matrix test(2, 2);
  EXPECT_EQ(*matrix_3x3 == *matrix_3x3, 1);
  EXPECT_EQ(*matrix_3x3 == *matrix_2x3, 0);
  EXPECT_EQ(test == *matrix_2x2, 0);
}

TEST_F(MatrixTest, SumMatrix) {
  S21Matrix result(2, 2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  matrix_2x2->SumMatrix(*matrix_2x2);
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, SumMatrixOverload1) {
  S21Matrix result(2, 2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  *matrix_2x2 = *matrix_2x2 + *matrix_2x2;
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, SumMatrixOverload2) {
  S21Matrix result(2, 2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  *matrix_2x2 += *matrix_2x2;
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, SumMatrixExeption1) {
  EXPECT_THROW(matrix_2x2->SumMatrix(*matrix_2x3), std::logic_error);
}

TEST_F(MatrixTest, SumMatrixExeption2) {
  EXPECT_THROW(matrix_1x1->SumMatrix(*matrix_1x1), std::overflow_error);
}

TEST_F(MatrixTest, SubMatrix) {
  S21Matrix result(3, 3);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  matrix_3x3->SubMatrix(*matrix_3x3);
  EXPECT_EQ(*matrix_3x3 == result, 1);
}

TEST_F(MatrixTest, SubMatrixOverload1) {
  S21Matrix result(3, 3);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  *matrix_3x3 = *matrix_3x3 - *matrix_3x3;
  EXPECT_EQ(*matrix_3x3 == result, 1);
}

TEST_F(MatrixTest, SubMatrixOverload2) {
  S21Matrix result(3, 3);
  result(0, 0) = 0;
  result(0, 1) = 0;
  result(0, 2) = 0;
  result(1, 0) = 0;
  result(1, 1) = 0;
  result(1, 2) = 0;
  result(2, 0) = 0;
  result(2, 1) = 0;
  result(2, 2) = 0;
  *matrix_3x3 -= *matrix_3x3;
  EXPECT_EQ(*matrix_3x3 == result, 1);
}

TEST_F(MatrixTest, SubMatrixExeption1) {
  EXPECT_THROW(matrix_3x3->SubMatrix(*matrix_2x2), std::logic_error);
}

TEST_F(MatrixTest, SubMatrixExeption2) {
  S21Matrix test(1, 1);
  test(0, 0) = -DBL_MAX;
  EXPECT_THROW(matrix_1x1->SubMatrix(test), std::overflow_error);
}

TEST_F(MatrixTest, MulNumber) {
  S21Matrix result(2, 2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  matrix_2x2->MulNumber(2);
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, MulNumberOverload1) {
  S21Matrix result(2, 2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  *matrix_2x2 = *matrix_2x2 * 2;
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, MulNumberOverload2) {
  S21Matrix result(2, 2);
  result(0, 0) = 2;
  result(0, 1) = 4;
  result(1, 0) = 6;
  result(1, 1) = 8;
  *matrix_2x2 *= 2;
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, MulNumberExeption) {
  EXPECT_THROW(matrix_1x1->MulNumber(DBL_MAX), std::overflow_error);
}

TEST_F(MatrixTest, MulMatrix) {
  S21Matrix result(2, 3);
  result(0, 0) = 12.87;
  result(0, 1) = 22.02;
  result(0, 2) = 21.43;
  result(1, 0) = 2145.12;
  result(1, 1) = 6582.24;
  result(1, 2) = 7581.36;
  matrix_2x3->MulMatrix(*matrix_3x3);
  EXPECT_EQ(*matrix_2x3 == result, 1);
}

TEST_F(MatrixTest, MulMatrixOverload1) {
  S21Matrix result(2, 3);
  result(0, 0) = 12.87;
  result(0, 1) = 22.02;
  result(0, 2) = 21.43;
  result(1, 0) = 2145.12;
  result(1, 1) = 6582.24;
  result(1, 2) = 7581.36;
  *matrix_2x3 = *matrix_2x3 * *matrix_3x3;
  EXPECT_EQ(*matrix_2x3 == result, 1);
}

TEST_F(MatrixTest, MulMatrixOverload2) {
  S21Matrix result(2, 3);
  result(0, 0) = 12.87;
  result(0, 1) = 22.02;
  result(0, 2) = 21.43;
  result(1, 0) = 2145.12;
  result(1, 1) = 6582.24;
  result(1, 2) = 7581.36;
  *matrix_2x3 *= *matrix_3x3;
  EXPECT_EQ(*matrix_2x3 == result, 1);
}

TEST_F(MatrixTest, MulMatrixExeption1) {
  EXPECT_THROW(matrix_1x1->MulMatrix(*matrix_1x1), std::overflow_error);
}

TEST_F(MatrixTest, MulMatrixExeption2) {
  EXPECT_THROW(matrix_2x2->MulMatrix(*matrix_3x3), std::logic_error);
}

TEST_F(MatrixTest, Transpose1) {
  S21Matrix result(2, 2);

  result(0, 0) = 1;
  result(0, 1) = 3;
  result(1, 0) = 2;
  result(1, 1) = 4;

  *matrix_2x2 = matrix_2x2->Transpose();
  EXPECT_EQ(*matrix_2x2 == result, 1);
}

TEST_F(MatrixTest, Transpose2) {
  S21Matrix result(3, 2);

  result(0, 0) = 5.21;
  result(1, 0) = 2.134;
  result(2, 0) = 1.532;
  result(0, 1) = 2145.12;
  result(1, 1) = 573;
  result(2, 1) = 0;

  *matrix_2x3 = matrix_2x3->Transpose();
  EXPECT_EQ(*matrix_2x3 == result, 1);
}

TEST_F(MatrixTest, Determinant1) { EXPECT_EQ(matrix_3x3->Determinant(), -40); }

TEST_F(MatrixTest, Determinant2) { EXPECT_EQ(matrix_2x2->Determinant(), -2); }

TEST_F(MatrixTest, DeterminantExeption) {
  EXPECT_THROW(matrix_2x3->Determinant(), std::logic_error);
}

TEST_F(MatrixTest, CalcComplements) {
  S21Matrix result(3, 3);
  S21Matrix test;

  result(0, 0) = 0;
  result(0, 1) = 10;
  result(0, 2) = -20;
  result(1, 0) = 4;
  result(1, 1) = -14;
  result(1, 2) = 8;
  result(2, 0) = -8;
  result(2, 1) = -2;
  result(2, 2) = 4;

  test = matrix_3x3->CalcComplements();

  EXPECT_EQ(test == result, 1);
}

TEST_F(MatrixTest, CalcComplementsExeption) {
  EXPECT_THROW(matrix_2x3->CalcComplements(), std::logic_error);
}

TEST_F(MatrixTest, InverseMatrix) {
  S21Matrix result(3, 3);
  S21Matrix test;

  result(0, 0) = 0;
  result(0, 1) = -0.1;
  result(0, 2) = 0.2;
  result(1, 0) = -0.25;
  result(1, 1) = 0.35;
  result(1, 2) = 0.05;
  result(2, 0) = 0.5;
  result(2, 1) = -0.2;
  result(2, 2) = -0.1;

  test = matrix_3x3->InverseMatrix();
  EXPECT_EQ(test == result, 1);
}

TEST_F(MatrixTest, InverseMatrixExeption1) {
  EXPECT_THROW(matrix_2x3->InverseMatrix(), std::logic_error);
}

TEST_F(MatrixTest, InverseMatrixExeption2) {
  S21Matrix result(3, 3);

  result(0, 0) = 1;
  result(0, 1) = 2;
  result(0, 2) = 3;
  result(1, 0) = 4;
  result(1, 1) = 5;
  result(1, 2) = 6;
  result(2, 0) = 7;
  result(2, 1) = 8;
  result(2, 2) = 9;

  EXPECT_THROW(result.InverseMatrix(), std::logic_error);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
