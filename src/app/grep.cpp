//
// Created by 31951 on 2020/10/22.
//
/**
 *  现在 grep 只考虑从 stdout 里读取数据
 *
 *  目前只实现 grep -i, grep -v, grep + regex
 *
 */
#include <Config.h>
#include <libintl.h>
#include <spdlog/spdlog.h>

#include <cxxopts.hpp>
#include <iostream>
#include <regex>

using namespace std;

#define _(str) (gettext(str))

std::regex::flag_type set_pattern_flags(const string& pattern, bool icase) {
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
    cxxopts::Options options(_("grep"), _("Search for PATTERNS in each FILE"));
    options.allow_unrecognised_options();
    options.show_positional_help();
    options.add_options(_("Pattern selection and interpretation"))
        ("T,regex-pattern", _("basic, extenden, awk, grep, egrep, optimize, ESMAScript(default)"),cxxopts::value<std::string>())
        ("e,regexp", _("use PATTERNS for matching"), cxxopts::value<std::string>())
        ("i,ignore-case",_("ignore case distinctions in patterns and data"))
        ("no-ignore-case", _("do not ignore case " "distinctions (default)"));

    options.add_options(_("Miscellaneous"))
        ("v,invert-match", _("select non-matching lines"))
        //            ("V,version", "display version information and exit")
        ("help", _("display this help text and exit"));

    options.add_options("debug")("log-level", _("set log level"), cxxopts::value<std::string>());
    options.parse_positional("regexp");
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
    } catch(const std::exception& e) { spdlog::set_level(spdlog::level::off); }

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

    bool        bit_exclude = result["v"].as<bool>();    // -v
    //    bool bit_sensitive = result["i"].as<bool>();    // -i 默认为大小写敏感

    std::string pattern_type;
    try {
        pattern_type = result["T"].as<std::string>();
    } catch(const std::exception& e) {
        // do nothing
    }
    // 构造正则表达式
    auto     pattern = std::make_shared<std::regex>(regex_str, set_pattern_flags(pattern_type, result["i"].as<bool>()));

    istream* fs      = &cin;
    string   tmp;
    smatch   math_result;
    // 进行解析
    while(!fs->eof()) {
        getline(*fs, tmp);
        regex_search(tmp, math_result, *pattern);
        if(!(math_result.empty() ^ bit_exclude)) cout << tmp << endl;    // empty 同或 exclude
    }
    return 0;
}
