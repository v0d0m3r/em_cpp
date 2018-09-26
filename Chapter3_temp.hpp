#ifndef CHAPTER3_TEMP_HPP
#define CHAPTER3_TEMP_HPP

//------------------------------------------------------------------------------

#include <Facilities.hpp>

//------------------------------------------------------------------------------
// 3.3 Предпочитайте объявление псевдонимов применению typedef
{

typedef
    std::unique_ptr<std::unordered_map<std::string, std::string>>
    uptr_map_ss;    // Устарело...

using uptr_map_ss =
    std::unique_ptr<std::unordered_map<std::string, std::string>>;


// FP является синонимом для указателя на функцию, принимающую
// int и const std::string& и ничего не возвращающую
typedef void (*FP)(int, const std::string&);
// То же самое, но как объявление псевдонима
using FP = void (*)(int, const std::string&);


// С typedef
template<class T>
struct My_alloc_list {
    typedef std::list<T, My_alloc<T>> type;
};
// Клиентский код
My_alloc_list<Widget>::type lw; // Клиентский код
// ...
template<class T>                           // Widget<T>
class Widget {                              // My_alloc_list<T>,
    typename My_alloc_list<T>::type list;   // как член-данные
    // Обратить внимание на typename
};


// My_alloc_list<T> является синонимом для std::list<T, My_alloc<T>>
template<class T>
using My_alloc_list = std::list<T, My_alloc<T>>;
// ...
template<class T>
class Widget {
    My_alloc_list list;   // Ни typename
                          // Ни ::type
};


class Wine {
    //...
};
// ...
template<>                  // Специализация My_alloc_list в
class My_alloc_list<Wine> { // которой T представляет собой Wine
    enum class Wine_type
    { white, red, rose };

    Wine_type type;         // В этом классе type представляет
                            // собой данные-член
};

std::remove_const<T>::type;         // Дает T из const T
std::remove_reference<T>::type;     // Дает T из T& и T&&
std::add_lvalue_reference<T>::type; // Дает T& из T


std::remove_const<T>::type;         // C++11: const T -> T
std::remove_const_t<T>;             // Эквивалент в C++14

std::remove_reference<T>::type;     // C++11: T&/T&& -> T
std::remove_reference_t<T>;         // Эквивалент в C++14

std::add_lvalue_reference<T>::type; // C++11: T -> T&
std::add_lvalue_reference_t<T>;     // Эквивалент в C++14

}

//------------------------------------------------------------------------------

#endif // CHAPTER3_TEMP_HPP
