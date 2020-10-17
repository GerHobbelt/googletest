// Copyright 2006, Google Inc.
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

#include <cstdio>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "RTOS2/FreeRTOS/Include/cmsis-freertos.h"

#include "synchapi.h"                       // Sleep

/// Google C++ Testing Framework supports two families of assertions with the same interface :
/// 
/// ASSERT : Fails fast, aborting the current function.
/// EXPECT : Continues after the failure.

#if defined(__MBED_CMSIS_RTOS_CM)
#include "mbed_boot.h"
#include "stm32h7xx.h"

int _system_pre_init(void) {
    system_pre_init_reg_setup();

    PWR->D3CR |= PWR_D3CR_VOSRDY;

    freertos_cmsis_rtos2_init();
    mbed_sdk_init();

    return (1);
}
#endif

#if defined(__FREERTOS_CMSIS_RTOS_CM)
#include "saml21_reg_stub.h"

uint32_t SystemCoreClock;

int _system_pre_init(void) {
    system_pre_init_reg_setup();

    freertos_cmsis_rtos2_init();

    SystemCoreClock = 12000000UL;

    return (1);
}
#endif

int static low_level_inited = _system_pre_init();

GTEST_API_ int main(int argc, char **argv) {
    int rc;

    printf("Running main() from %s\n", __FILE__);
    // The following line must be executed to initialize Google Mock
    // (and Google Test) before running the tests.
    testing::InitGoogleMock(&argc, argv);

    rc = RUN_ALL_TESTS();
    return (rc);
}
