#pragma once

#include <vector>
#include <stdexcept>

template <typename T>

class Stack {
    public: 
        Stack() = default;
        ~Stack() = default;

        void push(const T& value) {
            v.push_back(value);
        };
        T pop() {
            if (v.empty()) {
                throw std::underflow_error("Stack underflow: cannot pop from an empty stack.");
            }
            T topVal = v.back();
            v.pop_back();
            return topVal;
        };
        T& top() {
            if (v.empty()) {
                throw std::underflow_error("Stack underflow: cannot pop from an empty stack.");
            }
            return v.back();
        };
        bool empty() const {
            return v.empty();
        };
        std::size_t size() const {
            return v.size();
        };
    private:
        std::vector<T> v;
};
