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
//  uvm-EXAMPLE-single_cycle.cpp
//  - UVM applied to the example single_cycle module.
//
//  RETURN CODES:
//    0: unqualified success.
//    1: uvm fatality; implicit return code of one (1).
//         see: UVM_FATAL macro.
//         see: uvm_report_fatal function
//    2: test failed.
//         e.g. coverage was incomplete.
//         e.g. a signal value test failed.
//    3: the test did not set a return code.
//         NB: should not happen, as the return code is set by the
//             ::test::base_t::report_phase(...) method.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--required system includes
#include <iostream>
#include <memory>
#include <string>

//--required library includes
// <none>

//--required project includes
#include "SC.h"
#include "UVM_wrapper.h"

#include "EXAMPLE/single_cycle.h"
#include "UVM/test.h"

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

namespace sc_core
{
  inline void sc_trace(
    sc_trace_file *const tf    [[maybe_unused]],
    ::std::string const &value [[maybe_unused]],
    ::std::string const &name  [[maybe_unused]])
  {
    //throw "sc_trace - we no wanna do ::std::string.";
    ::sc_core::sc_trace(tf, value.c_str(), (name+".string"));
  }
}

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

int sc_main(int const    argc [[maybe_unused]],
            char **const argv [[maybe_unused]])
{ // cannot odify argc,
  // cannot modify argv,
  // can modify individual members of argv and strings at the bottom
  // see manpage for getopt_long(3)

  //--shorthands
  using vif_t = ::vif::EXAMPLE::single_cycle_t;
  using vif_ptr = vif_t*;

  //--run conditions
  SC::time const clock_period{1.0, SC::nsec};
  double const   clock_duty  {0.5};
  SC::time const clock_start {0.0, SC::nsec};

  SC::time const run_time    {(1 * 1000 * 1000), SC::nsec};

  //--top-level signals
  std::cout << "Create top-level signals." << std::endl;
  model::signal::clock_t clock{"clock",
                               clock_period,
                               clock_duty,
                               clock_start};

  //--module under test (MUT) instance
  std::cout << "Create module under test." << std::endl;
  EXAMPLE::single_cycle_t mut{"EXAMPLE_single_cycle_0"};

  //--collect exports (bound signals)
  std::cout << "Collect exported signals from module." << std::endl;
  auto &m0_control{mut.control_out()};
  auto &m0_data   {mut.data_out()   };
  auto &m0_debug  {mut.debug_out()  };

  //--build up the interface used to control and sample the DUT/MUT
  //--NB: vif constructor takes DUG/MUT exports
  std::cout << "Create VIF, binds clock and exports from MUT." << std::endl;
  vif_t vif
  {
    "VIF",
    clock,
    m0_control.is_advance,
    m0_control.is_error,
    m0_control.is_stall,
    m0_control.is_valid,
    m0_data.value,
    m0_debug.i_binary,
    m0_debug.i_number,
    m0_debug.is_advance,
    m0_debug.string
  };

  //--hook up the DUT/MUT inputs from the vif
  std::cout << "Bind input signals to ports in the MUT" << std::endl;
  mut.clock(vif.in.clock.clock);
  mut.control_in(vif.in.ctl.reset,
                 vif.in.ctl.is_advance,
                 vif.in.ctl.is_flush,
                 vif.in.ctl.is_valid);
  mut.data_in   (vif.in.data.value);
  mut.debug_in  (vif.in.debug.i_number,
                 vif.in.debug.i_binary,
                 vif.in.debug.is_error,
                 vif.in.debug.string);

  //---insert necessary information into the configuration database
  // agents are active
  uvm::uvm_config_db<bool>::set(uvm::uvm_root::get(), "*", "is_active", true);
  // VIF address
  uvm::uvm_config_db<vif_ptr>::set(uvm::uvm_root::get(), "*", "vif", &vif);

  //--tracing
  ::std::shared_ptr<SC::trace_file> tf
  {
    ::sc_core::sc_create_vcd_trace_file("/tmp/trace_file"),
    [](auto victim) {::sc_core::sc_close_vcd_trace_file(victim);}
  };
  vif.trace_to_file(tf.get());

  //--setup and run the test
  uvm::uvm_coreservice_t::get()->get_root()->set_timeout(run_time);
  uvm::run_test("::test::EXAMPLE::single_cycle_t");

  //--result code
  int return_code{(int)::utility::uvm_return_code_t::not_set};
  ::uvm::uvm_config_db<int>::get(nullptr, "", "return_code", return_code);

  //--various cleanups
  // NB: tf is implicitly cleaned up.

  //--end of compilation unit with explicit return
  return  return_code;
}

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
