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
//  sequence.h
//  - This is a number sequences for test the DUT/MUT.
//      -- random_loop_t;
//      -- flush_w_advance_never_advances_t;
//      -- error_w_advance_valid_t;
//      -- reset_advance_with_out_valid_t;
//      -- all_control_pairs_t;
//      -- random_weighted_t;
//
//  NOTES:
//  2024-07-19: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__SEQUENCE_H__)
# define __SEQUENCE_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  //--NB: std::format is not supported in GNU 12.2 compiler
# include <ranges>
# include <sstream>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "sequence_item.h"
# include "sequencer.h"
# include "utility.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace sequence
  { // UVM sequences. . .
    //--base class for all our sequences.
    //--NB: cannot be directly instantiated.
    //--NB: forces use of common naming.
    template<class REQUEST_SEQUENCE_ITEM_T,
             class RESPONSE_SEQUEMCE_ITEM_T = REQUEST_SEQUENCE_ITEM_T>
    class base_t;

    //--an item repested an arbitrary number times.
    //--NB: cannot be directly instantiated.
    //--NB: no response handling is implemented.
    template<class REQUEST_SEQUENCE_ITEM_T, unsigned COUNT = 10>
    class repeat_t;        // an item repested an arbitrary number times.
                           // NB: cannot be directly instantiated.
                           // NB: no response handling is implemented.

    //--an empty item repeated an arbitrary number of times.
    //--NB: this class is intended to be used used for rapid first-pass
    //--    development of a driver.
    template<unsigned COUNT = 10>
    using repeat_empty_t = repeat_t<::sequence_item::empty_t, COUNT>;
  } // namespace ::sequence

  namespace sequence::EXAMPLE::single_cycle_t
  { // ::EXAMPLE::single_cycle_t test sequences.
    class random_loop_t;
    class flush_w_advance_never_advances_t;
    class error_w_advance_valid_t;
    class reset_advance_with_out_valid_t;
    //--send all combinations of control signals
    class all_control_pairs_t;
    //--send randomly weighted control and data requests.
    class random_weighted_t;
  } // namespace ::sequence::EXAMPLE::single_cycle_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence::base_t
  //--common base type which provides some convenient member types.

  template <class REQUEST_T, class RESPONSE_T>
  class sequence::base_t
    : public ::uvm::uvm_sequence<REQUEST_T, RESPONSE_T>
  {
    public:     // forward declarations and type aliases
      using request_t       = REQUEST_T;
      using response_t      = RESPONSE_T;
      // FIXME: ought these only be declared using concepts, when
      //        the REQUEST_T and RESPONSE_T are of the same type?
      using item_t          = REQUEST_T;
      using sequence_item_t = REQUEST_T;

    private:    // forward declarations and type aliases
      using base_class_t
               = ::uvm::uvm_sequence<request_t, response_t>;
      using sequencer_base_t
               = ::sequencer::base_t<request_t, response_t>;

    public:     // UVM evil
      UVM_DECLARE_P_SEQUENCER(sequencer_base_t)

    public:     // c'tors and d'tor
    public:     // accessors
      unsigned count() const
      { // get the repeat count
        return  m_count;
      }

      base_t &count(unsigned const value)
      { // set the repeat count
        char const *error{nullptr};
        if  (1 > value)
          error = "Value assignment is illogical.";
        else if  (m_spinning)
          error = "The count cannot be changed mid-flight.";
        else
        { /* noOp */ }

        if (error)
          ::uvm::uvm_report_fatal("USAGE", error);

        return  ((m_count=value), *this);
      }

    public:     // methods
      virtual void do_body() = 0;  // body implementation, as guards are built into body().

      virtual void body() override final
      { // do the sequence with some implicit behaviour.
        // derived classes use do_body().
        //--error check
        if  (m_spinning)
          ::uvm::uvm_report_fatal("USAGE", "This method is not reentrant.");
        m_spinning = true;

        //--do the real work
        do_body();

        //--remove the error check
        m_spinning = false;

        //--end of compilation unit with implicit return
      }

      virtual void post_body() override
      { // we completed, we can leave the phase.
        // NB: use of alias name base_class_t, as MSVC seems
        //     to have difficulty implicitly following the
        //     inheritance hierarchy.
        auto starting_phase{base_class_t::get_starting_phase()};
        if  (starting_phase)
          starting_phase->drop_objection(this);
      }

      virtual void pre_body() override
      { // we cannot leave this UVM phase until the sequence completes.
        // NB: use of alias name base_class_t, as MSVC seems
        //     to have difficulty implicitly following the
        //     inheritance hierarchy.
        auto starting_phase{base_class_t::get_starting_phase()};
        if  (starting_phase)
          starting_phase->raise_objection(this);
      }

    protected:  // c'tors and d'tor
      base_t()
        : base_t{"::sequence::base_t", 1u}
      { /* noOp */ }
      base_t(unsigned const count)
        : base_t("::sequence::base_t", count)
      { /* noOp */ }
      base_t(::uvm::uvm_object_name name)
        : base_t{name, 1u}
      { /* noOp */ }

      base_t(::uvm::uvm_object_name name, unsigned const count)
        : base_class_t{name}  // use of alias name, as fully templated
                              // name is rather ungainly.
        , p_sequencer{}
        , m_count{count}
        , m_spinning{false}
      { /* noOp */ }

      virtual ~base_t() override
      { // make use of deleted objects problematic
        p_sequencer = nullptr;
        m_count = (compl 0u);
        m_spinning = true;
      }

      base_t(base_t const &) = delete;
      base_t &operator=(base_t const &) = delete;

    protected:  // accessors
    protected:  // methods
      void send_item(sequence_item_t &item)
      { // send the given item to the appropriate sequencer(s)
        base_class_t::start_item(&item);
        item.randomise();
        base_class_t::finish_item(&item);
      }

    private:    // methods
    private:    // types
    private:    // attrbutes
      unsigned  m_count;
      bool      m_spinning;
  };  // class ::sequence::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // class ::sequence::repeat_t
  // a sequence of a fixed item of type SEQUENCE_ITEM_T repeated COUNT times.

  template<class SEQUENCE_ITEM_T, unsigned COUNT>
  class sequence::repeat_t
    : public ::sequence::base_t<SEQUENCE_ITEM_T>
  { // a sequence of a fixed item of type SEQUENCE_ITEM_T repeated
    // COUNT times.
    // NB: COUNT is a template parameter, it can be overriden by
    //     invoking the method count(value).
    public:     // forward declarations and type aliases
    public:     // UVM evil
    public:     // c'tors and d'tor
      repeat_t()
        : repeat_t{"::sequence::repeat_t", COUNT}
      { /* noOp */ }
      repeat_t(unsigned const count)
        : repeat_t{"::sequence::repeat_t", count}
      { /* noOp */ }
      repeat_t(::uvm::uvm_object_name name)
        : repeat_t{name, COUNT}
      { /* noOp */ }
      repeat_t(::uvm::uvm_object_name name, unsigned const count)
        : base_t<SEQUENCE_ITEM_T>{name, count}
      { /* noOp */ }
      virtual ~repeat_t() override
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      virtual void do_body() override
      {
        using base = base_t<SEQUENCE_ITEM_T>;
//        base_t::sequence_item_t item;
//        typename repeat_t::base_t::sequence_item_t item;
//        typename base_t::sequence_item_t item;
        SEQUENCE_ITEM_T item{};
        for  (auto ix [[maybe_unused]]:
                ::std::ranges::views::iota(0u, base::count()))
        {
          base::start_item(&item);
          item.randomise();
          base::finish_item(&item);
        }
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // types
    private:    // attrbutes
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence::EXAMPLE::single_cycle_t::all_control_pairs_t
  // Test all controls pairs by force
  // NB:  There are five (5) current control bits and the sequence hard code
  //      that - maybe I can general it later.

  class sequence::EXAMPLE::single_cycle_t::all_control_pairs_t
    : public ::sequence::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
      static unsigned constexpr COUNT_DEFAULT{1 * 10};

    public:     // UVM evil
    public:     // c'tors and d'tor
      all_control_pairs_t()
        : base_t{
            "::sequence::EXAMPLE::single_cycle_t::all_control_pairs_t",
            COUNT_DEFAULT}
      { /* noOp */ }
      virtual ~all_control_pairs_t() override
      { /* noOp */ }

    public:     // attributes
    public:     // forward declarations and type aliases
    public:     // UVM evil
    public:     // c'tors and d'tor
    public:     // attributes
    public:     // methods
      virtual void do_body() override
      { // send the complete sequence.
        //--locals
        // all possible two-cycle combinations of five control bits
        unsigned constexpr bit_count{5};
        unsigned constexpr bit_mask{compl ((compl 0u) << bit_count)};
        unsigned constexpr sequence_cycles{2};
        unsigned constexpr sequence_bits{bit_count * sequence_cycles};
        unsigned constexpr sequence_count{1u << sequence_bits};

        sequence_item_t item{};
        sequence_item_t::value_fixed_t request{};

        //--spin iterations of all pairs.
        //--NB: first control-bit combination is all resets.
        using ::std::ranges::views::iota;
        for (auto const iteration [[maybe_unused]]: iota(0u, count()))
        { // each iteration covers all combinations of control bit sequences
          for (auto const sequence: iota(0u, sequence_count))
          { // all combinations
            for (auto const cycle: iota(0u, sequence_cycles))
            { // one cycle's worth of control bits
              //--take the high order bits first
              auto const block{sequence_cycles - cycle - 1};
              auto const shift{block * bit_count};
              auto const raw  {sequence >> shift};
              auto const bits {bit_mask bitand raw};

              //--set up the the control bits for the DUT/MUT
              //--NB: std::format not supported in GNU 12.2 compiler
              using ::utility::format::binary;
              std::ostringstream prefix{};
              prefix
                << "SEQUENCE " << binary(sequence, sequence_bits)
                << ": cycle (" << (cycle+1) << '/' << sequence_cycles
                << "), bits(" << binary(bits, bit_count) << ')';
              request.set_prefix(prefix.str());

              request.set_control
              (
                (0x01 bitand (bits >> 4)),  // reset
                (0x01 bitand (bits >> 3)),  // is_advance
                (0x01 bitand (bits >> 2)),  // is_error
                (0x01 bitand (bits >> 1)),  // is_flush
                (0x01 bitand (bits >> 0))   // is_valid
              );

              //--send the bits to the DUT/MUT
              send_item(item.set(request));
            }
          }
        }

        //--hold the output for two cycles
        item.select<sequence_item_t::hold_t>();
        send_item(item);
        send_item(item);

        //--end of compilation unit with implicit return
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // types
    private:    // attrbutes
  }; // class ::sequence::EXAMPLE::single_cycle_t::all_control_pairs_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence::EXAMPLE::single_cycle_t::random_weighted_t
  // Test to select and stimulate a series of weighted random
  //    controls, values and debug.
  // Start by sending two resets so at known state
  // Loop is to send random weighted stumuli the count times
  // End by sending two holds to let last random work it way thru.
  // 
  // NB: The count is either COUNT_DEFAULT or inputted when calling the test.

  class sequence::EXAMPLE::single_cycle_t::random_weighted_t
    : public ::sequence::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
      static unsigned constexpr COUNT_DEFAULT{20 * 1000};

    public:     // UVM evil
    public:     // c'tors and d'tor
      random_weighted_t()
        : base_t{
            "::sequence::EXAMPLE::single_cycle_t::random_weighted_t",
            COUNT_DEFAULT}
      { /* noOp */ }
      virtual ~random_weighted_t() override
      { /* noOp */ }

    public:     // attributes
    public:     // methods
      virtual void do_body() override
      { // send reset/reset/a number of weighted random sequences/hold/hold.
        //--locals
        sequence_item_t item{};

        //--a couple reset cycles so a know state
        item.select<sequence_item_t::reset_t>();
        send_item(item);
        send_item(item);

        //--send the weighted random inputs
        item.select<sequence_item_t::random_weighted_t>();
        using ::std::ranges::views::iota;
        for (auto const index [[maybe_unused]]: iota(0u, count()))
          send_item(item);

        //--hold state due to system lag/hysteresis
        item.select<sequence_item_t::hold_t>();
        send_item(item);
        send_item(item);

        //--end of compilation unit with implicit return
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // types
    private:    // attrbutes
  }; // class ::sequence::EXAMPLE::single_cycle_t::random_weighted_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  
  //--class ::sequence::EXAMPLE::single_cycle_t::random_loop_t
  class sequence::EXAMPLE::single_cycle_t::random_loop_t
    : public ::sequence::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    // COUNT times.
    // NB: COUNT is a template parameter, it can be overriden by
    //     invoking the method count(value).
    public:     // forward declarations and type aliases
      static unsigned constexpr COUNT_DEFAULT{20 * 1000};

    public:     // UVM evil
    public:     // c'tors and d'tor
      random_loop_t()
        : base_t{
            "::sequence::EXAMPLE::single_cycle_t::random_loop_t",
            COUNT_DEFAULT}
      { /* noOp */ }
      virtual ~random_loop_t() override
      { /* noOp */ }

    public:     // attributes
    public:     // methods
      virtual void do_body() override
      {
        //--spin out the possible sequence items used.
        sequence_item_t reset {sequence_item_t::reset_t{}};
        sequence_item_t hold  {sequence_item_t::hold_t{}};
        sequence_item_t random{sequence_item_t::random_weighted_t{}};

        //--a couple reset cycles so a know state
        send_item(reset);
        send_item(reset);

        using ::std::ranges::views::iota;
        for (auto const index [[maybe_unused]]: iota(0u, count()))
          send_item(random);

        //--hold state due to system lag/hysteresis
        send_item(hold);
        send_item(hold);

        //--end of compilation unit with implicit return
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // attrbutes
  };
  //--class ::sequence::EXAMPLE::single_cycle_t::random_loop_t


  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence::EXAMPLE::single_cycle_t::flush_w_advance_never_advances_t
  // This class flush_w_advance_never_advances_t will test that FLUSH
  //  works as expected and with specification.
  // If an is_valid(1) and not is_advance(0) with number of holds sent,
  //  followed by a is_flush(1) and is_advance(1) during same cycle, 
  //  will not advance but flush.
  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  class sequence::EXAMPLE::single_cycle_t::flush_w_advance_never_advances_t
    : public ::sequence::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
      // The number hold 
      static unsigned constexpr COUNT_DEFAULT{1 * 10}; 

    public:     // UVM evil
    public:     // c'tors and d'tor
      flush_w_advance_never_advances_t()
        : base_t{
            "::sequence::EXAMPLE::single_cycle_t::flush_w_advance_never_advances_t",
            COUNT_DEFAULT}
      { /* noOp */ }
      virtual ~flush_w_advance_never_advances_t() override
      { /* noOp */ }

    public:     // attributes
    public:     // methods
      virtual void do_body() override
      { // send the complete sequence.
        //--spin out the sequence items
        sequence_item_t reset {sequence_item_t::reset_t{}};
        sequence_item_t hold  {sequence_item_t::hold_t{} };
        sequence_item_t item  {};

        sequence_item_t::value_fixed_t request{};

        // Start the FLUSH squence test with moving to a known start.
        send_item(reset);
        send_item(reset);

        // Do is_valid (1) is_adavance (0) with data
        request.set_control(1, 0, 0, 0, 1); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE1000000, 0xE8000000);
        request.set_prefix("IS_VALID(1) with IS_ADVANCE(0)");

        send_item(item.set(request));  // clock a value into the DUT/MUT

        // Spin for awhile holding the computed datum.
        using ::std::ranges::views::iota;
        for (auto const index [[maybe_unused]]: iota(0u, count()))
          send_item(hold);

        // Do is_valid (0) is_adavance (1) is flush (1)
        request.set_control(1, 1, 0, 1, 0); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE2000000, 0xEA000000);
        request.set_prefix("Flush IS_VALID(0) with IS_ADVANCE(1)");
        send_item(item.set(request));

        // Hold for two more cycles, that we see the output.
        send_item(hold);
        send_item(hold);

        //--end of compilation unit with implicit return
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // types
    private:    // attrbutes
  }; // class ::sequence::EXAMPLE::single_cycle_t::flush_w_advance_never_advances_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence::EXAMPLE::single_cycle_t::reset_advance_with_out_valid_t
  // This class reset_advance_with_out_valid_t will test that 
  //  works as expected and with specification.
  // 
  // Start TEST1 with two holds/normal resets/is_adavance(1)/hold for count/
  //    /RESET and is_adavane(1)/two holds to handle timing.
  //
  // Start TEST2 with two resets/is_valid(1)&is_adavance(0)/
  //    /RESET&is_valid(0)&is_adavance(1)/two holds to handle timing.
  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  class sequence::EXAMPLE::single_cycle_t::reset_advance_with_out_valid_t
    : public ::sequence::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
    public:     // UVM evil
    public:     // c'tors and d'tor
      reset_advance_with_out_valid_t()
        : base_t{
            "::sequence::EXAMPLE::single_cycle_t::reset_advance_with_out_valid_t"}
      { /* noOp */ }
      virtual ~reset_advance_with_out_valid_t() override
      { /* noOp */ }

    public:     // attributes
    public:     // methods
      virtual void do_body() override
      { // send the complete sequence.
        //--spin out the sequence items
        sequence_item_t hold{sequence_item_t::hold_t{}};
        sequence_item_t item{};

        sequence_item_t::value_fixed_t request{};

        // Do the FLUSH squence test
        request.set_control(0, 0, 0, 0, 0); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE1000000, 0xE8000000);
        request.set_prefix("RESET TEST-1 #0)");
        send_item(item.set(request));
        request.set_prefix("RESET TEST-1 #1)");
        send_item(item.set(request));

        // Do is_valid (0) is_adavance (1) with data
        request.set_control(1, 1, 0, 0, 0); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE3000000, 0xEC000000);
        request.set_prefix("Data and IS_VALID(0) with IS_ADVANCE(1)");
        send_item(item.set(request));  // clock a value into the DUT/MUT

        // Spin for awhile holding the computed datum.
        using ::std::ranges::views::iota;
        for  (auto index [[maybe_unused]]: iota(0u, count()))
          send_item(hold);

        // Do RESET with is_valid (0) is_adavane (1) and data
        request.set_control(0, 1, 0, 0, 0); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE6000000, 0xEA000000);
        request.set_prefix("Data and IS_VALID(0) with IS_ADVANCE(1)");
        send_item(item.set(request));  // clock a value into the DUT/MUT

        send_item(hold);
        send_item(hold);

        // Another RESET TEST TEST-2
        request.set_control(0, 0, 0, 0, 0); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE4000000, 0xEE000000);
        request.set_prefix("RESET TEST-2 #0)");
        send_item(item.set(request));
        request.set_prefix("RESET test-2 #1)");
        send_item(item.set(request));
        // Do is_valid(1) is_adavance(0)
        request.set_control(1, 0, 0, 0, 1); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE2000000, 0xEA000000);
        request.set_prefix("Data with IS_VALID(1)/IS_ADVANCE(0)");
        send_item(item.set(request));

        // Do RESET&is_valid(0)&is_adavance(1)
        request.set_control(0, 1, 0, 0, 0); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE2000000, 0xEA000000);
        request.set_prefix("RESET with IS_VALID(0)/IS_ADVANCE(1)");
        send_item(item.set(request));

        // Hold for two more cycles, that we see the output.
        send_item(hold);
        send_item(hold);

        //--end of compilation unit with implicit return
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // types
    private:    // attrbutes
  }; // class ::sequence::EXAMPLE::single_cycle_t::reset_advance_with_out_valid_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence::EXAMPLE::single_cycle_t::error_w_advance_valid_t
  //    This class error_w_advance_valid_t will test that 
  //    works as expected and with specification.
  //
  //    Two Resets/is_valid(1)&is_adavane(1)
  //      /is_valid(1)&is_adavance(1)&is_error(1)&is_flush(0)
  //      //two holds to handle timing.
  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  class sequence::EXAMPLE::single_cycle_t::error_w_advance_valid_t
    : public ::sequence::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
    public:     // UVM evil
    public:     // c'tors and d'tor
      error_w_advance_valid_t()
        : base_t{
            "::sequence::EXAMPLE::single_cycle_t::error_w_advance_valid_t"}
      { /* noOp */ }
      virtual ~error_w_advance_valid_t() override
      { /* noOp */ }

    public:     // attributes
    public:     // methods
      virtual void do_body() override
      { // send the complete sequence.
        //--local variables and declarations
        sequence_item_t hold {sequence_item_t::hold_t{} };
        sequence_item_t item {};
        sequence_item_t reset{sequence_item_t::reset_t{}};

        sequence_item_t::value_fixed_t request{};  // copied into item

        // Do the FLUSH squence test
        send_item(reset);
        send_item(reset);

        // Do is_valid (1) is_adavane (1) with data
        // clocks valid data into DUT/MUT 
        request.set_control(1, 1, 0, 0, 1); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE1000000, 0xE8000000);
        request.set_prefix("IS_VALID(1) with IS_ADVANCE(1)");
        send_item(item.set(request));  // clock a value into the DUT/MUT

        // Do is_valid (1) is_adavance (1) is_error(1) is_flush (0)
        // clocks valid data into DUT/MUT whilst forcing error
        request.set_control(1, 1, 1, 0, 1); // reset, is_advance, is_error, is_flush, is_valid
        request.set_instruction(0xE2000000, 0xEA000000);
        request.set_prefix("ERROR: IS_VALID(1) with IS_ADVANCE(1)");
        send_item(item.set(request));

        // Hold for two more cycles, that we see the output.
        send_item(hold);
        send_item(hold);

        //--end of compilation unit with implicit return
      }

    protected:  // c'tors and d'tor
    protected:  // accessors
    protected:  // methods
    private:    // methods
    private:    // types
    private:    // attrbutes
  }; // class ::sequence::EXAMPLE::single_cycle_t::error_w_advance_valid_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace sequence
  {
    // <none>
  } // namespace sequence

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__SEQUENCE_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
