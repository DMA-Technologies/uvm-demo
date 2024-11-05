//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
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
//  TEST/EXAMPLE/base_test.h
//  - example of a "common" base class for SystemC unit tests.
//  - provides common behaviour and naming, all in one nice and neat package.
//
//  CHANGE LOG:
//  2023-08-10: first cut Moore and Peimann.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if not defined(__TEST_EXAMPLE_BASE_TEST_H__)
# define __TEST_EXAMPLE_BASE_TEST_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <sstream>

  //--required library includes
  // <none>

  //--required project includes
# include "SC.h"
# include "logger.h"
# include "model.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons
  namespace TEST
  {
    class base_test_t;
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions
  class TEST::base_test_t : public SC::module
  {
    public:      // c'tor and d'tor
      base_test_t(SC::module_name const& name, ::logger::logger &logger)
        : SC::module{name}
        , m_logger(logger)
      { /* noOp - no active SystemC methods or threads */ }

    public:     // accessors
    public:     // methods
    protected:  // methods
      void begin_test_group(char const *const group_name,
                            char const *const description)
      { // begin a test group within the logger
        m_logger.begin_group(group_name, description);
      }

      void begin_test_group(std::string const &group_name,
                            std::string const &description)
      { // begin a test group within the logger
        m_logger.begin_group(group_name, description);
      }

      void end_test_group()
      { // end a test group within the logger
        m_logger.end_group();
      }

      logger::logger logger()
      { // return the logger
        return  m_logger;
      }

      std::string const stamp(char const *const test_name,
                              char const *const display_text)
      { // generate a string with a time stamp, test name, and display text
        std::ostringstream now{};
        now << ::SC::time_stamp() << ": " << test_name << ": " << display_text;
          return  now.str();
      }

      std::string const stamp(std::string const &test_name,
                              std::string const &display_text)
      { // generate a string with a time stamp, test name, and display text
        std::ostringstream now{};
        now << ::SC::time_stamp() << ": " << test_name << ": " << display_text;
          return  now.str();
      }

      void test_false(char const *const test_name,
                      bool const        test_value)
      { // log the test for a false value
        m_logger.test_false(test_name, test_value);
      }

      void test_true(char const *const test_name,
                     bool const        test_value)
      { // log the test for a true value
        m_logger.test_true(test_name, test_value);
      }

      char const *tf(bool const value) const
      { // boolean -> true/false for printing.
        return  (value ? "true" : "false");
      }

      SC::time ts()
      { // SystemC time stamp value.
        return  SC::time_stamp();
      }

    private:    // methods
    private:    // local types
    private:    // attributes
      ::logger::logger  &m_logger;
  };

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--end of invocation guard
#endif // not defined(__TEST_EXAMPLE_BASE_TEST_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
