//
// Created by 31951 on 2020/10/22.
//

#include <Config.h>
#include <libintl.h>
#include <spdlog/spdlog.h>

#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

#define _(str) (gettext(str))

inline std::regex::flag_type set_pattern_flags(const string& pattern, bool icase) {
    using namespace std;
    regex::flag_type type = regex::ECMAScript;
    if(pattern == "basic") type = regex ::basic;
    else if(pattern == "extended")
        type = regex::extended;
    else if(pattern == "awk")
        type = regex::awk;
    else if(pattern == "grep")
        type = regex::grep;
    else if(pattern == "egrep")
        type = regex::egrep;
    else if(pattern == "optimize")
        type = regex::optimize;
    if(icase) {
        type |= regex::icase;
        return type;
    }
    return type;
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    bindtextdomain(Config::PACKAGE, Config::LOCALEDIR);
    textdomain(Config::PACKAGE);
    // cmd parse
    cxxopts::Options options(_("agrep"), _("Search for PATTERNS in each FILE"));
    options.allow_unrecognised_options();
    options.show_positional_help();
    // clang-format off
    options.add_options(_("Pattern selection and interpretation"))
        ("T,regex-pattern", _("basic, extenden, awk, grep, egrep, optimize, ESMAScript(default)"),cxxopts::value<std::string>())
        ("e,regexp", _("use PATTERNS for matching"), cxxopts::value<std::string>())
        ("F, FILE", _(""), cxxopts::value<std::string>())
        ("i,ignore-case",_("ignore case distinctions in patterns and data"))
        ("no-ignore-case", _("do not ignore case " "distinctions (default)"));

    options.add_options(_("Miscellaneous"))
        ("v,invert-match", _("select non-matching lines"))
        //            ("V,version", "display version information and exit")
        ("help", _("display this help text and exit"));

    options.add_options("debug")
        ("log-level", _("set log level"), cxxopts::value<std::string>());
    // clang-format on
    options.parse_positional({ "regexp", "FILE" });
    auto result = options.parse(argc, argv);
    try {
        auto level = result["log-level"].as<std::string>();
        if(level == "debug") spdlog::set_level(spdlog::level::debug);
        else if(level == "info")
            spdlog::set_level(spdlog::level::info);
        else if(level == "warn")
            spdlog::set_level(spdlog::level::warn);
        else if(level == "err")
            spdlog::set_level(spdlog::level::err);
        else if(level == "critical")
            spdlog::set_level(spdlog::level::critical);
        else if(level == "trace")
            spdlog::set_level(spdlog::level::trace);
    } catch(const std::exception& e) {
        // clang-format off
        spdlog::set_level(spdlog::level::off);
        // clang-format on
    }

    if(result["help"].as<bool>()) {
        std::cout << options.help();
        exit(EXIT_SUCCESS);
    }

    string regex_str;
    try {
        regex_str = result["e"].as<std::string>();
        spdlog::debug("the PATTERN is {}", regex_str);
    } catch(const std::domain_error& e) {
        spdlog::debug("no PATTERN input!");
        std::cout << options.help();
        exit(EXIT_SUCCESS);
    }

    std::string pattern_type;
    try {
        pattern_type = result["T"].as<std::string>();
    } catch(const std::exception& e) {
        // do nothing
    }
    // 构造正则表达式
    auto     pattern = std::make_shared<std::regex>(regex_str, set_pattern_flags(pattern_type, result["i"].as<bool>()));

    //    std::shared_ptr<std::istream> fs = std::make_shared<std::istream>(&cin);
    istream* fs      = &cin;
    // 设置文件
    try {
        auto filename = result["FILE"].as<std::string>();
        spdlog::debug("read data from {}", filename);
        if(filename != "-") fs = new ifstream(filename, ios::in);
    } catch(std::exception& e) {
        // do nothing
        spdlog::debug("read data from stdin");
    }
    string tmp;
    smatch math_result;
    // 进行解析

    while(!fs->eof()) {
        getline(*fs, tmp);
        regex_search(tmp, math_result, *pattern);
        if(!(math_result.empty() ^ (result["v"].as<bool>())) && !(tmp.empty()))
            cout << tmp << endl;    // empty 同或 exclude
    }
    // 析构
    if(fs != &cin) delete fs;
    return 0;
}
