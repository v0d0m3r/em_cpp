//------------------------------------------------------------------------------

// Глава 8: Тонкости

//------------------------------------------------------------------------------

#ifndef CHAPTER8_HPP
#define CHAPTER8_HPP

//------------------------------------------------------------------------------

#include "Facilities.hpp"

//------------------------------------------------------------------------------
// 8.1 Рассмотрите передачу по значению для копируемых параметров,
// которые легко перемещаются и всегда копируются
{


class Widget {
public:
    void add_name(const std::string& new_name)  // lvalue;
    { names.push_back(new_name); }              // копируем

    void add_name(const std::string&& new_name) // rvalue;
    { names.push_back(std::move(new_name)); }   // перемещаем
    // ...
private:
    std::vector<std::string> names;
};


class Widget {
public:
    template<class T>           // Получаем как lvalue,
    void add_name(T&& new_name) // так и rvalue; lvalue
                                // копируем, rvalue перемещаем
    { names.push_back(std::forward<T>(new_name)); }

    // ...
private:
    std::vector<std::string> names;
};


class Widget {
public:
    void add_name(std::string new_name)         // lvalue или
    { names.push_back(std::move(new_name)); }   // rvalue;
    // ...                                      // перемещаем его
private:
    std::vector<std::string> names;
};
// ...
Widget w;
// ...
std::string name{"Bart"};
w.add_name(name);           // Вызов add_name с lvalue
// ...
w.add_name(name + "Jenne"); // Вызов add_name с rvalue


class Widget {
public:
    void set_ptr(std::unique_ptr<std::string>&& ptr)
    { p = std::move(ptr); }
    // ...
private:
    std::unique_ptr<std::string> p;
};
Widget w;
// ...
w.set_ptr(std::make_unique<std::string>("Modern C++"));


class Widget {
public:
    void set_ptr(std::unique_ptr<std::string> ptr)
    { p = std::move(ptr); }
    // ...
private:
    std::unique_ptr<std::string> p;
};


class Widget {
public:
    void add_name(std::string new_name)
    {
        if (    (new_name.length() >= std::numeric_limits<size_t>::lowest()+1) &&
                (new_name.length() <= std::numeric_limits<size_t>::max()-1))
            names.push_back(std::move(new_name));
    }
    // ...
private:
    std::vector<std::string> names;
};


class Password {
public:
    explicit Password(std::string pwd)  // Передача по значению;
        : text{std::move(pwd)} {}       // конструирование text

    void change_to(std::string new_pwd) // Передача по значению;
    { text = std::move(new_pwd); }      // присваивание text
private:
    std::string text;                   // Текст пароля
};
// ...
std::string init_pwd{"Supercalifgrawsxxxssswwlkjeryesdadasdxc"};
Password p(init_pwd);
// ...
std::string new_password = "Beware the Jabberwock";
p.change_to(new_password);


class Password {
public:
    // ...
    void change_to(std::string& new_pwd) // Перегрузка для
    {                                    // для lvalue
        text = new_pwd; // При text.capaciy >= new_pwd.size()
                        // можно использовать память text
    }
    // ...
private:
    std::string text;                   // Текст пароля
};


class Widget { /* ... */ };                         // Базовый класс
class Special_widget : public Widget { /* ... */ }; // Производный класс
void process_widget(Widget w);                      // Функция для любового вида Widget,
                                                    // включая производные типы;
                                                    // подвержена проблеме срезки
// ...
Special_widget sw;
// ...
process_widget(sw);                                 // process_widget видит Widget,
                                                    // а не Special_widget!


}

//------------------------------------------------------------------------------

#endif // CHAPTER8_HPP

//------------------------------------------------------------------------------
