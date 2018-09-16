//------------------------------------------------------------------------------

#ifndef LAMBDAS_HPP
#define LAMBDAS_HPP

//------------------------------------------------------------------------------

#include "Facilities.hpp"

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

#endif // LAMBDAS_HPP

//------------------------------------------------------------------------------
