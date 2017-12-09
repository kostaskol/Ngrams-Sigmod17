#include "mvector.hpp"
#include "gtest/gtest.h"
#include "minHeap.hpp"
#include <string>

using mstd::vector;

class minHeapTesting : public ::testing::Test {
public:
    vector<pair> vec;

    bool invariantHolds(minHeap &heap) {
        if (!heap.is_heapified()) {
            return false;
        }

        vector<pair> * myArray = heap.get_Array();
        pair topPair = myArray->at(0);

        std::string min = topPair.get_word();

        for (size_t i = 0; i < myArray->size(); i++) {  //find the actuall min string
            if (myArray->at(i).get_word() < min) {
                min = myArray->at(i).get_word();
            }
        }

        return min <= topPair.get_word();
    }

    minHeapTesting() {
        pair a("test",1);
        pair b("to",1);
        pair c("beHeaped",1);
        pair d("oh la la a string",1);
        vec.push(a);
        vec.push(b);
        vec.push(c);
        vec.push(d);
    }
};

TEST_F(minHeapTesting, isHeap) {
    minHeap heap(vec);
    ASSERT_TRUE(invariantHolds(heap));
}

TEST_F(minHeapTesting, heapsortTest) {
    minHeap heap(vec);
    ASSERT_TRUE(invariantHolds(heap));
    heap.heapSort();
    ASSERT_FALSE(invariantHolds(heap));

    for (size_t i = 1; i < vec.size(); i++) {
        ASSERT_TRUE(vec.at(i-1) < vec.at(i));
    }
}
