add_library(
        DnsQuery STATIC
        DnsQuery.cpp
        ${CPP_BASE64_INCLUDE_DIRS}
        DnsQuery.h
)


# 设置依赖
find_package(fmt CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")
find_package(Poco COMPONENTS Foundation Net REQUIRED)
if(NOT MSVC)
    message(FATAL "only msvc required")
endif()
set(_targets DnsQuery)
foreach (_target ${_targets})
    target_include_directories(${_target} PRIVATE ${CPPCODEC_INCLUDE_DIRS})
    target_link_libraries(${_target} PRIVATE cxxopts::cxxopts fmt::fmt cpr CURL::libcurl
            Poco::Foundation Poco::Net)
    if (MSVC)
        target_compile_options(${_target} PRIVATE /source-charset:utf-8 /MDd)
        if (${VCPKG_TARGET_TRIPLET})
            string(FIND ${VCPKG_TARGET_TRIPLET} "static" vcpkg_static_build)
            if (NOT ${vcpkg_static_build} STREQUAL -1)
                MESSAGE(STATUS static build)
                target_compile_options(bing_wallpaper PRIVATE /MT)
            endif ()
        endif ()
    endif ()
endforeach ()
