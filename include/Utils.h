#ifndef TOOLS_UTILS_H
#define TOOLS_UTILS_H

#if defined(__GNUC__)
    #include <cxxabi.h>
#elif defined(_MSC_VER)
    #include <typeinfo>
#endif

namespace Utils {
#if defined(__GNUC__)

template<typename T> constexpr const char* getFullName(const T& any) {
    return abi::__cxa_demangle(typeid(any).name(), nullptr, nullptr, nullptr);
}

template<typename T> constexpr const char* getFullName() {
    return abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
}
#elif defined(_MSC_VER)
    #include <typeinfo>
template<typename T> const char* getFullName(const T& any) {
    return typeid(any).name();
}

template<typename T> const char* getFullName() {
    return typeid(T).name();
}

#endif
}    // namespace Utils

#endif    // TOOLS_UTILS_H
