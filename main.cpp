#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;
template <typename T>
using Vector = aisdi::Vector<T>;

std::chrono::nanoseconds performPrependOnList()
{
    LinearCollection<std::string> collection;

     auto startL = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < 10000; ++i)
        collection.prepend("prep");
    auto endL = std::chrono::system_clock::now();
    auto elapsedL =
        std::chrono::duration_cast<std::chrono::nanoseconds>(endL - startL);
    std::cout << "ListTest elapsed in   " << elapsedL.count() << " ns when prepending\n";

    return elapsedL;
}

std::chrono::nanoseconds performPopLastOnList()
{
    LinearCollection<std::string> collection;

    for (std::size_t i = 0; i < 10000; ++i)
        collection.prepend("prep");


    auto startL = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < 10000; ++i)
        collection.popLast();
    auto endL = std::chrono::system_clock::now();

    auto elapsedL =
        std::chrono::duration_cast<std::chrono::nanoseconds>(endL - startL);
    std::cout << "ListTest elapsed in   " << elapsedL.count() << " ns when popping last\n";

    return elapsedL;
}

std::chrono::nanoseconds performPrependOnVector()
{
    Vector<std::string> collection;
    auto startV = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < 10000; ++i)
        collection.prepend("prep");
    auto endV = std::chrono::system_clock::now();
    auto elapsedV =
        std::chrono::duration_cast<std::chrono::nanoseconds>(endV - startV);
    std::cout << "VectorTest elapsed in " << elapsedV.count() << " ns when prepending\n";

    return elapsedV;
}

std::chrono::nanoseconds performPopLastOnVector()
{
    Vector<std::string> collection;

    for (std::size_t i = 0; i < 10000; ++i)
        collection.prepend("prep");

    auto startL = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < 10000; ++i)
        collection.popLast();
    auto endL = std::chrono::system_clock::now();

    auto elapsedL =
        std::chrono::duration_cast<std::chrono::nanoseconds>(endL - startL);
    std::cout << "VectorTest elapsed in   " << elapsedL.count() << " ns when popping last\n";

    return elapsedL;
}

void AreThereLeaks()
{
    LinearCollection<std::string> collection;

    collection = {"a", "da", "ma"};
    collection.append("pp");
    collection.prepend("prepend");
    auto it = collection.begin();
    it = collection.end();


    collection.erase(collection.begin(), collection.end());

    Vector<std::string> vecto;

    vecto = {"a", "da", "ma"};
    vecto.append("pp");
    vecto.prepend("prepend");
    vecto = {"ala", "ma", "kota", "psa", "słonia", "konia", "psa", "trumpet", "d", "d"};
    auto it2 = vecto.cbegin();
    it2+8;
    vecto.erase(vecto.begin(), vecto.end());
}

} // namespace

int main(int argc, char** argv)
{
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;

    for (std::size_t i = 0; i < repeatCount; ++i)
        AreThereLeaks();
    if(repeatCount == 1)
        {
            std::cout << "Difference when prepending       " << (performPrependOnVector()-performPrependOnList()).count() << " ns\n";
            std::cout << "Difference when popping last       " << (performPopLastOnVector()-performPopLastOnList()).count() << " ns\n";
        }

    return 0;
}

