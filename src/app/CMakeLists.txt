find_package(fmt CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)


add_executable(grep grep.cpp)
add_executable(head head.cpp)
add_executable(teal teal.cpp)
add_executable(xsel xsel.cpp)
add_executable(xxd xxd.cpp)
add_executable(reboot reboot.cpp)
add_executable(cppname cppname.cpp)
add_executable(bing_wallpaper bing_wallpaper.cpp)

include(FindGetText)
target_include_directories(bing_wallpaper PRIVATE ${GetText_INCLUDE})
target_link_libraries(bing_wallpaper PRIVATE cxxopts::cxxopts fmt::fmt ${GetText_LIBRARIES})
