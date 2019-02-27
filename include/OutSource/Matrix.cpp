#include "stdafx.h"

#include "Matrix.h"
#include <iostream>
#include <cmath>
using namespace std;


void MatrixShow(float *mat)
{
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			cout<<mat[i*4+j]<<" ";
		}
		cout<<endl;
	}
}

void MatrixIdentity(float *mat)
{
	for(int i=0; i<16; i++) mat[i] = 0;
	for(int i=0; i<4; i++){
		mat[i*4+i] = 1;
	}
}

void MatrixCopy(float *src, float *dest)
{
	for(int i=0; i<16; i++) dest[i] = src[i];
}

void MatrixTranspose(float *mat)
{
	float temp;
	for(int i=0; i<4; i++){
		for(int j=i+1; j<4; j++){
			temp = mat[i*4+j];
			mat[i*4+j] = mat[j*4+i];
			mat[j*4+i] = temp;
		}
	}
}

void MatrixMult(float *mat1, float *mat2, float *result)
{
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			result[i*4+j] = 0;
			for(int k=0; k<4; k++){
				result[i*4+j] += mat1[i*4+k]*mat2[k*4+j];
			}
		}
	}
}

void MatrixRotateX(float *mat, float angle)
{
	angle = angle/180*3.141592653589793238;
	float rot[] = {
		1,0,0,0,
		0,cos(angle),-sin(angle),0,
		0,sin(angle),cos(angle),0,
		0,0,0,1
	};
	float res[16];
	MatrixMult(rot,mat,res);
	for(int i=0; i<16; i++) mat[i] = res[i];
}

void MatrixRotateY(float *mat, float angle)
{
	angle = angle/180*3.141592653589793238;
	float rot[] = {
		cos(angle),0,sin(angle),0,
		0,1,0,0,
		-sin(angle),0,cos(angle),0,
		0,0,0,1
	};
	float res[16];
	MatrixMult(rot,mat,res);
	for(int i=0; i<16; i++) mat[i] = res[i];
}