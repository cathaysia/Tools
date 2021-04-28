
# 输出变量为 GetText_LIBRARIES 和 GetText_INCLUDE

set(HINT_PATH ${Z_VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET})

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(HINT_PATH ${Z_VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET}/debug)
endif()

set(libraries
        gettextlib
        gettextpo
        gettextsrc
        intl
        iconv)

foreach(lib ${libraries})
    find_library(
            tmp-${lib}
            NAMES ${lib}
            HINTS ${HINT_PATH}/lib
    )
    list(APPEND GetText_LIBRARIES ${tmp-${lib}})
    unset(tmp-${lib})
endforeach()

find_path(
        GetText_INCLUDE
        NAMES libintl.h
        HINTS ${HINT_PATH}/include
)
