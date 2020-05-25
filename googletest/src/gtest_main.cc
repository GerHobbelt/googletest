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
#include "RTOS2/FreeRTOS/Include/cmsis-freertos.h"

#include "rtos/rtos.h"

#include "synchapi.h"

#define TEST_STACK_SIZE 256
#define ONE_MILLI_SEC 1000

volatile uint32_t elapsed_time_ms = 0;
static const int test_timeout = 40;

void update_tick_thread(Mutex* mutex) {
    int rc;

    while (true) {
        ThisThread::sleep_for(1);
        mutex->lock();
        ++elapsed_time_ms;
        mutex->unlock();

        rc = RUN_ALL_TESTS();
    }
}


#if GTEST_OS_ESP8266 || GTEST_OS_ESP32
#if GTEST_OS_ESP8266
extern "C" {
#endif
void setup() {
  testing::InitGoogleTest();
}

void loop() { RUN_ALL_TESTS(); }

#if GTEST_OS_ESP8266
}
#endif

#else

GTEST_API_ int main(int argc, char **argv) {
    int rc;

    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);

    freertos_cmsis_rtos2_init();
    Mutex mutex;

    Thread tick_thread(osPriorityHigh, TEST_STACK_SIZE);
    tick_thread.start(callback(update_tick_thread, &mutex));

    osKernelInitialize();
    osKernelStart();
    Sleep(1000);
    return (0);
}
#endif
