#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <vector>

// Function declarations
std::vector<std::string> read_lines_from_file(const std::string& filename);
std::string extract_domain(const std::string& url);
int create_iptables_rules(const std::vector<std::string>& blocked_list, const std::vector<std::string>& blocked_ips);

#endif // FILTER_H
