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
//  test_bench.h
//  - UVM based test benches for all DUTs/MUTs
//
//  NOTES:
//  2024-08-24: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__TEST_BENCH_H__)
# define __TEST_BENCH_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "environment.h"
# include "score_board.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace test_bench
  { // test benches
    //--base test_bench class; provides useful common behaviour
    template<class ENVIRONMENT_T>
    class base_t;
  } // namespace ::test_bench

  namespace test_bench::EXAMPLE
  { // test benches for modules in the ::EXAMPLE namespace.
    //--test bench for the ::EXAMPLE::single_cycle_t module.
    class single_cycle_t;
  } // namespace ::test_bench::EXAMPLE

  namespace test_bench::MODULE
  { // test benchs for processor modules
    //--test bench for the ::MODULE::memory_controller_t module;
    class memory_controller_t;
  } // namespace ::test_bench::MODULE

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::test_bench::base_t<ENVIRONMENT_T>
  // abstract base class for test benches.
  // NB: creates a nested environment which is specialiised by the derived
  //     test bench classes.

  template<class ENVIRONMENT_T>
  class test_bench::base_t
    : public ::uvm::uvm_env
  {
    public:     // uvm evil
    public:     // c'tors and d'tor
      base_t(::uvm::uvm_component_name name)
        : ::uvm::uvm_env{name}
        , m_environment{}
      { /* noOp */ }

      ~base_t() override
      { /* noOp */ }

    public:     // accessors
      virtual bool is_pass() const = 0;

      auto &environment() const
      { return  *m_environment; }

    public:     // phase methods
      virtual void build_phase(::uvm::uvm_phase &phase) override
      {
        //--parent class build phase
        uvm_env::build_phase(phase);

        //--create the environment
        m_environment.reset(
          environment_t::type_id::create("m_environment", this));
      }

    private:   // methods
    private:   // types
      using environment_t   = ENVIRONMENT_T;
      using environment_ptr = ::std::shared_ptr<environment_t>;

    private:   // attributes
      environment_ptr m_environment;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // 
  
  class test_bench::EXAMPLE::single_cycle_t final
    : public ::test_bench::base_t<::environment::EXAMPLE::single_cycle_t>
  {
    public:     // uvm evil
      UVM_COMPONENT_UTILS(::test_bench::EXAMPLE::single_cycle_t);

    public:     // c'tors and d'tor
      single_cycle_t(::uvm::uvm_component_name name)
        : base_t{name}
        , m_sb_controls{}
        , m_sb_values{}
        , m_sb_pairs{}
      { /* noOp */ }

      ~single_cycle_t() override
      { /* noOp */ }

    public:    // accessors
      virtual bool is_pass() const override
      {
        bool result{true};
        result = (result and m_sb_controls and m_sb_controls->is_pass());
        result = (result and m_sb_values and m_sb_values->is_pass());
        result = (result and m_sb_pairs and m_sb_pairs->is_pass());
        return  result;
      }

    public:    // methods
      virtual void build_phase(::uvm::uvm_phase& phase) override
      {
        //--parent class build phase
        base_t::build_phase(phase);

        //--set test bench configuration parameters
        // EXAMPLE: uvm::uvm_config_db<int>::set(this, "ubus0", "num_masters", 1);
        // EXAMPLE: uvm::uvm_config_db<int>::set(this, "ubus0", "num_slaves", 1);

        //--create the scoreboards
        //--theoretically can use multiple scoreboards,
        //  depending on what we want to measure
        m_sb_controls.reset(
          sb_controls_t::type_id::create("m_sb_controls", this));
        m_sb_values.reset(
          sb_values_t::type_id::create("m_sb_values", this));
        m_sb_pairs.reset(
          sb_pairs_t::type_id::create("m_sb_pairs", this));
      }

      void connect_phase(::uvm::uvm_phase& phase) override
      { // hook up signalling
        //--parent class connect phase
        uvm_env::connect_phase(phase);

        //--connect score-board(s) to suitable monitors
        m_sb_controls->connect_to(environment().monitor());
        m_sb_values->connect_to(environment().monitor());
        m_sb_pairs->connect_to(environment().monitor());
      }

    private:   // methods
    private:   // types
      using sb_controls_t = ::score_board::EXAMPLE::single_cycle_t::control_bits_t;
      using sb_controls_ptr = ::std::shared_ptr<sb_controls_t>;

      using sb_values_t = ::score_board::EXAMPLE::single_cycle_t::data_values_t;
      using sb_values_ptr = ::std::shared_ptr<sb_values_t>;

      using sb_pairs_t = ::score_board::EXAMPLE::single_cycle_t::control_bit_pairs_t;
      using sb_pairs_ptr = ::std::shared_ptr<sb_pairs_t>;

     private:   // attributes
      sb_controls_ptr m_sb_controls;
      sb_values_ptr   m_sb_values;
      sb_pairs_ptr    m_sb_pairs;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace test_bench
  {
    // <none>
  } // namespace ::test_bench

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__TEST_BENCH_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
