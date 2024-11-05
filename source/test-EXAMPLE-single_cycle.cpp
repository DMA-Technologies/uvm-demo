//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
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
//  test-EXAMPLE-single_cycle.cpp
//  - test of EXAMPLE/single_cycle.h class.
//  - test code is implemented using SystemC, not UVM/SystemC.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--system includes
#include <string>

//--library includes
// <none>

//--project includes
#include "SC.h"
#include "model.h"
#include "EXAMPLE/single_cycle.h"
#include "TEST/EXAMPLE/single_cycle.h"

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

namespace sc_core
{
  inline void sc_trace(
    sc_trace_file *const tf     [[maybe_unused]],
    std::string const   &string [[maybe_unused]],
    std::string const   &name   [[maybe_unused]])
  {
    throw "sc_trace - we no wanna do this.";
    //sc_core::sc_trace(tf, string.c_str(), (name+".string"));
  }
}

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

int sc_main(int const    argc [[maybe_unused]],
            char** const argv [[maybe_unused]])
{
  //--logging
  logger::logger logger{};

  //--run conditions
  SC::time const clock_period{1.0, SC::sec};
  double const   clock_duty  {0.5};
  SC::time const clock_start {0.0, SC::sec};

  SC::time const run_time{ (1000 * 1000), SC::sec };

  //--signals
  std::cout << "Create signals." << std::endl;
  model::signal::clock_t       clock{"clock", clock_period, clock_duty, clock_start};

  //--module instances
  std::cout << "Create module instances." << std::endl;
  EXAMPLE::single_cycle_t       MUT0 {"EXAMPLE_single_cycle_0"};
  TEST::EXAMPLE::single_cycle_t test0{"test0", logger};

  //--collect exports (bound signals)
  auto &t0_ctl{test0.control_out()};
  auto &m0_ctl{MUT0.control_out()};
  auto &t0_data{test0.data_out()};
  auto &m0_data{MUT0.data_out()};
  auto &t0_dbg{test0.debug_out()};
  auto &m0_dbg{MUT0.debug_out()};

  //--bind signals
  std::cout << "Bind signals to EXAMPLE/single_cycle_0." << std::endl;
  MUT0.clock(clock);
  MUT0.control_in(t0_ctl.reset, t0_ctl.is_advance, t0_ctl.is_flush, t0_ctl.is_valid);
  MUT0.data_in(t0_data.value);
  MUT0.debug_in(t0_dbg.i_number, t0_dbg.i_binary, t0_dbg.is_error, t0_dbg.string);

  test0.clock(clock);
  test0.control_in(m0_ctl.is_advance, m0_ctl.is_error, m0_ctl.is_stall, m0_ctl.is_valid);
  test0.data_in(m0_data.value);
  test0.debug_in(m0_dbg.i_number, m0_dbg.i_binary, m0_dbg.is_advance, m0_dbg.string);

  //--run the thing
  std::cout << "Run the simulation." << std::endl;
  SC::start(run_time);
  std::cout << "Simulation is complete." << std::endl;

  //--diagnostic printout
  std::cout << "Diagnostics of various sorts." << std::endl;
  logger.output(std::cout);

  //--end of compilation unit with explicit return
  return  0;
}

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
