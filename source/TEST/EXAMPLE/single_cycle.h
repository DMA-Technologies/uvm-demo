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
//  TEST/EXAMPLE/single_cycle.h
//  - basic unit tests for a "generic" single-cycle pipeline module.
//    * control signaling.
//  - assumes that control and data signals are orthogonal.  
//
//  CHANGE LOG:
//  2023-08-10: Moore, Peimann: first cut.
//  2023-08-11: Moore, Peimann: updated to data-driven test mechanism.
//  2023-08-14: Moore, Peimann: implemented random signal injection test.
//  2023-08-14: Moore, Peimann: fixed stimulus-response timing bug.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if not defined(__TEST_EXAMPLE_SINGLE_CYCLE_H__)
# define __TEST_EXAMPLE_SINGLE_CYCLE_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <cstdint>
# include <random>
# include <sstream>
# include <string>
# include <vector>

  //--required library includes
  // <none>

  //--required project includes
# include "SC.h"
# include "logger.h"
# include "model.h"
# include "../base_test.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons
  namespace TEST::EXAMPLE
  {
    class single_cycle_t;
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions
  class TEST::EXAMPLE::single_cycle_t final : protected base_test_t
  {
    public:    // declaratons and definitions
    private:   // declaratons and definitions
      struct mut_state_t;               // forward declaration
      struct mut_stimuli_t;             // forward declaration
      struct test_t;                    // forward declaration
      struct test_item_t;               // forward declaration

    public:    // c'tor and d'tor
      single_cycle_t(SC::module_name const& name, logger::logger& logger)
        : base_test_t{name, logger}
        , m_clock{}
        , m_mut_control{}
        , m_mut_data{}
        , m_mut_debug{}
      {
        SC_THREAD(spin);
        sensitive << m_clock.in.clock.pos();
      }

    public:   // accessors
    public:   // methods
      void clock(model::signal::clock_t &in_clock)
      { // bind system clocking signals
//      std::cout << __PRETTY_FUNCTION__ << std::endl;
        m_clock.in.clock.bind(in_clock);
      }

      void control_in(model::signal::is_advance_t &is_advance,
                      model::signal::is_error_t   &is_error,
                      model::signal::is_stall_t   &is_stall,
                      model::signal::is_valid_t   &is_valid)
      { // bind control signals from the module under test
        m_mut_control.in.is_advance.bind(is_advance);
        m_mut_control.in.is_error.bind(is_error);
        m_mut_control.in.is_stall.bind(is_stall);
        m_mut_control.in.is_valid.bind(is_valid);
      }

      auto &control_out()
      {
        return  m_mut_control.out;
      }

      void data_in(model::signal::value_t &value)
      { // bind data signals from the module under test
        m_mut_data.in.value.bind(value);
      }

      auto &data_out()
      {
        return  m_mut_data.out;
      }

      void debug_in(model::signal::i_number_t   &i_number,
                    model::signal::i_binary_t   &i_binary,
                    model::signal::is_advance_t &is_advance,
                    SC::signal<std::string>     &string)
      { // bind input debug signals to and from the module under test
        m_mut_debug.in.i_number.bind(i_number);
        m_mut_debug.in.i_binary.bind(i_binary);
        m_mut_debug.in.is_advance.bind(is_advance);
        m_mut_debug.in.string.bind(string);
      }

      auto &debug_out()
      {
        return  m_mut_debug.out;
      }

    protected:  // methods
    private:    // methods
      void spin()
      {
        //--initialise
        // <tbd>

        //--wait for two cycles for system startup to finish up.
        std::cout << "spin: start of execution: " << SC::time_stamp() << '\n';
        wait(2, SC::sec);

        //--chunk through the tests
        //do_test_basic(reset_a_i);
        //do_test_basic(reset_a_ii);

        do_test_evil(0xe255da1f41566654);

        //--signal testing is complete
        //--FIXME: how can we build a system wherein multiple tests can
        //--       be executed concurrently?
        SC::stop();
      }

#if false //FIXME: make this thing work
      void do_test_basic(test_t const &test)
      { // performs a basic module control signal test based on:
        //   1) the fixed input sequence (see argument test)
        //   2) predicted control signal output (see variable pred)
        //--be lazy in naming signals
        auto &in(m_mut_control.in);
        auto &out(m_mut_control.out);

        //--start the group
        begin_test_group(test.group_name, test.description);

        //--march down all of the test items and verify
        for  (auto const &item: test.item)
        {
          // predict outcome based on current state and stimulus
          auto const pred{predict_controls(in.is_error.read(),
                                           in.is_stall.read(),
                                           in.is_valid.read(),
                                           item.in.reset,
                                           item.in.is_advance,
                                           item.in.is_flush,
                                           item.in.is_valid)};

          // stimulate the the module under test
          out.reset->write(item.in.reset);
          out.is_advance->write(item.in.is_advance);
          out.is_flush->write(item.in.is_flush);
          out.is_valid->write(item.in.is_valid);

          // advance the module state
          wait();

          // capture output control signals and verify their correctness
          bool is_advance{in.is_advance.read()};
          bool is_error  {in.is_error.read()};
          bool is_stall  {in.is_stall.read()};
          bool is_valid  {in.is_valid.read()};

          test_true("(is_advance == pred.is_advance)",
                    (is_advance == pred.is_advance));
          test_true("(is_error == pred.is_error)",
                    (is_error == pred.is_error));
          test_true("(is_stall == pred.is_stall)",
                    (is_stall == pred.is_stall));
          test_true("(is_valid == pred.is_valid)",
                    (is_valid == pred.is_valid));

          // for symmetry
          wait(SC::zero_time);
        }

        //--end the group
        end_test_group();

        //--end of compilation unit with implicit return
      }
#endif

      void do_test_evil(std::uint64_t const seed)
      { // performs an evil module control signal test based on:
        //   1) a random input sequence (see argument seed)
        //   2) predicted control signal output (see variable pred)

        std::cout
//        << ts() << ' ' << __PRETTY_FUNCTION__ << '\n';
          << ts() << ' ' << __func__ << '\n';

        //--be lazy in naming things
        auto &in(m_mut_control.in);     // inputs from MUT
        auto &out(m_mut_control.out);   // outputs to  MUT

        struct test_information_t
        { //FIXME: comment here
          mut_state_t   state;          // expected state at start of cycle
          mut_stimuli_t stim;           // control signal stimulus
          mut_state_t   pred;           // predicted state at end of cycle
        };

        std::mt19937_64 random{seed};   // random number generator

        test_information_t info[2];     // several cycles of test information

        //--start the test group
        begin_test_group("evil", "random stimulous test.");

// initialise info array for two resets

        //--send first reset and wait
        std::cout
          << ts() << "   - first reset cycle of test. \n"
          << ts() << "   - EVIL RESET: #1. \n"
          << std::endl;

        info[0].state = {false, false, false, false}; // adv, err, stall, valid
        info[0].stim  = {0,false,false,false,false};  // rst, adv, err, fls, val
        info[0].pred  = {true, false, false, false};  // adv, err, stall, valid

        out.reset->write     (info[0].stim.reset);
        out.is_advance->write(info[0].stim.is_advance);
        out.is_flush->write  (info[0].stim.is_flush);
        out.is_valid->write  (info[0].stim.is_valid);
        m_mut_debug.out.string->write("EVIL RESET: #1.");

        wait();

        //--send second reset; wait is at top of process loop
        std::cout
          << ts() << "   - second reset cycle of test. \n"
          << ts() << "   - EVIL RESET: #2. \n"
          << std::endl;

        info[1] = info[0];

        info[0].state = {true, false, false, false};  // adv, err, stall, valid
        info[0].stim  = {0,false,false,false,false};  // rst, adv, err, fls, val
        info[0].pred  = {true, false, false, false};  // adv, err, stall, valid

        out.reset->write     (info[0].stim.reset);
        out.is_advance->write(info[0].stim.is_advance);
        out.is_flush->write  (info[0].stim.is_flush);
        out.is_valid->write  (info[0].stim.is_valid);
        m_mut_debug.out.string->write("EVIL RESET: #2.");

        wait();

        //--feed lots o' random into the system
        for  (unsigned cycle{0};  (cycle < 100000);  cycle++)
        {
          // debug
          std::cout
            << ts() << "  - top o'cycle: \n"
            << ts() << "  - cycle: " << cycle << '\n'
            << ts() << "  - debug.in.string: " << '\n'
            << m_mut_debug.in.string.read() << '\n'
            << std::endl;

          //FIXME: this test can be removed when testing is done
          // test that state is as expected
          // verify that the expected starting state for info[0] is the same
          //   as the predicted output for info[1]
          if  (info[0].state != info[1].pred)
            throw "(info[0].state != info[1].pred)";

          // capture MUT state for test purposes
          mut_state_t const state
          {
            m_mut_debug.in.is_advance.read(),  // NB: is_advance signal captured by MUT, as the in.is_advance might be recomputed during a delta cycle
            in.is_error.read(),
            in.is_stall.read(),
            in.is_valid.read(),
          };

          // verify the module under test state just read back
          test_true("(state == info[1].pred)", (state == info[1].pred));

          if  (state == info[1].pred) //FIXME: can remove after debug is complete.
          { // success confirmation
            std::cout
              << ts() << "  - state.is_advance: " << tf(state.is_advance) << '\n'
              << ts() << "  - state.is_error:   " << tf(state.is_error)   << '\n'
              << ts() << "  - state.is_stall:   " << tf(state.is_stall)   << '\n'
              << ts() << "  - state.is_valid:   " << tf(state.is_valid)   << '\n'
              << ts() << "  - success. \n"
              << std::endl;
          }
          else
          { // we flocked up
// fix this diagnostic output, so that correct information is shown
            auto &state0{info[1].state}; // input state
            auto &stim0 {info[1].stim};  // stimuli to module under test
            auto &pred0 {info[1].pred};  // predicted final state
            auto &state1{state};         // rename of captured final state
            std::cout
              << "state0.is_advance: " << tf(state0.is_advance) << '\n'
              << "state0.is_error:   " << tf(state0.is_error)   << '\n'
              << "state0.is_stall:   " << tf(state0.is_stall)   << '\n'
              << "state0.is_valid:   " << tf(state0.is_valid)   << '\n'
              << "stim0.reset:       " << stim0.reset           << '\n'
              << "stim0.is_advance:  " << tf(stim0.is_advance)  << '\n'
              << "stim0.is_flush:    " << tf(stim0.is_flush)    << '\n'
              << "stim0.is_valid:    " << tf(stim0.is_valid)    << '\n'
              << "pred0.is_advance:  " << tf(pred0.is_advance)  << '\n'
              << "pred0.is_error:    " << tf(pred0.is_error)    << '\n'
              << "pred0.is_stall:    " << tf(pred0.is_stall)    << '\n'
              << "pred0.is_valid:    " << tf(pred0.is_valid)    << '\n'
              << "state1.is_advance: " << tf(state1.is_advance) << '\n'
              << "state1.is_error:   " << tf(state1.is_error)   << '\n'
              << "state1.is_stall:   " << tf(state1.is_stall)   << '\n'
              << "state1.is_valid:   " << tf(state1.is_valid)   << '\n'
              << std::endl;
            throw "We done flocked up.";
          }

          // expected starting state from previous output
          mut_state_t const state0{info[0].pred};

          // create random stimulus, and predicted result therefrom
          auto const rand{random()};
          mut_stimuli_t const stim
          {
            (((rand >>  0) bitand 0x3f) != 0),  // reset;      p: 63/64
            (((rand >>  8) bitand 0x03) == 0),  // is_advance; p:  1/4
            (((rand >> 16) bitand 0x3f) == 0),  // is_error;   p:  1/64
            (((rand >> 24) bitand 0x0f) == 0),  // is_flush;   p:  1/16
            (((rand >> 32) bitand 0x01) == 0),  // is_valid;   p:  1/2
          };

          // predict state based on current state and input
          mut_state_t const pred{predict_controls(state0, stim)};

          // update the info array
          info[1] = info[0];
          info[0] = {state0, stim, pred};

          // write debug string
          std::ostringstream debug{};
          debug
            << "Debug for cycle " << cycle << ".\n"
            << "  expected start state A/E/S/V: "
               << tf(info[0].state.is_advance) << '/'
               << tf(info[0].state.is_error) << '/'
               << tf(info[0].state.is_stall) << '/'
               << tf(info[0].state.is_valid) << '\n'
            << "  stimulus RST/nA/ERR/FLSH/pV:  "
               << stim.reset << '/'
               << tf(stim.is_advance) << '/'
               << tf(stim.is_error) << '/'
               << tf(stim.is_flush) << '/'
               << tf(stim.is_valid) << '\n'
            << "  expected final state A/E/S/V: "
               << tf(pred.is_advance) << '/'
               << tf(pred.is_error) << '/'
               << tf(pred.is_stall) << '/'
               << tf(pred.is_valid);
          std::cout
            << ts() << "  - computed debug string for cycle " << cycle << ".\n"
            << debug.str() << "\n"
            << std::endl;

          // write stimulus
          std::cout
            << "evil: writing stimulus to MUT: " << SC::time_stamp() << '\n'
            << std::endl;
          out.reset->write     (stim.reset);
          out.is_advance->write(stim.is_advance);
          out.is_flush->write  (stim.is_flush);
          out.is_valid->write  (stim.is_valid);
          m_mut_debug.out.string->write(debug.str());
          m_mut_debug.out.is_error->write(stim.is_error);

          // wait for output from module under test
          std::cout
            << ts() << "  - BEFORE WAIT. \n"
            << std::endl;
          wait();
          std::cout
            << ts() << "  - FOLLOWING WAIT. \n"
            << std::endl;
        }

        //--end the group
        end_test_group();

        //--end of compilation unit with implicit return
      }

      mut_state_t predict_controls(mut_state_t const   &state,
                                   mut_stimuli_t const &stimuli)
      { // predict a single-cycle module's control signal output based on:
        //   1) current state (is_error, is_stall, is_valid)
        //   2) input control signals (reset, ..., prior_is_valid)

        //--a couple useful shorthands
        bool const reset         {(stimuli.reset == 0)};
        bool const reset_or_flush{(reset or stimuli.is_flush)};

        //--expected combinatoric advance at start of MUT "cycle"
        bool const advance{(not state.is_stall) and
                           ((not state.is_valid) or (stimuli.is_advance))};

        bool const error  {(not reset) and
                           (state.is_error or stimuli.is_error)};

        bool const stall  {false};

        bool const valid  {(not reset_or_flush) and
                           ((advance and stimuli.is_valid) or
                            (not advance and state.is_valid))};

        //--end of compilation unit with explicit return
        return  {advance, error, stall, valid};
      }

    private:  // local types
      using SC_CURRENT_USER_MODULE = single_cycle_t;

      struct clock_t final
      { // necessary clocks for this module to function correctly
        struct in_t
        {
          model::port::in::clock_t clock;             // system clock
        };
        struct out_t
        { /* empty for now */ };

        in_t  in;     // input signals
        out_t out;    // output signals
      };

      struct mut_control_t final
      { // module under test control signals
        struct in_t
        {
          model::port::in::is_advance_t is_advance;   // computed advance bit
          model::port::in::is_error_t   is_error;     // error was detected
          model::port::in::is_stall_t   is_stall;     // stall assertion
          model::port::in::is_valid_t   is_valid;     // data valid bit
        };
        struct out_t
        {
          model::exprt::reset_t         reset;        // system reset
          model::exprt::is_advance_t    is_advance;   // next stage advance bit
          model::exprt::is_flush_t      is_flush;     // stage flush bit
          model::exprt::is_valid_t      is_valid;     // prior stage data valid
        };
        struct signal_t
        {
          model::signal::reset_t        reset;        // system reset
          model::signal::is_advance_t   is_advance;   // next stage advance bit
          model::signal::is_flush_t     is_flush;     // stage flush bit
          model::signal::is_valid_t     is_valid;     // prior stage data valid
        };

        in_t     in;       // input ports for signals from module under test
        out_t    out;      // output exports for signals to module under test
        signal_t signal;   // output signals to module under test

        mut_control_t(): in{}, out{}, signal{}
        {
          out.reset.bind(signal.reset);
          out.is_advance.bind(signal.is_advance);
          out.is_flush.bind(signal.is_flush);
          out.is_valid.bind(signal.is_valid);
        }
      };

      struct mut_data_t final
      { // module under test data signals
        struct in_t
        {
          model::port::in::value_t  value;
        };
        struct out_t
        {
          model::exprt::value_t value;
        };
        struct signal_t
        {
          model::signal::value_t value;
        };

        in_t     in;       // input ports for signals from module under test
        out_t    out;      // output exports for signals to module under test
        signal_t signal;   // output signals to module under test

        mut_data_t(): in{}, out{}, signal{}
        {
          out.value.bind(signal.value);
        }
      };

      struct mut_debug_t final
      { // module under test debug signals
        struct in_t
        {
          model::port::in::i_number_t   i_number;
          model::port::in::i_binary_t   i_binary;
          model::port::in::is_advance_t is_advance;
          SC::in<std::string>           string;
        };
        struct out_t
        {
          model::exprt::i_number_t i_number;
          model::exprt::i_binary_t i_binary;
          model::exprt::is_error_t is_error;
          SC::exprt<SC::signal<std::string>> string;
        };
        struct signal_t
        {
          model::signal::i_number_t i_number;
          model::signal::i_binary_t i_binary;
          model::signal::is_error_t is_error;
          SC::signal<std::string>   string;
        };

        in_t     in;       // input ports for signals from module under test
        out_t    out;      // output exports for signals to module under test
        signal_t signal;   // output signals to module under test

        mut_debug_t(): in{}, out{}, signal{}
        {
          out.i_number.bind(signal.i_number);
          out.i_binary.bind(signal.i_binary);
          out.is_error.bind(signal.is_error);
          out.string.bind(signal.string);
        }
      };

      struct mut_state_t final
      {
        model::is_advance_t is_advance;  //
        model::is_error_t   is_error;
        model::is_stall_t   is_stall;
        model::is_valid_t   is_valid;

        bool operator==(mut_state_t const &that) const
        {
          return  ((is_advance == that.is_advance) and
                   (is_error   == that.is_error  ) and
                   (is_stall   == that.is_stall  ) and
                   (is_valid   == that.is_valid  ));
        }

        bool operator!=(mut_state_t const &that) const
        {
          return  (not (*this == that));
        }
      };

      struct mut_stimuli_t final
      {
        model::reset_t      reset;       // system reset; 0:reset; 1:process
        model::is_advance_t is_advance;  // next module is_advance
        model::is_error_t   is_error;    // force the error bit
        model::is_flush_t   is_flush;    // flush from below
        model::is_valid_t   is_valid;    // prior module is_valid
#if false
//FIXME: add these elements to the stimuli
        model::value_t      value;       // prior module value bits
        model::i_binary_t   i_binary;
        model::i_number_t   i_number;
#endif
      };

      struct test_t final
      {
        std::string const              group_name;
        std::string const              description;
        std::vector<test_item_t> const item;
      };

      struct test_item_t final
      {
        struct in_t final
        { // input to module under test
          unsigned      reset:1;       // 0:reset; 1:no-reset
          bool          is_advance:1;  // advance value (must take)
          bool          is_flush:1;    // flush stage
          bool          is_valid:1;    // input value is valid
          model::word_t value;         // input value
        };

        struct out_t final
        { // output expected from module under test
          bool          is_advance:1;  // advance value (must take)
          bool          is_error:1;    // an error was detected
          bool          is_stall:1;    // stall prior stages
          bool          is_valid:1;    // output value(s) are valid
          model::word_t value;         // output value
        };

        in_t  in;   // input to module under test
        out_t out;  // output expected from module under test
      };

    private:  // attributes
      // internal test variables
      // <none>
      // SystemC ports
      clock_t        m_clock;
      mut_control_t  m_mut_control;
      mut_data_t     m_mut_data;
      mut_debug_t    m_mut_debug;

      // test sequences
      test_t reset_a_i
      {
        "Single-cycle-reset",
        "Verify output control signals following single cycle of reset. \n"
        "* Single-cycle reset followed by a single non-reset cycle. \n"
        "* All other control bit combinations are asserted during reset. \n"
        "* NB: reset is a low-active signal. \n"
        "* NB: following reset, next stage ought always assert advance. \n",
        {
          { {0, 0, 0, 0, 0b000}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b000}, {1, 0, 0, 0, 0x00} },
          { {0, 0, 0, 1, 0b001}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b001}, {1, 0, 0, 0, 0x00} },
          { {0, 0, 1, 0, 0b010}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b010}, {1, 0, 0, 0, 0x00} },
          { {0, 0, 1, 1, 0b011}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b011}, {1, 0, 0, 0, 0x00} },
          { {0, 1, 0, 0, 0b100}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b100}, {1, 0, 0, 0, 0x00} },
          { {0, 1, 0, 1, 0b101}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b101}, {1, 0, 0, 0, 0x00} },
          { {0, 1, 1, 0, 0b110}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b110}, {1, 0, 0, 0, 0x00} },
          { {0, 1, 1, 1, 0b111}, {1, 0, 0, 0, 0x00} },
          { {1, 1, 0, 0, 0b111}, {1, 0, 0, 0, 0x00} },
        },
      };

      test_t reset_a_ii
      {
        "Multi-cycle-reset",
        "Verify output control signals while holding reset for 4 cycles."
        "* NB: reset is a low-active signal. \n"
        "* NB: following reset, next stage ought always assert advance. \n",
        {
          { {0, 0, 0, 0, 0x12345678}, {1, 0, 0, 0, 0x00000000} },
          { {0, 0, 0, 0, 0xcafebabe}, {1, 0, 0, 0, 0x00000000} },
          { {0, 0, 0, 0, 0xdeadbeef}, {1, 0, 0, 0, 0x00000000} },
          { {0, 0, 0, 0, 0xdeafd00d}, {1, 0, 0, 0, 0x00000000} },
        },
      };
  };

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--end of invocation guard
#endif // not defined(__TEST_EXAMPLE_SINGLE_CYCLE_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
