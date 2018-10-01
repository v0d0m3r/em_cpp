//------------------------------------------------------------------------------

// Глава 3: Переход к современному C++

//------------------------------------------------------------------------------

#ifndef CHAPTER3_HPP
#define CHAPTER3_HPP

//------------------------------------------------------------------------------

#include <Facilities.hpp>

//------------------------------------------------------------------------------
// 3.1 Различие между "{}" и "()" при создании объектов
{

int x(0);       // Инициализатор в круглых скобках
int y = 0;      // Инициализатор после "="
int z{ 0 };     // Инициализатор в фигурных скобках
int z = { 0 };  // Инициализатор использует = и фигурные скобки


Widget w1;      // Вызов конструктора по умолчанию
Widget w2 = w1; // Не присваивание, а копирующий конструктор
w1 = w2;        // Присваивание; вызов оператора operator=()


class Widget {
// ...
private:
    int x{0};   // Ок, значение x по умолчанию равно 0
    int y = 0;  // Тоже Ок
    int z(0);   // Ошибка
};


std::atomic<int> ai1{0};    // Ок
std::atomic<int> ai2(0);    // Ок
std::atomic<int> ai3 = 0;   // Ошибка


double x,y,z;
// ...
int sum1{x + y + z};    // Ошибка! Сумма double не
                        // может выражаться с помощью int
int sum2(x + y + z);    // Ок (значение выражения усекается до int)
int sum3 = x + y + z;   // Тоже Ок


Widget w1(10);  // Вызов конструктора с аргументом 10
Widget w2();    // Синтаксический анализ рассматривает это как
                // объявление функции w2, возвращающей Widget!
Widget w3{};    // Вызов конструктора Widget без аргументов


class Widget {
public:
    Widget(int i, bool b);   // Конструкторы не имеют
    Widget(int i, double d); // параметров std::initializer_list
    // ...
};
Widget w1(10, true);    // Вызов первого конструктора
Widget w2{10, true};    // Вызов первого конструктора
Widget w3(10, 5.0);     // Вызов второго конструктора
Widget w4{10, 5.0};     // Вызов второго конструктора

// Добавляем конструктор с std::initializer_list
class Widget {
public:
    Widget(int i, bool b);   // Как и ранее
    Widget(int i, double d); // Как и ранее

    Widget(std::initializer_list<long double> il); // Добавлен

    // ...
};
Widget w1(10, true);    // Использует круглые скобки и, как и
                        // ранее, вызывает первый конструктор
Widget w2{10, true};    // Использует фигурные скобки, но теперь
                        // вызывает третий конструктор
                        // (10 и true преобразуются в long double)
Widget w3(10, 5.0);     // Использует круглые скобки и, как и
                        // ранее, вызывает второй конструктор
Widget w4{10, 5.0};     // Использует фигурные скобки, но теперь
                        // вызывает третий конструктор
                        // (10 и 5.0 преобразуются в long double)

// При фигурной инициализации конструктор с std::initializer_list
// может перехватывать конструкторы копирования и перемещения!
class Widget {
public:
    Widget(int i, bool b);   // Как и ранее
    Widget(int i, double d); // Как и ранее
    Widget(std::initializer_list<long double> il); // Как и ранее

    operator float() const;  // Преобразование во float

    // ...
};
Widget w5(w4);              // Использует круглые скобки, вызывает
                            // копирующий конструктор
Widget w6{w4};              // Использует фигурные скобки, вызов
                            // конструктора с std::initializer_list
                            // (w4 преобразуется во float, а float
                            // преобразуется в long double)
Widget w7(std::move(w4));   // Использует круглые скобки, вызывает
                            // перемещающий конструктор
Widget w8{std::move(w4)};   // Использует фигурные скобки, вызов
                            // конструктора с std::initializer_list
                            // (все как для w6)

class Widget {
public:
    Widget(int i, bool b);   // Как и ранее
    Widget(int i, double d); // Как и ранее
    Widget(std::initializer_list<bool> ib); // Теперь тип
                                            // элемента - bool
    // Нет функций неявного преобразования
    // ...
};
Widget w{10, 5.0};  // Ошибка! Требуется сужающее преобразование


class Widget {
public:
    Widget(int i, bool b);   // Как и ранее
    Widget(int i, double d); // Как и ранее

    // Теперь тип элемента - std::string
    Widget(std::initializer_list<std::string> il);

    // Нет функций неявного преобразования
    // ...
};
Widget w1(10, true);    // Вызов первого конструктора
Widget w2{10, true};    // Вызов первого конструктора
Widget w3(10, 5.0);     // Вызов второго конструктора
Widget w4{10, 5.0};     // Вызов второго конструктора


class Widget {
public:
    // Конструктор по умолчанию
    Widget();

    // Конструктор с std::initializer_list
    Widget(std::initializer_list<int> il);

    // Нет функций неявного преобразования
    // ...
};
Widget w1;      // Вызов конструктора по умолчанию
Widget w2{};    // Вызов конструктора по умолчанию
Widget w3();    // Трактуется как объявление функции!
Widget w4({});  // Вызов конструктора с пустым
                // std::initializer_list
Widget w5{{}};  // Тоже самое

std::vector<int> v1(10, 20); // Использует конструктор без
                             // std::initializer_list: создает
                             // std::vector с 10 элементами;
                             // значение каждого равно 20
std::vector<int> v2{10, 20}; // Использует конструктор с
                             // std::initializer_list: создает
                             // std::vector с 2 элементами со
                             // значениями 10 и 20


template<class T,       // Тип создаваемого объекта
         class... Ts>   // Типы используемых аргументов
void do_some_work(Ts&&... params)
{
    // Создание локального объекта из params...
    // Первый способ
    T local_object(std::forward<Ts>(params)...); // Круглые скобки
    // Второй спсоб
    T local_object{std::forward<Ts>(params)...}; // Фигурные скобки
}
std::vector<int> v;
// ...
do_some_work<std::vector<int>>(10, 20); // Какой конструктор будет вызван?

}

//------------------------------------------------------------------------------
// 3.2 Предпочитайте nullptr значениям 0 и NULL
{

void f(int);    // Три перегрузки функции f
void f(bool);
void f(void*);
// ...
f(0);           // Вызов f(int), а не void f(void*)
f(NULL);        // Может не компилироваться, но обычно
                // вызывает f(int) и никогда - f(void*)
f(nullptr);     // Вызов f(void*)


auto find_record(); // Возвращает значение указателя на тип
// ...
auto result = find_record();
if (result == 0) {  // Неясно, имеет ли result
                    // тип указателя или целочисленный тип
    // ...
}
// С использованием nullptr, получается более ясный код
if (result == nullptr) {
    // ...
}


int f1(std::shared_ptr<Widget> spw);    // Вызывается только при
double f2(std::unique_ptr<Widget> upw); // блокировке соответствующего
bool f3(Widget* pw);                    // мьютекса
// ...
std::mutex f1m, f2m, f3m;   // Мьютексы для f1, f2, f3
using Mux_guard = std::lock_guard<std::mutex>;
//...
{
    Mux_guard g(f1m);           // Блокировка мьютекса для f1
    auto result = f1(0);        // Передача 0 функции f1
}                               // Разблокирование мьютекса
// ...
{
    Mux_guard g(f2m);           // Блокировка мьютекса для f2
    auto result = f1(NULL);     // Передача NULL функции f2
}                               // Разблокирование мьютекса
// ...
{
    Mux_guard g(f3m);           // Блокировка мьютекса для f3
    auto result = f1(nullptr);  // Передача nullptr функции f3
}                               // Разблокирование мьютекса
// Превращаем функции f1, f2, f3 в шаблон
template<class Func_type,
         class Mux_type,
         class Ptr_type>
decltype(auto) lock_and_call(Func_type func,
                             Mux_type& mutex,
                             Ptr_type ptr)
{
    using Mux_guard = std::lock_guard<std::mutex>;

    Mux_guard g{mutex};
    return func(ptr);
}
auto result1 = lock_and_call(f1, f1m, 0);       // Ошибка!
//...
auto result2 = lock_and_call(f2, f2m, NULL);    // Ошибка!
//...
auto result3 = lock_and_call(f3, f3m, nullptr); // Ок

}

//------------------------------------------------------------------------------
// 3.3 Предпочитайте объявления псевдонимов применению typedef
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
// My_alloc_list<T>::type синоним для std::list<T, My_alloc<T>>
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
    My_alloc_list<T> list;  // Ни typename
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
// 3.4 Предпочитайте перечисления с областью
// видимости перечислениям без таковой (представлены фрагменты новые для меня)
{

enum class Status;  // Базовый тип - int
enum class Status : std::uint32_t;  // Базовый тип для Status -
                                    // std::uint32_t (из <csdint>)
// В c++98 нельзя было объявлять перечисления
// В с++11 - можно, только так:
enum Color : std::uint8_t;  // Предварительное объявление
                            // перечисления без области видимости;
                            // базовый тип - std::uint8_t

enum class Status : std::uint32_t {
    good = 0,
    failed = 1,
    incomplete = 100
};


using User_info =
    std::tuple<std::string,     // Имя
               std::string,     // Адрес
               std::size_t>;    // Репутация
User_info us_info;              // Объект с типом кортежа
// ...
auto val(std::get<1>(us_info)); // Получение значение поля 1
// ...
enum User_info_fields           // Без области видимости
{ ui_name, ui_email, ui_reputation};
// ...
auto val(std::get<ui_email>(us_info)); // Значение адреса
// ...
enum class User_info_fields     // С областью видимости
{ ui_name, ui_email, ui_reputation};
// ...
auto val(std::get<static_cast<std::size_t>(User_info_fields::ui_email)>
         (us_info));
// ...
template<class E>   // C++14
constexpr auto to_utype(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}
// ...
auto val(std::get<to_utype(User_info_fields::ui_email)>(us_info)); // Значение адреса

}

//------------------------------------------------------------------------------

#endif // CHAPTER3_HPP

//------------------------------------------------------------------------------
