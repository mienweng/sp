#include <stdio.h>
#include <stdlib.h>

// 定義矩陣結構
typedef struct
{
    int rows;
    int cols;
    double *data;
} Matrix;

// 初始化矩陣
Matrix createMatrix(int rows, int cols)
{
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (double *)malloc(rows * cols * sizeof(double));
    return mat;
}

// 釋放矩陣內存
void freeMatrix(Matrix *mat)
{
    free(mat->data);
    mat->data = NULL;
    mat->rows = 0;
    mat->cols = 0;
}

// 打印矩陣
void dumpMatrix(Matrix mat)
{
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            printf("%f ", mat.data[i * mat.cols + j]);
        }
        printf("\n");
    }
}

// 矩陣轉置
Matrix transposeMatrix(Matrix mat)
{
    Matrix result = createMatrix(mat.cols, mat.rows);
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            result.data[j * result.cols + i] = mat.data[i * mat.cols + j];
        }
    }
    return result;
}

// 矩陣相加
Matrix addMatrix(Matrix mat1, Matrix mat2)
{
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols)
    {
        fprintf(stderr, "Matrix dimensions do not match for addition.\n");
        exit(EXIT_FAILURE);
    }
    Matrix result = createMatrix(mat1.rows, mat1.cols);
    for (int i = 0; i < mat1.rows; i++)
    {
        for (int j = 0; j < mat1.cols; j++)
        {
            result.data[i * result.cols + j] = mat1.data[i * mat1.cols + j] + mat2.data[i * mat2.cols + j];
        }
    }
    return result;
}

// 矩陣相乘
Matrix multiplyMatrix(Matrix mat1, Matrix mat2)
{
    if (mat1.cols != mat2.rows)
    {
        fprintf(stderr, "Matrix dimensions do not match for multiplication.\n");
        exit(EXIT_FAILURE);
    }
    Matrix result = createMatrix(mat1.rows, mat2.cols);
    for (int i = 0; i < mat1.rows; i++)
    {
        for (int j = 0; j < mat2.cols; j++)
        {
            result.data[i * result.cols + j] = 0;
            for (int k = 0; k < mat1.cols; k++)
            {
                result.data[i * result.cols + j] += mat1.data[i * mat1.cols + k] * mat2.data[k * mat2.cols + j];
            }
        }
    }
    return result;
}

// 改變矩陣形狀
Matrix reshapeMatrix(Matrix mat, int newRows, int newCols)
{
    if (mat.rows * mat.cols != newRows * newCols)
    {
        fprintf(stderr, "Matrix dimensions do not match for reshaping.\n");
        exit(EXIT_FAILURE);
    }
    Matrix result = createMatrix(newRows, newCols);
    for (int i = 0; i < newRows * newCols; i++)
    {
        result.data[i] = mat.data[i];
    }
    return result;
}

// 主函數測試矩陣函式庫
int main()
{
    Matrix mat1 = createMatrix(2, 3);
    Matrix mat2 = createMatrix(3, 2);

    // 初始化 mat1
    mat1.data[0] = 1;
    mat1.data[1] = 2;
    mat1.data[2] = 3;
    mat1.data[3] = 4;
    mat1.data[4] = 5;
    mat1.data[5] = 6;

    // 初始化 mat2
    mat2.data[0] = 7;
    mat2.data[1] = 8;
    mat2.data[2] = 9;
    mat2.data[3] = 10;
    mat2.data[4] = 11;
    mat2.data[5] = 12;

    // 打印 mat1 和 mat2
    printf("Matrix 1:\n");
    dumpMatrix(mat1);
    printf("\nMatrix 2:\n");
    dumpMatrix(mat2);

    // 矩陣相乘
    Matrix resultMultiply = multiplyMatrix(mat1, mat2);
    printf("\nMatrix 1 * Matrix 2:\n");
    dumpMatrix(resultMultiply);

    // 矩陣轉置
    Matrix resultTranspose = transposeMatrix(mat1);
    printf("\nTranspose of Matrix 1:\n");
    dumpMatrix(resultTranspose);

    // 改變矩陣形狀
    Matrix resultReshape = reshapeMatrix(mat1, 3, 2);
    printf("\nReshaped Matrix 1 (3x2):\n");
    dumpMatrix(resultReshape);

    // 釋放內存
    freeMatrix(&mat1);
    freeMatrix(&mat2);
    freeMatrix(&resultMultiply);
    freeMatrix(&resultTranspose);
    freeMatrix(&resultReshape);

    return 0;
}
