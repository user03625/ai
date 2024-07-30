///TODO: clean the code, make it run and work, optimize

#include<map>
#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<string>
#include<cassert>
#include<iomanip>
#include<algorithm>
#include<json.hpp>
#include<cmath>

class Util;
class Memory;
class State;
class Point;
class Action;
class Direction;
class Cell;
class Environment;
class TableEnvironment;
class Q;
class QTable;
class Optimizer;
class QLearning;
class Agent;
class Frame;
class RL;


class Util{
	public:
		template<typename T>
		static T** allocMatrix(int n,int m){
			T** a=new T*[n];
			for(auto i=0;i<n;i++)
				a[i]=new T[m];
			return a;
		}

		template<typename T>
		static void deleteMatrix(T** matrix,int n){
			for(auto i=0;i<n;i++)
				delete[] matrix[i];
			delete[] matrix;
		}
		static float clip(float value,float low,float high){
			if(value<low)
				return low;
			if(value>high)
				return high;
			return value;
		}
};

class State{
	public:
		virtual std::string toString()=0;
		virtual State* getData()=0;
		virtual State* copy()=0;
		int nrVisits;
};

class Point:public State{
	public:
		int i;
		int j;
		bool visited;
		Point(){
			i=0;
			j=0;
			visited=false;
		}
		Point(int ii,int jj):i(ii),j(jj){
			nrVisits=0;	
		}
		std::string toString(){
			return ""+std::to_string(i)+" "+std::to_string(j);
		}
		State* getData(){
			return this;
		}
		State* copy(){
			return new Point(i,j);
		}
		
};

class Frame{
	public:
		float nrVisits;
		float oldQValue;
		float reward;
		float maxQValue;
		float alpha;
		float oldStateValue;
		float newStateValue;
		float policyProbability; //<==============================================================
		float maxStateValue;
		State* oldState;
		State* newState;
		bool done;
		Action* action;
		Frame(){
			done=false;	
		}
		void print(){
			std::cout<<"nrVisits = "<<nrVisits<<std::endl;
			std::cout<<"oldQvalue = "<<oldQValue<<std::endl;
			std::cout<<"reward = "<<reward<<std::endl;
			std::cout<<"maxQValue = "<<maxQValue<<std::endl;
			std::cout<<"alpha = "<<alpha<<std::endl;
			std::cout<<"oldStateValue = "<<oldStateValue<<std::endl;
			std::cout<<"newStateValue = "<<newStateValue<<std::endl;
			std::cout<<"done = "<<done<<std::endl;

			std::cout<<"oldPoint = "<<((Point*)oldState)->i<<" "<<((Point*)oldState)->j<<std::endl;
			std::cout<<"newPoint = "<<((Point*)newState)->i<<" "<<((Point*)newState)->j<<std::endl;
			std::cout<<"================================================================================="<<std::endl;
		}
};

class Memory{
	private:
		std::vector<Frame*> memory;
	public:
		void save(Frame* frame){
			memory.push_back(frame);
		}
		void clear(){
			for(Frame* frame: memory){
				if(frame->newState!=NULL){
					delete frame->newState;
					frame->newState=NULL;
				}
				if(frame->newState!=NULL){
					delete frame->oldState;
					frame->oldState=NULL;
				}
				delete frame;
			}
			memory.clear();
		}
		std::vector<Frame*> getData(){
			return memory;
		}
};

class Action{
	public:
		virtual char toChar()=0;
		virtual float getValue()=0;
};


class Direction:public Action{
	public:
		enum Code {UP,RIGHT,DOWN,LEFT};
		static std::map<Direction::Code,Point> vectorMap;
		static std::map<Direction::Code,char> stringMap;

		Point vector;
		Direction::Code code;
		float qValue;
		float probability;
		Direction(){
			this->code=UP;
			this->vector=vectorMap[code];
		}

		Direction(Direction::Code code){
			this->code=code;
			this->vector=vectorMap[code];
		}
		char toChar(){
			return Direction::stringMap[code];
		}
		float getValue(){
			return qValue;
		}
};

std::map<Direction::Code,char> Direction::stringMap={{UP,'u'},{RIGHT,'r'},{DOWN,'d'},{LEFT,'l'}};
std::map<Direction::Code,Point> Direction::vectorMap={{UP,{-1,0}},{RIGHT,{0,1}},{DOWN,{1,0}},{LEFT,{0,-1}}};

class Cell{
	public:
		std::map<Direction::Code,Direction> directions;
		float stateValue;
		void set(Point location,int n,int m){
			if(location.i>0){
				Direction up(Direction::UP);
				directions[Direction::UP]=up;
			}
			if(location.j>0){
				Direction left(Direction::LEFT);
				directions[Direction::LEFT]=left;
			}
			if(location.i<n-1){
				Direction down(Direction::DOWN);
				directions[Direction::DOWN]=down;
			}
			if(location.j<m-1){
				Direction right(Direction::RIGHT);
				directions[Direction::RIGHT]=right;
			}
			float probability=1.0/directions.size();
			for(auto it=directions.begin();it!=directions.end();it++){
				it->second.probability=probability;
			}
		}
		void copyLegalActionsToScreen(char** screen,int ii,int jj,int cellSize){
			for(int i=ii;i<ii+cellSize;i++){
				for(int j=jj;j<jj+cellSize;j++){
					if(i==ii && j==jj+cellSize/2 && directions.find(Direction::UP)!=directions.end()){
						screen[i][j]=directions[Direction::UP].toChar();
					}	
					else if(i==ii+cellSize-1 && j==jj+cellSize/2 && directions.find(Direction::DOWN)!=directions.end()){
						screen[i][j]=directions[Direction::DOWN].toChar();
					}
					else if(i==ii+cellSize/2 && j==jj && directions.find(Direction::LEFT)!=directions.end()){
						screen[i][j]=directions[Direction::LEFT].toChar();
					}
					else if(i==ii+cellSize/2 && j==jj+cellSize-1 && directions.find(Direction::RIGHT)!=directions.end()){
						screen[i][j]=directions[Direction::RIGHT].toChar();
					}
					else{
						screen[i][j]=' ';
					}
				}
			}

		}
		void copyValuesToScreen(float** screen,int ii,int jj,int cellSize){
			for(int i=ii;i<ii+cellSize;i++){
				for(int j=jj;j<jj+cellSize;j++){
					if(i==ii && j==jj+cellSize/2 && directions.find(Direction::UP)!=directions.end()){
						screen[i][j]=directions[Direction::UP].qValue;
					}	
					else if(i==ii+cellSize-1 && j==jj+cellSize/2 && directions.find(Direction::DOWN)!=directions.end()){
						screen[i][j]=directions[Direction::DOWN].qValue;
					}
					else if(i==ii+cellSize/2 && j==jj && directions.find(Direction::LEFT)!=directions.end()){
						screen[i][j]=directions[Direction::LEFT].qValue;
					}
					else if(i==ii+cellSize/2 && j==jj+cellSize-1 && directions.find(Direction::RIGHT)!=directions.end()){
						screen[i][j]=directions[Direction::RIGHT].qValue;
					}
					else{
						screen[i][j]=0;
					}
				}
			}

		}
};

class Environment{
	public:
		virtual float getReward(Agent* agent)=0;
		virtual State* getState(State* state,Action* action)=0;
		virtual bool gameOver(State* state)=0;
		virtual void reset()=0;
	protected:
		std::vector<State*> goalStates;
		std::vector<State*> finalStates;
		std::vector<State*> rewardStates;
		float totalReward=0;
		

};

class Q{
	public:
		virtual float getStateValue(State* state)=0;
		virtual float getQValue(State* state,Action* action)=0;
		virtual float getMaxQValue(State* state)=0;
		virtual Action* getAction(State* state)=0;
		virtual float getPolicyProbability(State* state,Action* action)=0;
		virtual float getMaxStateValue(State* state)=0;

		virtual void setQValue(State* state,Action* action, float value)=0;
		virtual void setStateValue(State* state,float value)=0;
};

class Agent{
	private:
		const float ALPHA=0.2;
		Q* q;
	public:
		State* currentState;
		State* startState;
		int nrRewards;
		float rewards;
		Agent(Q* q,State* startState){
			this->q=q;	
			nrRewards=0;
			rewards=0;
			currentState=startState;
			this->startState=startState->copy();
		}
		void reset(){
			nrRewards=0;
			rewards=0;
			currentState=startState->copy();
		}
		Action* getAction(State* state){
			return q->getAction(state);	
		}
		float getStateValue(State* state){
			return q->getStateValue(state);	
		}
		float getQValue(State* state, Action* action){
			return q->getQValue(state,action);	
		}
		float getMaxQValue(State* state){
			return q->getMaxQValue(state);	
		}
		void setStateValue(State* state,float value){
			q->setStateValue(state,value);
		}
		void setQValue(State* state,Action* action,float value){
			q->setQValue(state,action,value);
		}
		Frame* step(Environment* environment){
			Frame* frame=new Frame();
			frame->oldStateValue=getStateValue(currentState);
			frame->action=getAction(currentState);
			frame->oldQValue=getQValue(currentState,frame->action);
			frame->oldState=currentState;
			frame->newState=environment->getState(currentState,frame->action);
			//std::cout<<frame->oldState->toString()<<" "<<frame->action->toChar()<<" "<<frame->newState->toString()<<std::endl;
			frame->newState->nrVisits++;
			frame->maxQValue=getMaxQValue(frame->newState);
			frame->newStateValue=getStateValue(frame->newState);
			frame->nrVisits=frame->newState->nrVisits;
			frame->alpha=ALPHA;
			frame->done=environment->gameOver(frame->newState);
			frame->policyProbability=q->getPolicyProbability(frame->oldState,frame->action);
			frame->maxStateValue=q->getMaxStateValue(frame->oldState);

			currentState=frame->newState;
			frame->reward=environment->getReward(this);
			return frame;		
		}
	
};
class TableEnvironment:public Environment{
	public:
		int n;
		int m;
		Point start;
		float** rewardsTable;

		TableEnvironment(const char* file,char* grid){
			using json=nlohmann::json;
			std::ifstream reader(file);
			json data=json::parse(reader);
			auto& currentGrid=data[grid];	
			n=currentGrid["dimension"][0];
			m=currentGrid["dimension"][1];
			rewardsTable = Util::allocMatrix<float>(n,m);
			start.i=currentGrid["start"][0];
			start.j=currentGrid["start"][1];
			for(auto& point:currentGrid["goalStates"]){
				goalStates.push_back(new Point(point[0],point[1]));
			}
			for(auto& point:currentGrid["finalStates"]){
				finalStates.push_back(new Point(point[0],point[1]));
			}
			for(auto& point:currentGrid["rewardStates"]){
				rewardStates.push_back(new Point(point[0],point[1]));
			}
			int i=0;
			for(float reward:currentGrid["grid"]){
				rewardsTable[i/m][i%m]=reward;
				i++;
			}
			
		}
		void reset(){
			for(State* q:rewardStates){
				Point* p=(Point*)q;
				p->visited=false;
			}
		}
	    float getReward(Agent* agent){
			Point* current=(Point*)agent->currentState;
			if(agent->nrRewards==rewardStates.size()-1){
				for(State* q:finalStates){
					Point* p=(Point*)q;
					if(p->i==current->i && p->j==current->j)
						return rewardsTable[current->i][current->j];
				}
				for(State* q:rewardStates){
					Point* p=(Point*)q;
					if(p->i==current->i && p->j==current->j){
						return 0;
					}
				}
				return rewardsTable[current->i][current->j];
			}
			else {
				for(State* q:rewardStates){
					Point* p=(Point*)q;
					if(p->i==current->i && p->j==current->j && !(p->visited)){
						p->visited=true;
						agent->nrRewards++;
					}
				}
				for(State* q:goalStates){
					Point* goalState=(Point*)q;
					if(current->i==goalState->i && current->j==goalState->j)
						return 0;
				}
				return rewardsTable[current->i][current->j];
			}
		}
		State* getState(State* state,Action* action){
			Point* s=new Point(((Point*)state)->i,((Point*)state)->j);
			Direction* direction=(Direction*)action;
			s->i+=direction->vector.i;
			s->j+=direction->vector.j;
			return s;
		}

		bool gameOver(State* state){
			for(State* q: goalStates){
				Point* goalState=(Point*)q;
				if(goalState->i==((Point*)state)->i && goalState->j==((Point*)state)->j)
					return true;
			}
			return false;
		}
		void printRewards(){
			std::cout<<"Rewards: "<<std::endl;
			for(int i=0;i<n;i++){
				for(int j=0;j<m;j++){
					std::cout.width(3);
					std::cout<<rewardsTable[i][j]<<" ";
				}
				std::cout<<std::endl;
			}
		}
		~TableEnvironment(){
			Util::deleteMatrix(rewardsTable,n);
		}
};


class QTable:public Q{
	private:
		Cell** table;
		std::pair<Direction::Code,Action*> max(Cell& cell){
			float maximum=cell.directions.begin()->second.qValue;
			Direction::Code code=cell.directions.begin()->first;
			for(auto it=cell.directions.begin();it!=cell.directions.end();it++){
				//std::cout<<it->first<<std::endl;
				if(it->second.qValue>maximum){
					maximum=it->second.qValue;
					code=it->first;
					//std::cout<<code<<std::endl;
				}
			}
			//std::cout<<code<<std::endl;
			return std::make_pair(code,&(cell.directions[code]));
		}
	public:
		int n;
		int m;

		QTable(int n,int m){
			this->n=n;
			this->m=m;
			table=Util::allocMatrix<Cell>(n,m);
			for(int i=0;i<n;i++){
				for(int j=0;j<m;j++){
					table[i][j].set((Point){i,j},n,m);
				}
			}
		}
		float getMaxStateValue(State* state){
			Point* current=(Point*)state;
			Point* p=&(table[current->i][current->j].directions.begin()->second.vector);
			float maximum = table[current->i+p->i][current->j+p->j].stateValue;
			for(auto it=table[current->i][current->j].directions.begin();it!=table[current->i][current->j].directions.end();it++){
				p=&(it->second.vector);
				if(table[current->i+p->i][current->j+p->j].stateValue>maximum){
					maximum=table[current->i+p->i][current->j+p->j].stateValue;
				}
			}
			return maximum;
		}
		float getPolicyProbability(State* state, Action* action){
			Point* p=(Point*)state;
			Direction* direction=(Direction*)action;
			return table[p->i][p->j].directions[direction->code].probability;	
		}

		float getStateValue(State* state){
			Point* location=(Point*)state;
			return table[location->i][location->j].stateValue;
		}
		float getQValue(State* state,Action* action){
			Point* location=(Point*)state;
			Direction* direction=(Direction*)action;
			return table[location->i][location->j].directions[direction->code].getValue();	
		}
		float getMaxQValue(State* state){
			Point* location=(Point*)state;
			Direction* direction=(Direction*)this->max(table[location->i][location->j]).second;
			return direction->qValue;		
		}
		Action* getAction(State* state){
			auto epsilon = 0.2;
			auto random = ((double)rand()/RAND_MAX);
			Point* location=(Point*)state;

			if(random<epsilon){
				Cell* cell=&table[location->i][location->j];
				auto it=cell->directions.begin();
				std::advance(it,rand() % cell->directions.size());
				Direction::Code code = it->first;
				return &cell->directions[code];

			} else {
				return this->max(table[location->i][location->j]).second;
			}
		}
		void setQValue(State* state,Action* action, float value){
			Point* location=(Point*)state;
			Direction* direction=(Direction*)action;
			table[location->i][location->j].directions[direction->code].qValue=value;
			//================= set probabilities ===============================================
			float sum=0;
			for(auto it=table[location->i][location->j].directions.begin();it!=table[location->i][location->j].directions.end();it++){
				sum+=exp(it->second.qValue);
			}
			for(auto it=table[location->i][location->j].directions.begin();it!=table[location->i][location->j].directions.end();it++){
				it->second.probability=exp(it->second.qValue)/sum;
			}
		}
		void setStateValue(State* state,float value){
			Point* location=(Point*)state;
			table[location->i][location->j].stateValue=value;
		}
		
		void print(){
			int cellSize=3;
			int height = n*(cellSize+1)+1;
			int width = m*(cellSize+1)+1;
			int cellWidth = 5;
			int precision = 2;
			float** screen = Util::allocMatrix<float>(height,width);
			for(int i=0;i<n;i++){
				for(int j=0;j<m;j++){
					table[i][j].copyValuesToScreen(screen,i*(cellSize+1)+1,j*(cellSize+1)+1,cellSize);					
				}
			}
			for(int i=0;i<height;i++){
				for(int j=0;j<width;j++){
					if(i%(cellSize+1)==0){
						std::cout.width(1);
						if(j==width-1)
							std::cout<<"-";
						else
							std::cout<<"-----";
					}
					else if(j%(cellSize+1)==0){
						std::cout.width(1);
						std::cout<<"| ";
					}
					else {
						if(screen[i][j]==0){
							std::cout.width(cellWidth);
							std::cout<<" ";
							std::cout<<" ";
						}
						else{
							std::cout<<std::fixed<<std::setw(cellWidth)<<std::setprecision(precision)<<screen[i][j]<<" ";
						}
					}
				}
				std::cout<<std::endl;
			}
			Util::deleteMatrix(screen,n);
		}
		void printPolicy(){
			for(int i=0;i<n;i++){
				for(int j=0;j<m;j++){
					std::cout<<max(table[i][j]).second->toChar()<<" ";
				}
				std::cout<<std::endl;
			}
		}
		void printLegalActions(){
			int cellSize=3;
			int height = n*(cellSize+1)+1;
			int width = m*(cellSize+1)+1;
			char** screen = Util::allocMatrix<char>(height,width);
			for(int i=0;i<n;i++){
				for(int j=0;j<m;j++){
					table[i][j].copyLegalActionsToScreen(screen,i*(cellSize+1)+1,j*(cellSize+1)+1,cellSize);					
				}
			}
			for(int i=0;i<height;i++){
				for(int j=0;j<width;j++){
					if(i%(cellSize+1)==0){
						if(j==width-1)
							std::cout<<"-";
						else
							std::cout<<"--";
					}
					else if(j%(cellSize+1)==0)
						std::cout<<"| ";
					else
						std::cout<<screen[i][j]<<" ";
				}
				std::cout<<std::endl;
			}
			Util::deleteMatrix(screen,n);	
		}
		~QTable(){
			Util::deleteMatrix<Cell>(table,n);
		}


};

class Optimizer{
	public:
		virtual void learn(Agent* agent,Memory* memory)=0;
};

class QLearning:public Optimizer{
	public:
		static const float DECAY;
		static const float ALPHA;

		void learn(Agent* agent,Memory* memory){
			for(Frame* frame: memory->getData()){
				float value=(1.0-QLearning::ALPHA)*frame->oldQValue+QLearning::ALPHA*(frame->reward+agent->rewards+QLearning::DECAY*frame->maxQValue);
				agent->setQValue(frame->oldState,frame->action,value);
			}
			memory->clear();
		}
};
const float QLearning::DECAY=0.99;
const float QLearning::ALPHA=0.005;

class PPO:public Optimizer{
	public:
		QTable* qTable;
		static const float EPSILON;
		static const float DISCOUNT;
		PPO(QTable* qTable){
			this->qTable=qTable;
		}
		void learn(Agent* agent,Memory* memory){
			for(Frame* frame: memory->getData()){
				float stateValue = frame->reward+DISCOUNT*frame->maxStateValue; 				
				qTable->setStateValue(frame->oldState,stateValue);
				float advantage=frame->oldStateValue-frame->oldQValue;
				float newPolicyProbability=qTable->getPolicyProbability(frame->oldState,frame->action);
				float ratio=newPolicyProbability/frame->policyProbability;
				float value=std::min(ratio*advantage,Util::clip(ratio*advantage,1-EPSILON,1+EPSILON));

				agent->setQValue(frame->oldState,frame->action,value);
			}
			memory->clear();
		}
};
const float PPO::EPSILON=0.2;
const float PPO::DISCOUNT=0.99;

class RL{
	private:
		Agent* agent;
		Environment* environment;
		Optimizer* optimizer;
		Memory* memory;	
		QTable* table;
		static const int N;
	public:
		RL(const char* file){
			char grid[256]="grid4";
			environment=new TableEnvironment(file,grid);
			((TableEnvironment*)environment)->printRewards();
			memory = new Memory();
			table = new QTable(((TableEnvironment*)environment)->n,((TableEnvironment*)environment)->m);
			optimizer = new QLearning();
			//optimizer = new PPO((QTable*)table);
			((QTable*)table)->printLegalActions();
			agent = new Agent(table,&((TableEnvironment*)environment)->start);
		
		}
		void run(int epochs);
		void print(){
			((QTable*)table)->print();
			((QTable*)table)->printPolicy();
		}
		~RL(){

			delete memory;
			delete optimizer;
			delete table;
			delete agent;
			delete environment;
		}
};

const int RL::N=10;

void RL::run(int epochs){
	Frame* frame;
	int steps;
	int maxSteps=20;
	for(int i=1;i<=epochs;i++){
		agent->reset();
		environment->reset();
		steps=0;
		do{
			frame=agent->step(environment);
			//frame->print();
			memory->save(frame);
			steps++;
		}while(!frame->done && steps<maxSteps);
		if(i%N==0){
			//std::cout<<"epoch: "<<i<<std::endl;
			optimizer->learn(agent,memory);
		}
	}
}


int main(int argc,char** argv){
	//assert(argc==2);
	RL rl("grids.json");
	std::cout<<"Training started"<<std::endl;
	rl.run(100000);
	std::cout<<"Training finished"<<std::endl;
	rl.print();

	return 0;
}
