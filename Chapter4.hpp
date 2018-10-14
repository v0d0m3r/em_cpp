//------------------------------------------------------------------------------

// Глава 4: Интелектуальные указатели

//------------------------------------------------------------------------------

#ifndef CHAPTER4_HPP
#define CHAPTER4_HPP

//------------------------------------------------------------------------------

#include <Facilities.hpp>

//------------------------------------------------------------------------------
// 4.1 Используйте std::unique_ptr для управления
// ресурсами путем исключительного владения
{

class Investment { /*...*/ };
class Stock       : public Investment { /*...*/ };
class Bond        : public Investment { /*...*/ };
class Real_estate : public Investment { /*...*/ };
// ...
template<class... Ts>            // Возвращает std::unique_ptr
std::unique_ptr<Investment>      // на объект, созданный из
make_investment(Ts&&... params); // данных аргументов

{
    // ...
    auto p_investment =                 // p_investment имеет тип
            make_investment(arguments); // std::unique_ptr<Investment>
}                                       // Уничтожение *p_investment
// ...
auto del_invmt = [] (Investment* p_investment)  // Пользовательский
            {                                   // удалитель
                make_log_entry(p_investment);   // (Лямбда-выражение)
                delete p_investment;
            };
// C++11
template<class... Ts>           // Исправленный
std::unique_ptr<Investment,     // возвращаемый тип
                decltype(del_invmt)>
make_investment(Ts&&... params)
{
    std::unique_ptr<Investment,
                    decltype(del_invmt)>    // Возвращаемый
            p_inv(nullptr, del_invmt);      // указатель
    if (/* Должен быть создан объект Stock */)
        p_inv.reset(new Stock(std::forward<Ts>(params)...));
    else if (/* Должен быть создан объект Bond */)
        p_inv.reset(new Bond(std::forward<Ts>(params)...));
    else if (/* Должен быть создан объект Real_estate */)
        p_inv.reset(new Real_estate(std::forward<Ts>(params)...));
    return p_inv;
}
class Investment {
public:
    // ...
    virtual ~Investment();  // Важная часть дизайна
    // ...
};

template<class... Ts>
auto make_investment(Ts&&... params) // C++14
{
    auto del_invmt = [] (Investment* p_investment)  // Теперь размещается в
                {                                   // пределах make_investment
                    make_log_entry(p_investment);
                    delete p_investment;
                };
    std::unique_ptr<Investment, decltype(del_invmt)>
            p_inv(nullptr, del_invmt);
    if (/* Должен быть создан объект Stock */)
        p_inv.reset(new Stock(std::forward<Ts>(params)...));
    else if (/* Должен быть создан объект Bond */)
        p_inv.reset(new Bond(std::forward<Ts>(params)...));
    else if (/* Должен быть создан объект Real_estate */)
        p_inv.reset(new Real_estate(std::forward<Ts>(params)...));

    return p_inv;
}


auto del_invmt1 = [] (Investment* p_investment)
{                                   // Пользовательский удалитель
    make_log_entry(p_investment);   // как лямбда выражение
    delete p_investment;            // без состояния
};
template<class... Ts>               // Возвращаемый тип имеет размер
std::unique_ptr<Investment,         // Investment*
                decltype(del_invmt1)>
make_investment(Ts&&... params);

auto del_invmt2(Investment* p_investment)
{                                   // Пользовательский удалитель
    make_log_entry(p_investment);   // как функция
    delete p_investment;
}
template<class... Ts>               // Возвращаемый тип имеет размер
std::unique_ptr<Investment,         // Investment* плюс как минимум
    decltype(void(*)(Investment*))> // размер указателя на функцию
make_investment(Ts&&... params);

}

//------------------------------------------------------------------------------
// 4.2 Используйте std::shared_ptr для управления
// ресурсами путем совместного владения
{

auto logging_del = [] (Widget* pw)  // Пользовательский удалитель
    {
        make_log_entry(pw);
        delete pw;
    };
std::unique_ptr<                    // Тип удалителя является
    Widget, decltype(logging_del)   // частью типа указателя
    > upw(new Widget, logging_del);

std::shared_ptr<Widget>             // Тип удалителя не является
    spw(new Widget, logging_del);   // частью типа указателя


auto custom_deleter1 = [] (Widget* pw) { /*...*/ }; // Пользовательские
auto custom_deleter2 = [] (Widget* pw) { /*...*/ }; // удалители
                                                    // разных типов
std::shared_ptr<Widget> pw1(new Widget, custom_deleter1);
std::shared_ptr<Widget> pw2(new Widget, custom_deleter2);
// ...
std::vector<std::shared_ptr<Widget>> vpw{pw1, pw2};


// Неопределенное поведение
auto pw = new Widget;       // pw - обычный указатель
// ...
std::shared_ptr<Widget>
    spw1(pw, logging_del);  // Создание управляющего блока для *pw
// ...
std::shared_ptr<Widget>     // Создание второго
    spw2(pw, logging_del);  // управляющего блока для *pw!


std::shared_ptr<Widget>
    spw1(new Widget,    // Непосредственное использование new
         logging_del);

std::shared_ptr<Widget> // spw2 использует тот же
    spw2(spw1);         // управляющий блок, что и spw1


std::shared_ptr<Widget> // spw2 использует тот же
    spw2(spw1);         // управляющий блок, что и spw1
std::vector<std::shared_ptr<Widget>> processed_widgets;
class Widget {
public:
    // ...
    void process();
    // ...
};
void Widget::process()
{
    // ...                                  // Обработка Widget
    processed_widgets.emplace_back(this);   // Добавление в список
}                                           // обработанных Widget;
                                            // Это не правильно!
// ...
std::shared_ptr<Widget> spw1(new Widget, logging_del);  // Создание управляющих блоков
std::shared_ptr<Widget> spw2(new Widget, logging_del);  // для соответствующих объектов
std::shared_ptr<Widget> spw3(new Widget, logging_del);  // Все ок!
// ...
spw1->process();    // Создание второго управляющего блока! Ошибка!
spw2->process();    // Тоже самое!
// Чтобы такое получилось нужно:
class Widget : public std::enable_shared_from_this<Widget> {
public:
    // ...
    void process();
    // ...
};
// Безопасная реализация Widget::process():
void Widget::process()
{
    // Обработка Widget
    // ...
    // Добавляем std::shared_ptr, указывающий на
    // текущий объект, в processed_widgets
    processed_widgets.emplace_back(shared_from_this());
}


class Widget : public std::enable_shared_from_this<Widget> {
public:
    // Фабричная функция, пересылающая
    // аргументы закрытому конструктору:
    template<class... Ts>
    static std::shared_ptr<Widget> create(Ts&&... params);
    // ...
    void process();
    // ...
private:
    // ...          // Конструкторы
};


}

//------------------------------------------------------------------------------
// 4.3 Используйте std::weak_ptr
// для std::shared_ptr-подобных указателей,
// которые могут быть висячими
{

auto spw =                      // После создания spw счетчик
    std::shared_ptr<Widget>();  // ссылок указываемого Widget
                                // равен 1.
// ...
std::weak_ptr<Widget> wpw(spw); // wpw указывает на тот же
                                // Widget, что и spw. Счетчик
                                // ссылок остается равным 1
// ...
spw = nullptr;                  // Счетчик ссылок равен 0, и
                                // Widget уничтожается
                                // wpw становится висячим
if (wpw.expired()) {/* ... */}  // Если wpw не указывает на объект


std::shared_ptr<Widget> spw1    // Если wpw просрочен
    = wpw.lock();               // spw1 нулевой
auto spw2 = wpw.lock();         // То же самое но с auto
std::shared_ptr<Widget> spw3(wpw);  // Если wpw просрочен генерируется
                                    // исключение std::bad_weak_ptr


class Widget_id {};
std::unique_ptr<const Widget> load_widget(Widget_id id);

std::shared_ptr<const Widget> fast_load_widget(Widget_id id)
{
    static std::unordered_map<Widget_id,
            std::weak_ptr<const Widget>> cache;

    auto obj_ptr                // obj_ptr является std::shared_ptr
            = cache[id].lock(); // для кешированного объекта и
                                // и нулевым указателем для объекта
                                // отсутствующего в кеше
    if (!obj_ptr) {             // При отсутсвии в кеше
        obj_ptr                 //объект загружается
            = load_widget(id);
        cache[id] = obj_ptr;    // кешируется
    }
    return obj_ptr;
}


struct B;
struct A {
    std::shared_ptr<B> spb;
};
struct B {
    std::shared_ptr<A> spa;
};
struct C { std::shared_ptr<B> spb; };

{
    std::shared_ptr<A> a(new A);
    std::shared_ptr<B> b(new B);
    a->spb = b;  // a ссылается на b с помощью shared_ptr, счетчик = 2
    b->spa = a;  // b аналогично ссылается на a, счетчик = 2
    // ...
}                // При уничтожении b счетчик ссылок в a->spb равен 1
                 // следовательно, ресурс B не освобождается
                 // Дальше уничтожается a, происходит уменьшение
                 // счетчика ссылок (т.к. член-данные все еще хранят std::shared_ptr
                 // Ресурсы A и B не смогут освободиться

}

//------------------------------------------------------------------------------
// 4.4 Предпочитайте использование std::make_unique
// и std::make_shared непосредственному
// использованию оператора new
{

auto upw1(std::make_unique<Widget>());      // С make-функцией
std::unique_ptr<Widget> upw2(new Widget);   // Без make-функции

auto spw1(std::make_shared<Widget>());      // С make-функцией
std::shared_ptr<Widget> spw2(new Widget);   // Без make-функции


void process_widget(std::shared_ptr<Widget> spw, int priority);
int compute_priority();
process_widget(                                 // Потенциальная
        std::shared_ptr<Widget>(new Widget),    // утечка
        compute_priority());                    // ресурса

process_widget(                                 // Потенциальной
        std::make_shared<Widget>(),             // утечки
        compute_priority());                    // ресурсов нет


//Создание std::initializer_list
auto init_list{ 10, 20 };
// ...
// Создание std::vector с помощью конструктора
// с параметром std::initializer_list
auto spv = std::make_shared<std::vector<int>>(init_list);


class Real_big_type {/* ... */};
// ...
auto p_big_obj =                            // Создание большого
        std::make_shared<Real_big_type>();  // объекта с помощью
                                            // std::make_shared
// ...      // Создание указателей std::shared_ptr и std::weak_ptr
            // на большой объект и работа с ними

// ...      // Уничтожение последнего указателя std::shared_ptr на
            // этот объект; остаются указатели std::weak_ptr

// ...      // Во время этого периода память, ранее занятая большим
            // объектом, остается занятой

// ...      // Уничтожение последнего указателя std::weak_ptr на
            // объект; освобождение памяти, выделенного для
            // управляющего блока и объекта

class Real_big_type {/* ... */};
// ...
// Создание большого объекта с помощью new
std::shared_ptr<Real_big_type> p_big_obj(new Real_big_type);

// ...      // Создание указателей std::shared_ptr и std::weak_ptr
            // на большой объект и работа с ними

// ...      // Уничтожение последнего указателя std::shared_ptr на
            // этот объект; остаются указатели std::weak_ptr
            // Память, выделенная для объекта, освобождается

// ...      // Во время этого периода остается зянятой только память,
            // ранее выделенная для управляющего блока

// ...      // Уничтожение последнего указателя std::weak_ptr на
            // объект; освобождение памяти, выделенного для
            // управляющего блока


void process_widget(std::shared_ptr<Widget> spw, int priority);
void cus_del(Widget* ptr);  // Пользовательский удалитель
int compute_priority();
// ...
process_widget(                                       // Потенциальная
        std::shared_ptr<Widget>(new Widget, cus_del), // утечка
        compute_priority());                          // ресурса
// ...
std::shared_ptr<Widget> spw(new Widget, cus_del);
process_widget(spw, compute_priority());    // Корректно, но не
                                            // оптимально, см. ниже

process_widget(std::move(spw),      // Эффективно и безопасно
               compute_priority()); // в смысле исключений


}

//------------------------------------------------------------------------------
// 4.5 При использовании идиомы указателя
// на реализацию определяйте специальные
// функции-члены
{

class Gadget {/*...*/};

class Widget {          // В заголовочном файле "Widget.hpp"
public:
    Widget();
    // ...
private:
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;  // Gadget - некий пользовательский тип
};


// C++98
class Widget {      // В заголовочном файле "Widget.hpp"
public:
    Widget();
    ~Widget();
    // ...
private:
    struct Impl;    // Объявление структуры реализации
    Impl* p_impl;   // и указателя на неё
};
// Файл реализации Widget.cpp
#include "Widget.hpp"
#include "Gadget.hpp"
#include <string>
#include <vector>

struct Widget::Impl {           // Определение Widget::Impl
    std::string name;           // с членами-данными, ранее
    std::vector<double> data;   // находившимися в Widget
    Gadget g1, g2, g3;
};

Widget::Widget()                // Создание членов-данных
    : p_iml(new Impl)           // для данного объекта Widget
{}
Widget::~Widget()               // Уничтожение членов-данных
{ delete p_impl;}               // для данного объекта


// C использованием std::unique_ptr
class Widget {      // В заголовочном файле "Widget.hpp"
public:
    Widget();
    ~Widget();      // Только объявление
    // ...
private:
    struct Impl;
    std::unique_ptr<Impl> p_impl;   // Интелектуальный указатель
                                    // вместо обычного
};

// Файл реализации Widget.cpp
#include "Widget.hpp"
#include "Gadget.hpp"
#include <string>
#include <vector>

struct Widget::Impl {           // Определение Widget::Impl
    std::string name;           // с членами-данными, ранее
    std::vector<double> data;   // находившимися в Widget
    Gadget g1, g2, g3;
};

Widget::Widget()
    : p_iml(std::make_unique<Impl>())
{}

Widget::~Widget() = default;    // Определение ~Widget


// Так как мы объявили деструктор явно,
// то, если нам нужны перемещающие операции,
// их тоже необходимо объявить
class Widget {      // В заголовочном файле "Widget.hpp"
public:
    Widget();
    ~Widget();      // Только объявление

    Widget(Widget&& rhs) = default;             // Идея верна
    Widget& operator=(Widget&& rhs) = default;  // код нет
    // ...
private:
    struct Impl;
    std::unique_ptr<Impl> p_impl;
};


class Widget {      // В заголовочном файле "Widget.hpp"
public:
    Widget();
    ~Widget();      // Только объявление

    Widget(Widget&& rhs);            // Только объявления
    Widget& operator=(Widget&& rhs);
    // ...
private:
    struct Impl;
    std::unique_ptr<Impl> p_impl;
};
// Файл реализации Widget.cpp
#include <string>
// ...
struct Widget::Impl { /* ... */};

Widget::Widget()
    : p_iml(std::make_unique<Impl>())
{}

Widget::~Widget() = default;    // Определение ~Widget
// Определения
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;


// Копирующие операции
class Widget {      // В заголовочном файле "Widget.hpp"
public:
    // ...
    Widget(const Widget& rhs);  // Только объявления
    Widget& operator=(const Widget& rhs);
    // ...
private:
    struct Impl;
    std::unique_ptr<Impl> p_impl;
};
// Файл реализации Widget.cpp
#include <string>
// ...
struct Widget::Impl { /* ... */};

Widget::Widget()
    : p_iml(std::make_unique<Impl>())
{}
// ....
Widget::Widget(const Widget& rhs)   // Копирующий конструктор
    : p_impl{nullptr}
{
    if (rhs.p_impl)
        p_impl = std::make_unique<Impl>(*rhs.p_impl);
}
                                    // Копирующее присваивание
Widget& Widget::operator=(const Widget& rhs)
{
    if (!rhs.p_impl)
        p_impl.reset();
    else if (!p_impl)
        p_impl = std::make_unique<Impl>(*rhs.p_impl);
    else
        *p_impl = *rhs.p_impl;

    return *this;
}


// Использование std::shared_ptr для Pimpl
class Widget {      // В заголовочном файле "Widget.hpp"
public:
    Widget();
    // ...          // Нет объявлений деструктора
                    // и перемещающих операций
private:
    struct Impl;
    std::shared_ptr<Impl> p_impl; // std::shared_ptr
                                  // вместо std::unique_ptr
};
// ...
Widget w1;
auto w2(std::move(w1)); // Перемещающее конструирование w2
w1 = std::move(w2);     // Перемещающее присваивание w1


}

//------------------------------------------------------------------------------

#endif // CHAPTER4_HPP

//------------------------------------------------------------------------------

