//
// Created by Drago on 2021/5/8.
//

#ifndef ATOOLS_DNSQUERY_H
#define ATOOLS_DNSQUERY_H

#include <map>
#include <regex>
#include <set>
#include <string>
#include <sstream>
#include <utility>

#include <curl/curl.h>
#include <cpr/cpr.h>
#include <Poco/Base64Encoder.h>
#include <Poco/Base64Decoder.h>
#include <Poco/Net/DNS.h>

#include <Config.h>

extern "C"{
#include "base64.h"
};

/**
 * 接口类：DnsQuery
 *
 */
template<typename T>
class DnsQuery {
public:
    typedef std::string                   DnsInfo;
    typedef std::set<DnsInfo>             DnsSet;
    /**
     * DnsMap 的键值代表域名，值域代表其 IP 地址列表
     */
    typedef std::map<std::string, DnsSet> DnsMap;
    DnsSet                                Query(const std::string& domain) {
        return std::move(this->getInstance().Query(domain));
    };

protected:
    // 禁止实例化此类
    DnsQuery() = default;
    // 静态多态，避免运行时开销
    T& getInstance() {
        return static_cast<T&>(*this);
    }
    void insert(const std::string& domain, const DnsInfo& dnsInfo) {
        dns_map_.insert_or_assign(domain, dnsInfo);
    }
    auto find(const std::string& domain) {
        return dns_map_.find(domain);
    }

private:
    // 是否使用缓存
    bool   use_cache_ = true;
    DnsMap dns_map_;
};

class SiteQuery : public DnsQuery<SiteQuery> {
public:
    SiteQuery() = default;
    DnsSet Query(const std::string& domain) {
        auto r = cpr::Get(cpr::Url(fmt::format("https://www.ipaddress.com/search/{}", domain)),
                          cpr::Header { { "User-Agent",
                                          "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                                          "Gecko) Chrome/87.0.4280.88 Safari/537.36 Edg/87.0.664.66" } });
        if(r.status_code != 200) {
            fmt::print(_("Query fail, the response code is {}, the url requested is {}"), r.status_code, r.url.str());
            exit(EXIT_FAILURE);
        }
        spdlog::debug("Query Url: {}", r.url.str());
        // 直接使用正则表达式匹配
        DnsSet      dns_set;
        std::smatch result;
        // 匹配 ipv4
        std::regex  pattern_ip(R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})");
        std::regex_search(r.text, result, pattern_ip);
        if(!result.empty()) {
            for(auto& i: result) {
                spdlog::debug("ipv4: {}", i.str());
                dns_set.insert(i.str());
            }
        }
        /**
         * 匹配 ipv6 的正则表达式： (([0-9a-fA-F]{1,4}:)|(::))(([0-9a-fA-F]{0,4}[:.])){2,16}[0-9a-fA-F]{0,4}
         * 只会匹配前六种，可以去 https://ihateregex.io/expr/ipv6/ 测试
         * 2001:0db8:85a3:0000:0000:8a2e:0370:7334
         * FE80:0000:0000:0000:0202:B3FF:FE1E:8329
         * fe80:0000:0000:0000:0204:61ff:fe9d:f156 // full form of IPv6
         * fe80:0:0:0:204:61ff:fe9d:f156 // drop leading zeroes
         * fe80::204:61ff:fe9d:f156 // collapse multiple zeroes to :: in the IPv6 address
         * fe80::204:61ff:254.157.241.86 // dotted quad at the end, multiple zeroes collapsed
         * test:test:test:test:test:test:test:test
         * a
         * fb:
         * fb:a
         * 23:59:59
         * 39.156.69.79
         * 192.168.1.1
         * 59
         * 9:
         * 9:59
         */
        pattern_ip = "(([0-9a-fA-F]{1,4}:)|(::))(([0-9a-fA-F]{0,4}[:.])){2,16}[0-9a-fA-F]{0,4}";
        std::regex_search(r.text, result, pattern_ip);
        if(!result.empty()) {
            for(auto& i: result) {
                spdlog::debug("ipv6: {}", i.str());
                dns_set.insert(i.str());
            }
        }
        return std::move(dns_set);
    }
};

class DohQuery : public DnsQuery<DohQuery> {
    std::string doh_server = "https://dns.alidns.com/dns-query";

public:
    DohQuery() = default;
    DnsSet Query(const std::string& domain) {
        //
        std::stringstream os;
        Poco::Base64Encoder encoder(os, Poco::Base64EncodingOptions::BASE64_NO_PADDING);
        encoder<<domain;
        encoder.close();
        std::string encoding_result;
        os>>encoding_result;
//        size_t pos = encoding_result.find('=');
//        while( pos != -1){
//            encoding_result.replace(pos, sizeof '=', "");
//            pos = encoding_result.find('=');
//        }
//        char* data_ptr[10];
//        size_t data_len;
//        auto encoding_result = Curl_base64url_encode(domain.c_str(), domain.length(), data_ptr, &data_len);
//        if(encoding_result != CURLE_OK || !(*data_ptr)){
//            spdlog::critical("base64url encoding transform failed");
//        }
//        if(!len){
//            spdlog::debug("trans fail {}", len);
//            exit(-1);
//        }
        spdlog::debug("The base64 encoding srtring is {}", encoding_result);
        auto r = cpr::Get(cpr::Url(fmt::format(
                              "{}?dns={}",
                              doh_server,
                              encoding_result)),
                          cpr::Header { { "Content-type", "application/dns-message" } });
        if(r.status_code != 200) {
            fmt::print(_("Query failed, the response code is {}, the url requested is {}"), r.status_code, r.url.str());
            exit(EXIT_FAILURE);
        }
        if(r.header["Content-type"] != "application/dns-message") {
            fmt::print(_("Query failed, message is not a dns message, it is: {}"), r.header["Content-type"]);
            exit(EXIT_FAILURE);
        }
        printf("%s", r.text.c_str());

        // 下面解析 dns 格式
    }
};

#endif    // ATOOLS_DNSQUERY_H
