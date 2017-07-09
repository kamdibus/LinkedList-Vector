#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

    template <typename Type>
    class LinkedList
    {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

    private:
        struct node{
            const Type obj;
            node *prev;
            node *next;

            node(const Type &A) :obj(A)
            {
              next = NULL;
              prev = NULL;

            }

            ~node()
            {
              next = NULL;
              prev = NULL;
            }

        };

        node *first;
        node *last;
        int length;

    public:
        LinkedList()
        {
          first = new node(Type());
          last = new node(Type());
          first->prev = NULL;
          first->next = last;
          last->prev = first;
          last->next = NULL;
          length = 0;
        }

        LinkedList(std::initializer_list<Type> l) :LinkedList()
        {
          for(auto it = l.begin(); it != (l.end()); ++it)
            append(*it);
        }

        LinkedList(const LinkedList& other) :LinkedList()
        {
          for(auto it = other.begin(); it != other.end(); ++it)
            append(*it);
        }

        LinkedList(LinkedList&& other) :LinkedList()
        {
            auto firstTemp = first;
            auto lengthTemp = length;
            auto lastTemp = last;

            first = other.first;
            length = other.length;
            last = other.last;

            other.first = firstTemp;
            other.length = lengthTemp;
            other.last = lastTemp;
        }

        ~LinkedList()
        {
            erase(begin(), end());
            delete first;
            delete last;
        }

        LinkedList& operator=(const LinkedList& other)
        {
          erase(begin(), end());
          for(auto it = other.begin(); it != other.end(); ++it)
            append(*it);
          return *this;
        }

        LinkedList& operator=(LinkedList&& other)
        {
            erase(begin(), end());

            auto firstTemp = first;
            auto lengthTemp = length;
            auto lastTemp = last;

            first = other.first;
            length = other.length;
            last = other.last;

            other.first = firstTemp;
            other.length = lengthTemp;
            other.last = lastTemp;

            return *this;
        }

        bool isEmpty() const
        {
            return first-> next == last;
        }

        size_type getSize() const
        {
            return length;
        }

        void append(const Type& item)
        {
          node* newNode = new node(item);
          node* curr = last->prev;
          last->prev = newNode;
          newNode->next = last;
          newNode->prev = curr;
          curr->next = newNode;
          length++;
        }

        void prepend(const Type& item)
        {
          node* newNode = new node(item);
          node* curr = first->next;
          first->next = newNode;
          newNode->next = curr;
          newNode->prev = first;
          curr->prev = newNode;
          length++;
        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {
          //Czy sprawdzać warunki i używać append/prepend
          //Nie ma takiej potrzeby, mamy 2 sentinele

          node* newNode = new node(item);
          node* prevNode = insertPosition.currNode->prev;
          newNode->next = insertPosition.currNode;
          newNode->prev = prevNode;
          prevNode->next = newNode;
          (insertPosition.currNode)->prev = newNode;
          length++;
        }

        Type popFirst()
        {
          if(isEmpty())
            throw std::logic_error("Cannot pop when empty");
          auto curr = *begin();
          erase(begin());
          return curr;
        }

        Type popLast()
        {
          if(isEmpty())
            throw std::logic_error("Cannot pop when empty");
          auto curr = *(--end());
          erase(end()-1);
          return curr;
        }

        void erase(const const_iterator& possition)
        {
          if(possition == end())
                throw std::out_of_range("Can't erase a sentinel node");

          --length;
          possition.currNode->next->prev = possition.currNode->prev;
          possition.currNode->prev->next= possition.currNode->next;
          delete possition.currNode;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {

          for(auto it = firstIncluded; it != lastExcluded;)
          {
            auto itt=it+1;
            erase(it);
            it=itt;
          }
        }

        iterator begin()
        {

            return iterator(cbegin());
        }

        iterator end()
        {
          return iterator(cend());
        }

        const_iterator cbegin() const
        {
          if(isEmpty())
            return  ConstIterator(last);
          else
            return ConstIterator(first->next);
        }

        const_iterator cend() const
        {
          return ConstIterator(last);;
        }

        const_iterator begin() const
        {
          return cbegin();
        }

        const_iterator end() const
        {
          return cend();
        }
    };

    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        node* currNode;

        explicit ConstIterator()
        {
          currNode = NULL;
        }
        ConstIterator(node* n)
        {
          currNode = n;
        }

        reference operator*() const
        {
          if(currNode->next != NULL)
            return currNode->obj;
          else
            throw std::out_of_range("Can't shell when pointing to end");
        }

        ConstIterator& operator++()
        {
          if(currNode->next != NULL)
          {
            currNode = currNode->next;
            return *this;
          }
          else
            throw std::out_of_range("Cannot increment when pointing to end");
        }

        ConstIterator operator++(int)
        {
          if(currNode->next == NULL)
            throw std::out_of_range("Can't increment when pointing to end");

          ConstIterator curr = *this;
          currNode = currNode->next;
          return curr;
        }

        ConstIterator& operator--()
        {
          if(currNode->prev->prev == NULL)
            throw std::out_of_range("Cannot decrement when pointing to first elem");

          currNode = currNode->prev;
          return *this;
        }

        ConstIterator operator--(int)
        {
          if(currNode->prev->prev == NULL)
            throw std::out_of_range("Cannot decrement when pointing to first elem");

          ConstIterator curr = *this;
          currNode = currNode->prev;
          return curr;
        }

        ConstIterator operator+(difference_type d) const
        {
          ConstIterator curr = *this;
          if(d < 0)
            while(d++ < 0)
              --curr;
          else
            while(d-- > 0)
              ++curr;
          return curr;
        }

        ConstIterator operator-(difference_type d) const
        {
          ConstIterator curr = *this;
          if(d <= 0)
            while(d++ < 0)
              --curr;
          else
            while(d-- > 0)
              --curr;
          return curr;
        }

        bool operator==(const ConstIterator& other) const
        {
            return currNode == other.currNode;
        }

        bool operator!=(const ConstIterator& other) const
        {
          return currNode != other.currNode;
        }
    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator()
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
          ConstIterator::operator++();
          return *this;
        }

        Iterator operator++(int)
        {
          auto result = *this;
          ConstIterator::operator++();
          return result;
        }

        Iterator& operator--()
        {
          ConstIterator::operator--();
          return *this;
        }

        Iterator operator--(int)
        {
          auto result = *this;
          ConstIterator::operator--();
          return result;
        }

        Iterator operator+(difference_type d) const
        {
          return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
          return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
          // ugly cast, yet reduces code duplication.
          return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H


