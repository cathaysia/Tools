find_package(fmt CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Intl REQUIRED)
#include(UseGettext)

# i18n
#configure_gettext(
#        DOMAIN ${PACKAGE}
#        TARGET_NAME translation
#        SOURCES ${cpp_src_list}
#        POTFILE_DESTINATION ${PROJECT_BINARY_DIR}/bin
#        LANGUAGES zh_CN
#        INSTALL_DESTINATION ${LOCALEDIR}
#        XGETTEXT_ARGS -k_ --from-code=utf-8
#)

# list */app/*.cpp and create target
file(GLOB_RECURSE cpp_src_list ${CMAKE_CURRENT_SOURCE_DIR}/*cpp)
foreach (file ${cpp_src_list})
    string(REGEX MATCH "app/.*" file ${file})
    string(REPLACE "app/" "" file ${file})
    string(REPLACE ".cpp" "" target_name ${file})
    MESSAGE(STATUS "creating target ${target_name}")
    add_executable(${target_name} ${file})
    # project properties setting
    target_include_directories(${target_name} PRIVATE ${Intl_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE
            cxxopts::cxxopts
            cpr
            fmt::fmt
            spdlog::spdlog_header_only
            ${Intl_LIBRARIES}
            DnsQuery)
    if (MSVC)
        target_compile_options(${target_name} PRIVATE /source-charset:utf-8 /MDd)
        # static build or not
        if (${VCPKG_TARGET_TRIPLET})
            string(FIND ${VCPKG_TARGET_TRIPLET} "static" vcpkg_static_build)
            if (NOT ${vcpkg_static_build} STREQUAL -1)
                MESSAGE(STATUS static build)
                target_compile_options(bing_wallpaper PRIVATE /MT)
            endif ()
        endif ()
    endif ()
endforeach ()

