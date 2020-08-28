// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//
// The Google C++ Testing and Mocking Framework (Google Test)
//
// This header file defines the public API for Google Test.  It should be
// included by any test program that uses Google Test.
//
// IMPORTANT NOTE: Due to limitation of the C++ language, we have to
// leave some internal implementation details in this header file.
// They are clearly marked by comments like this:
//
//   // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
//
// Such code is NOT meant to be used by a user directly, and is subject
// to CHANGE WITHOUT NOTICE.  Therefore DO NOT DEPEND ON IT in a user
// program!
//
// Acknowledgment: Google Test borrowed the idea of automatic test
// registration from Barthelemy Dagenais' (barthelemy@prologique.com)
// easyUnit framework.

// GOOGLETEST_CM0001 DO NOT DELETE

#ifndef GTEST_EXT_HPP
#define GTEST_EXT_HPP

#include "gtest/gtest.h"

#define EXPECT_ARRAYEQ(__a1__, __a2__, __size__)                EXPECT_TRUE(isArrayEqual(__a1__, __a2__, __size__))
#define EXPECT_2DARRAYEQ(__a1__, __a2__, __size1__, __size2__)  EXPECT_TRUE(is2dArrayEqual(__a1__, __a2__, __size1__, __size2__))

template<typename T, typename S>
static inline bool isArrayEqual(const T arr1, const S arr2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return (false);
        }
    }

    return (true);
}

template<typename T, typename S>
static inline bool is2dArrayEqual(const T arr1, const S arr2, size_t size1, size_t size2) {
    for (size_t i = 0; i < size1; i++) {
        for (size_t j = 0; j < size2; j++) {
            if (arr1[i][j] != arr2[i][j]) {
                return (false);
            }
        }
    }
    
    return (true);
}

#endif  // GTEST_EXT_HPP
