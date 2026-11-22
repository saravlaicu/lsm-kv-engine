#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace kstore {

class BloomFilter {
public:
    // expected_elements (n): cate chei estimam ca vom pune
    // bits_per_element (m/n): de ex. 10 biti per cheie (~1% false positive)
    BloomFilter(std::size_t expected_elements, std::size_t bits_per_element = 10);

    // Adauga o cheie in filtru
    void add(std::string_view key);

    // Returneaza false daca cheia SIGUR nu exista, true daca PROBABIL exista
    [[nodiscard]] bool possiblyContains(std::string_view key) const;

    // Utile mai tarziu cand salvam filtrul pe disc in SSTable
    [[nodiscard]] const std::vector<uint8_t>& data() const { return bits_; }
    [[nodiscard]] std::size_t bitCount() const { return num_bits_; }
    [[nodiscard]] std::size_t hashCount() const { return num_hashes_; }

private:
    std::size_t num_bits_;
    std::size_t num_hashes_;
    std::vector<uint8_t> bits_; // Fiecare uint8_t stocheaza 8 biti

    // Doua functii hash de baza pentru tehnica Double Hashing
    [[nodiscard]] static uint64_t hash1(std::string_view key);
    [[nodiscard]] static uint64_t hash2(std::string_view key);
};

} // namespace kstore