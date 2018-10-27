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



//------------------------------------------------------------------------------

#endif // CHAPTER5_HPP

//------------------------------------------------------------------------------
