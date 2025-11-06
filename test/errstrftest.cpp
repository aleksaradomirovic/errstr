/*
 * Copyright (C) 2025  Aleksa Radomirovic
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gtest/gtest.h>

#include "errstr.h"

TEST(errstrftest, clear) {
    ASSERT_NE(errstrf("%s", "test"), 0);
    ASSERT_TRUE(errstr_present());
    ASSERT_NE(errstrlen, 0);

    errstr_clear();
    EXPECT_EQ(errstrlen, 0);
    EXPECT_FALSE(errstr_present());
}

TEST(errstrftest, basic) {
    errstr_clear();

    EXPECT_EQ(errstrf("%s", "test"), strlen("test"));
    EXPECT_STREQ(errstr, "test");
    EXPECT_TRUE(errstr_present());
}

TEST(errstrftest, compound) {
    errstr_clear();

    ASSERT_EQ(errstrf("%s", "test"), strlen("test"));
    EXPECT_EQ(errstrf("%s", "test2: "), strlen("test2: ") + strlen("test"));
    EXPECT_STREQ(errstr, "test2: test");
    EXPECT_TRUE(errstr_present());
}
