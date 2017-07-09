#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#define ARRAYLENGTH 40
#include <cstddef>
#include <initializer_list>
#include <stdexcept>


namespace aisdi
{

template <typename Type>
class Vector
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
    int length;
    int nonitem;
    Type* array;

public:
    Vector()
    {
        array = new Type[ARRAYLENGTH];
        nonitem = 0;
        length = ARRAYLENGTH;
    }

    Vector(std::initializer_list<Type> l) : Vector()
    {
        if (l.size() > length)
            makeLongerArray(l.size());
        for(auto it = l.begin(); it != (l.end()); ++it)
            append(*it);
    }

    Vector(const Vector& other)
    {
        nonitem = 0;
        length = other.getSize()*2;
        array = new Type[other.getSize()*2];

        for(auto it = other.begin(); it != other.end(); ++it)
            append(*it);
    }

    Vector(Vector&& other) :Vector()
    {
        auto nonitempp = nonitem;
        auto lengthpp = length;
        auto arraypp = array;

        nonitem = other.nonitem;
        length = other.length;
        array = other.array;

        other.nonitem = nonitempp;
        other.length = lengthpp;
        other.array = arraypp;

    }

    ~Vector()
    {
        delete[] array;
    }

    Vector& operator=(const Vector& other)
    {
        delete[] array;
        nonitem = 0;
        if(other.getSize() > ARRAYLENGTH/2)
        {
            array = new Type[other.getSize()*2];
            length = other.getSize()*2;
        }
        else
        {
            array = new Type[ARRAYLENGTH];
            length = ARRAYLENGTH;

        }
        for(auto it = other.begin(); it != other.end(); ++it)
            append(*it);
        return *this;
    }

    Vector& operator=(Vector&& other)
    {
        erase(begin(),end());

        auto nonitemTemp = nonitem;
        auto lengthTemp = length;
        auto arrayTemp = array;

        nonitem = other.nonitem;
        length = other.length;
        array = other.array;

        other.nonitem = nonitemTemp;
        other.length = lengthTemp;
        other.array = arrayTemp;

        return *this;
    }

    bool isEmpty() const
    {
        return nonitem == 0;
    }

    size_type getSize() const
    {
        return nonitem;
    }

    void append(const Type& item)
    {
        if(length < nonitem+1)
            makeLongerArray();
        array[nonitem] = item;
        nonitem++;
    }

    void prepend(const Type& item)
    {
        if(length < nonitem+1)
            makeLongerArray();
        for(int i = nonitem; i > 0; i--)
        {
            array[i] = array[i-1];
        }
        array[0] = item;
        nonitem++;
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if (length < nonitem+1)
            makeLongerArray();
        if (insertPosition == cend())
        {
            append(item);
            return;
        }
        if (insertPosition == cbegin())
        {
            prepend(item);
            return;
        }

        for(int i = nonitem; i != insertPosition.currEl; i--)
            array[i] = array[i-1];
        nonitem++;
        array[insertPosition.currEl] = item;
    }

    Type popFirst()
    {
        if(isEmpty())
            throw std::logic_error("Cannot pop first element when vector is empty");

        Type temp = array[0];
        for(int i = 0; i < nonitem-1; i++)
            array[i] = array[i+1];
        nonitem--;

        return temp;
    }

    void makeLongerArray()
    {
        Type *newArray;
        newArray = new Type[length*2];
        length *= 2;
        for (int i = 0; i < nonitem; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
    }

    void makeLongerArray(int l)
    {
        Type *newArray;
        newArray = new Type[l*2];
        length *= 2;
        for (int i = 0; i < nonitem; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
    }

    Type popLast()
    {
        if(isEmpty())
            throw std::logic_error("Cannot pop last element when collection is empty");

        Type temp = array[nonitem-1];
        nonitem--;

        return temp;
    }

    void erase(const const_iterator& possition)
    {
        if(isEmpty() || possition.currEl == nonitem)
            throw std::out_of_range("Cannot erase when collection is empty nor when you are trying to erase the end sentinel");
        int erOne = possition.currEl;
        for(int i = erOne; i < nonitem; i++)
            array[i] = array[i+1];
        --nonitem;
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        if(firstIncluded == lastExcluded)
            return;

        int newLength = (getSize()-(firstIncluded.currEl-lastExcluded.currEl)+1);
        Type* tmp = new Type[newLength];
        int newNonItem = 0;
        for(auto it = cbegin(); it != cend(); ++it)
            if(it.currEl < firstIncluded.currEl || it.currEl >= lastExcluded.currEl)
            {
                tmp[newNonItem] = (*it);
                newNonItem++;
            }
        nonitem = newNonItem;
        delete[] array;
        array = tmp;
        length = newLength;
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
        return ConstIterator(0, this, &array[0]);
    }

    const_iterator cend() const
    {
        return ConstIterator(nonitem, this, &array[nonitem]);
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
class Vector<Type>::ConstIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;

private:
    const Vector<Type> *Cont;

public:
    mutable Type *pter;
    mutable int currEl;


    explicit ConstIterator()
    {
        pter = NULL;
        currEl = 0;
        Cont = NULL;
    }
    ConstIterator(int currEll, const Vector<Type>* Contt, Type* value)
    {
        currEl = currEll;
        Cont = Contt;
        pter = value;
    }

    reference operator*() const
    {
        if(Cont->getSize() == currEl)
            throw std::out_of_range("Cannot shell refer to value while pointing to the end sentinel");
        return *pter;
    }

    ConstIterator& operator++()
    {
        if(Cont->getSize() < currEl+1)
            throw std::out_of_range("Cannot increment beyond end sentinel");

        currEl++;
        pter++;
        return *this;
    }

    ConstIterator operator++(int)
    {
        if(Cont->getSize() < currEl+1)
            throw std::out_of_range("Cannot increment beyond end sentinel");

        ConstIterator cur = *this;
        currEl++;
        pter++;
        return cur;

    }

    ConstIterator& operator--()
    {
        if(currEl == 0)
            throw std::out_of_range("Cannot decrement before first");
        pter--;
        currEl--;
        return *this;

    }

    ConstIterator operator--(int)
    {
        if(currEl == 0)
            throw std::out_of_range("Cannot decrement before first");
        ConstIterator cur = *this;
        pter--;
        currEl--;
        return cur;
    }

    ConstIterator operator+(difference_type d) const
    {
        currEl += d;
        pter = &(Cont->array[currEl]);
        return *this;
    }

    ConstIterator operator-(difference_type d) const
    {
        currEl -= d;
        pter = &(Cont->array[currEl]);
        return *this;
    }

    bool operator==(const ConstIterator& other) const
    {
        return pter == other.pter;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return pter != other.pter;
    }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;


    explicit Iterator()
    {
    }

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

#endif // AISDI_LINEAR_VECTOR_H


