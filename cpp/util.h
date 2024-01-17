#include<cstdarg>
#include<vector>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<cassert>

template<typename T>
class Tensor{
	private:
		int size;
		T* data;
		std::vector<int> shape;
	public:
		Tensor(int n){
			shape.push_back(n);
		}
		template <typename... Args>
		Tensor(int first,Args... args):Tensor(args...){
			shape.push_back(first);
			Tensor(args...);
			std::reverse(shape.begin(),shape.end());
			size=1;
			for(int i:shape){
				size*=i;
			}
			this->data=new T[this->size];	

		}
		auto add(auto tensor);
		auto dot(auto tensor);
		auto mul(auto tensor);
		auto mean();
		auto sum();
		static auto add(auto t1,auto t2);
		static auto dot(auto t1,auto t2);
		static auto mul(auto t1,auto t2);
		static auto printMat(auto t1);
		auto random();
		auto zeros();
		auto ones();
		auto getSize();
		auto operator[](auto i){
			/*
			if(shape.size()==1)
				return data[i];
			else
			*/
			return &data[i];
		}
		auto print();
		auto getDim(){
			return shape.size();
		}
		auto getShape(){
			return shape;
		}
};

template<typename T>
auto Tensor<T>::printMat(auto t1){
	assert(t1.getDim()==2);
	int n=t1.getShape()[0];
	int m=t1.getShape()[1];
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++)
			std::cout<<t1[i][j]<<' ';
		std::cout<<'\n';
	}
	std::cout<<std::endl;
}

template<typename T>
auto Tensor<T>::mul(auto t1){
	
}
//    a1 a2 a3          b1 b2        a1*b1+a2*b3+a3*b5   a1*b2+a2*b4+a3*b6
//    a4 a5 a6     *    b3 b4   =    
//                      b5 b6        a4*b1+a5*b3+a6*b5   a4*b2+a5*b4+a6*b6

template<typename T>
auto Tensor<T>::mul(auto t1, auto t2){
	assert(t1.getDim()==2 && t2.getDim()==2);
	int n=t1.getShape()[0];
	int m=t2.getShape()[1];
	int columns = t1.getShape()[1];
	Tensor<T> t3(n,m);
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++){
			t3[i][j]=0;
			for(int k=0;k<columns;k++)
				t3[i][j]+=t1[i][k]*t2[k][j];
		}
	return t3;
}

template<typename T>
auto Tensor<T>::print(){
	for(int i=0;i<this->getSize();i++)
		std::cout<<this->data[i]<<" ";
	std::cout<<std::endl;
}

template<typename T>
auto Tensor<T>::getSize(){
	return this->size;
}

template<typename T>
auto Tensor<T>::random(){
	auto n=this->getSize();
	srand(time(NULL));
	for(int i=0;i<n;i++)
		this->data[i]=((double)rand()/(RAND_MAX));
}

template<typename T>
auto Tensor<T>::zeros(){
	auto n=this->getSize();
	for(int i=0;i<n;i++)
		this->data[i]=0;
}

template<typename T>
auto Tensor<T>::ones(){
	auto n=this->getSize();
	for(int i=0;i<n;i++)
		this->data[i]=1;
}

template<typename T>
auto Tensor<T>::mean(){
	auto m=this->sum();
	return m/this->getSize();
}

template<typename T>
auto Tensor<T>::sum(){
	auto s=0;
	auto n=this->getSize();
	for(int i=0;i<n;i++)
		s+=this->data[i];
	return s;
}

template<typename T>
auto Tensor<T>::add(auto t1,auto t2){
	Tensor<T> t3;
	for(int i=0;i<t1.getSize();i++){
		t3[i]=t1[i]+t2[i];
	}
	return t3;
}
	
template<typename T>
auto Tensor<T>::dot(auto t1,auto t2){
	auto dotProduct=0;
	for(int i=0;i<t1.getSize();i++)
		dotProduct+=t1[i]*t2[i];
	return dotProduct;
}

template<typename T>
auto Tensor<T>::add(auto tensor){
	for(int i=0;i<tensor.getSize();i++)
		this->data[i]+=tensor[i];
}

template<typename T>
auto Tensor<T>::dot(auto tensor){
	return Tensor::dot(tensor,*this);
}

