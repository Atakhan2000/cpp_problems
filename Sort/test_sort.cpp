#include "sort.h"
#include "profile.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>

const int ELEMENTS_COUNT = 1'000'000;

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

int CompareFunc(const int* pElem1, const int* pElem2)
{
  if (*pElem1 == *pElem2)
  {
    return 0;
  }
  else if (*pElem1 < *pElem2)
  {
    return 1;
  }
  else if (*pElem2 < *pElem1)
  {
    return -1;
  }
}

int strCompare(const TestStruct *pElem1, const TestStruct *pElem2)
{
  if (pElem1->key == pElem2->key)
  {
    return 0;
  }
  else if (pElem1->key < pElem2->key)
  {
    return 1;
  }
  else if (pElem2->key < pElem1->key)
  {
    return -1;
  }
}

int main()
{
  //numbers
  std::vector<int> randNum(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    randNum[i] = rand();
  }
  {
    LOG_DURATION("mergeSort for numbers");
    int **nums = new int *[randNum.size()];
    for (int i = 0; i < randNum.size(); ++i)
    {
      nums[i] = &randNum[i];
    }
    templates::mergeSort(nums, randNum.size(), CompareFunc);
  }
  std::vector<int> randNum_ = randNum;
  {
    LOG_DURATION("std::sort for numbers");
    std::sort(randNum_.begin(), randNum_.end());
  }


  //strings
  std::vector<TestStruct> random_str(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    generate(&random_str[i]);
  }
  {
    LOG_DURATION("mergeSort for strings");
    TestStruct **ppString_ = new TestStruct *[random_str.size()];
    for (int i = 0; i < random_str.size(); ++i)
    {
      ppString_[i] = &random_str[i];
    }
    templates::mergeSort(ppString_, random_str.size(), strCompare);
  }
  {
    LOG_DURATION("std::sort for strings");
    std::sort(random_str.begin(), random_str.end(), [](const TestStruct &pElem1, const TestStruct &pElem2){
      return pElem1.key < pElem2.key;
    });
  }
  return 0;
}


