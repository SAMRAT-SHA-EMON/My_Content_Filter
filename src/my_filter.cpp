#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>
#include <unistd.h> // For getuid
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring> // For strerror
#include <netdb.h> // For getaddrinfo
#include <arpa/inet.h> // For inet_ntoa

using namespace std;

// Function to read lines from a file and store them in a set
unordered_set<string> read_lines_from_file(const string &filename) {
    unordered_set<string> lines;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return lines;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) { // Avoid inserting empty lines
            lines.insert(line);
        }
    }

    file.close();
    return lines;
}

// Function to resolve a domain name to an IP address using getaddrinfo
vector<string> resolve_domain_to_ip(const string &domain) {
    vector<string> ip_addresses;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(domain.c_str(), NULL, &hints, &res) != 0) {
        cerr << "Error resolving domain: " << domain << endl;
        return ip_addresses;
    }

    for (struct addrinfo *p = res; p != nullptr; p = p->ai_next) {
        char ipstr[INET_ADDRSTRLEN];
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof ipstr);
        ip_addresses.push_back(string(ipstr));
    }

    freeaddrinfo(res); // Free the linked list
    return ip_addresses;
}

// Function to create iptables rules for blocking
int create_iptables_rules(const unordered_set<string> &blocked_list, const unordered_set<string> &blocked_ips) {
    vector<string> commands;

    // Generate commands for website blocking (by domain)
    for (const string &entry : blocked_list) {
        vector<string> ip_addresses = resolve_domain_to_ip(entry);

        // Skip if no IP address is found
        if (ip_addresses.empty()) {
            cerr << "Warning: Could not resolve IP for domain " << entry << endl;
            continue;
        }

        // Command to block the resolved IP addresses
        for (const string &ip_address : ip_addresses) {
            string cmd = "iptables -A INPUT -s " + ip_address + " -j DROP";
            commands.push_back(cmd);
        }
    }

    // Generate commands for IP blocking
    for (const string &ip_address : blocked_ips) {
        string cmd = "iptables -A INPUT -s " + ip_address + " -j DROP";
        commands.push_back(cmd);
    }

    // Execute the commands
    for (const string &cmd : commands) {
        int result = system(cmd.c_str());
        if (result != 0) {
            cerr << "Error executing iptables command: " << cmd << " - " << strerror(errno) << endl;
            return 1;  // Indicate an error
        }
    }

    return 0;  // Indicate success
}

int main() {
    // Check if the program is run as root
    if (getuid() != 0) {
        cerr << "This program must be run as root." << endl;
        return 1;
    }

    string blocked_sites_file = "/home/samrat/Desktop/my_content_filter/conf/blocked_sites.txt"; // Adjust path as needed
    string blocked_payment_file = "/home/samrat/Desktop/my_content_filter/conf/blocked_payment.txt"; // Adjust path
    string blocked_ips_file = "/home/samrat/Desktop/my_content_filter/conf/blocked_ips.txt"; // Adjust path as needed

    // Read blocked lists into sets
    unordered_set<string> blocked_sites = read_lines_from_file(blocked_sites_file);
    unordered_set<string> blocked_payments = read_lines_from_file(blocked_payment_file);
    unordered_set<string> blocked_ips = read_lines_from_file(blocked_ips_file);

    // Combine blocked sites and payment sites into one list
    blocked_sites.insert(blocked_payments.begin(), blocked_payments.end());

    // Create iptables rules
    if (create_iptables_rules(blocked_sites, blocked_ips) != 0) {
        return 1; // Indicate failure
    }

    cout << "Blocking rules have been successfully applied." << endl;
    return 0; // Indicate success
}

