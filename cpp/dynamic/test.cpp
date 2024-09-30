#include<iostream>

using namespace std;

class Neuron;
class NN;

class NN{
	private:
		void buildNetwork();
		void evolveNetwork();
		bool checkConvergence();
		
	public:
	       	vector<Neuron*> input;
		vector<Neuron*> output;
		vector<Neuron*> network;

		int inputSize;
		int outputSize;

		NN(int inputSize,int outputSize){
			this->inputSize=inputSize;
			this->outputSize=outputSize;
			buildNetwork();
		}
		
		void train(vector<vector<int>> input,vector<vector<int>> output,int epochs);
		void predict(int* input,int* output);


};

class Neuron{
	public:
		float w;
		float dw;
		vector<Neuron*> dendrites;
		vector<Neuron*> axons;
};


void NN::buildNetwork(){
	for(int i=0;i<inputSize;i++){
		input.push_back(new Neuron());
	}
	for(int i=0;i<outputSize;i++){
		output.push_back(new Neuron());
	}
	for(int i=0;i<inputSize;i++){
		for(int j=0;j<outputSize;j++){
			input[i]->axons.push_back(output[j]);
			output[j]->dendrites.push_back(intput[i]);
		}
	}
}

void NN::train(vector<vector<int>> input, vector<vector<int>> output,int epochs){

}


int main(){
	//xor
	vector<vector<int>> input={
		{1,1},
		{1,0},
		{0,1},
		{0,0}

	};
	vector<vector<int>> output={{0},{1},{1},{0}};
	vector<vector<int>> prediction;	

	int epochs=100;
	int inputSize=2;
	int outputSize=1;

	NN nn(intputSize,outputSize);

	nn.train(input,output,epochs);
	nn.predict(input,prediction);	

	for(int i=0;i<prediction.size();i++){
		cout<<"prediction: "<<prediction[i][0]<<" actual value: "<<output[i][0]<<endl;
		

	return 0;
}

