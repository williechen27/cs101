#include <iostream>
#include <string>
using namespace std;

template <class T>
int find(T a[], int size, T x) {
    int i = 0;
    int result;
    for (i=0; i<size; i++) {
        if (x == a[i]) {
            result = i;
            break;
        }
    }
    return result;
}

int main() {
    int a[5] = {1,2,3,4,5};
    double b[5] = {1.1,2.2,3.3,4.4,5.5};
    cout << "a[" << find(a, 5, 3) << "] = " << a[find(a, 5, 3)] << endl;
    cout << "b[" << find(b, 5, 4.4) << "] = " << b[find(b, 5, 4.4)] << endl;
    return 0;
}
