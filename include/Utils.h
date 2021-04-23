#ifndef TOOLS_UTILS_H
#define TOOLS_UTILS_H

#if defined(__GNUC__)
    #include <cxxabi.h>
#elif defined(_MSC_VER)
    #include <typeinfo>
#endif

namespace Utils {

/**
 * 此函数主要是为了在程序中使用。
 * MSVC 编译时无法解析 _ZNK4Json5ValueixEPKc 这种名字
 *
 */
template<typename T>
const char* getFullName() {
#if defined(__GNUC__)
    return abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
#elif defined(_MSC_VER)
    return typeid(T).name();
#endif
}

const char* getFullName(char* type_name) { // 这里类型必须是 char* 否则模板会匹配到另一个函数
    // 这里需要将 type_name
#if defined(__GNUC__)
    return abi::__cxa_demangle(type_name, nullptr, nullptr, nullptr);
#elif defined(_MSC_VER)
    return type_name;
#endif
}

template<typename T>
const char* getFullName(const T& any) {
#if defined(__GNUC__)
    return abi::__cxa_demangle(typeid(any).name(), nullptr, nullptr, nullptr);
#elif defined(_MSC_VER)
    return typeid(any).name();
#endif
}

}    // namespace Utils

#endif    // TOOLS_UTILS_H
