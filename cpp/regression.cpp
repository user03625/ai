#include<iostream>
#include"util.h"

using namespace std;

class LiniarRegresion{
	public:
		LiniarRegresion(auto X,auto Y){}
		auto mse();
		auto update();
		auto derivA();
		auto derivB();
		auto train();
		auto predict(auto input);
		auto test();
		
	private:
		auto inputs;
		auto outputs;
		auto weights;
		auto bias;
		auto ALPHA=0.001;
	       	auto EPOCHS=1000;
		auto EPSILON=0.1;	
};

int main(){
	LiniarRegresion liniarRegresion(X,Y);
	liniarRegresion.train();
	liniarRegresion.test();




	return 0;
}
