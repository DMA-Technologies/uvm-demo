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
//  EXAMPLE/single_cycle.h
//  - example of a generalised single-cycle pipeline module.
//  - provides basic state and signal handling.
//  - provides trivial data in and out.
//  - when a valid datum is accepted from the prior stage, the value is
//    inverted and clocked into the output datum.
//
//  Design Decisions:
//  - follows the basic four control signal module concept:
//      stall (to prior pipeline stage)
//      valid (to next pipeline stage)
//      advance (from next pipeline stage)
//      valid (in from prior pipeline stage)
//  - the stall signal is provided for signaling consistency amongst all
//    pipeline stages, for single-cycle modules it will never be asserted.
//  - provides an erorr detected signal which is stimulated through the
//    an input debug signal; for actual pipeline modules this error signal
//    is intended for reporting of an internally detected error.
//  - much of the class is constructed with data organisation in mind, for
//    example the data_t class which further groups the input signals,
//    exports, and output signals; we use structural nesting to provide
//    naming consistency, as opposed to using long names.
//
//  NOTES:
//  2023-08-10: Moore, Peimann: first cut.
//  2023-08-11: Peimann: revised control signals to generic model.
//  2023-09-09: Moore, Peimann: minor diagnostic changes.
//  2023-09-11: Moore, Peimann: minimise diagnostic output.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__EXAMPLE_SINGLE_CYCLE_H__)
# define __EXAMPLE_SINGLE_CYCLE_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <string>
# include <stdexcept>

  //--required library includes
  // <none>

  //--required project includes
# include "SC.h"
# include "model.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons
  namespace EXAMPLE
  {
    class single_cycle_t;
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions
  class EXAMPLE::single_cycle_t final : public SC::module
  {
    public:    // c'tor and d'tor
      single_cycle_t(SC::module_name const& name)
        : SC::module{name}
        , m_clock{}
        , m_control{}
        , m_data{}
        , m_debug{}
      {
        SC_METHOD(compute_advance);
        sensitive << m_control.in.is_advance;
        sensitive << m_control.out.is_stall;
        sensitive << m_control.out.is_valid;

        SC_METHOD(cycle);
        sensitive << m_clock.in.clock.pos();
      }

    public:   // accessors
    public:   // methods
      void clock(model::signal::clock_t &clock)
      { // clocking signals
        m_clock.in.clock.bind(clock);
      }

      void control_in(model::signal::reset_t      &reset,
                      model::signal::is_advance_t &is_advance,
                      model::signal::is_flush_t   &is_flush,
                      model::signal::is_valid_t   &is_valid)
      { // control signal inputs
        m_control.in.reset.bind(reset);
        m_control.in.is_advance.bind(is_advance);
        m_control.in.is_flush.bind(is_flush);
        m_control.in.is_valid.bind(is_valid);
      }

      auto &control_out()
      { // return control signal exports
        return  m_control.out;
      }

      void data_in(model::signal::value_t &value)
      {
        m_data.in.value.bind(value);
      }

      auto &data_out()
      { // return data signal exports
        return  m_data.out;
      }

      void debug_in(model::signal::i_number_t &i_number,
                    model::signal::i_binary_t &i_binary,
                    model::signal::is_error_t &is_error,
                    SC::signal<std::string>   &string)
      { // bind debug input signals
        m_debug.in.i_number.bind(i_number);
        m_debug.in.i_binary.bind(i_binary);
        m_debug.in.is_error.bind(is_error);
        m_debug.in.string.bind(string);
      }

      auto &debug_out()
      { // return debug signal exports
        return  m_debug.out;
      }

    protected:// methods
    private:  // methods
      void compute_advance()
      { // dynamically compute the advance/must-take signal.
#if  false  // works
        advance_input_t const in
        {
          m_control.out.is_stall->read(),
          m_control.out.is_valid->read(),
          m_control.in.is_advance->read(),
        };

        bool is_advance{(not in.is_stall) and
                        ((not in.is_valid) or in.is_advance)};
#elif false  // should work
        bool is_advance;
        if (m_control.out.is_stall->read())
          is_advance = false;                    // never
        else if  (not m_control.out.is_valid->read())
          is_advance = true;                     // we don't have a value
        else if  (m_control.in.is_advance->read())
          is_advance = true;                     // we have a value and "next" will take it
        else
          is_advance = false;                    // "next" won't take it
#elif false  // better naming?
        struct state_t
        { // "current" state as computed at start of cycle
          ::model::is_stall_t is_stall;
          ::model::is_valid_t is_valid;
        }
        state
        {
          m_control.out.is_stall->read(),
          m_control.out.is_valid->read(),
        };
        struct next_t
        { // "current" state from "next module
          ::model::is_advance_t is_advance;
        }
        next
        {
          m_control.in.is_advance->read(),
        };

        bool is_advance{(not state.is_stall) and
                        ((not state.is_valid) or next.is_advance)};
#else  // even better naming?
        // "current" state as computed at start of cycle
        ::model::is_stall_t state_is_stall{m_control.out.is_stall->read()};
        ::model::is_valid_t state_is_valid{m_control.out.is_valid->read()};

        // "current" state from next module
        ::model::is_advance_t next_is_advance{m_control.in.is_advance->read()};

        bool is_advance{(not state_is_stall) and
                        ((not state_is_valid) or next_is_advance)};
#endif

        m_control.out.is_advance->write(is_advance);
      }

      void cycle()
      { // 
        //--capture the current state of inputs into a local struct.
        cycle_input_t const in
        { // read input values directly into structure
          m_control.in.reset->read(),
          m_control.in.is_advance->read(),
          m_control.in.is_flush->read(),
          m_control.in.is_valid->read(),
          m_data.in.value->read(),
          m_debug.in.i_binary->read(),
          m_debug.in.i_number->read(),
          m_debug.in.is_error->read(),
          m_debug.in.string->read(),
        };

        //--capture the current State of The Union into a local struct.
        //--NB: this structure updates in-place and is written following
        //      the signal computation logic.
        cycle_state_t state
        { // read state values directly into structure
          m_control.out.is_advance->read(),
          m_control.out.is_error->read(),
          m_control.out.is_stall->read(),
          m_control.out.is_valid->read(),
          m_data.out.value->read(),
          m_debug.out.i_binary->read(),
          m_debug.out.i_number->read(),
          m_debug.out.string->read(),
        };

        //--do the ugly computation
        if  (in.reset == 0)
        { // minimal reset necessary
          // NB: shift back to well-known control signal state
          state.is_error = false;
          state.is_stall = false;
          state.is_valid = false;
          // not updated: value, i_binary, i_number
          state.string   = ("RESET: " + in.string);
        }
        else if  (in.is_flush)
        { // a branch occurred in a subsequent pipeline stage
          // NB: discard any held, valid data
          // NB: assume prior stage data are moot; take no data
          state.is_error = (state.is_error or in.is_error);
          state.is_stall = false;
          state.is_valid = false;
          // not updated: value, i_binary, i_number
          state.string   = ("FLUSH: " + in.string);
        }
        else if  (state.is_stall)
        { // single-cycle modules should never stall.
          state.is_error = true;
          state.is_stall = false;
          state.is_valid = false;
          // not updated: value, i_binary, i_number
          state.string   = ("STALL: " + in.string);

          //FIXME: remove exception for synthesis.
          throw  ::std::logic_error("Single-Vycle modules"
                                    " ought never stall.");
        }
        else if  (state.is_advance)
        { // pipeline advances take input data and transform them
          state.is_error = (state.is_error or in.is_error);
          state.is_stall = false;
          state.is_valid = in.is_valid;
          state.value    = (compl in.value);
          state.i_binary = in.i_binary;
          state.i_number = in.i_number;
          state.string   = ("ADVANCE: " + in.string);
        }
        else
        { // nothing to do
          state.is_error = (state.is_error or in.is_error);
          // not updated: is_stall, is_valid, value, i_binary, i_number
          state.string   = ("NOP: " + in.string);
        }

        //--update the outputs
        m_control.out.is_error->write(state.is_error);
        m_control.out.is_stall->write(state.is_stall);
        m_control.out.is_valid->write(state.is_valid);

        m_data.out.value->write(state.value);

        m_debug.out.i_binary->write(state.i_binary);
        m_debug.out.i_number->write(state.i_number);
        m_debug.out.is_advance->write(state.is_advance);
        m_debug.out.string->write(state.string);

        //--end of compilation unit with implicit return
      }

    private:  // local types
      using SC_CURRENT_USER_MODULE = single_cycle_t;

      struct advance_input_t final
      {
        bool const is_stall;    // stall bit from last cycle
        bool const is_valid;    // valid bit from last cycle
        bool const is_advance;  // advance bit from next module
      };

      struct clock_t final
      {
        struct in_t
        {
          model::port::in::clock_t clock;            // system clock
        } in;
        struct out_t
        {
        } out;
      };

      struct control_t final
      {
        struct in_t
        {
          model::port::in::reset_t       reset;      // system reset
          model::port::in::is_advance_t  is_advance; // next stage advance bit
          model::port::in::is_flush_t    is_flush;   // stage flush bit
          model::port::in::is_valid_t    is_valid;   // prior stage data valid
        };
        struct out_t
        {
          model::exprt::is_advance_t is_advance;  // computed advance bit
          model::exprt::is_error_t   is_error;    // computed error bit
          model::exprt::is_stall_t   is_stall;    // stall assertion
          model::exprt::is_valid_t   is_valid;    // data valid bit
        };
        struct signal_t
        {
          model::signal::is_advance_t is_advance; // computed advance bit
          model::signal::is_error_t   is_error;   // computed error bit
          model::signal::is_stall_t   is_stall;   // stall assertion
          model::signal::is_valid_t   is_valid;   // data valid bit
        };

        in_t     in;
        out_t    out;
        signal_t signal;

        control_t(): in{}, out{}, signal{}
        {
          out.is_advance.bind(signal.is_advance);
          out.is_error.bind(signal.is_error);
          out.is_stall.bind(signal.is_stall);
          out.is_valid.bind(signal.is_valid);
        }
      };

      struct cycle_input_t
      { // input control and diagnostic values for the cycle() method.
        model::reset_t      reset;
        model::is_advance_t is_advance;
        model::is_flush_t   is_flush;
        model::is_valid_t   is_valid;
        model::value_t      value;
        model::i_binary_t   i_binary;
        model::i_number_t   i_number;
        model::is_error_t   is_error;    // stimulus to force error bit
        std::string         string;      // diagnostic string
      };

      struct cycle_state_t
      { // state of this module; updates in-place in cycle() method.
        bool const        is_advance;
        model::is_error_t is_error;
        model::is_stall_t is_stall;
        model::is_valid_t is_valid;
        model::value_t    value;
        model::i_binary_t i_binary;
        model::i_number_t i_number;
        std::string       string;
      };

      struct data_t final
      {
        struct in_t
        {
          model::port::in::value_t value;  // data value from prior stage
        };
        struct out_t
        {
          model::exprt::value_t    value;  // data value to next stage
        };
        struct signal_t
        {
          model::signal::value_t   value;  // signal for output data value 
        };

        in_t     in;
        out_t    out;
        signal_t signal;

        data_t(): in{}, out{}, signal{}
        {
          out.value.bind(signal.value);
        }
      };

      struct debug_t final
      {
        struct in_t
        { // input ports
          model::port::in::i_number_t i_number;
          model::port::in::i_binary_t i_binary;
          model::port::in::is_error_t is_error;
          SC::in<std::string>         string;
        };
        struct out_t
        { // exports for signals
          model::exprt::i_number_t           i_number;
          model::exprt::i_binary_t           i_binary;
          model::exprt::is_advance_t         is_advance;
          SC::exprt<SC::signal<std::string>> string;
        };
        struct signal_t
        { // we own the signals we generate
          model::signal::i_number_t   i_number;
          model::signal::i_binary_t   i_binary;
          model::signal::is_advance_t is_advance;
          SC::signal<std::string>     string;
        };

        in_t     in;
        out_t    out;
        signal_t signal;

        debug_t(): in{}, out{}, signal{}
        { // c'tor - bind signals into exports
          out.i_number.bind(signal.i_number);
          out.i_binary.bind(signal.i_binary);
          out.is_advance.bind(signal.is_advance);
          out.string.bind(signal.string);
        }
      };

    private:  // attributes
      clock_t    m_clock;
      control_t  m_control;
      data_t     m_data;
      debug_t    m_debug;
  };

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--end of invocation guard
#endif // not defined(__EXAMPLE_SINGLE_CYCLE_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
