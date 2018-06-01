#include <iostream>
#include <vector>
#include <memory>

using namespace std;
typedef unsigned long long ull;


int main() {
    std::shared_ptr<std::vector<ull>> _data;
    std::shared_ptr<std::vector<ull>> _data1;

    new (&_data) std::shared_ptr<std::vector<ull>>(new std::vector<ull>(5));

    for(int i = 0; i < 5; ++i) {
        _data.get()[0][i] = i;
    }
    new (&_data1) std::shared_ptr<std::vector<ull>>(_data);

    for(int i = 0; i < 5; ++i) {
        cout << _data1.get()[0][i];
    }
}