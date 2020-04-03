#include <iostream>
#include <thread>
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <iomanip>
#include <mutex>

using namespace std;


void rand_array(int *arr, int n, int a = 0, int b = 10)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % (b - a) + a;
    }
}


void print_array(int *arr, int n)
{
    cout << "{ ";
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << ";";
    }
    cout << "}" << endl;
}



const int MAX_COUNT_SIN = 20;
mutex mutex_sum;

void calc_sum(int *arr, int n, int index, int step, int &total_sum)
{
    int sum = 0;
    for (int i = index; i < n; i += step)
    {
        int element = arr[i];
        for (int j = 0; j < MAX_COUNT_SIN; j++)
        {
            element = int(10 * sin(element));
        }
        sum += element;
    }
    lock_guard<mutex> lock(mutex_sum);
    total_sum += sum;
}


int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "Programm: many thread calculate sum with mutex" << endl;
    cout << endl;

    // ============ input ======================
    int n, m;
    do
    {
        cout << "n elements = ";
        cin >> n;
    }
    while(n <= 0);

    do
    {
        cout << "m threads = ";
        cin >> m;
    }
    while (m > n || m <= 0);
    // ==========================================


    struct timeval tv0, tv1;        // time variables
    gettimeofday(&tv0, nullptr);


    // ================ create and fill array ===============

    int *arr = new int[n];
    int sum = 0;

    rand_array(arr, n);
    //print_array(arr, n);
    // ======================================================


    // ============ create threads ==========================
    std::thread **ths = new std::thread*[m];

    for (int i = 0; i < m; i++)
    {
        ths[i] = new std::thread(calc_sum, arr, n, i, m, std::ref(sum));
    }
    // ======================================================

    for (int i = 0; i < m; i++)
    {
        ths[i]->join();
    }

    cout << "sum: " << sum << endl;

    //======================= free memory ===================
    delete [] arr;

    for (int i = 0; i < m; i++)
    {
        delete ths[i];
    }
    delete [] ths;
    // ==============================================================

    gettimeofday(&tv1, nullptr);

    cout << "time: " << static_cast<double>(tv1.tv_usec - tv0.tv_usec) / 1000000
                        + static_cast<double>(tv1.tv_sec - tv0.tv_sec) << " sec" << endl;
    return 0;
}
