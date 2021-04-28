/***
 * a program to set bing wallpaper
 *
 */

// run behind
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#include <Config.h>
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <libintl.h>
#ifdef fprintf
    #undef fprintf
    #include <spdlog/spdlog.h>
#endif
#include <windows.h>

#include <chrono>
#include <cxxopts.hpp>
#include <filesystem>
#include <iomanip>
#include <iostream>

#define _(str) (gettext(str))

constexpr char BASE_URL[] = "https://cn.bing.com";
constexpr char API[]      = "https://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=zh-CN";

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

/***
 * Get a filesystem path wallpaper showld be store
 */
string getFileName() {
    auto aTimet = system_clock::to_time_t(system_clock::now());

    auto atime  = new tm();
    localtime_s(atime, &aTimet);

    ostringstream os;
    os << put_time(atime, R"(%Y-%m-%d)");
    string filename = "D:/Pictures/bingWallpaper/" + os.rdbuf()->str() + ".png";
    return std::move(filename);
}

string getUrl() {
    cpr::Response r = cpr::Get(cpr::Url { API });
    if(r.status_code != 200) { spdlog::critical(_("get wallpaper url from {0} failed. terminate"), r.url.c_str()); }
    // 对 json 进行解析
    spdlog::debug(_("the JSON got from server is {0}"), r.text);
    auto   left  = r.text.find("url") + 6;
    auto   right = r.text.find('\"', left);
    string url   = BASE_URL;
    for_each(r.text.begin() + left, r.text.begin() + right, [&](auto el) {
        url += el;
    });
    spdlog::debug("the URL of wallpaper is {0}", url);
    return std::move(url);
}

int main(int argc, char** argv) {
    // i18n
    setlocale(LC_ALL, "");
    bindtextdomain(Config::PACKAGE, Config::LOCALEDIR);
    textdomain(Config::PACKAGE);
    // commandline parse
    cxxopts::Options options(_("bing_wallpaper"), _("Download bing wallpaper and set it"));
    //
    spdlog::set_level(spdlog::level::debug);
    auto filename = getFileName();
    if(!filesystem::exists(filename)) {
        spdlog::info(_("wallpaper download ready to: {0}"), filename);
        // 下载图片
        cpr::Response r = cpr::Get(cpr::Url { getUrl() });
        if(r.status_code != 200) { spdlog::critical(_("download wallpaper by {0} failed, terminate"), r.url.c_str()); }
        ofstream ofs(filename, ios::binary | ios::trunc);
        ofs.write(r.text.c_str(), r.text.length());
        ofs.close();
    } else {
        spdlog::info(_("wallpaper fill already exists, skip. {0}"), filename);
        return 0;
    }
    // wallpaper must be downloaded
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)filename.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
    return 0;
}
