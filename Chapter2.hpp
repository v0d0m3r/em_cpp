//------------------------------------------------------------------------------

// Глава 2: Объявление auto

//------------------------------------------------------------------------------

#ifndef CHAPTER2_HPP
#define CHAPTER2_HPP

//------------------------------------------------------------------------------

#include <Facilities.hpp>

//------------------------------------------------------------------------------
// 2.1 Предрочитайте auto явному объявлению типа

template<class It>      // Некий алгоритм, работающий с
void dwim(It b, It e)   // элементами из диапазона от b до e
{
    while (b != e) {
        typename std::iterator_traits<It>::value_type   // Ужасно многословно
                curr_value = *b;
        // ...
    }
}

template<class It>      // Некий алгоритм, работающий с
void dwim(It b, It e)   // элементами из диапазона от b до e
{
    while (b != e) {
        auto curr_value = *b;   // Гораздо лучше
        // ...
    }
}

auto def_up_less =                              // Функция сравнения
        [] (const std::unique_ptr<Widget>& p1,  // объектов Widget, на
            const std::unique_ptr<Widget>& p2)  // которые указывают
            { return *p1 < *p2; };              // std::unique_ptr

auto def_up_less =                  // Функция сравнения в C++14
        [] (const auto& p1,         // для значений, на которые
            const auto& p2)         // указывает что угодно
            { return *p1 < *p2; };  // указателеобразное
// Без использования auto
std::function<bool(const std::unique_ptr<Widget>&,
                   const std::unique_ptr<Widget>&)>
def_up_less =                                   // Функция сравнения
        [] (const std::unique_ptr<Widget>& p1,  // объектов Widget, на
            const std::unique_ptr<Widget>& p2)  // которые указывают
            { return *p1 < *p2; };              // std::unique_ptr

std::unordered_map<std::string, int> m;
// ...

for (const std::pair<std::string, int>& p : m)
{
    // Что-то делаем с p
    // Плохое объявление p
    // так как значение ключа в std::unordered_map имеет тип const
    // следовательно для каждого элемента будет создаваться временный
    // объект, и будет связываться с p, что несет в себе большую уязвимость
}
// Лучше использовать auto
for (const auto& p : m)
{
    // Как и ранее
}

//------------------------------------------------------------------------------
// 2.2 Если auto выводит нежелательный тип,
// используйте явно типизированный инициализатор

void process_widget(Widget, bool);
std::vector<bool> features(const Widget& w);

Widget w;
// ...
bool hight_priority = features(w)[5];   // Имеет ли w высокий
                                        // приоритет?
// ...
process_widget(w, hight_priority);      // Обработка w в соответствии
                                        // с приоритетом
// Использование auto вместо bool
auto hight_priority = features(w)[5];   // Имеет ли w высокий
                                        // приоритет?
process_widget(w, hight_priority);      // Неопределенное поведение!
                                        // hight_priority содержит
                                        // висячий указатель
// Ещё пример с неудачным использованием auto
class Matrix;

Matrix sum = m1 + m2 + m3;  // Все ок, неявное преобразование
                            // прокси-класса в Matrix
auto sum = m1 + m2 + m3;    // Неудача, вывод типа висячий объект прокси-класса

// Следовательно, нужно избегать объявления:
// auto some_var = выражение с типом "невидимого" прокси класса;

// Решение проблемы некорректного вывода типа:
// Идиома явной типизации инициализатора

auto hight_priority = static_cast<bool>(features(w)[5]);
auto sum = static_cast<Matrix>(m1 + m2 + m3);

double calc_epsilon();     // Возвращает отклонение

float ep = calc_epsilon(); // Неявное преобразование
                           // double -> float
// Более явное преобразование, нужно обратить внимание!
auto ep = static_cast<float>(calc_epsilon());

//------------------------------------------------------------------------------

#endif // CHAPTER2_HPP

//------------------------------------------------------------------------------
