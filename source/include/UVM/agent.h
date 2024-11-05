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
//  agent.h
//  - A UVM based agent interfaces with several other components.
//    - For example, an agent often contains a monitor, a driver, and 
//      a sequencer for the DUTs/MUTs. 
//    - An agent uses the other components to provide an interface to 
//      specific DUTs/MUTs under test.
//    - Initially, the design of this is coded for a single-cycle module
//      namely EXAMPLE::single_cycle_t.
//    - An [active] agent will substantiate the 
//    - An [passive] agent will sub
// 
//    - NB: The sequencer supplies the driver sequences.
//    - NB: The monitor supplies the score_board(s) with information as to the
//      requests (inputs) and responses (outputs) of the DUTs/MUTs
//
//  NOTES:
//  2024-08-19: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__AGENT_H__)
# define __AGENT_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "driver.h"
# include "monitor.h"
# include "sequencer.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace agent
  {
    // base class for all agents; just because we hate boiler-plate.
    template<class DRIVER_T, class MONITOR_T, class SEQUENCER_T>
    class base_t;

    namespace EXAMPLE
    { // agents for modules in the ::EXAMPLE namespace
      class single_cycle_t;
    } // namespace ::EXAMPLE::agent
  } // namespace ::agent

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::agent::base_t
  // UVM environment base class for all DUTs/MUTs
  template<class DRIVER_T, class MONITOR_T, class SEQUENCER_T>
  class agent::base_t
    : public ::uvm::uvm_agent
  {
    public:     // forward declarations and type aliases
      using driver_t        = DRIVER_T;
      using monitor_t       = MONITOR_T;
      using sequencer_t     = SEQUENCER_T;
      using driver_ptr      = ::std::shared_ptr<driver_t>;
      using monitor_ptr     = ::std::shared_ptr<monitor_t>;
      using sequencer_ptr   = ::std::shared_ptr<sequencer_t>;

    public:     // UVM evil
    public:     // c'tors and d'tor
    public:     // accessors
      driver_t &driver() const
      { return  *m_driver; }
      monitor_t &monitor() const
      { return  *m_monitor; }
      sequencer_t &sequencer() const
      { return  *m_sequencer; }

    public:     // methods
      void build_phase(uvm::uvm_phase &phase) override
      {
        uvm::uvm_agent::build_phase(phase);

        m_monitor.reset(monitor_t::type_id::create("m_monitor", this));
        if  (not m_monitor)
          UVM_FATAL("NO_CREATE", "Failed to create monitor.");

        if  (get_is_active() == uvm::UVM_ACTIVE)
        {
          m_sequencer.reset(sequencer_t::type_id::create("m_sequencer", this));
          m_driver.reset(driver_t::type_id::create("m_driver", this));
          if  ((not m_sequencer) or (not m_driver))
            UVM_FATAL("NO_CREATE", "Failed to create sequencer/driver.");
        }
      }

      void connect_phase(uvm::uvm_phase &phase [[maybe_unused]]) override
      {
        if  (get_is_active() == ::uvm::UVM_ACTIVE)
          m_driver->connect_to(m_sequencer);
      }

    protected:  // c'tors and d'tor
      base_t(::uvm::uvm_component_name name)
        : uvm::uvm_agent{name}
        , m_driver{nullptr}
        , m_monitor{nullptr}
        , m_sequencer{nullptr}
      { /* noOp */ }

      virtual ~base_t() override
      { /* noOp */ }

    protected:  // accessors
    protected:  // methods
    protected:  // types and declarations
    protected:  // attributes
    private:    // methods
    private:    // types and declarations
    private:    // attributes
      driver_ptr    m_driver;
      monitor_ptr   m_monitor;
      sequencer_ptr m_sequencer;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::agent::EXAMPLE::single_cycle_t
  // test agent for module ::EXAMPLE::single_cycle_t

  class agent::EXAMPLE::single_cycle_t final
    : public ::agent::base_t
      <
       ::driver::EXAMPLE::single_cycle_t,
        ::monitor::EXAMPLE::single_cycle_t,
        ::sequencer::EXAMPLE::single_cycle_t
      >
  {
    public:     // forward declarations and type aliases
    public:     // UVM evil
      UVM_COMPONENT_UTILS(::agent::EXAMPLE::single_cycle_t);

    public:     // c'tors and d'tor
      single_cycle_t(::uvm::uvm_component_name name)
        : base_t{name}
      { /* noOp */ }

      virtual ~single_cycle_t() override
      { /* noOp */ }
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace agent
  {
    // <none>
  } // namespace ::agent

//1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--end of invocation guard
#endif // not defined(__AGENT_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
