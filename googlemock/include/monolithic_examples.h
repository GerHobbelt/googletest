//
// part of googlemock
//

#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

int gmock_test_main(int argc, const char** argv);
int gmock_spec_builders_test_main(int argc, const char** argv);
int gmock_output_test_main(int argc, const char** argv);
int gmock_stress_test_main(int argc, const char** argv);

#ifdef __cplusplus
}
#endif

#endif
