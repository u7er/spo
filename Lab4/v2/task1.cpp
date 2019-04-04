#include <iostream>
#include <malloc.h>

using namespace std;

int main() {
    malloc_stats();
    cout << endl << endl;
    long totalSize = 0;
    for (int i = 1; i <= 1000; ++i) {
        int blockSize = i * 1024;
        //cout << malloc(size_t(blockSize)) << endl;
        totalSize += blockSize + 8;
    }
    
    cout << totalSize << endl;
    cout << endl << endl;
    malloc_stats();
    return 0;
}

