//
// Copyright (c) 2013 Mike Naquin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef NATIVE_BENCHMARK_H__
#define NATIVE_BENCHMARK_H__

#include "test.h"

#include <string>

#include <boost/timer/timer.hpp>

#define BENCHMARK(test_fixture, test_name) \
  BENCHMARK_IMPL(test_fixture, test_name, test_fixture, \
              ::testing::internal::GetTypeId<test_fixture>())

#define BENCHMARK_IMPL(test_case_name, test_name, parent_class, parent_id)\
class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class {\
 public:\
  GTEST_TEST_CLASS_NAME_(test_case_name, test_name)():\
   parent_class(#test_case_name, #test_name) {}\
 private:\
  virtual void TestBody();\
  static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN_(\
      GTEST_TEST_CLASS_NAME_(test_case_name, test_name));\
};\
\
::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name)\
  ::test_info_ =\
    ::testing::internal::MakeAndRegisterTestInfo(\
        #test_case_name, #test_name, NULL, NULL, \
        (parent_id), \
        parent_class::SetUpTestCase, \
        parent_class::TearDownTestCase, \
        new ::testing::internal::TestFactoryImpl<\
            GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>);\
void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()



class BenchmarkTest:
    public ::testing::Test {
public:
    static const std::size_t trial_count = 1000000;

    BenchmarkTest(const char* fixture, const char* name);

    virtual void SetUp();
    virtual void TearDown();

    template <typename Func>
    void benchmark(Func func)
    {
        auto fastest = std::numeric_limits<boost::timer::nanosecond_type>::max();
        boost::timer::nanosecond_type total = 0;
        boost::timer::cpu_times fastestElapsed;
        for (std::size_t i = 0; i < trial_count; ++i)
        {
            boost::timer::cpu_timer timer;
            func();
            timer.stop();
            const auto elapsed = timer.elapsed();
            if ( elapsed.wall < fastest )
            {
                fastestElapsed = elapsed;
                fastest = elapsed.wall;
            }
            total += elapsed.wall;
        }

        report(fastestElapsed, total);
    }

    virtual void report(const boost::timer::cpu_times& fastestElapsed,
                        const boost::timer::nanosecond_type& total);
protected:
    std::string fixture;
    std::string name;
    std::vector<boost::timer::cpu_timer> timers;
};

#endif
