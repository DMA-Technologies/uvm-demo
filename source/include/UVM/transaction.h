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
//  transaction.h
//  - 
//
//  NOTES:
//  2024-07-20: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__TRANSACTION_H__)
# define __TRANSACTION_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "model.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace transaction
  { // reports from the monitors
    template<typename TYPE_T, TYPE_T ugly_value>
    class basic_t;                   // a basic transaction wrapper which guards
                                     // against uninitialised values.

    struct single_cycle_signals_t;   // raw transaction data for the class
                                     // ::EXAMPLE::single_cycle

    //--declarations which are deferred to the end of this file
    //--NB: resolution of circular dependencies.
    //      Code is here for reference for programmers ease of location.
#if  false
    using single_cycle_t
      = basic_t<single_cycle_signals_t, single_cycle_signals_t{}>;
#endif
  } // namespace transaction

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::transaction::basic_t<type, ugly_value>
  //--wraps transaction data to insure that values are assigned and stable.
  //--NB: intended to catch development errors which would induce errors due to
  //--    uninitialised data.

  template<class TYPE_T, TYPE_T ugly_value>
  class transaction::basic_t final
    : public ::uvm::uvm_transaction    // NB: required by UVM transaction handling
  {
    public:  // exposed types
      using value_t = TYPE_T;

    public:  // c'tors and d'tor
      basic_t()
        : uvm_transaction()
        , m_good{false}
        , m_value{ugly_value}
      { /* noOp */ }

      explicit basic_t(::uvm::uvm_object_name name)
        : uvm_transaction(name)
        , m_good{false}
        , m_value{ugly_value}
      { /* noOp */ }

      virtual ~basic_t()
      { reset(); }

    public:  // accessors
      operator bool() const
      { return  good(); }

      bool good() const
      { return  m_good; }

      value_t const &value() const
      { // get transaction value (contents)
        if (not good())
        { // NB: macro UVM_FATAL is porly composed; braces prevent pain.
          UVM_FATAL("NO_GOOD", "Transaction value is not set.");
        }
        return  m_value;
      }

      void value(value_t const &value)
      { // set transaction value (contents)
        ((m_good = true), (m_value = value));
      }

    public:  // methods
      void invalidate()
      { // transaction is not good
        m_good = false;
      }

      void reset()
      { // reset transaction value; it is no longer good.
        ((m_good = false), (m_value = ugly_value));
      }

    private:  // attributes
      bool     m_good;    // good bit, managed internally
      value_t  m_value;   // luser assigned value
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--struct :: transaction::...
  //--all input and output signals for the ::EXAMPLE::single_cycle class.
  //--ought suitable for all scoreboard processing.

  struct transaction::single_cycle_signals_t
  { // inputs and (one cycle later) outputs to/from DUT/MUT
    struct mut_input_stimuli_t
    { // sampled input stimului (signals) to the DUT/MUT
      struct control_t
      { // control signal channel
        // NB: these signals are common to all pipeline stages.
        // NB: prefetch stage may have additional signals.
        // NB: prefetch stage sees "is_flush" as "branch".
        ::model::reset_t      reset;
        ::model::is_advance_t is_advance;
        ::model::is_flush_t   is_flush;
        ::model::is_valid_t   is_valid;
      };

      struct data_t
      { // data channel
        // NB: will be specific to each DUT/MUT
        ::model::value_t value;
      };

      struct debug_t
      { // debug channel
        // NB: we may want to create a common debug in the model
        ::model::i_binary_t i_binary;
        ::model::i_number_t i_number;
        ::model::is_error_t is_error;
        //::std::string       string;
      };

      control_t ctl;
      data_t    data;
      debug_t   debug;
    };

    struct mut_output_state_t
    { // sampled outputs from DUT/MUT one cycle later
      struct control_t
      { // control signal channel
        // NB: these signals are common to all pipeline stages.
        // NB: register-read stage will have an output "is_valid"
        //     signal for each functional unit below it.
        // NB: functional units will have an output "is_flush"
        //     signal.
        ::model::is_advance_t is_advance;
        ::model::is_error_t   is_error;
        ::model::is_stall_t   is_stall;
        ::model::is_valid_t   is_valid;
      };

      struct data_t
      { // data channel
        // NB: will be specific to each DUT/MUT
        ::model::value_t value;
      };

      struct debug_t
      { // debug channel
        ::model::i_binary_t   i_binary;
        ::model::i_number_t   i_number;
        ::model::is_advance_t is_advance;
        //::std::string         string;
      };

      control_t ctl;
      data_t    data;
      debug_t   debug;
    };

    mut_output_state_t  state_0;
    mut_input_stimuli_t stimuli;
    mut_output_state_t  state_1;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace transaction
  {
    using single_cycle_t =
      basic_t<single_cycle_signals_t, single_cycle_signals_t{}>;
  } // namespace transaction

//--end of invocation guard
#endif // not defined(__TRANSACTION_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
