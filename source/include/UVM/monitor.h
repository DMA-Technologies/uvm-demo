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
//  monitor.h
//  - UVM based monitors for all DUTs/MUTs
//
//  NOTES:
//  2024-08-02: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__MONITOR_H__)
# define __MONITOR_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "vif.h"
# include "transaction.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace monitor
  {
    template <class TRANSACTION_T>
    class base_t;

    // EXAMPLE with single_cycle_t
    namespace EXAMPLE
    {
      class single_cycle_t;
    }
  } // namespace monitor

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--ckass ....
  //--<description here>
  //--<notes here>
  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  template <class TRANSACTION_T>
  class monitor::base_t
    : public ::uvm::uvm_monitor
  {
    public:     // forward declarations, &c.
      using transaction_t = TRANSACTION_T;
      using output_port_t = ::uvm::uvm_analysis_port<transaction_t>;
      using vif_t         = ::vif::EXAMPLE::single_cycle_t;
      using vif_ptr       = vif_t*;

    public:     // UVM evil
    public:     // c'tors and d'tor
    public:     // accessors
      output_port_t &output()
      { // get port to receive sampled data from the monitor
        return  m_output_port;
      }

    public:     // methods
    protected:  // c'tors and d'tor
      base_t(::uvm::uvm_component_name name)
        : uvm_monitor(name)
        , m_output_port("m_output_port")
        , m_vif{nullptr}
      { /* noOp */ }

      virtual ~base_t() override
      { // deleted objects tell no tales
        m_vif = nullptr;  // NB: drop, do not destroy
      }

      base_t() = delete;                // no empty construction
      base_t(base_t const&) = delete;   // no copy construction
      base_t(base_t&&) = delete;        // no move construction (lazy)
      base_t &operator=(base_t const&) = delete;   // no copy assignment
      base_t &operator=(base_t&&) = delete;        // no move assignment (lazy)

    protected:  // accessors
      vif_t &vif()
      { // so you want the VIF?
        return  *m_vif;
      }

    protected:  // methods
      void build_phase(::uvm::uvm_phase& phase) override
      {
        //--parent class behaviour
        uvm_monitor::build_phase(phase);

        //--implicitly connect to the DUT bus
        if (not ::uvm::uvm_config_db<vif_ptr>::get(this, "", "vif", m_vif))
          UVM_FATAL("NO_VIF", "Virtual interface not found in config_db.");

        //--end of compilation unit with implicit return
      }

    protected:  // type and class definitions
    protected:  // attributes
    private:    // methods
    private:    // type and class definitions
    private:    // attributes
      output_port_t m_output_port;  // port for sampled transactions
      vif_ptr       m_vif;          // virtual bus interface
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  class monitor::EXAMPLE::single_cycle_t final
    : public ::monitor::base_t<::transaction::single_cycle_t>
  { // input/output monitor
    // NB: collects DUT/MUT inputs and outputs every cycle.
    // NB: drops first cycle sample, as there is simply no valid signal input.
    public:     // forward declarations, &c.
    public:     // UVM evil
      UVM_COMPONENT_UTILS(::monitor::EXAMPLE::single_cycle_t);

    public:     // c'tors and d'tor
      single_cycle_t(uvm::uvm_component_name name)
        : base_t(name)
      { /* noOp */ }

      virtual ~single_cycle_t() override
      { /* noOp */ }

    public:     // accessors
    protected:  // type and class definitions
    protected:  // attributes
    protected:  // methods
      void run_phase(uvm::uvm_phase &phase) override
      {
        base_t::run_phase(phase);
        SC_FORK
          sc_spawn(sc_bind(&single_cycle_t::collect_transactions, this)),
        SC_JOIN
      }

    private:    // methods
      void collect_transactions()
      { // collect inputs and outputs for every cycle the DUT/MUT executes.
        enum class state_t
        {
          complete,     // the monitor can die now
          start,        // start-up and one-time initialisation
          tr_initialise,// initialise sampled signals
          sample_begin, // sample transaction beginning (state and stimuli)
          wait,         // wait for a valid output to capture
          tr_begin,     // UVM mechanics
          sample_end,   // sample transaction completion state
          tr_end,       // UVM mechanics
          transmit,     // transmit the transaction to all listeners
        };

        transaction_t::value_t signals;
        state_t                state{state_t::start};
        transaction_t          transaction;

        while  (state != state_t::complete)
        {
          switch  (state)
          {
            case  state_t::complete:  // uh oh!  should never get here
              UVM_FATAL("STATE_BAD", "state_t::complete reached we stopped looping for transactions.");
              break;

            case  state_t::start:  // one-time start-up initialisation
              ::sc_core::wait(vif().clock_posedge());
              state = state_t::tr_initialise;
              break;

            case  state_t::tr_initialise:  // transaction start-up initialisilation
              transaction.reset();
              signals.state_1 = sample_state();
              state = state_t::sample_begin;
              break;

            case  state_t::sample_begin:  // sample input signals
              transaction.invalidate();            // we have incomplete data
              signals.state_0 = signals.state_1;   // capture start state and input
              signals.stimuli = sample_stimuli();
              state = state_t::wait;
              [[fallthrough]];

            case  state_t::wait:  // wait for output to capture
              ::sc_core::wait(vif().clock_posedge());
              state = state_t::tr_begin;
              [[fallthrough]];

            case  state_t::tr_begin:  // UVM mechanics - start the transaction
              begin_tr(transaction);
              state = state_t::sample_end;
              [[fallthrough]];

            case  state_t::sample_end:  // sample output signals
              signals.state_1 = sample_state();
              transaction.value(signals);
              state = state_t::tr_end;
              [[fallthrough]];

            case  state_t::tr_end:  // UVM mechanics - end the transaction
              end_tr(transaction);
              state = state_t::transmit;
              [[fallthrough]];

            case  state_t::transmit:
              output().write(transaction);
              state = state_t::sample_begin;
              break;

            default:
              UVM_FATAL("STATE_INVALID",
                        "Whisky Tango Foxtrot - how did we get here?");
              break;
          }
        }
      }

      transaction_t::value_t::mut_output_state_t sample_state()
      { // sample state that is currently output
        return 
        { // all output signals, excluding debug.string
          { // control signals
            vif().out.debug.is_advance.read(),
            vif().out.ctl.is_error.read(),
            vif().out.ctl.is_stall.read(),
            vif().out.ctl.is_valid.read(),
          },
          { // data signals
            vif().out.data.value.read(),
          },
          { // debug signals
            vif().out.debug.i_binary.read(),
            vif().out.debug.i_number.read(),
            vif().out.debug.is_advance.read(),
            //vif().in.debug.string.read(),
          },
        };
      } // END sample_state 

      transaction_t::value_t::mut_input_stimuli_t sample_stimuli()
      { // sample stimuli being fed into the DUT/MUT
        return
        { // all input signals, excluding debug.string
          { // control signals
            vif().in.ctl.reset.read(),
            vif().in.ctl.is_advance.read(),
            vif().in.ctl.is_flush.read(),
            vif().in.ctl.is_valid.read(),
          },
          { // data signals
            vif().in.data.value.read(),
          },
          { // debug signals, excluding string
            vif().in.debug.i_binary.read(),
            vif().in.debug.i_number.read(),
            vif().in.debug.is_error.read(),
            //vif().in.debug.string.read(),
          },
        };
      } // END sample_stimuli 

    private:    // type and class definitions
    private:    // attributes
  }; // class ::monitor::data_output_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace monitor
  {
    // <none>
  } // namespace monitor

//--end of invocation guard
#endif // not defined(__MONITOR_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
