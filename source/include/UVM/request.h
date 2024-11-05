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
//  request.h
//  - requests which will be carried by sequence items for the
//    DUT/MUT drivers.
//
//  NOTES:
//  2024-07-18: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__REQUEST_H__)
# define __REQUEST_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <random>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "model.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace request
  { // request types which are bound into UVM sequence items.
    //--base classes
    class base_t;        // base for all request types
    class base_empty_t;  // base for all content free request types

    //--common request types
    class development_t; // let the driver development run wild.
    class error_t;       // a forced error request - DUT/MUT dependent
    class flush_t;       // a module flush request - DUT/MUT dependent
    class hold_t;        // a hold output state request - DUT/MUT dependent
    class reset_t;       // a reset module request - DUT/MUT dependent
  } // namespace ::request

  namespace request::EXAMPLE::single_cycle_t
  { // request typess for the ::EXAMPLE::single_cycle_t module
    class random_weighted_t;  // weighted random over all inputs.
    class value_fixed_t;      // fixed values
                              // NB: generally set by the using sequence.
                              // NB: not subject to randomisation.
  } // namespace ::request::EXAMPLE::single_cycle_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::base_t
  //--base class for all requests.
  //--NB: forces commonality of behaviour.
  //--NB: requires implementation of randomise().
  //--NB: cannot be instantiated directly.

  class request::base_t
  { // base class for all bare requests.
    public:     // UVM evil
    public:     // c'tors and d'tor
      base_t()
      { /* noOp */ }

      virtual ~base_t()
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      virtual void randomise() = 0;

      static void random_seed(::std::uint64_t const seed)
      {
        g_random_number_generatior.seed(seed);
      }

    protected:  // methods
      static ::std::uint64_t random64u()
      {
        static_assert(
          (sizeof random64u() == sizeof g_random_number_generatior()),
          "Random number must return a 64 bit value.");
        return  g_random_number_generatior();
      }

    private:    // attributes and globals
      static ::std::mt19937_64  g_random_number_generatior;
  }; // class ::request::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--variable request::base_t::g_random_number_generatior
  //--NB: common random number generator for all request types

  ::std::mt19937_64 request::base_t::g_random_number_generatior{};

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::base_empty_t
  //--common base class for empty driver requests.
  //--NB: implements method randomise() for empty requests.
  //--NB: cannot be instantiated directly.

  class request::base_empty_t
    : public ::request::base_t
  {
    public:     // c'tors and d'tor
      base_empty_t()
        : base_t{}
      { /* noOp */ }

      virtual ~base_empty_t() override
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      virtual void randomise() override final
      { /* noOp - no attributes to randomise */ }
  }; // class ::request::empty_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::development_t
  //--for development purposes - let the driver run wild.
  //--NB: intended for driver development use.
  //--NB: used to let the driver designer go crazy.

  class request::development_t final
    : public ::request::base_empty_t
  {
    public:     // c'tors and d'tor
      development_t() : base_empty_t{}  { /* noOp */ }
      virtual ~development_t() override { /* noOp */ }
  }; // class ::request::empty_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::error_t
  //--represents a request that will hold the DUT/MUT state.
  //--NB: the driver will know how to do a hold for its DUT/MUT.

  class request::error_t final
    : public ::request::base_empty_t
  {
    public:     // c'tors and d'tor
      error_t() : base_empty_t{}  { /* noOp */ }
      virtual ~error_t() override { /* noOp */ }
  }; // class ::request::error_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::flush_t
  //--represents a request that will hold the DUT/MUT state.
  //--NB: the driver will know how to do a hold for its DUT/MUT.

  class request::flush_t final
    : public ::request::base_empty_t
  {
    public:     // UVM evil
    public:     // c'tors and d'tor
      flush_t() : base_empty_t{}  { /* noOp */ }
      virtual ~flush_t() override { /* noOp */ }
  }; // class ::request::flush_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::hold_t
  //--represents a request that will hold the DUT/MUT state.
  //--NB: the driver will know how to do a hold for its DUT/MUT.

  class request::hold_t final
    : public ::request::base_empty_t
  {
    public:     // UVM evil
    public:     // c'tors and d'tor
      hold_t() : base_empty_t{}  { /* noOp */ }
      virtual ~hold_t() override { /* noOp */ }
  }; // class ::request::hold_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::reset_t
  //--represents a request that will reset the DUT/MUT state.
  //--NB: the driver will know how to do a reset (e.g. signals, or a
  //--    required multi-cycle signal sequence).

  class request::reset_t final
    : public ::request::base_empty_t
  {
    public:     // UVM evil
    public:     // c'tors and d'tor
      reset_t() : base_empty_t{}  { /* noOp */ }
      virtual ~reset_t() override { /* noOp */ }
  }; // class ::request::hold_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::EXAMPLE::single_cycle_t::random_weighted_t
  //--a weighted random request to the example DUT/MUT implemented in
  //--  class ::EXAMPLE::single_cycle_t.

  class request::EXAMPLE::single_cycle_t::random_weighted_t final
    : public ::request::base_t
  {
    public:     // UVM evil
    public:     // c'tors and d'tor
      random_weighted_t()
        : base_t{}
        , m_alive{true}
        , m_reset{}
        , m_is_advance{}
        , m_is_error{}
        , m_is_flush{}
        , m_is_valid{}
        , m_i_binary{}
        , m_i_number{}
      { /* noOp */ }

      virtual ~random_weighted_t() override
      {
        m_alive = false;
      }

    public:     // accessors
      // Need to single signal accessors for weight or all signals at a time?
      // the signal:weight OR data:weight is for the current stimuli.
      // Take the bundle vif to stimuli likely be the way whether no matter how
      //  the type is constructed.
      auto i_binary() const
      { return  (assert_alive(), m_i_binary); }
      auto i_number() const
      { return  (assert_alive(), m_i_number); }
      auto is_advance() const
      { return  (assert_alive(), m_is_advance); }
      auto is_error() const
      { return  (assert_alive(), m_is_error); }
      auto is_flush() const
      { return  (assert_alive(), m_is_flush); }
      auto is_valid() const
      { return  (assert_alive(), m_is_valid); }
      auto reset() const
      { return  (assert_alive(), m_reset); }

    public:     // methods
      virtual void randomise() override final
      {
        assert_alive();

        auto bits{random64u()};
        m_reset      = ((bits bitand 0x0000'0000'00FF) != 0);  // 1 in 256; NB: negative signal
        m_is_advance = ((bits bitand 0x0000'0000'0300) != 0);  // 3 in 4
        m_is_error   = ((bits bitand 0x0000'03FF'0000) == 0);  // 1 in 1024
        m_is_flush   = ((bits bitand 0x003F'0000'0000) == 0);  // 1 in 64
        m_is_valid   = ((bits bitand 0x0300'0000'0000) != 0);  // 3 in 4
        m_i_binary   = (0xFFFFFFFF bitand random64u());
        m_i_number   = (0xFFFFFFFF bitand random64u());
      }

    protected:  // methods
    private:    // methods
      void assert_alive() const
      {
        if  (not m_alive)
          ::uvm::uvm_report_fatal(
            "UNALIVE",
            "Thou art attempting necromancy - Just NO, Lazarus.");
      }

    private:    // types
    private:    // attributes
      bool                   m_alive;    // true: object is live;
                                         // false: object is unusable
      ::model::reset_t       m_reset;
      ::model::is_advance_t  m_is_advance;
      ::model::is_error_t    m_is_error;
      ::model::is_flush_t    m_is_flush;
      ::model::is_valid_t    m_is_valid;
      ::model::i_binary_t    m_i_binary;
      ::model::i_number_t    m_i_number;
  }; // class ::request::EXAMPLE::single_cycle_t::random_weighted_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::request::EXAMPLE::single_cycle_t::value_fixed_t
  //--a request with fixed values, not subject to random predation or
  //--  randomisation by the using sequence.
  //--NB: randomise() method is a noOp.

  class request::EXAMPLE::single_cycle_t::value_fixed_t final
    : public ::request::base_t
  {
    public:     // UVM evil
    public:     // c'tors and d'tor
      value_fixed_t()
        : base_t{}
        , m_alive{true}
        , m_reset{}
        , m_is_advance{}
        , m_is_error{}
        , m_is_flush{}
        , m_is_valid{}
        , m_i_binary{}
        , m_i_number{}
        , m_prefix{}
      { /* noOp */ }

      virtual ~value_fixed_t() override
      {
        m_alive = false;
      }

    public:     // accessors
      // Need to single signal accessors for weight or all signals at a time?
      // the signal:weight OR data:weight is for the current stimuli.
      // Take the bundle vif to stimuli likely be the way whether no matter how
      //  the type is constructed.
      auto i_binary() const
      { return  (assert_alive(), m_i_binary); }
      auto i_number() const
      { return  (assert_alive(), m_i_number); }
      auto is_advance() const
      { return  (assert_alive(), m_is_advance); }
      auto is_error() const
      { return  (assert_alive(), m_is_error); }
      auto is_flush() const
      { return  (assert_alive(), m_is_flush); }
      auto is_valid() const
      { return  (assert_alive(), m_is_valid); }
      auto prefix() const
      { return  (assert_alive(), m_prefix); }
      auto reset() const
      { return  (assert_alive(), m_reset); }

      value_fixed_t &set_control(
        ::model::reset_t const      reset,
        ::model::is_advance_t const is_advance,
        ::model::is_error_t const   is_error,
        ::model::is_flush_t const   is_flush,
        ::model::is_valid_t const   is_valid)
      {
        assert_alive();
        m_reset      = reset;
        m_is_advance = is_advance;
        m_is_error   = is_error;
        m_is_flush   = is_flush;
        m_is_valid   = is_valid;
        return  *this;
      }

      value_fixed_t &set_instruction(
        ::model::i_binary_t const i_binary,
        ::model::i_number_t const i_number)
      {
        assert_alive();
        m_i_binary = i_binary;
        m_i_number = i_number;
        return  *this;
      }

      value_fixed_t &set_prefix(::std::string const &prefix)
      {
        assert_alive();
        m_prefix = prefix;
        return  *this;
      }

  public:     // methods
      virtual void randomise() override final
      { /* noOp - this request does not randomise */ }

    protected:  // methods
    private:    // methods
      void assert_alive() const
      {
        if (not m_alive)
          ::uvm::uvm_report_fatal(
            "UNALIVE",
            "Thou art attempting necromancy - Just NO, Lazarus.");
      }

    private:    // types
    private:    // attributes
      bool                   m_alive;    // true: object is live;
                                         // false: object is unusable
      ::model::reset_t       m_reset;
      ::model::is_advance_t  m_is_advance;
      ::model::is_error_t    m_is_error;
      ::model::is_flush_t    m_is_flush;
      ::model::is_valid_t    m_is_valid;
      ::model::i_binary_t    m_i_binary;
      ::model::i_number_t    m_i_number;
      ::std::string          m_prefix;
  }; // class ::request::EXAMPLE::single_cycle_t::value_fixed_t

  //..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace request
  {
    // <none>
  } // namespace ::request

  //..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  //--end of invocation guard
#endif // not defined(__REQUEST_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
