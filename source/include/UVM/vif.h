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
//  vif.h
//  - virtual interfaces for the DUTs/MUTs.
//
//  NOTES:
//  2024-07-10: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__VIF_H__)
# define __VIF_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <string>

  //--required library includes
  // <none>

  //--required project includes
# include "model.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace vif
  { // virtual interfaces for everyone!
    //--base class for all VIFs.
    class base_t;

    namespace EXAMPLE
    { // virtual interfaces for various example modules which were developed.
      //--complete interface to the ::EXAMPLE::single_cycle_t module.
      class single_cycle_t;
    } // namespace ::vif::EXAMPLE
  } // namespace ::vif

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::vif::base_t

  class vif::base_t
    : public ::SC::module
  {
#pragma message("Differences betwixt software, emulator, FPGA, and silicon make clocking difficult.")
#pragma message("How ought we compose tests which are independent of target implementation.")
    public:   // forward declarations
    public:   // c'tors and d'tor
      base_t(SC::module_name name)
        : SC::module{name}
      { /* noOp */ }
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::vif::EXAMPLE::single_cycle_t

  class vif::EXAMPLE::single_cycle_t final
    : public ::vif::base_t
  {
    public:   // forward declarations
    public:   // c'tors and d'tor
      single_cycle_t(
        SC::module_name              name,
        model::signal::clock_t      &system_clock,
        model::signal::is_advance_t &ctl_is_advance,
        model::signal::is_error_t   &ctl_is_error,
        model::signal::is_stall_t   &ctl_is_stall,
        model::signal::is_valid_t   &ctl_is_valid,
        model::signal::value_t      &data_value,
        model::signal::i_binary_t   &dbg_i_binary,
        model::signal::i_number_t   &dbg_i_number,
        model::signal::is_advance_t &dbg_is_advance,
        SC::signal<std::string>     &dbg_string)
        : base_t{name}
        , in{{system_clock}, {}, {}, {}}
        , out{{ctl_is_advance, ctl_is_error, ctl_is_stall, ctl_is_valid},
              {data_value}, 
              {dbg_i_binary, dbg_i_number, dbg_is_advance, dbg_string}}
      { /* noOp */ }

    public:   // accessors
    public:   // methods
      SC::event const &clock_posedge() const
      {
        return  in.clock.clock.posedge_event();
      }

      void set_control_inputs(model::reset_t const      reset,
                              model::is_advance_t const is_advance,
                              model::is_flush_t const   is_flush,
                              model::is_valid_t const   is_valid)
      {
        in.ctl.reset.write(reset);
        in.ctl.is_advance.write(is_advance);
        in.ctl.is_flush.write(is_flush);
        in.ctl.is_valid.write(is_valid);
      }

      void set_data_inputs(model::value_t const value)
      {
        in.data.value.write(value);
      }

      void set_debug_inputs(model::i_binary_t const i_binary,
                            model::i_number_t const i_number,
                            model::is_error_t const is_error,
                            std::string const      &string)
      {
        in.debug.i_binary.write(i_binary);
        in.debug.i_number.write(i_number);
        in.debug.is_error.write(is_error);
        in.debug.string.write(string);
      }

      void trace_to_file(SC::trace_file *const tf)
      { // start tracing signals
        // also writes all values to file

        //--NB: clock cannot be traced for some reason
        //SC::trace(tf, in.clock.clock.get_interface(), "vif.in.clock.clock");

        //--control signals
        SC::trace(tf, in.ctl.reset,       "vif.in.ctl.reset");
        SC::trace(tf, in.ctl.is_advance,  "vif.in.ctl.is_advance");
        SC::trace(tf, in.ctl.is_flush,    "vif.in.ctl.is_flush");
        SC::trace(tf, in.ctl.is_valid,    "vif.in.ctl.is_valid");
        SC::trace(tf, out.ctl.is_advance, "vif.out.ctl.is_advance");
        SC::trace(tf, out.ctl.is_error,   "vif.out.ctl.is_error");
        SC::trace(tf, out.ctl.is_stall,   "vif.out.ctl.is_stall");
        SC::trace(tf, out.ctl.is_valid,   "vif.out.ctl.is_valid");

        //--data signals
        SC::trace(tf, in.data.value,  "vif.in.data.value");
        SC::trace(tf, out.data.value, "vif.out.data.value");

        //--debug signals
        SC::trace(tf, in.debug.i_binary,    "vif.in.debug.i_binary");
        SC::trace(tf, in.debug.i_number,    "vif.in.debug.i_number");
        SC::trace(tf, in.debug.is_error,    "vif.in.debug.is_error");
        SC::trace(tf, in.debug.string,      "vif.in.debug.string");
        SC::trace(tf, out.debug.i_binary,   "vif.out.debug.i_binary");
        SC::trace(tf, out.debug.i_number,   "vif.out.debug.i_number");
        SC::trace(tf, out.debug.is_advance, "vif.out.debug.is_advance");
        SC::trace(tf, out.debug.string,     "vif.out.debug.string");

        //--end of compilation unit with implicit return
      }

    public:   // convenience classes
              // NB: these group signals into structured bundles
      struct mut_input_t
      { // inputs to DUT/MUT
        struct clock_t
        { // system clocking channel
          // NB: the clock is inherited from the test setup
          model::signal::clock_t &clock;
        };

        struct control_t
        { // control signal channel
          // NB: these signals are common to all pipeline stages.
          // NB: prefetch stage may have additional signals.
          // NB: prefetch stage sees "is_flush" as "branch".
          model::signal::reset_t      reset     {"ctl_reset"     };
          model::signal::is_advance_t is_advance{"ctl_is_advance"};
          model::signal::is_flush_t   is_flush  {"ctl_is_flush"  };
          model::signal::is_valid_t   is_valid  {"ctl_is_valid"  };
        };

        struct data_t
        { // data channel
          // NB: will be specific to each DUT/MUT
          model::signal::value_t value{"data_value"};
        };

        struct debug_t
        { // debug channel
          // NB: we may want to create a common debug in the model
          model::signal::i_binary_t i_binary{"dbg_i_binary"};
          model::signal::i_number_t i_number{"dbg_i_number"};
          model::signal::is_error_t is_error{"dbg_is_error"};
          SC::signal<::std::string> string  {"dbg_string"};
        };

        clock_t   clock;
        control_t ctl;
        data_t    data;
        debug_t   debug;
      };

      struct mut_output_t
      { // outputs from DUT/MUT
        struct control_t
        { // control signal channel
          // NB: these signals are common to all pipeline stages.
          // NB: register-read stage will have an output "is_valid"
          //     signal for each functional unit below it.
          // NB: functional units will have an output "is_flush"
          //     signal.
          ::sc_core::sc_signal_in_if<model::is_advance_t> &is_advance;
          ::sc_core::sc_signal_in_if<model::is_error_t>   &is_error;
          ::sc_core::sc_signal_in_if<model::is_stall_t>   &is_stall;
          ::sc_core::sc_signal_in_if<model::is_valid_t>   &is_valid;
        };

        struct data_t
        { // data channel
          // NB: will be specific to each DUT/MUT
          ::sc_core::sc_signal_in_if<model::value_t> &value;
        };

        struct debug_t
        { // debug channel
          ::sc_core::sc_signal_in_if<model::i_binary_t>   &i_binary;
          ::sc_core::sc_signal_in_if<model::i_number_t>   &i_number;
          ::sc_core::sc_signal_in_if<model::is_advance_t> &is_advance;
          ::sc_core::sc_signal_in_if<::std::string>       &string;
        };

        control_t ctl;
        data_t    data;
        debug_t   debug;
      };

    public:    // attributes
               // NB: these must follow their structure declarations
      mut_input_t   in;    // inputs to DUT/MUT
      mut_output_t  out;   // output from DUT/MUT

    private:   // methods
    private:   // local types and declarations
      using SC_CURRENT_USER_MODULE = ::vif::EXAMPLE::single_cycle_t;
    private:   // attributes
  };

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--end of invocation guard
#endif // not defined(__VIF_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
