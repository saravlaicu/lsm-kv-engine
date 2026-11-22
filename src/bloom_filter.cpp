#include "bloom_filter.h"
#include <algorithm>
#include <functional>

namespace kstore {
    BloomFilter::BloomFilter(std::size_t expected_elements, std::size_t bits_per_element) {
       // num_bits_=expected_elements*bits_per_element;
        num_bits_=std::max<std::size_t>(8,expected_elements*bits_per_element); // in caz ca am 0 expected elements
        bits_.resize((num_bits_+7)/8,0);//aloc vectorul bits cu (num_bits_+7)/8 elemente
       // num_hashes_=bits_per_element*0.693;
        num_hashes_=std::max<std::size_t>(1,static_cast<std::size_t>(bits_per_element*0.693));

    }

    uint64_t BloomFilter::hash1(std::string_view key) {
        return std::hash<std::string_view>{}(key); //calculeaza hash fara sa copieze string ul
    }

    //algoritmul FNV-1a pe 64 biti = algoritm de hashing
    uint64_t BloomFilter::hash2(std::string_view key) {
        uint64_t hash= 14695981039346656037ULL; //ULL= unsinged long long
        for (unsigned char c : key) {
            hash ^= c;
            hash *= 1099511628211ULL;
        }
        return hash;
    }


    void BloomFilter::add(std::string_view key) {
        uint64_t h1= hash1(key);
        uint64_t h2= hash2(key);

        for (std::size_t i=0; i< num_hashes_;i++) {
            uint64_t combined_hash= h1+(i*h2);
            uint64_t bit_idx= combined_hash % num_bits_;

            //byte_ =blocul
            uint64_t byte_idx= bit_idx / 8; //etajul
            uint64_t bit_offset = bit_idx % 8; // camera
                                // shiftez la stg (al doilea:)
            bits_[byte_idx] |= (1U << bit_offset); // 0000 0001
            //                                       0000 1000
            //                                  --------------------
            //                                       0000 1001

        }
    }

    bool BloomFilter::possiblyContains(std::string_view key) const {
        uint64_t h1= hash1(key);
        uint64_t h2= hash2(key);

        for (std::size_t i=0; i< num_hashes_;i++) {
            uint64_t combined_hash= h1+(i*h2);
            uint64_t bit_idx= combined_hash % num_bits_;


            uint64_t byte_idx= bit_idx / 8;
            uint64_t bit_offset = bit_idx % 8;
            if ((bits_[byte_idx] & (1U << bit_offset)) == 0) {
                return false;
            }
        }
        return true;
    }
}
