//------------------------------------------------------------------------------

// Глава 5: Rvalue-ссылки, семантика перемещения
// и прямая передача

//------------------------------------------------------------------------------

#ifndef CHAPTER5_HPP
#define CHAPTER5_HPP

//------------------------------------------------------------------------------

#include <Facilities.hpp>

//------------------------------------------------------------------------------
// 5.1 Азы std::move и std::forward
{


// C++11
template<class T>   // В пространстве имен std
typename remove_reference<T>::type&&
move(T&& param)
{
    using Return_type = typename remove_reference<T>::type&&;
    return static_cast<Return_type>(param);
}
// C++14
template<class T>   // В пространстве имен std
decltype(auto) move(T&& param)
{
    using Return_type = remove_reference_t<T>&&;
    return static_cast<Return_type>(param);
}


class Annotation {
public:
    explicit Annotation(std::string text);  // Параметр
                    // копируемый, так что согласно
};                  // разделу 8.1 он передается по значению
// Использование const в конструкторе
class Annotation {
public:
    explicit Annotation(const std::string text);
};
// Применяем std::move к text
class Annotation {
public:
    explicit Annotation(const std::string text)
        : value{std::move(text)}    // "Перемещение" text в value;
    {/* ... */ }                    // этот код не делает того,
                                    // что от него ожидается!
    // ...
private:
    std::string value;
};
class string {  // std::string в действительности представляет
public:         // собой typedef для std::basic_string<char>
    // ...
    string(const string& rhs);  // Копирующий конструктор
    string(string&& rhs);       // Перемещающий конструктор
};


void process(const Widget& lval_arg); // Обработка lvalue
void process(Widget&& rval_arg);      // Обработка rvalue
// ...
template<class T>                     // Шаблон, передающий
void log_and_process(T&& param)       // param на обработку
{
    auto now =                        // Получает текущее время
            std::chrono::system_clock::now();
    make_log_entry("Вызов 'process'", now);
    process(std::forward<T>(param));
}
Widget w;
// ...
log_and_process(w);             // Вызов с lvalue
log_and_process(std::move(w));  // Вызов с rvalue

// Желательная реализация с std::move
class Widget {
public:
    Widget (Widget&& rhs)
        : s{std::move(rhs.s)}
    { ++move_ctor_calls; }
    // ...
private:
    static std::size_t move_ctor_calls;
    std::string s;
};
// Не желательная реализация с std::forward
class Widget {
public:
    Widget (Widget&& rhs)
        : s{std::forward<std::string>(rhs.s)}
    { ++move_ctor_calls; }
    // ...
private:
    static std::size_t move_ctor_calls;
    std::string s;
};


}

//------------------------------------------------------------------------------
// 5.2 Отличие универсальных ссылок от rvalue-ссылок
{


void f(Widget&& param);         // Вывод типа отсутствует;
                                // param - rvalue-ссылка
// ...
Widget&& var1 = Widget();       // Вывод типа отсутствует;
                                // var1 - rvalue-ссылка
// ...
auto&& var2 = var1;             // var2 - универсальная ссылка
// ...
template<class T>
void f(std::vector<T>&& param); // param - rvalue-ссылка
// ...
template<class T>
void f(T&& param);              // param - универсальная ссылка


template<class T>
void f(T&& param);              // param - универсальная ссылка
// ...
Widget w;
f(w);               // В f передается lvalue; тип param -
                    // Widget& (т.е. lvalue-ссылка)
// ...
f(std::move(w));    // В f передается rvalue; тип param -
                    // Widget&& (т.е. rvalue-ссылка)


template<class T>
void f(std::vector<T>&& param); // param - rvalue-ссылка
// ...
std::vector<int> v;
f(v);           // Ошибка! Невозможно связать lvalue
                // с rvalue-ссылкой
// Обратить внимание на const
template<class T>
void f(const T&& param);    // param - rvalue-ссылка
// ...
template<class T,
         class Allocator = allocator<T>>    // Из стандарта C++
class vector {
public:
    void push_back(T&& x);
    // ...
};
std::vector<Widget> v;  // Приводит к следующему инстанцированию
                        // шаблона:
template<Widget,
         allocator<Widget>>
class vector {
public:
    void push_back(Widget&& x); // rvalue-ссылка
    // ...
};
// emplace_back - выполняет вывод типа
template<class T,
         class Allocator = allocator<T>>    // Из стандарта C++
class vector {
public:
    template<class... Args>
    void emplace_back(Args&&... args);
    // ...
};


template<class My_template_type>
void some_func(My_template_type&& param); // param - универсальная ссылка


auto time_func_invocation =
[] (auto&& func, auto&&.. params)                   // C++14
{
    // Запуск таймера;
    std::forward<decltype(func)>(func)(             // Вызов func
        std::forward<decltype(params)>(params)...   // с params
    );
    // Остановка таймера и запись времени;
}


}

//------------------------------------------------------------------------------
// 5.3 Используйте std::move для rvalue-ссылок,
// а std::forward - для универсальных ссылок
{


class Widget {
public:
    Widget(Widget&& rhs);   // rhs, определенно, ссылается на
    // ...                  // объект, который можно перемещать
};
// ...
class Widget {
public:
    Widget(Widget&& rhs)    // rhs является rvalue ссылкой
        : name{std::move(rhs.name)},
          p   {std::move(rhs.p)}
    {/* ... */}
private:
    std::string name;
    std::shared_ptr<Some_data_structure> p;
};


class Widget {
public:
    template<class T>
    void set_name(T&& new_name)             // new_name - универсальная ссылка
    { name = std::forward<T>(new_name); }
    // ...
};
// То же самое только std::move вместо std::forward
class Widget {
public:
    template<class T>
    void set_name(T&& new_name)     // new_name - универсальная ссылка
    { name = std::move(new_name); } // Компилируется, но это
    // ...                          // очень плохое решение!
};
// ...
std::string get_widget_name();      // Фабричнаяя функция
Widget w;
auto n(get_widget_name());          // n - локальная переменная
w.set_name(n);                      // Перемещение n в w!!!
// ...                              // Значение n теперь неизвестно


class Widget {
public:
    void set_name(const string& new_name) // Устанавливается из
    { name = new_name; }                  // const lvalue
    void set_name(string&& new_name)      // Устанавливается
    { name = std::move(new_name); }       // из rvalue

    // ...
};


template<class T>
void set_sign_text(T&& text)        // text - универсальная ссылка
{
    sign.set_text(text);            // Используем текст,
                                    // но не изменяем его
    auto now =                      // Получение текущего времени
            std::chrono::system_clock::now();
    sign_history.add(now,
         std::forward<T>(text));    // Условное приведение
}                                   // text к rvalue


class Matrix;
// ...
Matrix                      // Возврат по значению
operator+(Matrix&& lhs, const Matrix& rhs)
{
    lhs += rhs;
    return std::move(lhs);  // Перемещение lhs в
                            // возвращаемое значение
}
// Без std::move() (Нежелательно)
Matrix                      // Возврат по значению
operator+(Matrix&& lhs, const Matrix& rhs)
{
    lhs += rhs;
    return lhs;  // Копирование lhs в
                 // возвращаемое значение
}


class Fraction;
// ...
template<class T>
Fraction                            // Возврат по значению
reduce_and_copy(T&& frac)           // Универсальная ссылка
{
    frac.reduce();
    return std::forward<T>(frac);   // Перемещение rvalue и
}                                   // копирование lvalue в
                                    // возвращаемое значение


Widget make_widget()    // "Копирующая" версия make_widget
{
    Widget w;           // Локальная переменная
    // ...              // Настройка w
    return w;           // "Копирование" w в возвращаемое значение
}
// ...
Widget make_widget()        // "Перемещающая" версия make_widget
{
    Widget w;               // Локальная переменная
    // ...                  // Настройка w
    return std::move(w);    // "Перемещение" w в возвращаемое
}                           // значение (не делайте этого!)


}

//------------------------------------------------------------------------------
// 5.4 Избегайте перегрузок для универсальных ссылок
{


std::multiset<std::string> names;   // Глобальная структура данных
// ...
void log_and_add(const string& name)
{
    auto now =                      // Получение текущего времени
            std::chrono::system_clock::now();
    log(now, "log_and_add");        // Создание журнальной записи
    names.emplace(name);            // Добавление name в глобальную
}                                   // структуру данных
// ...
std::string pet_name("Darla");
log_and_add(pet_name);                  // lvalue типа std::string
log_and_add(std::string("Persephone")); // rvalue типа std::string
log_and_add("Patty Dog");               // Строковый литерал
// ...
template<class T>
void log_and_add(T&& name)
{
    auto now = std::chrono::system_clock::now();
    log(now, "log_and_add");
    names.emplace(std::forward<T>(name));
}
std::string pet_name("Darla");          // Как и ранее
log_and_add(pet_name);                  // Как и ранее, копирование
                                        // lvalue в multiset

log_and_add(std::string("Persephone")); // Перемещение rvalue
                                        // вместо копирования

log_and_add("Patty Dog");               // Создание std::string
                                        // в multiset вместо копирования
                                        // временного std::string
// ...
std::string name_from_idx(int idx);     // Возвращает имя,
                                        // соответсвующее idx
void log_and_add(int idx)               // Новая перегрузка
{
    auto now = std::chrono::system_clock::now();
    log(now, "log_and_add");
    names.emplace(name_from_idx(idx));
}
// Разрешение перегрузки работает, как и следовало ожидать
std::string pet_name("Darla");          // Как и ранее
log_and_add(pet_name);                  // Как и ранее, эти вызовы
log_and_add(std::string("Persephone")); // приводят к использованию
log_and_add("Patty Dog");               // перегрузки для T&&

log_and_add(22);                        // Вызов int-перегрузки
// ...
short name_idx;
// ...                                  // Задается значение переменной name_idx
log_and_add(name_idx);                  // Ошибка!!!!!!
                                        // Вызов перегрузки для T&&
class Person {
    std::string name;
public:
    template<class T>
    explicit Person(T&& n)              // Конструктор с прямой передачей
        : name(std::forward<T>(n)){}    // инициализирует члены-данные

    explicit Person(int idx)            // Конструктор с параметром int
        : name(name_from_idx(idx)) {}

    Person(const Person& rhs);          // Копирующий конструктор
                                        // (сгенерирован компилятором)
    Person(Person&& rhs);               // Перемешающий конструктор
                                        // (сгенерирован компилятором)
};
Person p("Nancy");
auto clone_of_p(p); // Создание нового объекта Person из p;
                    // этот код не компилируется!
// В результате инстанцирования auto clone_of_p(p):
class Person {
public:
    explicit Person(Person& n)              // Инстанцирован из
        : name(std::forward<Person&>(n)){}  // шаблона с прямой
                                            // передачей

    explicit Person(int idx);               // Как и ранее
    Person(const Person& rhs);              // Копирующий конструктор
                                            // (сгенерирован компилятором)
    // ...
};
const Person p("Nancy");    // Теперь объект константный
auto clone_of_p(p);         // Вызов копирующего конструктора!
// В результате инстанцирования auto clone_of_p(p):
class Person {
public:
    explicit Person(const Person& n); // Инстанцирован из шаблона
    Person(const Person& rhs);        // Копирующий конструктор
                                      // (сгенерирован компилятором)
    // ...
};
// Наследование с подвохом!!
class Special_person {
public:
    Special_person(const Special_person& rhs)   // Копирующий конструктор;
        : Person(rhs)                           // вызывает конструктор
    { /* ... */ }                               // базового класса с прямой передачей

    Special_person(Special_person&& rhs)        // Перемещающий конструктор;
        : Person(std::move(rhs))                // вызывает конструктор
    { /* ... */ }                               // базового класса с прямой передачей
};


}

//------------------------------------------------------------------------------
// 5.5 Знакомство с альтернативами перегрузки
// для универсальных ссылок
{


// 1) Отказ от перегрузки


// 2) Передача const T&


// 3) Передача по значению
{


class Person {
    std::string name;
public:
    explicit Person(std::string n)      // Замена конструктора с T&&
        : name(std::move(n)){}

    explicit Person(int idx)            // Как и ранее
        : name(name_from_idx(idx)) {}
    // ...
};


}


// 4) Диспетчеризация дескрипторов (tag dispatch)
{


std::multiset<std::string> names;   // Глобальная структура данных
// ...
template<class T>
void log_and_add(T&& name)
{
    auto now = std::chrono::system_clock::now();
    log(now, "log_and_add");
    names.emplace(std::forward<T>(name));
}


// Нецелочисленный аргумент добавляется
// в глобальную структуру данных
template<class T>
void log_and_add_impl(T&& name, std::false_type)
{
    auto now = std::chrono::system_clock::now();
    log(now, "log_and_add");
    names.emplace(std::forward<T>(name));
}

std::string name_from_idx(int idx);             // Как в 5.4

// Целочисленный аргумент: поиск имени и
// вызов с этим именем функции log_and_add
void log_and_add_impl(int idx, std::true_type)
{
    log_and_add(name_from_idx(int idx));
}

template<class T>
void log_and_add(T&& t)
{
    log_and_add_impl(std::forward<T>(t),
                     std::is_integral<
                        std::remove_reference_t<T>
                     >()
    );
}


}


// 5) Ограничения шаблонов, получающих универсальные ссылки
{


class Person {
public:
    template<class T,
            typename = typename std::enable_if<условие>::type>
    explicit Person(T&& n);
};


class Person {
public:
    template<class T,
            typename = typename std::enable_if<
            !std::is_same<Person,
                          typename std::decay<T>::type
                         >::value
            >::type
    >
    explicit Person(T&& n);
};


// Наследование с подвохом!!
class Special_person {
public:
    Special_person(const Special_person& rhs)   // Копирующий конструктор;
        : Person(rhs)                           // вызывает конструктор
    { /* ... */ }                               // базового класса с прямой передачей

    Special_person(Special_person&& rhs)        // Перемещающий конструктор;
        : Person(std::move(rhs))                // вызывает конструктор
    { /* ... */ }                               // базового класса с прямой передачей
};


// C++11
class Person {
public:
    template<class T,
            typename = typename std::enable_if<
                !std::is_base_of<Person,
                             typename std::decay<T>::type
                 >::value
            >::type
    >
    explicit Person(T&& n);
};
// C++14
class Person {
public:
    template<class T,
             typename = std::enable_if_t<
                !std::is_base_of_v<Person,
                                   std::decay_t<T>
                 >
            >
    >
    explicit Person(T&& n);
};


class Person {
    std::string name;
public:
    template<class T,
             typename = std::enable_if_t<
                !std::is_base_of_v<Person,
                                   std::decay_t<T>
                 >
                &&
                !std::is_integral_v<std::remove_reference_t<T>>
            >
    >
    explicit Person(T&& n)          // Конструктор для std::string и
        : name(std::forward<T>(n))  // аргументов, приводимых к
    { /* ... */ }                   // std::string

    explicit Person(int idx)        // Конструктор для
        : name(name_from_idx(idx))  // целочисленных аргументов
    { /* ... */ }

    // ...   // Копирующий и перемещающий конструкторы и т.д.
};


// Обратить внимание на std::static_assert
class Person {
    std::string name;
public:
    template<                       // Как и ранее
        class T,
        typename = std::enable_if_t<
            !std::is_base_of_v<Person,
                std::decay_t<T>
            >
            &&
            !std::is_integral_v<std::remove_reference_t<T>>
        >
    >
    explicit Person(T&& n)
        : name(std::forward<T>(n))
    {
        static_assert(
            std::is_constructible_v<std::string, T>,
            "Параметр n не может использоваться для "
            "конструирования std::string"
        );
            // Здесь код обычного конструктора
    }
            // Остальная часть класса Person (как и ранее)
};


}


}

//------------------------------------------------------------------------------
// 5.6 Свертывание ссылок
{


template<class T>
void func(T&& param);
// ...
Widget widget_factory();    // Функция возвращающая rvalue
Widget w;                   // Переменная (lvalue)
func(w);                    // Вызов функции с lvalue; тип T
                            // представляет собой Widget&
func(widget_factory());     // Вызов функции с rvalue; тип T
                            // представляет собой Widget


int x;
// ...
auto & & rx = x;    // Ошибка! Объявлять ссылки на сылки нельзя
// ...
func(w);            // Вызов функции с lvalue; тип T
                    // представляет собой Widget&
// Если возьмем тип Widget& (выведенный для T)
// и используем дл инстанцирования шаблона, то получим следующее:
void func (Widget& && param);
// Но на самом деле компилятор выведет:
void func (Widget& param);  // Компилятор использует
                            // свертывание ссылок (reference collapsing)


template<class T>
void f(T&& fparam)
{
    // ...                              // Некоторая работа
    some_func(std::forward<T>(fparam)); // Передача fparam в some_func
}
template<class T>      // В простанстве имен std
T&& forward(typename
            std::remove_reference<T>::type& param)
{
    return static_cast<T&&>(param);
}
Widget w;
f(w);       // Передача lvalue
// В результате std::forward:
Widget& && forward(typename
                   remove_reference<Widget&>::type& param)
{
    return static_cast<Widget& &&>(param);
}
// В результате подстановки typename
// remove_reference<Widget&>::type дает Widget
Widget& && forward(Widget& param)
{
    return static_cast<Widget& &&>(param);
}
// Применяем сворачивание ссылок
Widget& forward(Widget& param)
{ return static_cast<Widget&>(param); }
// Для rvalue
f(widget_factory());        // Вызов функции с rvalue; тип T
                            // представляет собой Widget
Widget&& forward(typename
                 remove_reference<Widget>::type& param)
{ return static_cast<Widget&&>(param); }
// В результате подстановки typename
// remove_reference<Widget>::type дает Widget
Widget&& forward(Widget& param)
{ return static_cast<Widget&&>(param); }

// C++14
template<class T>      // В простанстве имен std
decltype(auto) forward(remove_reference_t<T>& param)
{
    return static_cast<T&&>(param);
}


template<class T>
void func(T&& param);
// ...
Widget widget_factory();    // Функция возвращающая rvalue
Widget w;                   // Переменная (lvalue)
func(w);                    // Вызов функции с lvalue; тип T
                            // представляет собой Widget&
func(widget_factory());     // Вызов функции с rvalue; тип T
                            // представляет собой Widget
// Тоже самое с помощью auto
auto&& w1 = w;
// w - lvalue, auto => Widget&
Widget& && w1 = w;
// После сворачивания ссылок:
Widget& w1 = w; // w1 - lvalue-ссылка
// ...
auto&& w2 = widget_factory();
// w - rvalue, auto => Widget
Widget&& w2 = w;


template <class T>
class Widget {
public:
    typedef T&& rvalue_ref_to_t;
};

Widget<int&> w;
// Подстановка int& вместо T в шаблоне Widget:
typedef int& && rvalue_ref_to_t;
// В результате сворачивания ссылок:
typedef int& rvalue_ref_to_t;


}

//------------------------------------------------------------------------------
// 5.7 Считайте, что перемещающие операции
// отсутствуют, дороги или не используются
{


std::vector<Widget> vw1;
// Размещение данных в vw1
// ...
// Перемещение vw1 в vw2. Выполняется
// за константное время, изменяя
// только указатели в vw1 и vw2
auto vw2(std::move(vw1));


std::array<Widget, 10000> aw1;
// Размещение данных в aw1
// ...
// Перемещение aw1 в aw2. Выполняется
// за линейное время. Все элементы
// aw1 перемещаются в aw2
auto aw2(std::move(aw1));


}

//------------------------------------------------------------------------------
// 5.8 Познакомьтесь с случаями некорректной
// работы прямой передачи
{


// Общее
{


template<class T>
void fwd(T&& param)             // Принимает любой аргумент
{
    f(std::forward<T>(param));  // Передача аргумента в f
}

// Вариативный шаблон (variadic template)
template<class... Ts>
void fwd(Ts&&... params)            // Принимает любые аргументы
{
    f(std::forward<Ts>(params)...); // Передача аргументов в f
}

f ( expression );   // Если этот вызов выполняет что-то одно,
fwd ( expression ); // а этот - нечто иное, прямая передача
                    // функцией fwd функции f неудачна


}


// Инициализаторы в фигурный скобках
{


void f(const std::vector<int>& v);
f({0, 1, 2, 3});    // Ок "{0, 1, 2, 3}" неявно преобразуется
                    // в std::vector<int>
// ...
fwd({0, 1, 2, 3});  // Ошибка! Код не компилируется!
// ...
auto il = {0, 1, 2, 3}; // Тип il выводится как
                        // std::initializer_list<int>
f(il);                  // Ок, прямая передача il в f


}


// 0 и NULL в качестве нулевых указателей


// Целочисленные члены-данные static const
// и constexpr без определений
{


class Widget {
public:
    // Объявление min_vals:
    static constexpr std::size_t min_vals = 28;
    // ...
};
// ...      // Определения min_vals нет
std::vector<int> widget_data;
widget_data.reserve(Widget::min_vals);  // Использование min_vals

void f(std::size_t val);
f(Widget::min_vals);        // Ок, рассматривается как "f(28)"
fwd(Widget::min_vals);      // Ошибка! Не должно компоноваться
// fwd требует определения min_vals, решение:
constexpr std::size_t Widget::min_vals; // В .cpp-файле Widget


}


// Имена перегруженных функций и имена шаблонов
{


void f(int (*pf)(int));  // pf - функция обработки
void f(int pf(int));     // Объявление той же f, что и выше
// ...
int process_val(int value);
int process_val(int value, int priority);
// ...
f(process_val);         // Без проблем
fwd(process_val);       // Ошибка! Какая process_val?
// ...
template<class T>
T work_on_val(T param)  // Шаблон обработки значений
{ /*...*/ }
// ...
frd(work_on_val);       // Ошибка! Какое инстанцировании work_on_val
// ... Решение этих проблем:
using Process_func_type =
        int (*)(int);
Process_func_type process_val_ptr = // Определяем необходимую
        process_val;                // сигнатуру process_val
// ...
fwd(process_val_ptr);               // Ок
fwd(static_cast<Process_func_type>(work_on_val));   // Так же ок


}


// Битовые поля
{


struct Ip_v4_header {
    std::uint32_t version: 4,
                  IHL: 4,
                  DSCP: 6,
                  ECN: 2,
                  total_lenght: 16;
    // ...
};
void f(std::size_t sz); // Вызываемая функция
Ip_v4_header h;
// ...
f(h.total_lenght);      // Все в порядке
fwd(h.total_lenght);    // Ошибка!
// Решение проблемы:
// Копирование значения битового поля
auto lenght = static_cast<std::uint16_t>(h.total_lenght);
fwd(lenght);            // Передача копии


}


}

//------------------------------------------------------------------------------

#endif // CHAPTER5_HPP

//------------------------------------------------------------------------------
