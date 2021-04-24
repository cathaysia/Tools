#include <libintl.h>
#include <cxxopts.hpp>
#include <fmt/format.h>
#include <iostream>
#include <Config.h>

#define _(str) (gettext(str))
//#define  _(str) (str)


int main(int argc, char** argv){
    setlocale(LC_ALL, "");
    bindtextdomain(Config::PACKAGE, Config::LOCALEDIR);
    textdomain(Config::PACKAGE);
    cxxopts::Options options(_("bing_wallpaper"), _("Download bing wallpaper and set it"));
    std::cout<<options.help();
    return 0;
}
