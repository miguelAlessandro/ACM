#include <iostream>

using namespace::std;

int main(){
    int n;
    cin >> n;
    cout << (1LL << (n+1)) - 2LL << endl;
    return 0;
}
