#pragma once
#pragma GCC diagnostic ignored "-Wreturn-type"
#include <string_view>
#include <tuple>

template <char... args>
struct static_string {
    static constexpr const char str[] = {args...};
    operator const char *() const { return static_string::str; }
};
template <typename Object>
constexpr auto get_type_name() {
    constexpr std::string_view fully_name = __PRETTY_FUNCTION__;
    constexpr std::size_t begin = [&]() {
        for (std::size_t i = 0; i < fully_name.size(); i++)
            if (fully_name[i] == '=')
                return i + 2;
    }();
    constexpr std::size_t end = [&]() {
        for (std::size_t i = 0; i < fully_name.size(); i++)
            if (fully_name[i] == ']')
                return i;
    }();
    constexpr auto type_name_view = fully_name.substr (begin, end - begin);
    constexpr auto indices = std::make_index_sequence<type_name_view.size()>();
    constexpr auto type_name = [&]<std::size_t... indices> (
        std::integer_sequence<std::size_t, indices...>) {
        constexpr auto str = static_string<type_name_view[indices]..., '\0'>();
        return str;
    }
    (indices);
    return type_name;
}
#pragma GCC diagnostic warning "-Wreturn-type"