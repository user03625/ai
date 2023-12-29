#include<iostream>
#include "util.h"

using namespace std;

int main(){
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
	*/

	Tensor<double> t1(2,3)={1,2,3,4,5,6};
	Tensor<double>::printMat(t1);

	Tensor<double> t2(3,2)={7,8,9,10,11,12};
	Tensor<double>::printMat(t2);

	//Tensor<double> t3=Tensor::mul(t1,t2);
	

	return 0;
}
