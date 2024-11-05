//
//  Copyright 2024 Douglas John Moore and Scott Meyer Peimann.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//  logger.h
//  - the basic test logger.
//  - intended for use with ad-hoc SystemC based module tests.
//
//  NOTES:
//  2023-06-28: Moore and Peimann: first cut.
//  2023-08-10: Peimann: C++17 hack to use std::vector::emplace_back().
//  2023-08-11: Peimann: added string friendliness.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--invocation guard
#if not defined(__LOGGER_H__)
# define __LOGGER_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <iostream>
# include <string>
# include <vector>

  //--required library includes
  // <none>

  //--required project includes
# include "SC.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace logger
  {
    class logger;
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions

  class logger::logger final
  {
    public:     // c'tors and d'tor
      logger()
        : m_group{}
        , m_results{}
        , m_test{}

      { /* noOp - naught to do */ }
      ~logger() = default;

    public:     // accessors
    public:     // methods
      void begin_group(char const* const group_name,
                       char const* const description)
      { // begin a group of tests
        m_group.initialise(group_name, description);
      }

      void begin_group(std::string const &group_name,
                       std::string const &description)
      { // begin a group of tests
        m_group.initialise(group_name, description);
      }

      void end_group()
      { // end a group of tests
        m_results.push_back(m_group);  // copies current group to end of m_results
        m_group.reset();               // reset (empty) current group tracker
      }

      void output(std::ostream &out)
      { // walk all results and make pretty
        // throw an exception if m_result_group is not ended ("valid").
        //
        // example output:
        //
        //   Test group reset_000: description.
        //     10 s: test "(is_ready == 0)": pass
        //     10 s: test "(is_valid == 0)": fail
        //     Summary:
        //       pass 1 of 8.
        //       fail 7 of 8.
        // 
        //   Test group reset_001: another desecription.
        //   . . .
        // 
        //   Overall summary:
        //     Test results:
        //       pass 4 of 60.
        //       fail 56 of 60.
        // 
        //     Test group reset_000 results:
        //       pass 1 of 4.
        //       fail 3 of 4.
        // 
        //   Outcome FAILURE.
        //
        out << std::dec
            << "All test results: \n"
            << "  m_results.size(): " << m_results.size() << ". \n";
        struct
        {
          std::size_t pass;
          std::size_t fail;
          std::size_t count;
        } total{0, 0, 0};

        for (auto group : m_results)
        {
          out << "  Test group " << group.name << ": " << group.description << '\n';

          auto const all_count{group.test.size()};
          auto       pass_count{0u};
          for (auto test : group.test)
            pass_count += (test.pass ? 1 : 0);
          auto fail_count{all_count - pass_count};

#if false
          for (auto test : group.test)
          {
            auto result{test.pass ? "[PASS]" : "[FAIL]"};
            out << "    " << test.time << ": \"" << test.name << "\" " << result << "\n";
          }
#endif
          out << "\n"
              << "    Summary: \n"
              << "      pass " << pass_count << " of " << all_count << ". \n"
              << "      fail " << fail_count << " of " << all_count << ". \n"
              << "\n";

          total.pass += pass_count;
          total.fail += fail_count;
          total.count += (pass_count + fail_count);
        }

        out << "  Overall Summary: \n"
            << "    Test Results: \n"
            << "      pass " << total.pass << " of " << total.count << ". \n"
            << "      fail " << total.fail << " of " << total.count << ". \n"
            << "\n";

        for (auto group : m_results)
        {
          out << "    Test Group \"" << group.name << "\" Results: \n";

          auto const all_count{group.test.size()};
          auto       pass_count{0u};
          for (auto test : group.test)
            pass_count += (test.pass ? 1 : 0);
          auto const fail_count{all_count - pass_count};
          out << "      pass " << pass_count << " of " << all_count << ". \n"
              << "      fail " << fail_count << " of " << all_count << ". \n"
              << "\n";
        }

        out << "  Outcome: " << ((total.fail == 0) ? "PASS" : "FAILURE") << "\n";
      }

      void test_false(char const* const name, bool value)
      { // log test where the expected value is false
        m_group.test.emplace_back(name, __func__, (not value));
      }

      void test_true(char const* const name, bool value)
      { // log test where the expected value is true
        m_group.test.emplace_back(name, __func__, value);
      }

    private:    // methods

    private:    // types
      struct test_t;  // forward declaration

      struct group_t final
      {
        bool        valid;
        std::string name;
        std::string description;
        std::vector<test_t> test;

        group_t() : valid{false}, name{}, description{}, test{}
        { /* noOp - naught else to do */}

        void initialise(char const* const group_name,
                        char const* const group_description)
        { // initialise object; throw exception if already "valid".
          if  (valid)
            throw  "Initialising an already valid group is bad1.";
          valid = true;
          name = group_name;
          description = group_description;
          test.clear();
        }

        void initialise(std::string const &group_name,
                        std::string const &group_description)
        { // initialise object; throw exception if already "valid".
          if  (valid)
            throw  "Initialising an already valid group is bad2.";
          valid = true;
          name = group_name;
          description = group_description;
          test.clear();
        }

        void reset()
        { // reset test group attributes and mark invalid
          valid = false;
          name.clear();
          description.clear();
          test.clear();
        }
      };

      struct test_t final
      {
        SC::time    time;
        std::string name;
        std::string type;
        bool        pass;

        test_t()
          : time{}
          , name{}
          , type{}
          , pass{}
        { /* noOp - naught more to do */ }

        // Standard String pointer test_name and test_type with PASS/FAIL
        test_t(std::string const &test_name,
               std::string const &test_type,
               bool const         test_pass)
          : time{SC::time_stamp()}
          , name{test_name}
          , type{test_type}
          , pass{test_pass}
        { /* noOp - naught more to do */ }

        // SystemC time, Standard String pointer test_name and test_type with PASS/FAIL
        test_t(SC::time const     test_time,
               std::string const &test_name,
               std::string const &test_type,
               bool const         test_pass)
          : time{test_time}
          , name{test_name}
          , type{test_type}
          , pass{test_pass}
        { /* noOp - naught more to do */ }

        void reset()
        { // reset test attributes and mark invalid
          *this = test_t{};
        }
      };

    private:    // attributes
      group_t               m_group;          // current test group
      std::vector<group_t>  m_results;        // vector of all test groups
      test_t                m_test;           // current test 
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

#endif // not defined(__LOGGER_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
