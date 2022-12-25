#include <iostream>
#include <cassert>
#include "Array.h"

static int cnt = 7;

class Test {
public:
    Test() : value(new int) {
        (*value) = cnt;
    }
    ~Test() {
        delete value;
    }
    int get() {
        assert(*(this->value) == cnt);
        return (*value);
    }
private:
    int* value;
};

int main() {
    Array<std::string> strArr;
    strArr.insert("cd");
    strArr.insert(0, "huhuhu");
    strArr.remove(1);
    for (auto iter = strArr.iterator(); iter.hasNext(); iter.next()) {
        std::cout << iter.get() << std::endl;
    }
}

