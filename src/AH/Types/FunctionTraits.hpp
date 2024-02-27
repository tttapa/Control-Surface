#pragma once

#include <AH/STL/cstddef>
#include <AH/STL/tuple>

#include <AH/Settings/NamespaceSettings.hpp>

BEGIN_AH_NAMESPACE

template <class T>
struct function_traits;

template <class Return, class... Args>
struct function_traits<Return(Args...)> {
    static constexpr size_t number_arguments = sizeof...(Args);

    using return_t = Return;
    template <size_t Index>
    struct argument {
        using type =
            typename std::tuple_element<Index, std::tuple<Args...>>::type;
    };

    template <size_t Index>
    using argument_t = typename argument<Index>::type;
};

END_AH_NAMESPACE
