#ifndef MINHEAP
#define MINHEAP

#include <cstdlib>
#include <iostream>
#include "mvector.hpp"
#include "pair.hpp"

using std::cout;
using std::endl;
using std::string;
using mstd::vector;


class minHeap{
private:

    void _swap(pair *a, pair *b) {
        pair temp = *a;
        *a = *b;
        *b = temp;
    }

    void _shiftRight(int low, int high) {
        int root = low;
        while ((root*2)+1 <= high) {
            int leftChild = (root * 2) + 1;
            int rightChild = leftChild + 1;
            int swapIdx = root;

            if (_array->at(swapIdx) < _array->at(leftChild)) {
                swapIdx = leftChild;
            }

            if ((rightChild <= high) && (_array->at(swapIdx) < _array->at(rightChild))) {
                swapIdx = rightChild;
            }

            if (swapIdx != root) {
                _swap(&_array->at(root), &_array->at(swapIdx));
                root = swapIdx;
            }

            else {
                break;
            }
        }
        return;
    }

    void _minHeapify() {
        int midIdx = (_size - 1) / 2;
        while (midIdx >= 0) {
            _shiftRight(midIdx, _size - 1);
            --midIdx;
        }
        _heapified = true;
        return;
    }

    vector<pair> *_array;
    int _size;
    bool _heapified;
    bool my_array;  //To determine whether *_array should be deallocated
public:

    minHeap(pair from[], int size) : _size(size), my_array(true), _heapified(false) {  //Given array will be copied to the class' vector.
        _array = new vector<pair>(from, size);
        _minHeapify();
    }

    minHeap(mstd::vector<pair> &from) : my_array(false), _heapified(false) {   //Vector pointer will point to the given vector, no copying involved.
        _size = from.size();
        _array = &from;
        _minHeapify();
    }

    ~minHeap() {
        if (my_array) {
            delete _array;
        }
    }

    pair &getMin() {
        return _array->at(0);
    }

    void print_heap() {
        std::cout << "[";
        for (int i = 0; i < _size; i++) {
            _array->at(i).print();
            if (i != _size-1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << '\n';
    }

    void heapSort() {
        if (!_heapified) {
            _minHeapify();
        }
        int high = _size - 1;
        while (high > 0) {
            _swap(&_array->at(high),&_array->at(0));
            --high;
            _shiftRight(0, high);
        }
        _heapified = false;
        return;
    }

    /*----------------------------for unit testing----------------------------*/
    vector<pair> *get_Array() {
        return _array;
    }

    bool is_heapified(){
        return _heapified;
    }
};

#endif //MINHEAP
