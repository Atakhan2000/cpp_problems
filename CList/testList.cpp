#include "list.h"
#include <cassert>
#include <string>
#include <queue>
#include <stack>
#include <deque>

const int ELEMENTS_COUNT = 1000;

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
    bool operator==(const TestStruct& t) const {
        return (key == t.key) && (value1 == t.value1) && (value2 == t.value2);
    }
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);

    // сделать случайную строку
    for (int i = 0; i < length; ++i) {
        s.push_back('a' + rand() % 25);
    }
    s += "tmp";
    return s;
}

static void generate(TestStruct *pts)
{
    pts->key = makeRandomString(1, 2);
    pts->value1 = makeRandomString(1, 2);
    pts->value2 = makeRandomString(5, 1);
}

void TestListFunction()
{
    ///-------------------------------------------------------------------------------------
    ///-----------------------------тест односвязного списка--------------------------------
    ///-------------------------------------------------------------------------------------
    TestSingleList single_list;
    std::queue<TestStruct> test_q;
    std::stack<TestStruct> test_s;
    std::deque<TestStruct> test_d;

    //------------------------------PushFront--------------------------------------
    //заполняем спереди
    for (int i = 0; i < ELEMENTS_COUNT / 2; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushFront(ts);
        test_s.push(ts);
    }
    assert(single_list.getSize() == ELEMENTS_COUNT / 2);

    //------------------------------PushBack--------------------------------------
    //заполняем сзади
    for (int i = 0; i < ELEMENTS_COUNT / 2; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushBack(ts);
        test_q.push(ts);
    }
    assert(single_list.getSize() == (ELEMENTS_COUNT / 2 + ELEMENTS_COUNT / 2));

    //------------------------- Begin isValid operator++ -------------------------
    //проверяем что получилось через те же функции стека и очереди
    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it)
    {
        if (!test_s.empty()) {
            assert(it.getData() == test_s.top());
            test_s.pop();
            continue;
        }
        if (!test_s.empty()) {
            assert(it.getData() == test_q.front());
            test_q.pop();
        }
    }
    assert(single_list.getSize() == (ELEMENTS_COUNT / 2 + ELEMENTS_COUNT / 2));

    //----------------------------- Erase --------------------------------------
    //проверка erase
    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it)
    {
        single_list.erase(it);
    }
    assert(single_list.getSize() == 0);

    //----------------------------- Clear --------------------------------------
    //проверка clear
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushFront(ts);
    }
    single_list.clear();
    assert(single_list.getSize() == 0);
//    return;
    //----------------------------- Random -------------------------------------
    //рандомизированная провернка на pushFront и pushBack
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        TestStruct ts;
        generate(&ts);

        bool make_pushFront = rand() % 2;
        if(make_pushFront) {
            single_list.pushFront(ts);
            test_d.push_front(ts);
        } else {
            single_list.pushBack(ts);
            test_d.push_back(ts);
        }
    }

    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it) {
        assert(it.getData() == test_d.front());
        test_d.pop_front();
        single_list.erase(it);
    }
    assert(single_list.getSize() == 0);


    ///-------------------------------------------------------------------------------------
    ///-----------------------------тест двусвязного списка---------------------------------
    ///-------------------------------------------------------------------------------------

    TestList list;
    test_d.clear();
    //------------------------------PushFront--------------------------------------
    //заполняем спереди
    for (int i = 0; i < ELEMENTS_COUNT / 2; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushFront(ts);
        test_d.push_front(ts);
    }
    assert(list.getSize() == ELEMENTS_COUNT / 2);

    //------------------------------PushBack--------------------------------------
    //заполняем сзади
    for (int i = 0; i < ELEMENTS_COUNT / 2; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
        test_d.push_back(ts);
    }
    assert(list.getSize() == (ELEMENTS_COUNT / 2 + ELEMENTS_COUNT / 2));

    //------------------------- Begin isValid operator++ -------------------------
    //проверяем что получилось через те же функции дека и оператор ++

    for (TestList::CIterator it = list.begin(); it.isValid(); ++it)
    {
        assert(it.getData() == test_d.front());
        test_d.push_back(test_d.front());
        test_d.pop_front();
    }
    assert(list.getSize() == (ELEMENTS_COUNT / 2 + ELEMENTS_COUNT / 2));

    //------------------------- End isValid operator-- -------------------------
    //проверяем что получилось через те же функции дека и оператор ++
    for (TestList::CIterator it = list.end(); it.isValid(); --it)
    {
        assert(it.getData() == test_d.back());
        test_d.push_front(test_d.back());
        test_d.pop_back();
    }
    assert(list.getSize() == (ELEMENTS_COUNT / 2 + ELEMENTS_COUNT / 2));

    //----------------------------- Erase --------------------------------------
    //проверка erase

    TestList::CIterator it = list.begin();
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        assert(it.getData() == test_d.front());
//        std::cout << it.getData().key << " " << test_d.front().key << "\n";
        test_d.push_back(test_d.front());
        list.pushBack(test_d.front());
        test_d.pop_front();
        list.erase(it);
        ++it;
    }
    assert(list.getSize() == ELEMENTS_COUNT);

    //------------------------- Erase EraseAndNext -----------------------------
    it = list.end();
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
//        test_d.push_front(test_d.back());
        list.pushFront(test_d.back());
        test_d.pop_back();
        list.eraseAndNext(it);
        --it;
    }
    assert(list.getSize() == ELEMENTS_COUNT);


    //----------------------------- Clear --------------------------------------
    //проверка clear
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushFront(ts);
    }
    list.clear();
    assert(list.getSize() == 0);

    //----------------------------- Random -------------------------------------
    //рандомизированная провернка на pushFront и pushBack
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        TestStruct ts;
        generate(&ts);

        bool make_pushFront = rand() % 2;
        if(make_pushFront) {
            list.pushFront(ts);
            test_d.push_front(ts);
        } else {
            list.pushBack(ts);
            test_d.push_back(ts);
        }
    }

    for (TestList::CIterator it = list.begin(); it.isValid(); ++it) {
        assert(it.getData() == test_d.front());
        test_d.pop_front();
        list.erase(it);
    }
    assert(list.getSize() == 0);
}

int main()
{
    TestListFunction();
}

