#include <iostream>
#include <set>
#include <utility>

#include <fmt/format.h>
#include <cxxopts.hpp>

#include <Config.h>
#include "DnsQuery.h"

constexpr const char* PROGRAM_NAME = "anslookup";

template<typename T>
void query(DnsQuery<T>* query, const std::string& domain) {
    auto query_result = query->Query(domain);
    for(auto& item: query_result) { std::cout << item << std::endl; }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    bindtextdomain(Config::PACKAGE, Config::LOCALEDIR);
    textdomain(Config::PACKAGE);
#ifdef _DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif
    // 解析命令行
    cxxopts::Options options(_(PROGRAM_NAME), _("Query ips of special domain"));
    options.allow_unrecognised_options();
    options.show_positional_help();
    // clang-format off
    options.add_options(_("Query"))
            ("domain", _("The domain you need query"), cxxopts::value<std::string>())
            ("d,Doh",_("Use Doh or not"), cxxopts::value<bool>())
            ("doh-server", _("Custom Doh Server"), cxxopts::value<std::string>())
            ("help", _("display this help text and exit"), cxxopts::value<bool>())
            ;
    // clang-format on
    options.parse_positional({ "domain" });
    auto result = options.parse(argc, argv);

    if(result["help"].as<bool>()) {
        std::cout << options.help();
        exit(EXIT_SUCCESS);
    }
    std::string domain;
    try {
        domain = result["domain"].as<std::string>();
    } catch(std::domain_error& e) {
        std::cout << options.help();
        exit(EXIT_SUCCESS);
    }
    SiteQuery::DnsSet query_result;
//    if(result["Doh"].as<bool>()){
//        spdlog::debug("Use DOH query DNS");
//        query_result = DohQuery().Query(domain);
//    }else{
//        query_result = SiteQuery().Query(domain);
//    }
    query_result = SiteQuery().Query(domain);
    for(auto& item: query_result) { std::cout << item << std::endl; }
    return 0;
}
