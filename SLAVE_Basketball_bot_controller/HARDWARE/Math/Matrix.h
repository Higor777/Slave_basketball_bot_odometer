//-------------------------------------------//
//此头文件包含一些通常的矩阵运算
//-------------------------------------------//
#ifndef _MATRIX_H_
#define _MATRIX_H_

//void MatrixAdd( float* fMatrixA,float* fMatrixB,float* Result,unsigned int m,unsigned int n );
void MatrixSub( float* fMatrixA,float* fMatrixB,float* Result,unsigned int m,unsigned int n );
void MatrixMultiply(float* fMatrixA,unsigned int uRowA,unsigned int uColA,float* fMatrixB,unsigned int uRowB,unsigned int uColB,float* MatrixResult );
void MatrixTranspose(float* fMatrixA,unsigned int m,unsigned n,float* fMatrixB);
//void MatrixE(float* fMatrixA,unsigned int n);
void dhdet(float *a,int n,float det);
//int MatrixInverse2(float* fMatrixA,float* fMatrixB);
int MatrixInverse(float* fMatrixA,int n,float ep);
//int bchol(float* a,int n,float *det);
//int rinv(float* a,int n) ;
void UD(float * A,int n,float * U,float * D);
//void LD(float * A,int n,float * U,float * D);
//void QR(float *a,float* q,float *r1); 
void Jacobi_Cyclic_Method(float *eigenvalues, float *eigenvectors,float *A, int n);

#endif

