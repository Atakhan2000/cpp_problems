#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include "profile.h"

#include "hash.h"
#include "sort.h"

struct TestStruct {
  std::wstring key1;
  std::wstring key2;
  std::wstring value1;
};


const wchar_t ALPHABET[] =
    L"01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзи"
    L"йклмнопрстуфхцчшщьъэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЪЭЮЯ";
const int LENGTH = sizeof(ALPHABET) / sizeof(wchar_t);

static std::wstring makeRandomString(int minL = 7, int maxL = 20) {
  int length = rand() % maxL + minL;
  std::wstring s;
  s.reserve(length);
  wchar_t ch;
  for (int i = 0; i < length; i++) {
    int choose = rand() % (LENGTH - 1);
    ch = ALPHABET[choose];
    s += ch;
  }
  return s;
}


static void generate(TestStruct* pts) {
  pts->key1 = makeRandomString();
  pts->key2 = makeRandomString();
  pts->value1 = makeRandomString();
}


int Compare(const TestStruct* el1, const TestStruct* el2) {
  int cmp = std::wcscmp(el1->key1.c_str(), el2->key1.c_str());
  if (0 != cmp) {
    return cmp;
  }

  return std::wcscmp(el1->key2.c_str(), el2->key2.c_str());
}

unsigned int HashFunc(const TestStruct* pElement )
{
  const int p = 31;
  unsigned int hash = 0;
  size_t p_pow = 1;

  for (char i: (pElement->key1 + pElement->key2))
  {
    hash += (i-'A') * p_pow;
    p_pow *= p;
  }
  return hash;
}

void generateArray(TestStruct* pArray, int count) {
  for (int i = 0; i < count; i++) {
    generate(&pArray[i]);
  }
}


TestStruct* binarySearch(const TestStruct& element, TestStruct** pPArray, int size) {
  if (0 == size) return nullptr;
  TestStruct el = element;
  int cmp = Compare(&el, pPArray[size / 2]);
  if (cmp > 0) {
    return binarySearch(element, pPArray, size / 2);
  } else if (cmp < 0) {
    if (size == 1) return nullptr;
    return binarySearch(element, pPArray + size / 2, size - size / 2);
  } else {
    return pPArray[size / 2];
  }
}

void accommodation(const int ELEMENTS_COUNT, TestStruct* nElementsArray,
                   TestStruct** nPElementsArray, lab618::CHash<TestStruct, HashFunc, Compare>& h_table)
{

  {
    LOG_DURATION("Sort_time");
    templates::mergeSort(nPElementsArray, ELEMENTS_COUNT, Compare);
  }

  {
    LOG_DURATION("HashTable_time");
    for (int i = 0; i < ELEMENTS_COUNT; i++)
    {
      h_table.add(&nElementsArray[i]);
    }
  }

}


// поиск
// Найти один раз все объекты из исходных данных;
void search(int ELEMENTS_COUNT, TestStruct* nElementsArray,TestStruct** nPElementsArray,
            lab618::CHash<TestStruct, HashFunc, Compare>& h_table) {

  //Замер сортировки
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(&(nElementsArray[i]) == binarySearch(nElementsArray[i], nPElementsArray, ELEMENTS_COUNT));
  }

  {
    LOG_DURATION("Sort_time_search");
    for (int i = 0; i < ELEMENTS_COUNT; i++) {
      binarySearch(nElementsArray[i], nPElementsArray, ELEMENTS_COUNT);
    }
  }

  //Замер хеш таблицы
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(&(nElementsArray[i]) == h_table.find(nElementsArray[i]));
  }

  {
    LOG_DURATION("HashTable_time_search");
    for (int i = 0; i < ELEMENTS_COUNT; i++) {
      h_table.find(nElementsArray[i]);
    }
  }
}


// поиск 2N объектов
void search2(int ELEMENTS_COUNT, TestStruct** nPElementsArray, TestStruct* doubleNELementsArray,
            lab618::CHash<TestStruct, HashFunc, Compare>& h_table) {

  //Замер сортировки
  {
    LOG_DURATION("Sort_time_search2");
    for (int i = 0; i < 2*ELEMENTS_COUNT; i++) {
      binarySearch(doubleNELementsArray[i], nPElementsArray, ELEMENTS_COUNT);
    }
  }

  //Замер хеш таблицы
  {
    LOG_DURATION("HashTable_time_search2");
    for (int i = 0; i < 2*ELEMENTS_COUNT; i++) {
      h_table.find(doubleNELementsArray[i]);
    }
  }
}


// удаление
void deleteElements(const int ELEMENTS_COUNT, TestStruct* nElementsArray, TestStruct** nPElementsArray,
                    TestStruct* doubleNELementsArray, lab618::CHash<TestStruct, HashFunc, Compare>& h_table)
{
  //Замер сортировки
  {
    LOG_DURATION("Sort_time_delete");
    delete[] nPElementsArray;
  }

  //Замер хеш таблицы
  {
    LOG_DURATION("HashTable_time_delete");
    h_table.clear();
  }
}


void deleteTreeHash(const int ELEMENTS_COUNT, TestStruct* nElementsArray,
                    lab618::CHash<TestStruct, HashFunc, Compare>& h_table) {

  //Замер хеш таблица
  {
    LOG_DURATION("HashTable_time_deleteTreeHash");
    for (int i = 0; i < ELEMENTS_COUNT; i++) {
      h_table.remove(nElementsArray[i]);
    }
  }
}


void timeMeasurement( int ELEMENTS_COUNT,
                     TestStruct* nElementsArray,
                     TestStruct* doubleNELementsArray)
{
  lab618::CHash<TestStruct, HashFunc, Compare> h_table(ELEMENTS_COUNT, 100000);

  TestStruct** nPElementsArray = new TestStruct*[ELEMENTS_COUNT];
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    nPElementsArray[i] = &(nElementsArray[i]);
  }
}


int main() {
  const int N = 1000000;
  int doubleN = 2 * N;
  int leftLim = 10000;
  int rightLim = 1000000;
  TestStruct* nElementsArray = new TestStruct[N];
  TestStruct* doubleNELementsArray = new TestStruct[doubleN];

  {
    LOG_DURATION("TIME");
    generateArray(nElementsArray, N);
    generateArray(doubleNELementsArray, doubleN);
  }

  for (int i = leftLim; i < rightLim; i = int(double(i) * 1.2f)) {
    std::cout << std::to_string(i) << "\n";
    timeMeasurement(i, nElementsArray, doubleNELementsArray);
  }
  delete[] nElementsArray;
  delete[] doubleNELementsArray;
  return 0;
}

