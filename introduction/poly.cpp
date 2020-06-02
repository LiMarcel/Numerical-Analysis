/* 
 * 使用并行计算的方法计算多项式的值
*/

#include <iostream>
#include <mpi.h>
#include <cmath>
#include <random>

using namespace std;

int main(int argc, char* argv[])
{
    int N = 100;
    double X = 1.1;
    double XX = pow(X, 2);
    double* para = new double[N];

    default_random_engine rng;
    uniform_real_distribution<double> U(0, 100);
    for(int i = 0; i < N; i++)
    {
        para[i] = U(rng);
        // para[i] = i;
        // cout << para[i] << " ";
    }
    // cout << endl;

    double begin, end;
    int myrank;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    begin = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // MPI_Comm_size(MPI_COMM_WORLD, &mysize);

    int start = ((N - 1) % 2 == myrank)?(N - 1):(N - 2);
    double result = para[start];
    for(int i = start - 2; i >= 0 ; i = i - 2)
    {
        result = result * XX + para[i];
    }

    if(myrank == 1)
    {
        result = result*X;
        MPI_Send(&result, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
    }
    else if(myrank == 0)
    {
        double temp_result;
        MPI_Recv(&temp_result, 1, MPI_DOUBLE, 1, 99, MPI_COMM_WORLD, &status);
        result += temp_result;
        end = MPI_Wtime();
        cout << "result = " << result << endl;
        cout << "time = " << end - begin << endl;
    }

    MPI_Finalize();

    return 0;
}



