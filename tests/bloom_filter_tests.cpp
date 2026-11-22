#include <gtest/gtest.h>
#include <string>
#include "bloom_filter.h"

TEST(BloomFilterTest, AddedElementsAreFound) {
    kstore::BloomFilter bf(100, 10);

    bf.add("user_1");
    bf.add("student_ubb");
    bf.add("kaist_2027");

    EXPECT_TRUE(bf.possiblyContains("user_1"));
    EXPECT_TRUE(bf.possiblyContains("student_ubb"));
    EXPECT_TRUE(bf.possiblyContains("kaist_2027"));
}

TEST(BloomFilterTest, FalsePositiveRateIsLow) {
    const std::size_t num_elements = 10000;
    kstore::BloomFilter bf(num_elements, 10); // ~1% rata teoretica de false positives

    // 1. Adaugam 10.000 de chei: "key_0" ... "key_9999"
    for (std::size_t i = 0; i < num_elements; ++i) {
        bf.add("key_" + std::to_string(i));
    }

    // 2. Verificam ca TOATE cele 10.000 sunt gasite (0% False Negatives!)
    for (std::size_t i = 0; i < num_elements; ++i) {
        ASSERT_TRUE(bf.possiblyContains("key_" + std::to_string(i)));
    }

    // 3. Testam 10.000 de chei care NU au fost adaugate: "other_0" ... "other_9999"
    std::size_t false_positives = 0;
    for (std::size_t i = 0; i < num_elements; ++i) {
        if (bf.possiblyContains("other_" + std::to_string(i))) {
            ++false_positives;
        }
    }

    // Cu 10 biti/element, ar trebui sa avem sub 200 de false positives din 10.000 (< 2%)
    EXPECT_LT(false_positives, 200);
}