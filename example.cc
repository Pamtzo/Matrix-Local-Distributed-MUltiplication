/*
Paralelismo local calculando celda por celda de una matriz con hilos
*/
#include <iostream>
#include <vector>
#include <chrono>

#include "Threadpool.h"

using namespace std;
const int n = 3;

int multiplicacion(const int A[n][n], const int B[n][n], int i, int j){
    int resultado = 0;
    for(int k=0;k<n;k++)
        resultado += A[i][k]*B[k][j];
    return resultado;
}

int* multiplicacionf(const int A[n][n], const int B[n][n],int* C, int i){
    for(int j=0;j<n;j++){
        for(int k=0;k<n;k++)
            C[j] += A[i][k]*B[k][j];
    }
    return C;
}


int main()
{
    int A[n][n] = {{1,2,3},  //  1 2 3     1 2 3     6 12 18
                   {2,3,4},  //  2 3 4  X  1 2 3  =  9 18 27
                   {3,4,5}}; //  3 4 5     1 2 3    12 24 36

    int B[n][n] = {{1,2,3},
                   {1,2,3},
                   {1,2,3}};
    // cout <<thread::hardware_concurrency() << endl;
    ThreadPool pool(thread::hardware_concurrency());
    /*
    vector<future<int>> results;
    for(int i = 0; i < n;i++){
        for(int j = 0;j<n;j++){
            // cout<<i<<j<<endl;
            results.emplace_back(
                pool.enqueue([A,B,i,j] {
                    return multiplicacion(A,B,i,j);
                })
            );
        }
    }
    for(auto && result: results){
        int resultados = result.get();
        cout << resultados << ' '<<*&resultados<<' ';
    }
    */
    int matrizresult[n][n];
    int* col;
    vector<future<int*>> results;
    for(int i = 0; i < n;i++){
        results.emplace_back(
            pool.enqueue([A,B,i,col] {
                return multiplicacionf(A,B, col, i);
            })
        );
    }
    for(auto && result: results){
        int* resultados = result.get();
        cout << resultados <<' ';
    }
    return 0;
}