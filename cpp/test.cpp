#include<iostream>
#include<fstream>

#include "matrix.h"

using namespace std;

int main(){
	ifstream reader("data.in");
	int n1;
	int m1;
	int n2;
	int m2;
	reader>>n1>>m1;
	Matrix<int> a(n1,m1);
	for(int i=0;i<n1;i++){
		for(int j=0;j<m1;j++){
			reader>>a[i][j];
		}
	}
	reader>>n2>>m2;
	Matrix<int> b(n2,m2);
	for(int i=0;i<n2;i++){
		for(int j=0;j<m2;j++){
			reader>>b[i][j];
		}
	}
	//Matrix<int> c(n1,m2);
	(a*b).print();
	//c.print();
	/*
	Tensor<double> t1(2,5);
	Tensor<double> t2(2,5);
	cout<<t1.getSize()<<endl;
	t1.random();
	t2.zeros();

	t1.print();
	t2.print();
	t1.add(t2);
	t1.print();

	Tensor<double> t1(2,3)={1,2,3,4,5,6};
	Tensor<double>::printMat(t1);

	Tensor<double> t2(3,2)={7,8,9,10,11,12};
	Tensor<double>::printMat(t2);
	
	//Tensor<double> t3=Tensor::mul(t1,t2);
	
	int m[3][4]={
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12}};
	int mT[4][3];
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			mT[i][j]=m[j][i];
		}
	}
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			cout<<m[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			cout<<mT[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	int a[3*4]={1,2,3,4,5,6,7,8,9,10,11,12};
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			cout<<a[i*3+j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;	
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			cout<<a[j*4+i]<<" ";
		}
		cout<<endl;
	}
	*/
	return 0;
}
