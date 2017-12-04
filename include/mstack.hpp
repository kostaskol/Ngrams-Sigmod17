#ifndef MSTACK
#define MSTACK
#include <cstring>

namespace mstd {

    template <typename T>
    class stack {
    private:
        class node {
        private:
            T _entry;
            node *_next;
        public:
            node(T ent, node *next = nullptr) {
                _entry = ent;
                _next = next;
            }

            node(const node &other) {
                _entry = other._entry;
                if (other._next != nullptr)
                    _next = new node(other._next->_entry);
            }

            ~node() {
                // Do nothing here
            }

            node *get_next() { return _next; }

            void set_next(node *next) {
                _next = next;
            }

            void add_next(node *next) {
                _next = new node(next->_entry);
            }

            T &get_entry() { return _entry; }

            node &operator=(node &other) {
                _entry = other._entry;
                _next = new node(other._next->_entry);
                return *this;
            }
        };

        node *_head;
        size_t _size;
        int _max;

    public:
        explicit stack(int max = -1);

        stack(const stack &other)=delete;

        ~stack();

        void push(T ent);

        T pop();

        T &peek();

        void clear();

        /* For unit test purposes only */
        T &get_element_at(int pos);

        size_t size();

        bool full();

        bool empty();

        stack &operator=(const stack &other)=delete;
    };
}

template <typename T>
mstd::stack<T>::stack(int max): _head(nullptr), _size(0), _max(max) { }

template <typename T>
mstd::stack<T>::~stack() {
    node *tmp = _head;
    node *next;
    while (tmp) {
        next = tmp->get_next();
        delete tmp;
        tmp = next;
    }
}

template <typename T>
void mstd::stack<T>::push(T ent) {
    if (full()) throw std::runtime_error("stack is full");
    if (_head == nullptr) {
        _head = new node(ent);
        _size++;
        return;
    }
    auto *new_node = new node(ent, _head);
    _head = new_node;
    _size++;
}

template <typename T>
T mstd::stack<T>::pop() {
    if (empty()) throw std::runtime_error("stack is empty");
    T tmp = _head->get_entry();
    node *tmp_node = _head;
    _head = _head->get_next();
    delete tmp_node;
    _size--;
    return tmp;
}

template <typename T>
T &mstd::stack<T>::peek() {
    if (empty()) throw std::runtime_error("stack is empty");
    return _head->get_entry();
}

template <typename T>
void mstd::stack<T>::clear() {
    node *curr;
    while(_head) {
        curr = _head;
        _head = _head->get_next();
        delete curr;
    }
    _head = nullptr;
    _size = 0;
}

/* For unit test purposes only */
template <typename T>
T &mstd::stack<T>::get_element_at(int pos){
    if (_head == nullptr)
        throw std::runtime_error("stack is empty");
    if (pos < 0 || pos > _size){
        throw std::out_of_range("index out of range");
    }

    node* aux = _head;
    for (size_t i = 0; i < pos; i++) {
        aux = aux->get_next();
    }
    return aux->get_entry();
}

template <typename T>
size_t mstd::stack<T>::size() { return _size; }

template <typename T>
bool mstd::stack<T>::full() { return (_size == _max) && (_max > 0); }

template <typename T>
bool mstd::stack<T>::empty() { return _size == 0; }

#endif // MSTACK