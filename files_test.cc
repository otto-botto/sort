#include "gtest/gtest.h"

extern "C" {
#include "files_server.h"
}

TEST(FirstTest, IntegerTest) {
    EXPECT_EQ(5, test_gtest());
}

int* return_array() {
    int* num_array;
    num_array = (int*)malloc(sizeof(int) * 5);
    num_array[0] = 100;
    num_array[1] = -76;
    num_array[2] = 3;
    num_array[3] = 1;
    num_array[4] = 100;
    return num_array;
}

void free_array(int* array){
    free(array);
}

void free_array_str(void *string) {
    free(string);
}

TEST(TestNumSort, SortNumOne) {
    int* array = return_array();
    sort_numbers(array, 5);

    EXPECT_EQ(-76, array[0]);
    EXPECT_EQ(1, array[1]);
    EXPECT_EQ(3, array[2]);
    EXPECT_EQ(100, array[3]);
    EXPECT_EQ(100, array[4]);
    free_array(array);
}

TEST(TestStrSort, SortStrOne) {
    char* strings[] = {"otto", "botto", "mom", "man"};

    sort_alpha(strings, 4);

    EXPECT_EQ("botto", strings[0]);
    EXPECT_EQ("man", strings[1]);
    EXPECT_EQ("mom", strings[2]);
    EXPECT_EQ("otto", strings[3]);
}

TEST(TestStrSort, SortStrTwo) {
    char* strings[] = {"otto", "botto", "manhood", "man"};

    sort_alpha(strings, 4);

    EXPECT_EQ("botto", strings[0]);
    EXPECT_EQ("man", strings[1]);
    EXPECT_EQ("manhood", strings[2]);
    EXPECT_EQ("otto", strings[3]);
}

TEST(TestStrSort, SortDates) {
    char* strings[] = {"1986-04-19", "1986-04-15", "2004-11-23", "2024-12-31"};

    sort_alpha(strings, 4);

    EXPECT_EQ("1986-04-15", strings[0]);
    EXPECT_EQ("1986-04-19", strings[1]);
    EXPECT_EQ("2004-11-23", strings[2]);
    EXPECT_EQ("2024-12-31", strings[3]);
}