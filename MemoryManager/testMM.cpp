#include "mm.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



const int ELEMENTS_COUNT = 100000;
const int BLOCK_SIZE = 10;

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;

    TestStruct()
    {
    };

    ~TestStruct()
    {
    };

    TestStruct(const TestStruct& struct_)
    {
        key = struct_.key;
        value1 = struct_.value1;
        value2 = struct_.value2;
    }
    bool operator==(const TestStruct& t) const
    {
        return (key == t.key) && (value1 == t.value1) && (value2 == t.value2);
    }

};


std::string randomString(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length,0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

static void generate(TestStruct *pts)
{
    pts->key = randomString(10);
    pts->value1 = randomString(20);
    pts->value2 = randomString(20);
}

typedef lab618::CMemoryManager<TestStruct> CMManager;

void TEST_TestStruct()
{
    TestStruct* arr[ELEMENTS_COUNT];
    for (auto & i : arr)
    {
        i = new TestStruct();
        generate(i);
    }

    for(TestStruct* & i : arr)
    {
        i->~TestStruct();
    }
    std::cout << "TEST_TestStruct: "  << "OK\n";
}

void TEST_NewDelete()
{
    TestStruct* arr[ELEMENTS_COUNT];
    CMManager manager(BLOCK_SIZE);
    for (auto & i : arr)
    {
        i = manager.newObject();
    }
    for (auto & i : arr)
    {
        manager.deleteObject(i);
    }
    std::cout << "TEST_NewDelete: "  << "OK\n";
}


int main() {
    TEST_TestStruct();
    TEST_NewDelete();
}


