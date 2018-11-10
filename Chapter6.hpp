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
//{


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


//}

//------------------------------------------------------------------------------
// Тип для момента времени
using Time = std::chrono::steady_clock::time_point;

//------------------------------------------------------------------------------

enum class Sound { Beep, Siren, Whistle };

//------------------------------------------------------------------------------

using Duration = std::chrono::steady_clock::duration;

//------------------------------------------------------------------------------
// В момент t издать звук s продолжительностью d
void set_alarm([[maybe_unused]] Time t,
               [[maybe_unused]]	Sound s,
               [[maybe_unused]] Duration d) {}

//------------------------------------------------------------------------------
// Громкость звука
enum class Volume { Normal, Loud, Loud_plus_plus };

//------------------------------------------------------------------------------
// В момент t издать звук s продолжительностью d громкостью
void set_alarm([[maybe_unused]] Time t,
               [[maybe_unused]]	Sound s,
               [[maybe_unused]] Duration d,
               [[maybe_unused]] Volume v) {}

//------------------------------------------------------------------------------
// set_sound_l ("l" - означет "лямбда-выражение") - функциональный
// объект, позволяющий указать сигнал будильника, который должен
// звучать через час в течение 30 с
/* c++11
auto set_sound_l = [] (Sound s) {
    using namespace std::chrono;
    set_alarm(steady_clock::now()+hours(1),
              s,
              seconds(30));
};
*/
// c++14
auto set_sound_l = [] (Sound s) {
    using namespace std::chrono;
    using namespace std::literals;
    set_alarm(steady_clock::now() + 1h,
              s,
              30s);
};

//------------------------------------------------------------------------------
// Для bind
using namespace std::chrono;
using namespace std::literals;
using namespace std::placeholders;

//------------------------------------------------------------------------------

using Set_alarm_3_param_type = void (*) (Time t, Sound s, Duration d);

//------------------------------------------------------------------------------

auto set_sound_b =                  // "b" - означает bind
    std::bind(static_cast<Set_alarm_3_param_type>(set_alarm),
              std::bind(std::plus<>(),
                        std::bind(steady_clock::now),
                        1h),
              _1,
              30s);

//------------------------------------------------------------------------------

inline void use_betwen()
{
    auto low_val    = 5;
    auto high_val   = 10;

    [[maybe_unused]] auto between_l =
            [low_val, high_val] (const auto& val)
            { return low_val <= val && val <= high_val; };
    [[maybe_unused]] auto between_b =
            std::bind(std::logical_and<>(),
                      std::bind(std::less_equal<>(), low_val, _1),
                      std::bind(std::less_equal<>(), _1, high_val));
}

//------------------------------------------------------------------------------

enum class Comp_level { low, normal, high }; // Уровень сжатия

//------------------------------------------------------------------------------

class Widget {
public:
    Widget() {}
};

//------------------------------------------------------------------------------

inline Widget compress(const Widget& w,
                      [[maybe_unused]] Comp_level lev)
{
    return w;
}

//------------------------------------------------------------------------------

inline void use_compress()
{
    Widget w;
    [[maybe_unused]] auto comress_rate_b
            = std::bind(compress, w, _1);

    [[maybe_unused]] auto compress_rate_l =
            [w] (Comp_level lev)
            { return compress(w, lev); };

}

//------------------------------------------------------------------------------

#endif // CHAPTER6_HPP

//------------------------------------------------------------------------------
