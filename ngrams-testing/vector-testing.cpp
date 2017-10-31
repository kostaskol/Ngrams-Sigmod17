#include "mvector.hpp"
#include <gtest/gtest.h>

using mstd::vector;

class VectorTest : public ::testing::Test {
public:
    vector<int> v;
    VectorTest() {
        v;
    }
};

TEST_F(VectorTest, sizeTest) {
    for (int i = 0; i < 100; i++) {
        v.push(i);
    }
    EXPECT_EQ(v.size(), 100);
}

TEST_F(VectorTest, capacityIncTest) {
    EXPECT_EQ(v.capacity(), 1);
    v.push(5);
    EXPECT_EQ(v.capacity(), 1);
    v.push(6);
    EXPECT_EQ(v.capacity(), 2);
    v.push(2);
    EXPECT_EQ(v.capacity(), 4);
    v.push(1);
    EXPECT_EQ(v.capacity(), 4);
}

TEST_F(VectorTest, capacityDecTest) {
    EXPECT_EQ(v.capacity(), 1);
    v.push(5); v.push(7); v.push(2); v.push(1);
    EXPECT_EQ(v.capacity(), 4);
    v.remove_at(0);
    v.remove_at(0);
    EXPECT_EQ(v.capacity(), 4);
}

TEST_F(VectorTest, outOfRangeTest) {
    try {
        v[0];
        FAIL() << "Out of range not thrown";
    } catch (std::out_of_range &e) {
        SUCCEED() << "Out of range thrown";
    } catch (...) {
        FAIL() << "Unknown exception thrown";
    }
}

TEST_F(VectorTest, pushTest) {
    v.push(5);
    EXPECT_EQ(v[0], 5);
}

TEST_F(VectorTest, insertAtTest) {
    int x = 2;
    v.m_insert_at(0, x);
    v.push(3);
    x = 10;
    v.m_insert_at(2, x);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 10);

}

TEST_F(VectorTest, removeAtTest) {
	v.push(5); v.push(7); v.push(2); v.push(1);
	v.remove_at(0);
	EXPECT_NE(v[0], 5);
	EXPECT_EQ(v.size(), 3);

	try {
        v.remove_at(5);
        FAIL() << "Bad index not thrown";
    } catch (std::out_of_range &e) {
        SUCCEED() << "Bad index thrown";
    } catch (...) {
        FAIL() << "Unknown exception thrown";
    }
}

TEST_F(VectorTest, insertAtExceptionTest) {
    int x = 5;
    try {
        v.m_insert_at(5, x);
        FAIL() << "No exception thrown";
    } catch (std::out_of_range &e) {
        SUCCEED() << "Out of range exception thrown";
    } catch (...) {
        FAIL() << "Unknown exception thrown";
    }
}

TEST_F(VectorTest, sortTest) {
    v.push(4); v.push(5); v.push(2); v.push(3);
    v.sort();
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 5);
}

TEST_F(VectorTest, copyCTest) {
    v.push(5); v.push(6);
    vector<int> v1(v);
    EXPECT_EQ(v1[0], 5);
    EXPECT_EQ(v1[1], 6);
}

TEST_F(VectorTest, moveCTest) {
    v.push(5); v.push(6);
    vector<int> v1(std::move(v));
    EXPECT_EQ(v1[0], 5);
    EXPECT_EQ(v1[1], 6);
}

TEST_F(VectorTest, copyOpTest) {
    v.push(5); v.push(6);
    vector<int> v1;
    v1 = v;
    EXPECT_EQ(v1[0], 5);
    EXPECT_EQ(v1[1], 6);
}

TEST_F(VectorTest, moveOpTest) {
    v.push(5); v.push(6);
    vector<int> v1;
    v1 = std::move(v);
    EXPECT_EQ(v1[0], 5);
    EXPECT_EQ(v1[1], 6);
    try {
        v[0];
    } catch (std::out_of_range &e) {
        SUCCEED() << "successfully thrown out_of_range error";
    } catch (...) {
        FAIL() << "expected runtime error";
    }
}

TEST_F(VectorTest, clearTest) {
    v.push(5); v.push(3);
    v.clear(5);
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 5);
}

TEST_F(VectorTest, shrinkToSizeTest) {
    v.push(5); v.push(3); v.push(6);
    EXPECT_EQ(v.capacity(), 4);
    v.shrink_to_size();
    EXPECT_EQ(v.capacity(), 3);
}

TEST_F(VectorTest, setAtTest) {
    v.push(5);
    v.set_at(0, 2);
    EXPECT_EQ(v[0], 2);
}

TEST_F(VectorTest, setAtExceptionTest) {
    try {
        v.set_at(0, 2);
        FAIL() << "No exception thrown";
    } catch (std::out_of_range &e) {
        SUCCEED() << "out of range exception thrown";
    } catch (...) {
        FAIL() << "Unknown exception thrown";
    }
}

TEST_F(VectorTest, forEachTest) {
    v.push(5); v.push(6);
    int j = 0;
    for (int i : v) {
        EXPECT_EQ(i, v[j++]);
    }
}
