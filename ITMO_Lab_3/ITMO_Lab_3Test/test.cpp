#include "pch.h"
#include "gtest/gtest.h"
#include "../../ITMO_Lab_2/ITMO_Lab_2/Array.h"
#include "../ITMO_Lab_3/Sort.h"


static const int seed = 0;

TEST(SortTest, StringSortingAscending)
{
    std::string a[5] = { "abs", "c", "fasd", "hgsdfgsafasd", "fu" };
    ASSERT_NO_THROW(sort(a, a + 5, [](auto a, auto b) { return a.size() < b.size(); }));
    for (int i = 0; i < 4; ++i)
        ASSERT_LE(a[i].size(), a[i + 1].size());
}

TEST(SortTest, IntSortingAscending)
{
    int a = 5;
    ASSERT_NO_THROW(sort(&a, &a, [](auto a, auto b) { return a < b; }));
    EXPECT_EQ(a, 5);
}

TEST(SortTest, FiveIntSortingAscending)
{
    int a[5] = { 5, 2, 6, 8, 1 };
    ASSERT_NO_THROW(sort(a, a + 5, [](auto a, auto b) { return a < b; }));

    for (int i = 0; i < 4; ++i)
        ASSERT_LE(a[i], a[i + 1]);
}

TEST(SortTest, HundredIntSortingAscending)
{
    srand(seed);
    int a[100];
    for (auto& i : a)
        i = rand() % 100;

    ASSERT_NO_THROW(sort(a, a + 100, [](auto a, auto b) { return a < b; }));

    for (int i = 0; i < 99; ++i) {
        std::cout << a[i] << " ";
    }

    for (int i = 0; i < 99; ++i) {
        ASSERT_LE(a[i], a[i + 1]);
    }
    
    
}

TEST(SortTest, HundredIntSortingDescending)
{
    srand(seed);
    int a[100];
    for (auto& i : a)
        i = rand() % 100;

    ASSERT_NO_THROW(sort(a, a + 100, [](auto a, auto b) { return a > b; }));

    for (int i = 0; i < 99; ++i)
        ASSERT_GE(a[i], a[i + 1]);
}

TEST(SortTest, FiveFloatSortingAscending)
{
    float a[5] = { 5.2f, 2.5f, 6.1f, 8.7f, 1.9f };
    ASSERT_NO_THROW(sort(a, a + 5, [](auto a, auto b) { return a < b; }));

    for (int i = 0; i < 4; ++i)
        ASSERT_LE(a[i], a[i + 1]);
}

TEST(SortTest, HundredFloatSortingAscending)
{
    srand(seed);
    float a[100];
    for (auto& i : a)
        i = rand() % 100 + 1.0f / (rand() + 1);

    ASSERT_NO_THROW(sort(a, a + 100, [](auto a, auto b) { return a < b; }));

    for (int i = 0; i < 99; ++i)
        ASSERT_LE(a[i], a[i + 1]);
}

TEST(SortTest, HundredFloatSortingDescending)
{
    srand(seed);
    float a[100];
    for (auto& i : a)
        i = rand() % 100 + 1.0f / (rand() + 1);

    ASSERT_NO_THROW(sort(a, a + 100, [](auto a, auto b) { return a > b; }));

    for (int i = 0; i < 99; ++i)
        ASSERT_GE(a[i], a[i + 1]);
}

TEST(SortTest, HundredFloatInArraySortingAscending)
{
    srand(seed);
    Array<float> a;
    for (int i = 0; i < 100; i++)
        a.insert(rand() % 100 + 1.0f / (rand() + 1));

    ASSERT_NO_THROW(sort(a.begin(), a.end(), [](auto a, auto b) { return a < b; }));

    for (int i = 0; i < 99; ++i)
        ASSERT_LE(a[i], a[i + 1]);
}

TEST(SortTest, HundredStructInArraySortingAscending)
{
    struct test
    {
        int id;
        float value;
    };

    srand(seed);
    Array<test> a;
    for (int i = 0; i < 100; i++)
        a.insert(test{ rand(), rand() % 100 + 1.0f / (rand() + 1) });

    ASSERT_NO_THROW(sort(a.begin(), a.end(), [](auto a, auto b) { return a.value < b.value; }));

    for (int i = 0; i < 99; ++i)
        ASSERT_LE(a[i].value, a[i + 1].value);
}

TEST(SortTest, NullSortingThrow)
{
    ASSERT_ANY_THROW(sort((int*)nullptr, (int*)nullptr, [](auto a, auto b) { return a < b; }));
}

TEST(SortTest, EmptyArraySortingSameBegin)
{
    Array<int> a;
    auto was = *a.begin();
    ASSERT_NO_THROW(sort(a.begin(), a.end(), [](auto a, auto b) { return a < b; }));

    EXPECT_EQ(*a.begin(), was);
}