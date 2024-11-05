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
//  environment.h
//  - UVM based environment for the monitors for all DUTs/MUTs
//    - Within is the single_cycle_t monitor.
//    - Within is the XXXXXX monitors.
// 
//
//  NOTES:
//  2024-08-19: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__ENVIRONMENT_H__)
# define __ENVIRONMENT_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "agent.h"
# include "monitor.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace environment
  {
    template<class VIRTUAL_INTERFACE_T>
    class base_t;

    namespace EXAMPLE
    {
      class single_cycle_t;
    } //namespace EXAMPLE
  } // namespace environment

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::environment::base_t
  // UVM environment base class for all DUTs/MUTs
  // NB: provides lookup for VIF, assuming it is required.

  template<class VIRTUAL_INTERFACE_T>
  class environment::base_t
    : public uvm::uvm_env
  { // basic interface for all test environments
    public:     // forward declarations and type aliases
      using vif_t   = VIRTUAL_INTERFACE_T;
      using vif_ptr = vif_t*;

    public:     // c'tors and d'tor
      base_t(::uvm::uvm_component_name name)
        : uvm_env{name}
        , m_vif{nullptr}
      { /* noOp */ }

      virtual ~base_t()
      {
        m_vif = nullptr;
      }

      base_t(base_t const &) = delete;
      base_t &operator=(base_t const &) = delete;

    public:     // methods
      void build_phase(uvm::uvm_phase& phase) override
      {
        uvm_env::build_phase(phase);

        if (not uvm::uvm_config_db<vif_ptr>::get(this, "", "vif", m_vif))
          uvm_report_fatal(
            "NO_VIF",
            "Virtual interface not found in config_db.");
      }

    protected:  // methods
      vif_t &vif() const
      { return  *m_vif; }

    protected:  // declarations
    protected:  // attributes
    private:    // methods
    private:    // declarations
    private:    // attributes
      vif_ptr  m_vif;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::environment::EXAMPLE::single_cycle_t
  // test environment for module ::EXAMPLE::single_cycle_t

  class environment::EXAMPLE::single_cycle_t 
    : public ::environment::base_t<::vif::EXAMPLE::single_cycle_t>
  { // instance class for first pass of UVM development
    private:  // forward declarations and type aliases
      using agent_t     = ::agent::EXAMPLE::single_cycle_t;
      using agent_ptr   = ::std::shared_ptr<agent_t>;
      using monitor_t   = ::monitor::EXAMPLE::single_cycle_t;
      using monitor_ptr = ::std::shared_ptr<monitor_t>;

    public:  // UVM evil
      UVM_COMPONENT_UTILS(::environment::EXAMPLE::single_cycle_t);

    public:  // c'tors and d'tor
      single_cycle_t(::uvm::uvm_component_name name)
        : base_t{name}
        , m_agent{}
        , m_monitor{}
      { /* noOp */ }

      virtual ~single_cycle_t() override
      { /* noOp */ }

    public:  // accessors
      agent_t &agent()
      { return  *m_agent; }
      monitor_t &monitor()
      { return  *m_monitor; }

    public:  // methods
      void build_phase(uvm::uvm_phase &phase) override
      {
        base_t::build_phase(phase);

        m_monitor.reset(
          monitor_t::type_id::create("m_monitor", this));
        m_agent.reset(
          agent_t::type_id::create("m_agent", this));
      }

    private:  // attributes
      agent_ptr    m_agent;
      monitor_ptr  m_monitor;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace environment
  {
    // <none>
  } // namespace ::environment

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__ENVIRONMENT_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
