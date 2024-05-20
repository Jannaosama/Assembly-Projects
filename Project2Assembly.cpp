#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <cmath>
#include <string>
#include <sstream>

const int MAIN_MEMORY_ACCESS = 100;

// Convert hexadecimal string to a binary string
std::string hex_to_binary(const std::string& hex_string) {
    unsigned int hex_value;
    std::stringstream ss;
    ss << std::hex << hex_string;
    ss >> hex_value;
    std::bitset<32> binary(hex_value); // 32 bits for example; adjust as needed
    return binary.to_string();
}

class Address {
public:
    std::string hex, tag, index, offset;
    Address(const std::string& original, const std::string& binary_str, int index_size, int offset_size) {
        int tag_size = binary_str.length() - index_size - offset_size;
        hex = original;
        tag = binary_str.substr(0, tag_size);
        index = binary_str.substr(tag_size, index_size);
        offset = binary_str.substr(tag_size + index_size);
    }
};

class CacheLine {
public:
    bool valid;
    std::string tag, data, index;
    CacheLine() : valid(false), tag(""), data(""), index("") {}
};

std::vector<Address> create_addresses(int index_size, int offset_size, const std::string& file_path) {
    std::vector<Address> addresses;
    std::ifstream file(file_path);
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string cell;
        while (getline(iss, cell, ',')) {
            std::string binary_address = hex_to_binary(cell.substr(2));
            addresses.emplace_back(cell, binary_address, index_size, offset_size);
        }
    }
    return addresses;
}

void process_cache_accesses(const std::vector<Address>& addresses, std::vector<CacheLine>& cache, int hit_penalty, int miss_penalty) {
    int hits = 0, misses = 0, accesses = 0;
    for (const auto& address : addresses) {
        int n = std::stoi(address.index, nullptr, 2);
        if (!cache[n].valid || cache[n].tag != address.tag) {
            cache[n].valid = true;
            cache[n].tag = address.tag;
            cache[n].data = address.offset;
            cache[n].index = address.index;
            misses++;
        }
        else {
            hits++;
        }
        accesses++;
        double hit_ratio = static_cast<double>(hits) / accesses;
        double miss_ratio = 1 - hit_ratio;
        double amat = hit_ratio * hit_penalty + miss_ratio * miss_penalty;

        std::cout << "Accessing " << address.hex << std::endl;
        std::cout << "Index | Tag      | Data" << std::endl;
        for (const auto& line : cache) {
            if (line.valid) {
                std::cout << line.index << " | " << line.tag << " | " << line.data << std::endl;
            }
        }
        std::cout << "Total Number of accesses: " << accesses << std::endl;
        std::cout << "Hits ratio: " << hit_ratio * 100 << "%" << std::endl;
        std::cout << "Misses ratio: " << miss_ratio * 100 << "%" << std::endl;
        std::cout << "AMAT: " << amat << " cycles" << std::endl;
        std::cout << "------------------------------" << std::endl;
    }
}

int main() {
    int S = 32; // Cache size in bytes
    int L = 4; // Cache line size in bytes
    int cache_access = 5; // Cache access time in cycles
    std::string instr_file_path = "Instr_Addresses.txt";
    std::string data_file_path = "Data_Addresses.txt";

    int disp_size = std::log2(L);
    int C = S / L;
    int indx_size = std::log2(C);

    int hit_penalty = cache_access;
    int miss_penalty = cache_access + MAIN_MEMORY_ACCESS;

    std::vector<Address> instr_addresses = create_addresses(indx_size, disp_size, instr_file_path);
    std::vector<Address> data_addresses = create_addresses(indx_size, disp_size, data_file_path);

    std::vector<CacheLine> instr_cache(C);
    std::vector<CacheLine> data_cache(C);

    process_cache_accesses(instr_addresses, instr_cache, hit_penalty, miss_penalty);
    process_cache_accesses(data_addresses, data_cache, hit_penalty, miss_penalty);

}
