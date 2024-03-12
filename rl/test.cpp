#include<iostream>
#include<algorithm>
#include<random>
#include<list>
#include<vector>
#include<fstream>
#include<iomanip>
#include<assert.h>

using namespace std;

struct Actions{
	float actions[4];
	list<int> legalActions;
	int nrVisits=0;
};

struct Point{
	int i;
	int j;
};

struct Agent{
	Point location;
	int nrRewards=0;
};

struct Visit{
	Point location;
	Point oldLocation;
	float nrVisits;
	float oldQ;
	float r;
	float rFuture;
	float alpha;
};

struct Memory{
	list<Visit> visits;
	float totalRewward=0;
}

const int LEFT=0;
const int UP=1;
const int RIGHT=2;
const int DOWN=3;

const int EPOCHS = 100000;
const float ALPHA = 1;
const float DECAY = 0.99;
const int NR_ACTIONS = 4;

template<typename T>
T** allocMatrix(int n,int m){
	T** a=new T*[n];
	for(int i=0;i<n;i++)
		a[i]=new T[m];
	return a;
}

template<typename T>
void deleteMatrix(T** matrix,int n){
	for(int i=0;i<n;i++)
		delete[] matrix[i];
	delete[] matrix;
}

void runEpisode(Agent& agent,Actions** qTable,float** table,int n,int m,list<Point>& finalStates,int i,float totalReward,list<Point>& rewardsLocations,Point goalState);
int takeAction(Actions& cell,int n,int m,int i,int oldAction);
float maximum(Actions& cell);
bool gameOver(Agent& agent,list<Point>& finalStates);
void printQTable(Actions** qTable,int n,int m);
const char* toString(int direction);
void printPolicy(Actions** table,int n,int m);
int argMax(Actions& cell,int oldAction);
void printLegalActions(Actions** table,int n,int m);
void printRewards(float** table,int n,int m);
void qLearning(char* file);
int opposite(int action);
void getQTable(Actions** qTable,int n,int m);
void getFinalStates(list<Point>& finalStates,ifstream& reader);
void getRewards(float** rewardsTable,int n,int m,float& totalReward,list<Point>& rewardsLocations,Point goalState,ifstream& reader);
float getReward(float** rewardsTable,Agent& agent,list<Point>& finalStates,list<Point>& rewardsLocations,Point goalState);

int main(int argc,char* argv[]){
	assert(argc==2);
	//qLearning(argv[1]);
	ppo(argv[1]);
	return 0;

}

void ppo(char* file){
	ifstream reader(file);

	int n;
	int m;
	Agent agent;
	Point goalState;
	reader>>n>>m;
	reader>>agent.location.i>>agent.location.j;
	reader>>goalState.i>>goalState.j;

	Actions** qTable=allocMatrix<Actions>(n,m);
	float** rewardsTable=allocMatrix<float>(n,m);
	float totalReward=0;
	list<Point> finalStates;
	list<Point> rewardsLocations;


	getQTable(qTable,n,m);
	getRewards(rewardsTable,n,m,totalReward,rewardsLocations,goalState,reader);
	getFinalStates(finalStates,reader);


	Memory paths[NR_PATHS];
	for(int i=0;i<EPOCHS;i++){
		if(i%NR_PATHS==0){
			learn();
		}
		runEpisodePPO(agent,qTable,rewardsTable,n,m,finalStates,i,totalReward,rewardsLocations,goalState,paths[i%NR_PATHS]);

	}

	printRewards(rewardsTable,n,m);
	printLegalActions(qTable,n,m);
	printQTable(qTable,n,m);
	printPolicy(qTable,n,m);
	deleteMatrix(qTable,n);
	deleteMatrix(rewardsTable,n);
}

void runEpisodePPO(Agent& agent,Actions** qTable,float** table,int n,int m,list<Point>& finalStates,int i,float totalReward,list<Point>& rewardsLocations,Point goalState,Memory paths[]){
	Point old;
	int action=-1;
	int step=0;
	int limit=2*n*m;
	list<Memory> path;
	float runReward=0;
	while(!gameOver(agent,finalStates) && step<limit){
		action = takeAction(qTable[agent.location.i][agent.location.j],n,m,i,action);
		old=agent.location;
		switch(action){
			case LEFT:
				agent.location.j--;
				break;
			case UP:
				agent.location.i--;
				break;
			case RIGHT:
				agent.location.j++;
				break;
			case DOWN:
				agent.location.i++;
				break;
		}
		path.push_front(agent.location)
		//qTable[agent.location.i][agent.location.j].nrVisits++;
		//float nrVisits = qTable[agent.location.i][agent.location.j].nrVisits;
		//float oldQ=qTable[old.i][old.j].actions[action];
		//float r=getReward(table,agent,finalStates,rewardsLocations,goalState)/totalReward;
		//float rFuture = maximum(qTable[agent.location.i][agent.location.j]);
		//float alpha = ALPHA/nrVisits;
		//qTable[old.i][old.j].actions[action] = (1.0-alpha)*oldQ + alpha*(r+DECAY*rFuture); 
		float r=getReward(table,agent,finalStates,rewardsLocations,goalState);
		runReward+=r;
		step++;
	}			

	float advantage;
	for(Point cell:path){
		advantage=qTable[cell.i][cell.j].

}

void qLearning(char* file){
	ifstream reader(file);

	int n;
	int m;
	Agent agent;
	Point goalState;
	reader>>n>>m;
	reader>>agent.location.i>>agent.location.j;
	reader>>goalState.i>>goalState.j;

	Actions** qTable=allocMatrix<Actions>(n,m);
	float** rewardsTable=allocMatrix<float>(n,m);
	float totalReward=0;
	list<Point> finalStates;
	list<Point> rewardsLocations;


	getQTable(qTable,n,m);
	getRewards(rewardsTable,n,m,totalReward,rewardsLocations,goalState,reader);
	getFinalStates(finalStates,reader);


	Point start=agent.location;
	for(int i=0;i<EPOCHS;i++){
		agent.location=start;
		runEpisode(agent,qTable,rewardsTable,n,m,finalStates,i,totalReward,rewardsLocations,goalState);
	}

	printRewards(rewardsTable,n,m);
	printLegalActions(qTable,n,m);
	printQTable(qTable,n,m);
	printPolicy(qTable,n,m);
	deleteMatrix(qTable,n);
	deleteMatrix(rewardsTable,n);
}

void runEpisode(Agent& agent,Actions** qTable,float** table,int n,int m,list<Point>& finalStates,int i,float totalReward,list<Point>& rewardsLocations,Point goalState){
	Point old;
	int action=-1;
	int step=0;
	int limit=2*n*m;
	while(!gameOver(agent,finalStates) && step<limit){
		action = takeAction(qTable[agent.location.i][agent.location.j],n,m,i,action);
		old=agent.location;
		switch(action){
			case LEFT:
				agent.location.j--;
				break;
			case UP:
				agent.location.i--;
				break;
			case RIGHT:
				agent.location.j++;
				break;
			case DOWN:
				agent.location.i++;
				break;
		}
		qTable[agent.location.i][agent.location.j].nrVisits++;
		float nrVisits = qTable[agent.location.i][agent.location.j].nrVisits;
		float oldQ=qTable[old.i][old.j].actions[action];
		float r=getReward(table,agent,finalStates,rewardsLocations,goalState)/totalReward;
		float rFuture = maximum(qTable[agent.location.i][agent.location.j]);
		float alpha = ALPHA/nrVisits;
		qTable[old.i][old.j].actions[action] = (1.0-alpha)*oldQ + alpha*(r+DECAY*rFuture); 
		step++;
	}			
}

void getQTable(Actions** qTable,int n,int m){
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			for(int k=0;k<NR_ACTIONS;k++){
				qTable[i][j].actions[k]=0;
				
				switch(k){
					case LEFT:
						if(j-1>=0) qTable[i][j].legalActions.push_back(LEFT);
						break;
					case UP:
						if(i-1>=0) qTable[i][j].legalActions.push_back(UP);
						break;
					case RIGHT:
						if(j+1<m) qTable[i][j].legalActions.push_back(RIGHT);
						break;
					case DOWN:
						if(i+1<n) qTable[i][j].legalActions.push_back(DOWN);
						break;
				}
			}
		}
	}
}

void getFinalStates(list<Point>& finalStates,ifstream& reader){
	int a;
	int b;
	while(reader>>a>>b){
		finalStates.push_back({a,b});
	}
}

void getRewards(float** rewardsTable,int n,int m,float& totalReward,list<Point>& rewardsLocations,Point goalState,ifstream& reader){
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			reader>>rewardsTable[i][j];
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			if(rewardsTable[i][j]>0){
				totalReward+=rewardsTable[i][j];
				if(i!=goalState.i || j!=goalState.j)
					rewardsLocations.push_back({i,j});
			}

}

const char* toString(int direction){
	switch(direction){
		case LEFT:
			return "left";
		case UP:
			return "up";
		case RIGHT:
			return "right";
		case DOWN:
			return "down";
	}
	return "";
}

const char* toSign(int direction){
	switch(direction){
		case LEFT:
			return "\u2B05";
		case UP:
			return "\u2B06";
		case RIGHT:
			return "\u27A1";
		case DOWN:
			return "\u2B07";
	}
	return "";
}

float getReward(float** rewardsTable,Agent& agent,list<Point>& finalStates,list<Point>& rewardsLocations,Point goalState){
	if(agent.nrRewards==rewardsLocations.size()){
		for(Point p:finalStates)
			if(p.i==agent.location.i && p.j==agent.location.j)
				return rewardsTable[agent.location.i][agent.location.j];
		return 0;
	}
	else {
		for(Point p:rewardsLocations){
			if(p.i==agent.location.i && p.i==agent.location.j)
				agent.nrRewards++;
		}
		if(agent.location.i==goalState.i && agent.location.j==goalState.j)
			return 0;
		return rewardsTable[agent.location.i][agent.location.j];
	}
}

int opposite(int action){
	if(action == UP)
		return DOWN;
	if(action == DOWN)
		return UP;
	if(action == LEFT)
		return RIGHT;
	if(action == RIGHT)
		return LEFT;
	return -1;
}

int takeAction(Actions& cell,int n,int m,int i,int oldAction){
	float epsilon = (1.0-((float)i)/EPOCHS);
	double r = ((double) rand() / RAND_MAX);
	if(r<epsilon){
		vector<int> out;
		sample(cell.legalActions.begin(),cell.legalActions.end(),back_inserter(out),1,mt19937{random_device{}()});
		return out[0];	
	}
	else{
		return argMax(cell,oldAction);
	}
}

float maximum(Actions& cell){
	float maxim=cell.actions[cell.legalActions.front()];
	for(int action:cell.legalActions)
		if(cell.actions[action]>maxim)
			maxim=cell.actions[action];
	return maxim;
}

int argMax(Actions& cell,int oldAction){
	int maxim=cell.legalActions.front();
	for(int action:cell.legalActions)
		if(cell.actions[action]>cell.actions[maxim])
			maxim=action;
	return maxim;
}

bool gameOver(Agent& agent,list<Point>& finalStates){
	for(auto pair:finalStates)
		if(pair.i==agent.location.i && pair.j==agent.location.j)
			return true;
	return false;
}

void printQTable(Actions** qTable,int n,int m){
	cout<<"Q Table: "<<endl;
	int width=5;
	int separatorWidth = m*3*width+m*2-1;	
	for(int i=0;i<n*3;i++){
		int subI=i%3;
		if(subI==0){
			cout<<"|";
			for(int j=0;j<separatorWidth;j++){
				cout<<"-";
			}
			cout<<"|"<<endl;
		}
		cout<<"|";
		if(subI==0 || subI==2){
			for(int j=0;j<m*3;j++){
				int subJ=j%3;
				cout.width(width);
				if(subJ==1){
					cout<<fixed<<setprecision(2)<<qTable[i/3][j/3].actions[subI==0?UP:DOWN];
				}
				else{
					cout<<" ";
				}
				if(subJ==2)
					cout<<" |";
			}
		}
		else{
			for(int j=0;j<m*3;j++){
				int subJ=j%3;
				cout.width(width);
				if(subJ==1){
					cout<<" ";
				}
				else{
					cout<<fixed<<setprecision(2)<<qTable[i/3][j/3].actions[subJ==0?LEFT:RIGHT];
				}
				if(subJ==2)
					cout<<" |";
			}
		}
		cout<<endl;
	}
	cout<<"|";
	for(int j=0;j<separatorWidth;j++){
		cout<<"-";
	}
	cout<<"|"<<endl;
}

void printLegalActions(Actions** qTable,int n,int m){
	cout<<"Legal actions: "<<endl;
	int width=1;
	int separatorWidth = m*3*width+m*2-1;	
	for(int i=0;i<n*3;i++){
		int subI=i%3;
		if(subI==0){
			cout<<"|";
			for(int j=0;j<separatorWidth;j++){
				cout<<"-";
			}
			cout<<"|"<<endl;
		}
		cout<<"|";
		if(subI==0 || subI==2){
			for(int j=0;j<m*3;j++){
				int subJ=j%3;
				cout.width(width);
				if(subJ==1){
					auto end=qTable[i/3][j/3].legalActions.end();
					auto begin=qTable[i/3][j/3].legalActions.begin();
					int action = subI==0?UP:DOWN;
					if(find(begin,end,action)!=end)
						cout<<toSign(action);
					else
						cout<<" ";	
					
				}
				else{
					cout<<" ";
				}
				if(subJ==2)
					cout<<" |";
			}
		}
		else{
			for(int j=0;j<m*3;j++){
				int subJ=j%3;
				cout.width(width);
				if(subJ==1){
					cout<<" ";
				}
				else{
					auto end=qTable[i/3][j/3].legalActions.end();
					auto begin=qTable[i/3][j/3].legalActions.begin();
					int action = subJ==0?LEFT:RIGHT;
					if(find(begin,end,action)!=end)
						cout<<toSign(action);
					else
						cout<<" ";	
				}
				if(subJ==2)
					cout<<" |";
			}
		}
		cout<<endl;
	}
	cout<<"|";
	for(int j=0;j<separatorWidth;j++){
		cout<<"-";
	}
	cout<<"|"<<endl;
}
		
void printPolicy(Actions** table,int n,int m){
	cout<<"Policy: "<<endl;
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			cout.width(3);
			cout<<toSign(argMax(table[i][j],-1))<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void printRewards(float** table,int n, int m){
	cout<<"Rewards: "<<endl;
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			cout.width(3);
			cout<<table[i][j]<<" ";
		}
		cout<<endl;
	}
}
