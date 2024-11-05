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
//  SC.h
//  - restricted SystemC interface.
//  - restriction of items keeps life saner, we hopes.
//
//  NOTES:
//  2023-06-20: Moore and Peimann: first cut.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if not defined(__SC_H__)
# define __SC_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
# if  defined(__GNUC__)
#   pragma GCC diagnostic push

#   pragma GCC diagnostic ignored "-Warith-conversion"
#   pragma GCC diagnostic ignored "-Wcast-align"
#   pragma GCC diagnostic ignored "-Wcast-qual"
#   pragma GCC diagnostic ignored "-Weffc++"
#   pragma GCC diagnostic ignored "-Wfloat-equal"
#   pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#   pragma GCC diagnostic ignored "-Wredundant-decls"
#   pragma GCC diagnostic ignored "-Wshadow"
#   pragma GCC diagnostic ignored "-Wsign-conversion"
#   pragma GCC diagnostic ignored "-Wswitch-enum"
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Wuseless-cast"
# endif

# include <systemc>

# if  defined(__GNUC__)
#   pragma GCC diagnostic pop
# endif

  //--required project includes
  // <none>

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace SC
  {
    //--types (and classes) from SystemC
    //using bit = ::sc_dt::sc_bit;      // sc_bit is deprecated

    template <int W>
    using bv = ::sc_dt::sc_bv<W>;

    using clock = ::sc_core::sc_clock;

    using event = ::sc_core::sc_event;

    template <typename IF>
    using exprt = ::sc_core::sc_export<IF>;

    template <typename T>
    using in = ::sc_core::sc_in<T>;

    //template <typename T>             //--sc_inout cannot synthesize
    //using inout = ::sc_core::sc_inout<T>;

    using module = ::sc_core::sc_module;
    using module_name = ::sc_core::sc_module_name;

    template <typename T>
    using out = ::sc_core::sc_out<T>;

    template <typename T>
    using signal = ::sc_core::sc_signal<T>;

    using time = ::sc_core::sc_time;

    using trace_file = ::sc_core::sc_trace_file;

    template <int W>
    using uint = ::sc_dt::sc_uint<W>;

    //--manifest constants from SystemC; renamed to provide clarity START
    auto constexpr fsec = ::sc_core::sc_time_unit::SC_FS;
    auto constexpr psec = ::sc_core::sc_time_unit::SC_PS;
    auto constexpr nsec = ::sc_core::sc_time_unit::SC_NS;
    auto constexpr usec = ::sc_core::sc_time_unit::SC_US;
    auto constexpr msec = ::sc_core::sc_time_unit::SC_MS;
    auto constexpr sec  = ::sc_core::sc_time_unit::SC_SEC;

    auto const zero_time = ::sc_core::SC_ZERO_TIME;
    //--manifest constants from SystemC; renamed to provide clarity END

    //--functions and macro wrappers for SystemC
    //--NB: we strip off the leading "sc_".
    inline void REPORT_FATAL(char const *const message,
                             char const *const file,
                             int const         line)
    {
      ::sc_core::sc_report_handler::report(sc_core::sc_severity::SC_FATAL,
                                           nullptr,
                                           message,
                                           file,
                                           line);
    }

    inline void start()
    {
      ::sc_core::sc_start();
    }

    inline void start(::sc_core::sc_time const& duration,
                      ::sc_core::sc_starvation_policy const p = ::sc_core::SC_RUN_TO_TIME)
    {
      ::sc_core::sc_start(duration, p);
    }

    inline void stop()
    {
      ::sc_core::sc_stop();
    }

    inline time const &time_stamp()
    {
      return  ::sc_core::sc_time_stamp();
    }

    template<class IF>
    inline void trace(::sc_core::sc_trace_file *const tf,
                      IF const                       &item,
                      ::std::string const            &name)
    {
      ::sc_core::sc_trace(tf, item, name);
    }
  } // namespace SC

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--end of invocation guard
#endif // not defined(__SC_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
