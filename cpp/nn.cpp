#include<iostream>
#include<vector>
#include "util.h"

class Layer{
	public:
		virtual Tensor<float>* forward(Tensor<float>* input)=0;
		virtual void backward()=0;
};

class Full:public Layer{
	public:
		Tensor<float>* weights;
		Tensor<float>* bias;
		Tensor<float>* output;
		Tensor<float>* weightsGradient;
		Tensor<float>* inputGradient;
		Tensor<float>* input;
		int inputSize;
		int outputSize;
		Full(int inputSize,int outputSize){
			this->inputSize=inputSize;
			this->outputSize=outputSize;
			this->weights=new Tensor<float>(outputSize,inputSize);
			this->output=new Tensor<float>(1,outputSize);
			this->bias=new Tensor<float>(1,outputSize);
			this->weightsGradient<float>(inputSize,outputSize);
			this->inputGradient=new Tensor<float>(inputSize,1);

		}
	       	Tensor<float>* forward(Tensor<float>* input);
		void backward();
		~Full(){
			delete weights;
			delete bias;
			delete output;
			delete weightsGradient;
			delete inputGradient;
		}
};

class Tanh:public Layer{
	public:
		Tensor<float>* forward(Tensor<float>* input);
		void backward();
};

class MSE{
	public:
		float mse();
		float msePrime();
};

class NN{
	private:
		std::vector<Layer*> layers;
	public:
		NN(std::initializer_list<Layer*> list):layers(list){
			std::cout<<"layers size "<<layers.size()<<std::endl;	
		}
		void add(Layer* layer);
		void train(int epochs);
		~NN(){
			for(Layer* layer:layers)
				delete layer;
		}
};

Tensor<float>* Full::forward(Tensor<float>* input){
	Tensor<float>::mul(w,input,output);
	output->add(bias);
	this->input=input;
	return output;
}
void Full::backward(Tensor<float>* outputGradient){
	Tensor<float>::mul(outputGradient,input,weightsGradient);
	//weights->sum(
}
Tensor<float>* Tanh::forward(Tensor<float>* input){
	return NULL;
}
void Tanh::backward(){}


int main(){
	NN nn={new Full(2,3),new Tanh,new Full(3,1),new Tanh};
	return 0;
}

