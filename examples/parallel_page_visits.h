//
// Created by usman on 2/28/22.
//

#ifndef CONCURRENCY_PARALLEL_PAGE_VISITS_H
#define CONCURRENCY_PARALLEL_PAGE_VISITS_H
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>
#include <execution>
#include <random>
#include <thread>
#include <boost/algorithm/string.hpp>

struct log_info {
    std::string page;
    std::time_t visit_time;
    std::string browser;
    // any other fields
    log_info(std::string & page_, std::time_t & visit_time_, std::string & browser_) :
        page(page_), visit_time(visit_time_), browser(browser_)
    {
    }
};

using visit_map_type= std::unordered_map<std::string, unsigned long long>;

class ParallelPageVisits {
public:
    std::vector<std::string> create_log_lines() {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, 2); // define the range
        std::vector<std::string> vec;
        std::string browsers [] = {"chrome", "safari", "firefox"};
        std::string sites [] = {"amazon", "fb", "google"};
        char buffer[100];
        for (int i =0; i < 1000000; i++) {
            std::string str;
            str += sites[distr(gen)] + ",";
            std::time_t now = std::time(NULL);
            std::tm * ptm = std::localtime(&now);
            std::strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", ptm);
            str += buffer;
            str += + ",";
            str += browsers[distr(gen)];
            vec.push_back(str);
            //std::this_thread::sleep_for (std::chrono::milliseconds (5));
        }
        return vec;
    }


    static log_info parse_log_line(std::string const &line) {
        std::vector<std::string> results;
        boost::algorithm::split(results, line, boost::is_any_of(","));
        std::string page = results[0];
        struct std::tm tm;
        std::istringstream ss(results[1]); //"16:35:12"
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // or just %T in this case
        std::time_t time = mktime(&tm);
        std::string browser = results[2];
        log_info log(page, time, browser);
        return log;
    }

    visit_map_type count_visits_per_page(std::vector<std::string> const &log_lines) {

        //a reduction op to reduce the data set
        struct combine_visits {
            visit_map_type
            operator()(visit_map_type lhs, visit_map_type rhs) const {
                if(lhs.size() < rhs.size())
                    std::swap(lhs, rhs);
                for(auto const &entry : rhs) {
                    lhs[entry.first]+= entry.second;
                }
                return lhs;
            }
            //increment log in map
            visit_map_type operator()(log_info log, visit_map_type map) const {
                ++map[log.page];
                return map;
            }
            //increment log in map
            visit_map_type operator()(visit_map_type map, log_info log) const {
                ++map[log.page];
                return map;
            }
            //merge the log lines into the reduced map
            visit_map_type operator()  (log_info log1, log_info log2) const {
                visit_map_type map;
                ++map[log1.page];
                ++map[log2.page];
                return map;
            }
        };

        return std::transform_reduce(
                std::execution::par, log_lines.begin(), log_lines.end(),
                visit_map_type(), combine_visits(), &ParallelPageVisits::parse_log_line);
    }
};
#endif //CONCURRENCY_PARALLEL_PAGE_VISITS_H
