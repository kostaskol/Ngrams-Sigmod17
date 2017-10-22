#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdexcept>

namespace mstd {
// To be used only as pointer. Copying doesn't work
    template<typename T>
    class queue {
    private:
        class queue_node {
        private:
            T _entry;
            queue_node *_next;
            queue_node *_prev;
        public:
            explicit queue_node(T ent, queue_node *next = nullptr, queue_node *prev = nullptr) {
                _entry = ent;
                _next = next;
                _prev = prev;
            }

            queue_node(const queue_node &other) {
                _entry = other._entry;
                if (other._next != nullptr)
                    _next = new queue_node(other._next->_entry);

                if (other._prev != nullptr)
                    _prev = new queue_node(other._prev->_entry);
            }

            ~queue_node()= default;

            queue_node *get_next() { return _next; }

            queue_node *get_prev() { return _prev; }

            void set_next(queue_node *next) {
                _next = next;
            }

            void add_next(queue_node *next) {
                _next = new queue_node(next->_entry);
            }

            void set_prev(queue_node *prev) {
                _prev = prev;
            }

            void add_prev(queue_node *prev) {
                _prev = new queue_node(prev->_entry);
            }

            T &get_entry() { return _entry; }

            queue_node &operator=(const queue_node &other) {
                _entry = other._entry;
                _next = new queue_node(other._next->_entry);
                _prev = new queue_node(other._prev->_entry);
                return *this;
            }
        };

        queue_node *_head;
        queue_node *_last;
        size_t _size;
        int _max;
    public:
        explicit queue(int max = -1) : _head(nullptr), _last(nullptr), _size(0), _max(max) {}

        queue(const queue &) = delete;

        ~queue() {
            queue_node *curr;
            while(_head) {
                curr = _head;
                _head = _head->get_next();
                delete curr;
            }
        }

        void push(T ent) {
            if (full()) throw std::runtime_error("Queue is full");
            if (_head == nullptr) {
                _last = _head = new queue_node(ent);
                _size++;
                return;
            }

            auto *curr = new queue_node(ent, nullptr, _last);
            _last->set_next(curr);
            _last = curr;
            _size++;
        }

        bool empty() {
            return _size == 0;
        }

        T pop() {
            if (_head == nullptr) {
                throw std::runtime_error("queue is empty");
            }

            T tmp = _head->get_entry();
            queue_node *curr = _head->get_next();
            if (curr != nullptr)
                curr->set_prev(nullptr);
            delete _head;
            _head = curr;
            _size--;
            return tmp;
        }

        size_t size() { return _size; }

        T &peek() {
            if (_head == nullptr)
                throw std::runtime_error("queue is empty");

            return _head->get_entry();
        }

        bool full() {
            return ((int) _size == _max) && (_max > 0);
        }

        queue &operator=(const queue &other)= delete;
    };
}
#endif /* QUEUE_H_ */
