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
};

struct Coord{
	int i;
	int j;
};


const int LEFT=0;
const int UP=1;
const int RIGHT=2;
const int DOWN=3;

const int EPOCHS = 10000;
const float EPSILON = 0.2;
const float ALPHA = 0.4;
const float DECAY = 1;
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

void runEpisode(Coord lizard,Actions** qTable,float** table,int n,int m,list<pair<int,int>>& finalStates);
int takeAction(Coord lizard,Actions& cell,int n,int m);
float maximum(Actions& cell);
bool gameOver(Coord lizard,list<pair<int,int>>& finalStates);
void printQTable(Actions** qTable,int n,int m);
const char* toString(int direction);
void printPolicy(Actions** table,int n,int m);
int argMax(Actions& cell);
void printLegalActions(Actions** table,int n,int m);
void printRewards(float** table,int n,int m);

int main(int argc,char* argv[]){
	assert(argc==2);
	ifstream reader(argv[1]);
	int n;
	int m;
	reader>>n>>m;
	Actions** qTable=allocMatrix<Actions>(n,m);
	Coord lizard;
	reader>>lizard.i>>lizard.j;

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

	float** table=allocMatrix<float>(n,m);
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			reader>>table[i][j];
	list<pair<int,int>> finalStates;
	int a;
	int b;
	while(reader>>a>>b){
		finalStates.push_back(make_pair(a,b));
	}


	for(int i=0;i<EPOCHS;i++){
		runEpisode(lizard,qTable,table,n,m,finalStates);
	}
	printRewards(table,n,m);
	printLegalActions(qTable,n,m);
	printQTable(qTable,n,m);
	printPolicy(qTable,n,m);
	deleteMatrix(qTable,n);
	deleteMatrix(table,n);
	return 0;
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

void runEpisode(Coord lizard,Actions** qTable,float** table,int n,int m,list<pair<int,int>>& finalStates){
	Coord old;
	int action;
	int step=0;
	int limit=1000;
	while(!gameOver(lizard,finalStates) && step<limit){
		action = takeAction(lizard,qTable[lizard.i][lizard.j],n,m);
		old=lizard;
		//cout<<lizard.i<<" "<<lizard.j<<" "<<toString(action)<<endl;
		switch(action){
			case LEFT:
				lizard.j--;
				break;
			case UP:
				lizard.i--;
				break;
			case RIGHT:
				lizard.j++;
				break;
			case DOWN:
				lizard.i++;
				break;
		}
		qTable[old.i][old.j].actions[action]= (1.0-ALPHA)*qTable[old.i][old.j].actions[action]+ALPHA*(table[lizard.i][lizard.j]+DECAY*maximum(qTable[lizard.i][lizard.j])-qTable[old.i][old.j].actions[action]);
		//qTable[old.i][old.j].actions[action]= (1.0-ALPHA)*qTable[old.i][old.j].actions[action]+ALPHA*(table[lizard.i][lizard.j]+DECAY*maximum(qTable[lizard.i][lizard.j]));
		step++;
	}			
}

int takeAction(Coord lizard,Actions& cell,int n,int m){
	double r = ((double) rand() / RAND_MAX);
	if(r<EPSILON){
		vector<int> out;
		sample(cell.legalActions.begin(),cell.legalActions.end(),back_inserter(out),1,mt19937{random_device{}()});
		return out[0];	
	}
	else{
		return argMax(cell);
	}
}

float maximum(Actions& cell){
	float maxim=cell.actions[cell.legalActions.front()];
	for(int action:cell.legalActions)
		if(cell.actions[action]>maxim)
			maxim=cell.actions[action];
	return maxim;
}
bool gameOver(Coord lizard,list<pair<int,int>>& finalStates){
	for(auto pair:finalStates)
		if(pair.first==lizard.i && pair.second==lizard.j)
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
			cout<<toSign(argMax(table[i][j]))<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

int argMax(Actions& cell){
	int maxim=cell.legalActions.front();
	for(int action:cell.legalActions)
		if(cell.actions[action]>cell.actions[maxim])
			maxim=action;
	return maxim;
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
