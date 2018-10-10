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
// 3.5 Предпочитайте удаленные функции закрытым неопределенным
{

// C++98
template<class CharT, class traits = char_traits<CharT>>
class basic_ios : public ios_base {
public:
    // ...
private:
    basic_ios(const basic_ios&);            // Не определен
    basic_ios& operator=(const basic_ios&); // Не определен
};
// C++11
template<class CharT, class traits = char_traits<CharT>>
class basic_ios : public ios_base {
public:
    // ...
    basic_ios(const basic_ios&) = delete;
    basic_ios& operator=(const basic_ios&) = delete;
    // ...
};

bool is_lucky(int number);
if (is_lucky('a')) {/*...*/}    // Является ли 'a' счастливым числом?
if (is_lucky('true')) {/*...*/} // Является ли 'true' счастливым числом?
if (is_lucky(3.5)) {/*...*/}    // Следует ли выполнить усечение до 3
                                // перед проверкой на "счастливость"?

bool is_lucky(int number);      // Исходная функция
bool is_lucky(char)   = delete; // Отвергаем символы
bool is_lucky(bool)   = delete; // Отвергаем булевы значения
bool is_lucky(double) = delete; // Отвергаем double и float


template<class T>
void process_pointer(T* ptr);

template<>
void process_pointer<void>(void*) = delete;

template<>
void process_pointer<char>(char*) = delete;

template<>
void process_pointer<const void>(const void*) = delete;

template<>
void process_pointer<const char>(const char*) = delete;
// В стиле c++98
class Widget {
public:
    // ...
    template<class T>
    void process_pointer(T* ptr)
    {/*...*/}
private:
    template<>                          // Ошибка!
    void process_pointer<void>(void*);
};
// В стиле C++11
class Widget {
public:
    // ...
    template<class T>
    void process_pointer(T* ptr)
    {/*...*/}
    // ...
};
template<>  // Все еще public, но удаленная
void Widget::process_pointer<void>(void*) = delete;

}

//------------------------------------------------------------------------------
// 3.6 Объявляйте перекрывающие функции как override
{

class Base {
public:
    virtual void do_work();     // Виртуальная функция
                                // базового класса
    // ...
};

class Derived : public Base {
public:
    virtual void do_work();          // Перекрывает Base::do_work
                                     // ключевое слово virtual
                                     // здесь не обязательно
    // ...
};
std::unique_ptr<Base> upb =          // Указатель на базовый класс
        std::make_unique<Derived>(); // указывает на объект
                                     // производного класса
// ...
upb->do_work();                      // Вызов do_work через указатель
                                     // на базовый класс; вызывается
                                     // функция производного класса
class Widget {
public:
    // ...
    void do_work() &;    // Эта версия do_work применима, только
                         // если *this представляет собой lvalue
    void do_work() &&;   // Эта версия do_work применима, только
                         // если *this представляет собой rvalue
};
// ...
Widget make_widget();    // Фабричная функция (возвращает rvalue)
Widget w;                // Обычный объект lvalue
// ...
w.do_work();             // Вызов Widget::do_work для lvalue
                         // (т.е. Widget::do_work &)
make_widget().do_work(); // Вызов Widget::do_work для rvalue
                         // (т.е. Widget::do_work &&)

class Base {
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    virtual void mf4() const;
}

class Derived : public Base {
public:
    virtual void mf1() const override;
    virtual void mf2(int x) override;
    virtual void mf3() & override;
    void mf4() const override;
}

void do_something(Widget& w);   // Принимает только lvalue Widget
void do_something(Widget&& w);  // Принимает только rvalue Widget

class Widget {
public:
    using Data_type = std::vector<double>;
    // ...
    Data_type& data() { return values; }
    // ...
private:
    Data_type values;
};
// ...
Widget w;
// ...
auto vals1(w.data());   // Копирует w.values в vals1
// ...
Widget make_widget();
// ...
auto vals2(make_widget().data());   // Копирование значений
                                    // Widget в vals2
class Widget {
public:
    using Data_type = std::vector<double>;
    // ...
    Data_type& data() &             // Для lvalue Widget
    { return values; }              // возвращает lvalue
    Data_type&& data() &&           // Для rvalue Widget
    { return std::move(values); }   // возвращает rvalue
    // ...
private:
    Data_type values;
};
// ...
auto vals1(w.data());               // Вызывает lvalue-перегрузку
                                    // Widget::data(), vals1
                                    // создается копированием
auto vals2(make_widget().data());   // Вызывает rvalue-перегрузку
                                    // Widget::data(), vals2
                                    // создается перемещением
}

//------------------------------------------------------------------------------
// 3.7 Предпочитайте итераторы const_iterator итераторам iterator
{

// C++98
std::vector<int> values;
// ...
std::vector<int>::iterator it =
        std::find(values.begin(), values.end(), 1983);
values.insert(it, 1998);
// ...
typedef std::vector<int>::iterator Iter;
typedef std::vector<int>::const_iterator Const_iter;
// ...
std::vector<int> values;
// ...
Const_iter ci =
        std::find(static_cast<Const_iter>(values.begin()),
                  static_cast<Const_iter>(values.end()),
                  1983);
values.insert(static_cast<Iter>(ci),     // Может не
              1998);                     // компилироваться!


// С++11
std::vector<int> values;
// ...
auto it(std::find(values.cbegin(),       // Используем cbegin()
                  values.cend(), 1983)); // и cend()
values.insert(it, 1998);


// С++14
template<class C, class V>
void find_and_insert(C& container,        // В container находит
                     const V& target_val, // первое значение
                     const V& insert_val) // target_val, затем
{                                         // вставляет insert_val
    using std::cbegin();
    using std::cend();

    auto it(std::find(cbegin(container),  // Не член cbegin
                      cend(container),    // Не член cend
                      target_val));

    container.insert(it, insert_val);
}
// С++11: нет функций не членов cbegin, cend и др. Есть только begin, end
template<class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
    return std::begin(container);
}

}

//------------------------------------------------------------------------------
// 3.8 Если функции не генерируют исключений,
// объявляйте их как noexcept
{

int f(int x) throw();   // f - не генерирует исключений: C++98
int f(int x) noexcept;  // f - не генерирует исключений: C++11


Ret_type function(params) noexcept; // Наиболее оптимизируема
Ret_type function(params) throw();  // Менее оптимизируемая
Ret_type function(params);          // Менее оптимизируемая


std::vector<Widget> vw;
// ...
Widget w;
// ...
vw.push_back(w);


template<class T, size_t N>
void swap(T(&a)[N],
          T(&b)[N]) noexcept(noexcept(swap(*a, *b)));

template<class T1, class T2>
struct pair {
    // ...
    void swap(pair& p) noexcept(noexcept(swap(first, p.first)) &&
                                noexcept(swap(second, p.second)));
};


// Компиляторы не предлагают помощи в выявлении
// несоответствий между реализациями функций и их
// спецификаций исключений:
void setup();   // Функции, определенные в другом месте
void cleanup();
// ...
void do_work() noexcept
{
    setup();    // Настройка для выполнения некоторой работы
    //...       // Выполнение работы
    cleanup();  // Очистка после выполнения работы
}

}

//------------------------------------------------------------------------------
// 3.9 Используйте, где это возможно, constexpr
{

int sz;                         // Неконстантная переменная
// ...
constexpr auto array_size1(sz); // Ошибка! Значение sz
                                // неизвестно при компиляции
std::array<int, sz> data1;      // Ошибка! Та же проблема
constexpr auto array_size2(10); // Ок, 10 представляет собой
                                // константу времени компиляции
std::array<int,                 // Ок, array_size2 представляет
           array_size2> data2;  // собой constexpr


int sz;
// ...
const auto array_size(sz);      // Ок, array_size является
                                // константной копией sz
std::array<int,                 // Ошибка! Значение array_size
           array_size> data;    // неизвестно при компиляции


constexpr                           // pow является constexpr
int pow(int base, int exp) noexcept // Не генерирует исключения
{
    // ...                          // Её реализация ниже
}
constexpr auto num_conds(5);        // Количество условий
std::array<int, pow(3, num_conds)>  // result содержит
        results;                    // 3^num_conds элементов
// ...
auto base(read_from_db("base"));    // Эти значения получаются
auto exp(read_from_db("exponent")); // во время выполнения
auto base_to_exp(pow(base, exp));   // Вызов функции pow
                                    // во время выполнения
// C++11
constexpr int pow(int base, int exp) noexcept
{
    return (exp == 0 ? 1 : base * pow(base, exp - 1));
}
// C++14
constexpr int pow(int base, int exp) noexcept
{
    auto result(1);
    for (auto i(0); i < exp; ++i) result *= base;
    return result;
}


// Обратить внимание на constexpr в конструкторе
// и функциях-членах
class Point {
public:
    constexpr Point(double x_val = 0, double y_val = 0) noexcept
        : x{x_val}, y{y_val}
    {}

    constexpr double x_value() const noexcept { return x; }
    constexpr double y_value() const noexcept { return y; }

    void set_x(double new_x) noexcept { x = new_x; }
    void set_y(double new_y) noexcept { y = new_y; }
private:
    double x;
    double y;
};
constexpr Point p1(9.4, 27.7);  // Ок, во время компиляции
                                // работает constexpr конструктор
constexpr Point p2(28.8, 5.3);  // То же самое

constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept
{
    return {(p1.x_value()+p2.x_value())/2,  // Вызов
            (p1.y_value()+p2.y_value())/2}; // constexpr функции-члена
}
constexpr auto mid(midpoint(p1, p2));       // Инициализация constexpr
                                            // объекта результатом constexpr-функции
class Point {
public:
    // ...

    constexpr void set_x(double new_x) noexcept // C++14
    { x = new_x; }
    constexpr void set_y(double new_y) noexcept // C++14
    { y = new_y; }
    // ...
};
// Возвращает отражение точки p
// относительно начала координат (C++14)
constexpr Point reflection(const Point& p) noexcept
{
    Point result;               // Неконстантный объект Point
    result.set_x(-p.x_value()); // Установка его полей x и y
    result.set_y(-p.y_value());
    return result;              // Возврат копии
}
// ...
constexpr Point p1(9.4, 27.7);
constexpr Point p2(28.8, 5.3);
constexpr auto mid(midpoint(p1, p2));
constexpr auto reflected_mid =  // reflected_mid представляет
        reflection(mid);        // собой (-19.1 -16.5) и
                                // известно во время компиляции

}

//------------------------------------------------------------------------------
// 3.10 Делайте константные функции-члены безопасными
// в смысле потоков
{

class Polynomial {
public:
    using Root_type =           // Структура данных, хранящая
        std::vector<double>;    // значения, где полином равен нулю
    // ...
    Root_type roots() const
    {
        if (!roots_are_valid) { // Если кэш не корректен,
                                // вычисляем корни и сохраняем
                                // их в root_vals
            // ...
            roots_are_valid = true;
        }
        return root_vals;
    }
private:
    mutable bool roots_are_valid{false};
    mutable Root_type root_vals{};
};
Polynomial p;
// Гонка данных!!
/*-------Поток 1--------*/      /*-------Поток 2--------*/
auto roots_ofp = p.roots();     auto vals_giving_zero = p.roots();

class Polynomial {
public:
    using Root_type = std::vector<double>;
    Root_type roots() const
    {
        std::lock_guard<std::mutex> g{m};   // Блокировка мьютекса
        if (!roots_are_valid) {             // Если кэш не корректен,
                                            // вычисляем корни
            // ...
            roots_are_valid = true;
        }
        return root_vals;
    }                                       // Разблокирование
private:
    mutable std::mutex m;
    mutable bool roots_are_valid{false};
    mutable Root_type root_vals{};
};


class Point {                       // Двумерная точка
public:
    //...
    double distance_from_origin() const noexcept
    {
        ++call_count;               // Атомарный инкремент
        return std::hypot(x, y);    // Новинка C++11
    }
private:
    mutable std::atomic<unsigned> call_count{0};
    double x, y;
};


class Widget {
public:
    // ...
    int magic_value() const
    {
        if (cached_valid) return cached_value;
        else {
            auto val1(expensive_computation1());
            auto val2(expensive_computation2());
            cached_value = val1 + val2; // Часть 1
            cached_valid = true;        // Часть 2
            return cached_value;
        }
    }

private:
    mutable std::atomic<bool> cached_valid{false};
    mutable std::atomic<int> cached_value;
};
// Поменяем местами Часть1 и часть 2
class Widget {
public:
    // ...
    int magic_value() const
    {
        if (cached_valid) return cached_value;
        else {
            auto val1(expensive_computation1());
            auto val2(expensive_computation2());
            cached_valid = true;                // Часть 1
            return cached_value = val1 + val2;  // Часть 2
        }
    }
// ...
};

class Widget {
public:
    // ...
    int magic_value() const
    {
        std::lock_guard<std::mutex> g{m};   // Блокировка мьютекса
        if (cached_valid) return cached_value;
        else {
            auto val1(expensive_computation1());
            auto val2(expensive_computation2());
            cached_value = val1 + val2;     // Часть 1
            cached_valid = true;            // Часть 2
            return cached_value;
        }
    }                                       // Разблокирование
private:
    mutable std::mutex m;
    mutable bool cached_valid{false};   // Не атомарное
    mutable int cached_value;           // Не атомарное
};

}

//------------------------------------------------------------------------------
// 3.11 Генерация специальных функций-членов
{

class Widget {
public:
    // ...
    ~Widget();                      // Пользовательский деструктор

    // ...                          // Поведение копирующего
    Widget(const Widget&)           // конструктора по умолчанию
        = default;                  // правильное

    Widget&                         // Поведение копирующего
        operator=(const Widget&)    // присваивания по умолчанию
            = default;              // правильное
};


class Base {
public:
    virtual ~Base() = default;      // Делает деструктор виртуальным

    Base(Base&&) = default;         // Поддержка перемещения
    Base& operator=(Base&&) = default;

    Base(const Base&) = default;    // Поддержка копирования
    Base& operator=(const Base&) = default;
};


class String_table {
public:
    String_table() {}
    // ...  // Функции втавки, удаления, поиска и т.п., но нет
            // функциональности копирования/перемещения/деструкции
private:
    std::map<int, std::string> values;
};
// Добавим логгирование
void make_log_entry(const std::string&);
class String_table {
public:
    String_table()
    { make_log_entry("Создание String_table"); }    // Добавлено
    ~String_table()
    { make_log_entry("Уничтожение String_table"); } // Добавлено
    // ...                                          // Прочие функции
private:
    std::map<int, std::string> values;
};


class Widget {
public:
    // ...
    template<class T>                   // Создание Widget
    Widget(const T& rhs);               // из чего угодно

    template<class T>                   // Присваивание Widget
    Widget& operator=(const T& rhs);    // из чего угодно
    // ...
};

}

//------------------------------------------------------------------------------

#endif // CHAPTER3_HPP

//------------------------------------------------------------------------------
