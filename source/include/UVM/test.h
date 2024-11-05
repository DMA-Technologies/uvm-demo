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
//  test.h
//  - UVM based tests for all DUTs/MUTs
//
//  NOTES:
//  2024-08-24: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__TEST_H__)
# define __TEST_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "sequence.h"
# include "test_bench.h"
# include "utility.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace test
  { // all defined tests live in this namespace at some level
    //--base test class; provides useful common behaviour.
    template<class TEST_BENCH_T>
    class base_t;
  } // namespace ::test

  namespace test::EXAMPLE
  { // tests defined for modules in the ::EXAMPLE namespace.
    //--comprehensive test for the ::EXAMPLE::single_cycle_t module.
    class single_cycle_t;
  } // namespace ::test::EXAMPLE

  namespace test::MODULE
  { // tests for processor modules
    //--test for the ::MODULE::memory_controller_t processor module;
    class memory_controller_t;
  } // namespace ::test::MODULE

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::test::base_t
  // base class for all tests.
  // NB: provides basic common behaviour for all tests.

  template<class TEST_BENCH_T>
  class test::base_t
    : public ::uvm::uvm_test
  { // base class for all tests
    public:    // abstracting types
      using printer_t      = ::uvm::uvm_table_printer;
      using printer_ptr    = ::std::shared_ptr<printer_t>;
      using test_bench_t   = TEST_BENCH_T;
      using test_bench_ptr = ::std::shared_ptr<test_bench_t>;

    public:    // UVM evil
    public:    // c'tors and d'tor
      base_t(::uvm::uvm_component_name name)
        : ::uvm::uvm_test(name)
        , m_pass{false}
        , m_printer{}
        , m_test_bench{} // Like several test_bench(s) 
      { /* noOp */ }

      ~base_t() override
      {
        m_pass = false;  // destroy the value
      }

    public:    // accessors
      bool is_pass() const
      { return  m_pass; }
      printer_t &printer() const
      { return  *m_printer; }
      test_bench_t &test_bench() const
      { return  *m_test_bench; }

    public:    // methods
      virtual void build_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        uvm_test::build_phase(phase);

        // Enable transaction recording for everything (Not SURE What)
        ::uvm::uvm_config_db<int>::set(this, "*", "recording_detail", uvm::UVM_FULL);

        // Create the test bench (Like will require several different test_bench(s)
        //  maybe name them for the type like single_cycle_t, decoder_t, memory_controller_t
        m_test_bench.reset(test_bench_t::type_id::create("test_bench", this));

        // Create a specific depth printer for printing the created topology
        m_printer.reset(new printer_t{});
        m_printer->knobs.depth = 7;
      }

      void end_of_elaboration_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        uvm_test::end_of_elaboration_phase(phase);
          
        //--a diagnostic
        uvm_report_info(
          "TOPOLOGY",
          ("Test topology :\n" + sprint(m_printer.get())),
          uvm::UVM_LOW);
      }

      void report_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        uvm_test::report_phase(phase);

        //--top level reporting - success or fail?
        if  (is_pass())
          uvm_report_info("PASS/FAIL",
                          "** UVM TEST PASSED **",
                          uvm::UVM_NONE);
        else
          uvm_report_error("PASS/FAIL",
                           "** UVM TEST FAIL **");

        //--return code
        {
          using RC_t = ::utility::uvm_return_code_t;
          auto result{is_pass() ? RC_t::success : RC_t::test_fail};
          auto return_code{static_cast<int>(result)};
          ::uvm::uvm_config_db<int>::set(
            nullptr,
            "",
            "return_code",
            return_code);
        }
      }

    protected:  // accessors
      bool pass() const
      { return  m_pass; }
      void pass(bool const value)
      { m_pass = value; }

    protected:  // methods
    protected:  // types and declarations
    protected:  // attributes
    private:    // methods
    private:    // types
    private:    // attributes
      bool            m_pass;
      printer_ptr     m_printer;
      test_bench_ptr  m_test_bench;
  }; // class ::test::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::test::EXAMPLE::single_cycle_t
  // all tests for the ::EXAMPLE::single_cycle_t module.

  class test::EXAMPLE::single_cycle_t final
    : public ::test::base_t<::test_bench::EXAMPLE::single_cycle_t>
  { // base class for all tests
    public:    // UVM evil
      UVM_COMPONENT_UTILS(::test::EXAMPLE::single_cycle_t);

    public:    // c'tors and d'tor
      single_cycle_t(::uvm::uvm_component_name name)
        : base_t{name}
      { /* noOp */ }

      ~single_cycle_t() override
      { /* noOp */ }

    public:    // accessors
    public:    // methods
      virtual void build_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        //--NB: builds the test_bench
        base_t::build_phase(phase);
      }

      void end_of_elaboration_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        //--NB: prints the test topology
        base_t::end_of_elaboration_phase(phase);
      }

      void run_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        base_t::run_phase(phase);

        phase.raise_objection(this);
        auto sequencer{&(test_bench().environment().agent().sequencer())};

        if  (true)
        {
          namespace seq_t = ::sequence::EXAMPLE::single_cycle_t;
          using seq_item_t = ::sequence_item::EXAMPLE::single_cycle_t;

          ::std::vector<::sequence::base_t<seq_item_t>*> sequences
          {
            new ::sequence::repeat_t<seq_item_t>,
            new ::sequence::repeat_t<seq_item_t, 1>{},
            new ::sequence::repeat_t<seq_item_t>{5},
            new seq_t::all_control_pairs_t{},
            new seq_t::random_weighted_t{},
            new seq_t::random_loop_t{},
            new seq_t::flush_w_advance_never_advances_t{},
            new seq_t::error_w_advance_valid_t{},
            new seq_t::reset_advance_with_out_valid_t{},
          };

          for (auto sequence: sequences)
          {
            uvm_report_info("SEQUENCE_START", sequence->get_name());
            sequence->start(sequencer);
          }
        }

        phase.drop_objection(this);

        //--end of compilation unit with implicit return
      }

      void extract_phase(uvm::uvm_phase &phase) override
      {
        //--defer to parent class
        base_t::extract_phase(phase);

        //--extract: the test bench knows about passing!
        //--NB: this depends on the test topology.
        pass(test_bench().is_pass());
      }

      void report_phase(uvm::uvm_phase& phase) override
      {
        //--defer to parent class
        //--NB: reports pass/fail of the test
        base_t::report_phase(phase);
      }
  }; // class ::test::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace test
  {
    // <none>
  } // namespace ::test

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__TEST_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
