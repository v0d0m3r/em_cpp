//------------------------------------------------------------------------------

// Глава 6: Лямбда выражения

//------------------------------------------------------------------------------

#ifndef CHAPTER6_HPP
#define CHAPTER6_HPP

//------------------------------------------------------------------------------

#include "Facilities.hpp"

//------------------------------------------------------------------------------
// 6.0 Общая информация
{


std::find_if(container.begin(), container.end(),
             [](int val) { return 0 < val && val < 10; }));


{
    int x;                      // x - локальная переменная
    // ...
    auto c1 =                   // c1 - копия замыкания,
        [x] (int y) {           // сгенерированного
            return x*y > 55; }; // лямбда-выражением
    auto c2 = c1;               // c2 копия c1
    auto c3 = c2;               // c3 копия c2
    // ...
}


}

//------------------------------------------------------------------------------
// 6.1 Избегайте режимов захвата по умолчанию
{


using Filter_container = std::vector<std::function<bool(int)>>;
// ...
Filter_container filters;   // Функции фильтрации
filters.emplace_back(
    [] (int value) { return value % 5 == 0; }
);
// ...
void add_divisor_filter()
{
    auto calc1 = compute_some_value1();
    auto calc2 = compute_some_value2();
    auto divisor = compute_divisor(calc1, calc2);
    filters.emplace_back(   // Опасно! Ссылка на divisor повиснет
        [&] (int value) { return value % divisor == 0; }
    );
}
// Использование явного захвата divisor по ссылке
filters.emplace_back(                   // Опасно! Ссылка на
    [&divisor] (int value)              // на divisor все равно
    { return value % divisor == 0; }    // повисает!
);


// С++11
template<class C>
void work_with_container(const C& container)
{
    auto calc1 = compute_some_value1();
    auto calc2 = compute_some_value2();
    auto divisor = compute_divisor(calc1, calc2);

    using Cont_elem_t = typename C::value_type;

    using std::begin;
    using std::end;

    if (std::all_of(                    // Все значения
            begin(container)),          // в контейнере
            end(container),             // кратны divisor?
            [&] (const Cont_elem_t& value)
            { return value % divisor == 0; }
        ) {
        // ...                          // Да
    } else {
        // ...                          // Как минимум одно - нет
    }
}
// С++14
template<class C>
void work_with_container(const C& container)
{
    auto calc1 = compute_some_value1();
    auto calc2 = compute_some_value2();
    auto divisor = compute_divisor(calc1, calc2);

    using std::begin;
    using std::end;

    if (std::all_of(                    // Все значения
            begin(container)),          // в контейнере
            end(container),             // кратны divisor?
            [&] (const auto& value)     // C++14
            { return value % divisor == 0; }
        ) {
        // ...                          // Да
    } else {
        // ...                          // Как минимум одно - нет
    }
}


filters.emplace_back(                   // Теперь
    [=] (int value)                     // divisor
    { return value % divisor == 0; }    // не может
);                                      // зависнуть


class Widget {
public:
    // ...                      // Конструкторы и т.п.
    void add_filter() const;    // Добавление элемента в filters
private:
    int divisor;                // Используется в фильтре
};
// ...
void Widget::add_filter() const
{
    filters.emplace_back(
        [=] (int value)
        { return value % divisor == 0; }
    );
}
void Widget::add_filter() const     // Генерирует компилятор для функции выше!
{
    auto current_object_ptr = this;
    filters.emplace_back(
        [current_object_ptr] (int value)
        { return value % current_object_ptr->divisor == 0; }
    );
}
// ...
void Widget::add_filter() const
{
    filters.emplace_back(       // Ошибка! Нет захватываемой
        [divisor] (int value)   // локальной переменной divisor!
        { return value % divisor == 0; }
    );
}
// ...
void do_some_work()
{
    auto pw =
        std::make_unique<Widget>();

    pw->add_filter();   // Добавление фильтра
                        // с Widget::divisor
    // ...
}   // Уничтожение Widget; filters хранит висячий указатель!
// Решение проблемы выше создание локальной копии
void Widget::add_filter() const
{
    auto divisor_copy = divisor;                // Копирование
    filters.emplace_back(                       // члена-данных
        [divisor_copy] (int value)              // Захват копии
        { return value % divisor_copy == 0; }   // Ее использование
    );
}
// Захват по умолчанию тоже сработает с локальной копией
void Widget::add_filter() const
{
    auto divisor_copy = divisor;                // Копирование
    filters.emplace_back(                       // члена-данных
        [=] (int value)                         // Захват копии
        { return value % divisor_copy == 0; }   // Ее использование
    );
}
// С++14
void Widget::add_filter() const
{
    filters.emplace_back(
        [divisor = divisor] (int value)     // Копирование divisor
                                            // в замыкание
        { return value % divisor == 0; }    // Использование копии
    );
}


// Использование статических переменных
void add_divisor_filter()
{
    // Статические
    static auto calc1
        = compute_some_value1();
    static auto calc2
        = compute_some_value2();
    static auto divisor
        = compute_divisor(calc1, calc2);

    filters.emplace_back(
        [=] (int value)                     // Ничего не захватывает
        { return value % divisor == 0; }    // Ссылка на статическую
    );                                      // переменную

    ++divisor;                              // Изменение divisor
}


}

//------------------------------------------------------------------------------
// 6.2 Используйте инициализирующий захват
// для перемещения объектов в замыкания
{


class Widget {                      // Некоторый полезный тип
public:
    // ...
    bool is_validated() const;
    bool is_processed() const;
    bool is_archived() const;
private:
    // ...
};
auto pw =
    std::make_unique<Widget>();     // Создание Widget
// ...                              // Настройка *pw
auto func = [pw = std::move(pw)]    // Инициализация члена
    { return pw->is_validated()     // в замыкании с помощью
          && pw->is_archived(); };  // std::move(pw)


auto func = [pw = std::make_unique<Widget>()]   // Инициализация
    { return pw->is_validated()     // члена-данных в замыкании
          && pw->is_archived(); };  // результатом вызова make_unique


class Is_val_and_arch {
public:
    using Data_type = std::unique_ptr<Widget>;

    explicit Is_val_and_arch(Data_type&& ptr)
        : pw{std::move(ptr)} {}

    bool operator() () const
    { return  pw->is_validated() && pw->is_archived(); }
private:
    Data_type pw;
};
auto func = Is_val_and_arch{std::make_unique<Widget>()};


// C++14
std::vector<double> data;               // Объект, перемещаемый
                                        // в замыкание
// ...                                  // Наполнение данными
auto func = [data = std::move(data)]    // Инициализирующий захват
            { /* Использование данных */ };
// C++11
std::vector<double> data;
// ...
auto func =
    std::bind(                                  // Эмуляция в С++11
        [] (const std::vector<double>& data)    // инициализирующего
        { /* Использование данных */ },         // захвата
        std::move(data)
    );


auto func =
    std::bind(                                  // Эмуляция в С++11
        [] (std::vector<double>& data) mutable  // инициализирующего
        { /* Использование данных */ },         // захвата для лямбда-
        std::move(data)                         // выражения, объяв-
    );                                          // ленного mutable


auto func = [pw = std::make_unique<Widget>()]   // Инициализация
    { return pw->is_validated()     // члена-данных в замыкании
          && pw->is_archived(); };  // результатом вызова make_unique
// C++11
auto func = std::bind(
    [] (const std::unique_ptr<Widget>& pw)
    { return pw->is_validated()
          && pw->is_archived(); },
    std::make_unique<Widget>()
    );


}

//------------------------------------------------------------------------------
// 6.3 Используйте параметры decltype для auto&&
// для передачи с помощью std::forward
{


auto f = [](auto x) { return  normalize(x); };
class Some_compiler_generated_class_name {
public:
    template<class T>
    auto operator() (T x) const
    { return normalize(x); }
                                // Прочая функциональность
};                              // класса замыкания


auto f = [](auto&& x)
{ return  normalize(std::forward<???>(x)); };


auto f = [](auto&& x)
{ return  normalize(std::forward<decltype(x)>(x)); };


auto f = [](auto&&... xs)
{ return  normalize(std::forward<decltype(xs)>(x)...); };


}

//------------------------------------------------------------------------------
// 6.4 Предпочитайте лямбда-выражения
// применению std::bind
{


// Тип для момента времени
using Time = std::chrono::steady_clock::time_point;
enum class Sound { Beep, Siren, Whistle };
using Duration = std::chrono::steady_clock::duration;
// ...
// В момент t издать звук s продолжительностью d
void set_alarm(Time t, Sound s, Duration d);


// set_sound_l ("l" - означет "лямбда-выражение") - функциональный
// объект, позволяющий указать сигнал будильника, который должен
// звучать через час в течение 30 с
auto set_sound_l =
        [] (Sound s)
        {
            // Делает доступными компоненты std::chrono
            using namespace  std::chrono;

            set_alarm(steady_clock::now()+hours(1), // Будильник через
                      s,                            // час, звучит
                      seconds(30));                 // 30 секунд
        };
// c++14
auto set_sound_l =
        [] (Sound s)
        {
            using namespace std::chrono;
            using namespace std::literals;          // Суффиксы с++14

            set_alarm(steady_clock::now() + 1h,
                      s,
                      30s);
        };


// Для bind
using namespace std::chrono;
using namespace std::literals;
using namespace std::placeholders;      // Необходимо для "_1"


// Первая ошибочная версия
auto set_sound_b =                      // "b" - означает bind
    std::bind(set_alarm,
              steady_clock::now() + 1h, // Ошибка
              _1,
              30s);
// Вторая получше версия
auto set_sound_b =                      // "b" - означает bind
    std::bind(set_alarm,
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s);
// С++11: явно указываем тип std::plus
auto set_sound_b =                      // "b" - означает bind
    std::bind(set_alarm,
              std::bind(std::plus<steady_clock::time_point>(),
                        std::bind(steady_clock::now),
                        hours(1)),
              _1,
              seconds(30));


// При перегрузке set_alarm
// Громкость звука
enum class Volume { Normal, Loud, Loud_plus_plus };
// В момент t издать звук s продолжительностью d и громкостью v
void set_alarm(Time t, Sound s, Duration d, Volume v);


// Версия с лямбда-выражениями
auto set_sound_l =
        [] (Sound s)
        {
            using namespace std::chrono;
            using namespace std::literals;

            set_alarm(steady_clock::now() + 1h, // Ок, вызывает
                      s,                        // set_alarm с тремя
                      30s);                     // аргументами
        };


// Версия с std::bind не компилируется!
auto set_sound_b =              // Ошибка!! Какая из
    std::bind(set_alarm,        // функций set_alarm?
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s);
// Чтобы std::bind компилировался:
using Set_alarm_3_param_type = void (*) (Time t, Sound s, Duration d);
auto set_sound_b =                  // Теперь все в порядке
    std::bind(static_cast<Set_alarm_3_param_type>(set_alarm),
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s);


auto low_val(0);
auto high_val(5);
auto between_l =
        [low_val, high_val]
        (const auto& val)                               // C++14
        { return low_val <= val && val <= high_val; };


using namespace std::placeholders;
auto between_b =
        std::bind(std::logical_and<>(),                 // C++14
                  std::bind(std::less_equal<>(), low_val, _1),
                  std::bind(std::less_equal<>(), _1, high_val));


// Для C++11 явно указываем типы в функциональных объектах
auto between_b =
        std::bind(std::logical_and<bool>(),                 // C++14
                  std::bind(std::less_equal<int>(), low_val, _1),
                  std::bind(std::less_equal<int>(), _1, high_val));
// C++11 для лямбд, вместо auto используем конкретный тип
auto between_l =
        [low_val, high_val]
        (int val)                               // C++11
        { return low_val <= val && val <= high_val; };


enum class Comp_level { low, normal, high}; // Уровень сжатия
// ...
Widget compress(const Widget& w,            // Создание сжатой
                Comp_level lev);            // копии w
// ...
Widget w;
auto compress_rate_b = std::bind(compress, w, _1);


auto compress_rate_l =                  // Захват w по значению;
        [w] (Comp_level lev)            // lev передается по значению
        { return compress(w, lev); };


compress_rate_l(Comp_level::high);      // arg передается по значению
// Для bind-объекта
compress_rate_b(Comp_level::high);      // Как передается arg?


class Poly_widget {
public:
    template<class T>
    void operator() (const T& param) const;
};
Poly_widget pw;
auto bound_pw = std::bind(pw, _1);
// ...
bound_pw(1930);         // Передача int Poly_widget::operator
bound_pw(nullptr);      // Передача nullptr Poly_widget::operator
bound_pw("Rosebud");    // Передача строкового литерала
// То же самое в С++11 нельзя, а в С++14 можно:
auto bound_pw = [pw] (const auto& param)    // C++14
                { pw(param); };


}

//------------------------------------------------------------------------------

#endif // CHAPTER6_HPP

//------------------------------------------------------------------------------
