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
// 8.2 Рассмотрите применение размещения вместо вставки
{


template<class T,                           // Из стандарта
         class Allocator = allocator<T>>    // С++11
class vector {
public:
    // ...
    void push_back(const T& x);             // Вставка lvalue
    void push_back(T&& x);                  // Вставка rvalue
    // ...
};
std::vector<std::string> vs;                // Контейнер std::string
vs.push_back("xyzzy");                      // Добавление строкового литерала
// Для вызова push_back со строковым литералом
// компиляторы рассмотрят вызов следующим образом:
vs.push_back(std::string("xyzzy");          // Создание временного
// объекта std::string и передача его функции push_back


// Чтобы избежать создание временного объекта:
vs.emplace_back("xyzzy");   // Создает std::string в vs
                            // непосредственно из "xyzzy"
vs.emplace_back(50, 'x');   // Создает std::string из
                            // 50 символов 'x'


std::string queen_of_disco{"Donna Summer"};
vs.push_back(queen_of_disco);       // Копирующее конструирование
                                    // queen_of_disco в конце vs
vs.emplace_back(queen_of_disco);    // Тоже самое


std::vector<std::string> vs;        // Как и ранее
// ...                              // Добавление элементов в vs
vs.emplace(vs.begin(), "xyzzy");    // Добавление "xyzzy" в начало vs


vs.emplace_back("xyzzy");   // Копирует новое значение в конец
                            // контейнера; тип аргумента отличен от
                            // типа, хранимого в контейнере;
                            // контейнер не отвергает дубликаты
vs.emplace_back(50, 'x');   // Тоже самое


std::list<std::shared_ptr<Widget>> ptrs;
void kill_widget(Widget* p_widget);

// Использование push_back, с ресурсом все ОК
ptrs.push_back(std::shared_ptr<Widget>(new Widget, kill_widget));
ptrs.push_back({new Widget, kill_widget});
// Использование emplace_back, потенциальная утечка ресурса
ptrs.emplace_back(new Widget, kill_widget);
// Более лучший стиль
std::shared_ptr<Widget> spw(new Widget,     // Создание Widget и
                           kill_widget);    // передача его spw
ptrs.push_back(std::move(spw));             // Добавление spw
                                            // как rvalue
// Версия с emplace_back:
ptrs.emplace_back(std::move(spw));


std::vector<std::regex> regexes;
regexes.emplace_back(nullptr);      // Компилируется, "все ок"
std::regex r = nullptr;             // Ошибка! Не компилируется! Конструктор explicit
regexes.push_back(nullptr);         // Ошибка! Не компилируется! Конструктор explicit
std::regex upper_case_word("[A-Z]+");
std::regex r(nullptr);              // Компилируется
std::regex r1 = nullptr;            // Ошибка! Не компилируется!
std::regex r2(nullptr);             // Компилируется


// Вывод:
regexes.emplace_back(nullptr);  // Компилируется. Прямая инициализация
                                // разрешает использовать коннструктор
                                // explicit std::regex, получающий указатель
regexes.push_back(nullptr);     // Ошибка! Копирующая инициализация такие
                                // конструкторы не использует


}

//------------------------------------------------------------------------------

#endif // CHAPTER8_HPP

//------------------------------------------------------------------------------
