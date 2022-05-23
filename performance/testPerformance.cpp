#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;

// global
const unsigned arraySize = 32768;
const unsigned roundTime = 100000;
int nums[arraySize];

void init() {
    for (unsigned i = 0; i < arraySize; ++i) {
        nums[i] = rand() % 256;
    }
}

void test1() {
    auto startTime = chrono::system_clock::now();
    long long sum = 0;
    for (unsigned t = 0; t < roundTime; ++t) {
        for (unsigned i = 0; i < arraySize; ++i) {
            if (nums[i] >= 128) {
                sum += nums[i];
            }
        }
    }
    auto endTime = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    double elapsed = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
    cout << __func__ << " spend " << elapsed << "(s)" << endl;
    //cout << sum << endl;
}

void test2() {
    auto startTime = chrono::system_clock::now();
    long long sum = 0;
    for (unsigned t = 0; t < roundTime; ++t) {
        for (unsigned i = 0; i < arraySize; ++i) {
            if (nums[i] >= 128) {
                sum += nums[i];
            }
        }
    }
    auto endTime = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    double elapsed = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
    cout << __func__ << " spend " << elapsed << "(s)" << endl;
    cout << "sum " << sum << endl;
}

void test3() {
    std::sort(nums, nums + arraySize);

    auto start = chrono::system_clock::now();
    long long sum = 0;
    for (unsigned t = 0; t < roundTime; ++t) {
        for (unsigned i = 0; i < arraySize; ++i) {
            if (nums[i] >= 128) {
                sum += nums[i];
            }
        }
    }
    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
    double elapsed  = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
    cout << __func__ << " spend " << elapsed << "(s)" << endl;
    cout << "sum " << sum << endl;
}

void test4() {
    auto start = chrono::system_clock::now();
    long long sum = 0;
    for (unsigned t = 0; t < roundTime; ++t) {
        for (unsigned i = 0; i < arraySize; ++i) {
            int t = (nums[i] - 128) >> 31;
            sum += ~t & nums[i];
        }
    }
    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
    double elapsed  = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
    cout << __func__ << " spend " << elapsed << "(s)" << endl;
    cout << "sum " << sum << endl;
}

int main(int argc, char *argv[])
{
    init();

    test1();

    test2();

    test4();

    test3();

    return 0;
}
