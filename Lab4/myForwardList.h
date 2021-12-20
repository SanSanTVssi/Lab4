#pragma once

#include <utility>
#include <memory>
#include <exception>
#include <iostream>
#include <functional>

namespace my_std {

    template<class value_type>
    class forward_list {
        struct Node;
        using node = std::shared_ptr<Node>;
        using weak_node = std::weak_ptr<Node>;
        std::size_t Size;
        node root;
    public:

        forward_list() : Size(0), root(nullptr) {}

        explicit forward_list(std::size_t);

        forward_list(std::initializer_list<value_type>);

        forward_list(forward_list&) = default;

        forward_list(forward_list&&) noexcept;

        ~forward_list() { clear(); }

        forward_list& operator=(forward_list&) = default;

        forward_list& operator=(forward_list&&) noexcept;

        std::size_t size() const { return Size; }

        void push_front(value_type&);

        void push_front(value_type&&);

        void push_back(value_type&);

        void push_back(value_type&&);

        void insert(value_type&, int);

        void insert(value_type&&, int);

        void removeAt(int);

        void pop_front();

        void pop_back();

        void clear();

        bool empty() { return (root) ? false : true; }

        value_type& operator[](int);

        value_type operator[](int) const;

        forward_list<value_type> split_when(std::function<bool(value_type)> SplitPredicate);

        class iterator {
        protected:
            node pointer;
        public:
            iterator() = default;

            iterator(node right_side_hand) : pointer(right_side_hand) {}

            iterator operator++() {
                pointer = pointer->next;
                return *this;
            }

            bool operator!=(iterator right_side_hand) { return right_side_hand.pointer != pointer; }

            int& operator*() { return pointer->data; }

            ~iterator() { pointer.reset(); }
        };

        class const_iterator : public iterator {
        public:
            const_iterator() = default;
            const_iterator(node right_side_hand) : iterator(right_side_hand) { }

            int operator*() { return pointer->data; }
        };

        iterator begin() { return iterator{ root }; }

        iterator end() { return iterator{}; }

        const_iterator begin() const { return const_iterator{ root }; }

        const_iterator end() const { return const_iterator{}; }

    private:

        struct Node {
            value_type data;
            node next;

            explicit Node(value_type& Data = value_type(), node nextNode = nullptr) : data(Data), next(nextNode) {}

            explicit Node(value_type&& Data = value_type(), node nextNode = nullptr) : data(std::move(Data)), next(nextNode) {}
        };

        node getNodeByIndex(std::size_t index) const {
            node temp = root;
            for (int i = 0; i < index; ++i) {
                temp = temp->next;
            }
            return temp;
        }
    };

    template<class value_type>
    forward_list<value_type>::forward_list(std::initializer_list<value_type> list) : forward_list<value_type>() {
        for (value_type item : list) {
            forward_list<value_type>::push_back(item);
        }
    }

    template<class value_type>
    forward_list<value_type>::forward_list(forward_list&& other) noexcept : Size(other.Size), root(other.root) {
        other.Size = 0;
        other.root.reset();
    }

    template<class value_type>
    forward_list<value_type>& forward_list<value_type>::operator=(forward_list<value_type>&& other) noexcept
    {
        clear();
        Size = other.Size;
        root = other.root;
        other.Size = 0;
        other.root.reset();
        return *this;
    }

    template<class value_type>
    void forward_list<value_type>::push_front(value_type& item)
    {
        root.reset(new Node(item, root));
        Size++;
    }

    template<class value_type>
    void forward_list<value_type>::push_front(value_type&& item)
    {
        root.reset(new Node(std::move(item), root));
        Size++;
    }

    template<class value_type>
    void forward_list<value_type>::push_back(value_type&& item) {
        if (root) {
            getNodeByIndex(Size - 1)->next.reset(new Node(std::move(item)));
        }
        else {
            root.reset(new Node(std::move(item)));
        }
        ++Size;
    }

    template<class value_type>
    value_type& forward_list<value_type>::operator[](int index) {
        if (index < Size) {
            return getNodeByIndex(index)->data;
        }
        else {
            throw std::out_of_range("Out of Range! [forward_list<value_type>::operator[]]");
        }
    }

    template<class value_type>
    inline value_type forward_list<value_type>::operator[](int index) const
    {
        if (index < Size) {
            return getNodeByIndex(index)->data;
        }
        else {
            throw std::out_of_range("Out of Range! [forward_list<value_type>::operator[]]");
        }
    }

    template<class value_type>
    inline forward_list<value_type> forward_list<value_type>::split_when(std::function<bool(value_type)> SplitPredicate)
    {
        node temp = root;
        auto resultList = forward_list<value_type>();
        while (!SplitPredicate(temp->data) && temp->next != nullptr) {
            temp = temp->next;
        }
        if (SplitPredicate(temp->data)) {
            while (temp != nullptr) {
                resultList.push_back(temp->data);
                temp = temp->next;
            }
        }
        return resultList;
    }

    template<class value_type>
    void forward_list<value_type>::push_back(value_type& item) {
        if (root) {
            getNodeByIndex(Size - 1)->next.reset(new Node((item)));
        }
        else {
            root.reset(new Node((item)));
        }
        ++Size;
    }

    template<class value_type>
    void forward_list<value_type>::removeAt(int index)
    {
        if (index < this->Size) {
            if (!index) {
                pop_front();
            }
            else {
                node previous_temp = getNodeByIndex(index - 1);
                weak_node to_delete_temp = previous_temp->next;
                previous_temp->next = previous_temp->next->next;
                to_delete_temp.reset();
                --Size;
            }
        }
    }

    template<class value_type>
    forward_list<value_type>::forward_list(std::size_t size) : forward_list<value_type>() {
        for (int i = 0; i < size; ++i) {
            forward_list<value_type>::push_back(0);
        }
    }

    template<class value_type>
    void forward_list<value_type>::pop_front() {
        if (root) {
            weak_node temp = root;
            root = root->next;
            temp.reset();
            --Size;
        }
    }

    template<class value_type>
    void forward_list<value_type>::clear() {
        while (Size) {
            pop_front();
        }
    }

    template<class value_type>
    void forward_list<value_type>::pop_back() {
        if (root) {
            removeAt(Size - 1);
        }
    }

    template<class value_type>
    void forward_list<value_type>::insert(value_type& item, int index) {
        if (index < Size) {
            if (!index) {
                push_front(item);
            }
            else {
                node temp_previous = getNodeByIndex(index - 1);
                temp_previous->next.reset(new Node(item, temp_previous->next));
                Size++;
            }
        }
        else {
            throw std::out_of_range("Out of Range! [forward_list<value_type>::insert]");
        }
    }

    template<class value_type>
    void forward_list<value_type>::insert(value_type&& item, int index)
    {
        if (index < Size) {
            if (!index) {
                push_front(std::move(item));
            }
            else {
                node temp_previous = getNodeByIndex(index - 1);
                temp_previous->next.reset(new Node(std::move(item), temp_previous->next));
                Size++;
            }
        }
        else {
            throw std::out_of_range("Out of Range! [forward_list<value_type>::insert]");
        }
    }

}

