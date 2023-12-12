// Copyright 2015, Google Inc.
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

#include "gtest/gtest.h"

#include "googletest/include/monolithic_examples.h"

namespace {
class DummyInvalidName1Test : public ::testing::TestWithParam<const char *> {};

TEST_P(DummyInvalidName1Test, Dummy) {
}

INSTANTIATE_TEST_SUITE_P(InvalidTestName,
                         DummyInvalidName1Test,
                         ::testing::Values("InvalidWithQuotes"),
                         ::testing::PrintToStringParamName());

}  // namespace

static void my_crt_terminate_handler()
{
	fprintf(stderr, "In my_crt_terminate_handler.\n");
	exit(EXIT_FAILURE);
}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)	gtest_param_inv_name1_test_main(cnt, arr)
#endif

int main(int argc, const char** argv) {
	try
	{
		set_terminate(my_crt_terminate_handler);
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
	}
	catch (...)
	{
		fprintf(stderr, "Exception thrown...\n");
		return EXIT_FAILURE;
	}
}

