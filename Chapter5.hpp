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

}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#endif // CHAPTER5_HPP

//------------------------------------------------------------------------------
