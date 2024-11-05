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
//  predictor.h
//  - UVM based predictors for all DUTs/MUTs
//
//  NOTES:
//  2024-07-31: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__PREDICTOR_H__)
# define __PREDICTOR_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "model.h"
# include "transaction.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace predictor
  {
    namespace EXAMPLE
    {
      namespace single_cycle_t
      {
#if true
        struct controls_t;
        struct state_t;
#else
        struct controls_t
        {
          ::model::is_advance_t is_advance;
          ::model::is_error_t   is_error;
          ::model::is_stall_t   is_stall;
          ::model::is_valid_t   is_valid;
        };

        struct state_t
        {
          ::model::is_advance_t is_advance;
          ::model::is_error_t   is_error;
          ::model::is_stall_t   is_stall;
          ::model::is_valid_t   is_valid;
          ::model::value_t      value;
        };
#endif

        using transaction_t = ::transaction::single_cycle_t;

        controls_t predict_controls(transaction_t const &transaction);
        state_t predict_state(transaction_t const &transaction);
      } // namespace predictor::EXAMPLE::single_cycle_t
    } // namespace predictor::EXAMPLE
  } // namespace predictor

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--struct ::predictor::EXAMPLE::single_cycle_t::controls_t
  //--data container for predicted control bit outputs for the
  //--::EXAMPLE::single_cycle_t class.

  struct predictor::EXAMPLE::single_cycle_t::controls_t final
  {
    ::model::is_advance_t is_advance;
    ::model::is_error_t   is_error;
    ::model::is_stall_t   is_stall;
    ::model::is_valid_t   is_valid;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--struct ::predictor::EXAMPLE::single_cycle_t::state_t
  //--data container for all predicted outputs for the
  //--::EXAMPLE::single_cycle_t class.
  //--NB: by intent, the "debug" outputs are not included.

  struct predictor::EXAMPLE::single_cycle_t::state_t final
  {
    ::model::is_advance_t is_advance;
    ::model::is_error_t   is_error;
    ::model::is_stall_t   is_stall;
    ::model::is_valid_t   is_valid;
    ::model::value_t      value;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--function ::predictor::EXAMPLE::single_cycle_t::predict_controls(...)
  //--predicts control signal outputs for the ::EXAMPLE::single_cycle_t
  //--class based on the DUT/MUT observed state and the input stimuli
  //--contained in the given transaction.
  //--NB: defers to the complete state predicctor.
  //--NB: was implemented stand-alone during initial development.

  inline predictor::EXAMPLE::single_cycle_t::controls_t
    predictor::EXAMPLE::single_cycle_t::predict_controls(
      transaction_t const &transaction)
  {
    auto state{predict_state(transaction)};
    return  {state.is_advance, state.is_error, state.is_stall, state.is_valid,};
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--function ::predictor::EXAMPLE::single_cycle_t::predict_state(...)
  //--predicts the complete output state of the ::EXAMPLE::single_cycle_t
  //--class based on the DUT/MUT observed state and the input stimuli
  //--contained in the given transaction.

  inline predictor::EXAMPLE::single_cycle_t::state_t
    predictor::EXAMPLE::single_cycle_t::predict_state(
      transaction_t const &transaction)
  { // predict a single-cycle module's control signal output based on:
    //   1) starting state (is_error, is_stall, is_valid)
    //   2) input control signals (reset, ..., prior_is_valid)
    //--shorthand access into the transaction
    auto &x_value{transaction.value()};
    auto &state_0{x_value.state_0};
    auto &stimuli{x_value.stimuli};

    //--a couple useful shorthands
    bool const reset         {stimuli.ctl.reset == 0};
    bool const reset_or_flush{reset or stimuli.ctl.is_flush};

    //--expected combinatoric advance at start of MUT "cycle"
    bool const advance{(not state_0.ctl.is_stall) and
                        ((not state_0.ctl.is_valid) or (stimuli.ctl.is_advance))};

    bool const error  {(not reset) and
                        (state_0.ctl.is_error or stimuli.debug.is_error)};

    bool const stall  {false};  // a single cycle module cannot stall

    bool const valid  {(not reset_or_flush) and
                        ((advance and stimuli.ctl.is_valid) or
                        (not advance and state_0.ctl.is_valid))};

    auto const value  {advance ? (compl stimuli.data.value)
                                : state_0.data.value};

    //--end of compilation unit with explicit return
    return  {advance, error, stall, valid, value,};
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace predictor
  {
    // <none>
  } // namespace predictor

//--end of invocation guard
#endif // not defined(__PREDICTOR_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
