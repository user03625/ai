#include<iostream>

using namespace std;

template<typename T>
class Matrix{
    private:
        T* data;
    public:
        int n;
        int m;
    Matrix(int n,int m){
        this->n=n;
        this->m=m;
        data=new T[n*m];
        for(int i=0;i<n*m;i++)
            data[i]=0;
	}
        auto operator[](int i){
            return &data[i*m];
        }
        Matrix<T> operator+(Matrix<T> t){
            Matrix aux(n,m);
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    aux[i][j]=(*this)[i][j]+t[i][j];
                }
            }
            return aux;
        }
        Matrix<T> operator-=(Matrix<T> t){
            Matrix aux(n,m);
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    aux[i][j]=(*this)[i][j]-t[i][j];
                }
            }
            return aux;
        }
        Matrix<T> operator*(Matrix<T>& t){
            Matrix<T> aux(n,t.m);
            for(int i=0;i<aux.n;i++){
                for(int j=0;j<aux.m;j++){
                    for(int k=0;k<m;k++){
                        aux[i][j]+=(*this)[i][k]*t[k][j];
                    }
                }
            }
            return aux;
        }
        Matrix<T> operator+=(T s){
            Matrix aux(n,m);
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    aux[i][j]=(*this)[i][j]+s;
                }
            }
            return aux;
        }
        Matrix<T> operator-=(T s){
            Matrix<T> aux(n,m);
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    aux[i][j]=(*this)[i][j]-s;
                }
            }
            return aux;
        }
        Matrix<T> operator*=(T s){
            Matrix<T> aux(n,m);
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    aux[i][j]=data[i][j]*s;
                }
            }
            return aux;
        }
        void print(){
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    cout<<(*this)[i][j]<<' ';
                }
                cout<<'\n';
            }
        }

        ~Matrix(){
            if(data!=NULL){
                delete[] data;
                data=NULL;
            } 
        }
        
};
