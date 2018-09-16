//------------------------------------------------------------------------------

// Глава 1: Вывод типов

//------------------------------------------------------------------------------

#ifndef CHAPTER1_HPP
#define CHAPTER1_HPP

//------------------------------------------------------------------------------

#include <Facilities.hpp>

//------------------------------------------------------------------------------

// I вывод типа шаблона

//------------------------------------------------------------------------------
// 1 случай param представляет собой ссылку или указатель

// a)
template<class T>
f(T& param);        // param представляет собой ссылку

int x = 27;         // x имеет тип int
const int cx = x;   // cx имеет тип cons int
const int& rx = x;  // rx является ссылкой на x как на const x

// Выводимые типы будут:
f(x);   // T - int,       тип param - int&
f(cx);  // T - const int, тип param - const int&
f(rx);  // T - const int, тип param - const int&

// б)
template<class T>
f(const T& param);  // param представляет собой ссылку на const

// Выводимые типы будут:
f(x);   // T - int, тип param - const int&
f(cx);  // T - int, тип param - const int&
f(rx);  // T - int, тип param - const int&

// в)
template<class T>
f(T* param);  // param представляет собой указатель

const int* px = &x;

// Выводимые типы будут:
f(&x);  // T - int, тип param - int*
f(px);  // T - const int, тип param - const int*

//------------------------------------------------------------------------------
// 2 случай param является универсальной ссылкой

template<class T>
f(T&& param);  // param представляет собой универсальную ссылку

// Выводимые типы будут:
f(x);   // x  - lvalue, => T - int&,       тип param - int&
f(cx);  // cx - lvalue, => T - const int&, тип param - const int&
f(rx);  // rx - lvalue, => T - const int&, тип param - const int&
f(27);  // 27 - rvalue, => T - int,        тип param - int&&

//------------------------------------------------------------------------------
// 3 случай param является ни указателем, ни ссылкой

template<class T>
f(T param);  // param передается по значению

// Выводимые типы будут:
f(x);   // T - int, тип param - int
f(cx);  // T - int, тип param - int
f(rx);  // T - int, тип param - int

const char* const ptr = "Fun with pointers";  // Константный указатель
                                              // на константный объект
f(ptr); // T и param - const char*

//------------------------------------------------------------------------------
// Аргументы массивы

const char name[] = "R. N. Briggs";    // Тип name - const char[13]
const char* ptr_to_name = name;        // Массив становится указателем

// Передача массива по значению
template<class T>
f(T param);  // param передается по значению

f(name); // name - массив, но T - const char*

// Передача массива по ссылке
template<class T>
f(T& param);        // param представляет собой ссылку

f(name);    // T - const char[13], param - const char(&)[13]

// Возвращает размер массива как константу времени компиляции.
// Параметр не имеет имени, поскольку кроме количества
// содержащихся в нем элементов, нас ничто не интересует.
template<class T, std::size_t N>
constexpr std::size_t array_size(T(&)[N]) noexcept
{ return N; }

int key_vals[] = { 1, 3, 7, 9, 11, 22, 35};
int mapped_vals[array_size(key_vals)];

std::array<int, array_size(key_vals)> mapped_vals2;

//------------------------------------------------------------------------------
// Аргументы функции

void some_func(int, double); // some_func - функция;
                             // её тип - void(int, double)
template<class T>
f1(T param);                 // param передается по значению

template<class T>
f2(T& param);                // param представляет собой ссылку

f1(some_func);               // param выводится как указатель на
                             // функцию; тип - void(*)(int, double)
f2(some_func);               // param выводится как ссылка на
                             // функцию; тип - void(&)(int, double)

//------------------------------------------------------------------------------

// II Вывод типа auto

//------------------------------------------------------------------------------

auto        x  = 27;    // Случай 3 (x не указатель и не ссылка)
const auto  cx = x;     // Случай 3 (x не указатель и не ссылка)
const auto& rx = x;     // Случай 1 (rx неуниверсальная ссылка)

template<class T>            // Концептуальный шаблон для
func_for_x(T param);         // для вывода x

func_for_x(27);              // Концептульный вызов: выведенный
                             // тип param является типом x

template<class T>            // Концептуальный шаблон для
func_for_cx(const T param);  // для вывода cx

func_for_x(x);              // Концептульный вызов: выведенный
                             // тип param является типом cx

template<class T>            // Концептуальный шаблон для
func_for_rx(const T& param); // для вывода rx

func_for_rx(x);              // Концептульный вызов: выведенный
                             // тип param является типом rx
// 2 случай
auto&& uref1 = x;   // x - int и lvalue, так что тип uref1 - int&
auto&& uref2 = cx;  // cx - const int и lvalue, так что тип uref2 - const int&
auto&& uref3 = 27;  // 27 - int и rvalue, так что тип uref3 - int&&

// Массивы и функции
const char name[] =          // Тип name - const char[13]
        "R. N. Briggs";

auto arr1  = name;           // Тип arr1 - const char*
auto& arr2 = name;           // Тип arr2 - const char(&)[13]

void some_func(int, double); // some_func - функция;
                             // её тип - void(int, double)
auto func1  = some_func;     // Тип func1 - void(*)(int, double)
auto& func1 = some_func;     // Тип func1 - void(&)(int, double)

// Вывод типа auto для инициализатора в фигурных скобках
auto x = {11, 23, 9};   // Тип x - std::initializer_list

template<class T>       // Объявление шаблона с параметром
f(T param);             // эквивалентно объявлению x

f({11 , 23, 9});        // Ошибка вывод типа для T

auto create_init_list()
{
    return { 1, 2, 3};  // Ошибка: невозможно вывести
                        // тип для { 1, 2, 3}
}

std::vector<int> v;

auto resetv =
        [&v] (const auto& new_value) { v = new_value; };    // c++14

resetv({ 1, 2, 3});     // Ошибка: невозможно вывести
                        // тип для { 1, 2, 3}

//------------------------------------------------------------------------------

// III - Знакомство decltype

//------------------------------------------------------------------------------

const int i = 0;         // decltype(i) - const int

bool f(const Widget& w); // decltype(w) - const Widget&
                         // decltype(w) - bool(const Widget&)

struct Point {
    int x, y;            // decltype(Point::x) - int
                         // decltype(Point::y) - int
};

Widget w;                // decltype(w) - Widget

f(w);                    // decltype(f(w)) - bool

template<class T>        // Упрощенная версия std::vector
class Vector {
public:
    // ...
    T& operator[](std::size_t index);
    // ...
};

Vector<int> v;           // decltype(v) - Vector<int>
v[0] = 0;                // decltype(v[0]) - int&

Widget w;
const Widget& cw;

auto my_widget1 = cw;           // Вывод типа auto:
                                // тип my_widget1 - Widget
decltype(auto) my_widget2 = cw; // Вывод типа decltype:
                                // тип my_widget2 - const Widget&

template<class Container, class Index>
decltype(auto) auth_access(Container&& c, Index i)
{
    authenticate_user();
    return std::forward<Container>(c)[i];
}

decltype(auto) f1()
{
    int x = 0;
    // ...
    return x;   // decltype(x) представляет собой int,
                // так что f1 возвращает int
}

decltype(auto) f2()
{
    int x = 0;
    // ...
    return (x); // decltype((x)) представляет собой int&,
                // так что f2 возвращает int&
}

//------------------------------------------------------------------------------

#endif // CHAPTER1_HPP

//------------------------------------------------------------------------------
