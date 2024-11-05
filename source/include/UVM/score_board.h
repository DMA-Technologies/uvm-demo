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
//  score_board.h
//  - UVM based score boards for all DUTs/MUTs
//
//  NOTES:
//  2024-08-02: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__SCORE_BOARD_H__)
# define __SCORE_BOARD_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <iomanip>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "monitor.h"
# include "predictor.h"
# include "transaction.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--forward declaratons

  namespace score_board
  { // we love scoring ourselves.
    //--base class for all score boards
    //--  provides requirements (virtual methods) for all project scoreboards.
    template<typename TRANSACTION_T>
    class base_t;
  } // namespace ::score_board

  namespace score_board::counter
  { // various counter types for the score boards.
    //--count pairs of control bits injected into the DUT/MUT.
    //--NB: observes control bit pairs between transaction stimuli.
    //--NB: manages observation hysteresis.
    template<unsigned N_BITS>
    struct control_bit_pairs_t;

    //--counts transaction passes and failures.
    struct pass_fail_count_t;
  } // namespace ::score_board::counter

  namespace score_board::EXAMPLE::single_cycle_t
  { // score boards for the ::EXAMPLE::single_cycle_t module.
    //--verify that DUT/MUT control outputs responds correctly to input.
    class control_bits_t;

    //--DUT/MUT control bit pair checker
    //--checks control bit input pairs for completeness.
    class control_bit_pairs_t;

    //--verify that DUT/MUT data value output responds correctly to input.
    class data_values_t;
  } // namespace ::score_board::EXAMPLE::single_cycle_t
#if 0
  namespace score_board
  {
    //--convenience names
    template<unsigned N_BITS>
    using control_bit_pairs_t = counter::control_bit_pairs_t<N_BITS>;
    using pass_fail_count_t = counter::pass_fail_count_t;

  } // namespace score_board
#endif

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--struct ::score_board::counter::control_bit_pairs_t<N_BITS>
  //--support class to observe and count control bit pairs.
  //--NB: templatised to allow for an arbitrary number of bits.
  //--NB: this is a struct, thus the user can reach inside with impunity.
  //--NB: caveat emptor.

  template<unsigned N_BITS>
  struct score_board::counter::control_bit_pairs_t final
  { // Captures the count of control bit pairs observed.
    // NB: used to verify the coverage of control bit combinations
    //     which DUT/MUT was subject to during test.
    // NB: N_BITS is the number of control bits observed
    // NB: N_ENTRY is the characteristic dimension of each sample,
    //     equal to 2**N_BITS.
    //--necessary evil
    static_assert(
      (N_BITS >= 1),
      "Must have at least one control bit.");

    //--useful things
    // number of possible control bit combinations in a single observation
    static unsigned constexpr N_BIT_COMBINATIONS{1 << N_BITS};
    // total number of possible observation pairs
    static unsigned constexpr N_PAIRS{
      N_BIT_COMBINATIONS * N_BIT_COMBINATIONS};

    // attributes
    unsigned check_array[N_BIT_COMBINATIONS][N_BIT_COMBINATIONS];
    unsigned prior_bits;
    bool     prior_valid;

    // c'tor and d'tor
    control_bit_pairs_t()
      : check_array{}
      , prior_bits{0}
      , prior_valid{false}
    { /* noOp */ }

    ~control_bit_pairs_t()
    { // dirty the state, that there is difficulty using a
      // stale instance of this object.
      for (auto &row: check_array)
        for (auto &item: row)
          item = 0xDEADBABE;
      prior_bits = 0xDEADCAFE;
      prior_valid = false;
    }

    // helper methods
    double coverage_fraction() const
    { // returns the coverage fraction observed.
      // - computed value is number covered pairs relative
      //   to all possibilities.
      // NB: return value is in range [0.0, 1.0].
      return  (coverage_observations() / double{N_PAIRS});
    }

    unsigned coverage_observations() const
    { // returns the number of covered pairs observed.
      // NB: not the total number of observations.
      unsigned count{ 0 };
      for  (auto &row : check_array)
        for  (auto &item : row)
          count += ((item == 0) ? 0 : 1);
      return  count;
    }

    bool is_coverage_complete() const
    { // verifies complete coverage of all observed control
      // bit pairs.
      bool result{true};
      for  (auto const &row : check_array)
        for  (auto const &item : row)
          result = (result and (item > 0));
      return  result;
    }

    unsigned maximum_observation() const
    { // returns the maximum count across all observed pairs.
      unsigned count{check_array[0][0]};
      for  (auto &row: check_array)
        for  (auto &item: row)
          count = ::std::max(count, item);
      return  count;
    }

    unsigned minimum_observation() const
    { // returns the minimum count across all observed pairs.
      unsigned count{check_array[0][0]};
      for  (auto &row: check_array)
        for  (auto &item: row)
          count = ::std::min(count, item);
      return  count;
    }

    void observe(unsigned const bits)
    { // observe a singular set of control bits.
      // NB: allows sampling the control bits on a per-cycle basis.
      // NB: manages the necessary observation hysteresis.
      using ::uvm::uvm_report_fatal;

      if  (bits  >=  N_BIT_COMBINATIONS)
        uvm_report_fatal(
          "INVALID_SAMPLE",
          "Invalid observed control bits value.");

      if  (prior_valid)
        check_array[prior_bits][bits]++;

      prior_bits = bits;
      prior_valid = true;
    }
  };
  
  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--struct ::score_board::utility::pass_fail_count_t
  //--support class to count passes, failures, and optionally moot results.
  //--NB: this is a struct, thus the user can reach inside with impunity.
  //--NB: caveat emptor.

  struct score_board::counter::pass_fail_count_t final
  { // counter type for pass/fail transactions.
    // NB: provides for counting moot transactions, if desired.
    //--physical counters
    unsigned m_fail;  // value computation failed
    unsigned m_moot;  // no value to check; transaction was moot
    unsigned m_pass;  // value was successfully computed

    //--c'tor and d'tor
    pass_fail_count_t()
      : m_fail{0}
      , m_moot{0}
      , m_pass{0}
    { /* noOp */ }

    ~pass_fail_count_t()
    { // dirty up the object so that stale usage is:
      // 1) poorly behaved
      // 2) relatively easily observable
      m_fail = 0xDEADCAFE;
      m_moot = 0xDEADBABE;
      m_pass = 0xDEADBEEF;
    }

    //--methods
    void fail()
    { // observe a failed transaction.
      m_fail++;
    }

    bool is_pass() const
    { // were all observations successful?
      // NB: by intent requires at least one successful pass.
      // NB: ignores observations where pass/fail was moot.
      return  ((m_pass > 0) and (m_fail == 0));
    }

    void moot()
    { // observe a moot transaction.
      // NB: has no affect on the is_pass() method.
      // MB: useful for calculating sadistics.
      m_moot++;
    }

    void pass()
    { // observe a passing transaction.
      m_pass++;
    }

    unsigned total_observations() const
    { // total number of recorded observations
      return  (m_fail + m_moot + m_pass);
    }
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::score_board::base_t
  //--base class for score boards.
  //--NB: provides consistent interface naming.
  //--NB: requires implementation of several abstract methods.
  //--NB: implements convenience methods to display various counter types
  //--    (see: support classes) types.

  template<typename TRANSACTION_T>
  class score_board::base_t
    : public uvm::uvm_scoreboard
  { // base class for score boards
    // provides templated abstraction for:
    //  1) communication to compatible monitor object(s)
    //  2) common support method "connect_to(monitor_base)
    //  3) abstract method is_pass() to ascertain pass/fail
    //  4) abstract method write() to accept transactions
    public:     // published types, &c
      using transaction_t    = TRANSACTION_T;
      using monitor_base_t   = ::monitor::base_t<transaction_t>;
      using monitor_export_t = ::uvm::uvm_analysis_port<transaction_t>;
      using score_board_import_t =
        ::uvm::uvm_analysis_imp<transaction_t, base_t>;

    public:     // UVM evil
    public:     // c'tors and d'tor
      base_t(::uvm::uvm_component_name name)
        : uvm_scoreboard{name}
        , m_import{"m_item_port", this}
      { /* noOp */ }

      virtual ~base_t()
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      void connect_to(monitor_base_t &monitor)
      { // hook up to the monitor's export
        //monitor.output().bind(m_import);   // connect() simply maps to bind()
        //monitor.output().unbind(m_import); // unbind is safe to use at this stage
        monitor.output().connect(m_import);
      }

      virtual bool is_pass() const = 0;
      virtual void write(transaction_t const &item) = 0;

    protected:  // methods
      template<unsigned N_BITS>
      static inline void report_summary(
        ::std::string const                        &title,
        counter::control_bit_pairs_t<N_BITS> const &count)
      { // report a summary of the given counter's interesting
        // sadistics as a UVM information-level output.
        //--local variables and definitions
        auto const observed  {count.coverage_observations()};
        auto const fraction  {100 * count.coverage_fraction()};
        bool const incomplete{observed != count.N_PAIRS};
        auto const completion{incomplete ? "INCOMPLETE" : "complete"};
        auto const minimus   {count.minimum_observation()};
        auto const maximus   {count.maximum_observation()};

        //--make and send the information
        //--NB: std::format is unavailable in GCC 12.2
        std::ostringstream message{};
        message
          << "\n"
          << "  " << title << ": \n"
          << "    Pair coverage is " << completion << ". \n"
          << "    Observations:        "
            << std::setw(6) << observed
            << "  (" << std::setprecision(2) << std::fixed
            << fraction << "%). \n"
          << "    Minimum observation: "
            << std::setw(6) << minimus << ". \n"
          << "    Maximum observation: "
            << std::setw(6) << maximus << ". \n"
          << "    TOTAL PAIRS:         "
            << std::setw(6) << count.N_PAIRS << ". \n";
        ::uvm::uvm_report_info("SCORE_BOARD", message.str());

        //--end of compilation unit with implicit return
      }

      static inline void report_summary(
        ::std::string const              &title,
        counter::pass_fail_count_t const &counter)
      { // report a summary of the given counter's interesting
        // sadistics as a UVM information-level output.
        // NB: GNU 12.2 does not support std::format(...)
        using std::fixed;
        using std::left;
        using std::right;
        using std::setprecision;
        using std::setw;

        //--local variables and definitions
        auto const pct_ratio{100.0 / counter.total_observations()};

        auto const format_all
        { // ("Moot", 420) -> "    Moot:      420 ( 85.7%). \n"
          [=](::std::string const& name, auto const count)
          {
            std::ostringstream iob{};
            iob
              << "    " << left << setw(7) << name
              << " " << right << setw(6) << count
              << "  (" << right << setw(6) << setprecision(2)
                << fixed << (count*pct_ratio)
              << "%). \n";
            return iob.str();
          }
        };

        auto const format_count
        { // ("TOTAL", 127) -> "   TOTAL:    127. \n"
          [](::std::string const &name, auto const count)
          {
            std::ostringstream iob{};
            iob
              << "    " << left << setw(7) << name
              << " " << right << setw(6) << count
              << ". \n";
            return  iob.str();
          }
        };

        //--make and send the information
        ::std::ostringstream message{};
        message
          << "\n"
          << "  " << title << ": \n"
          << format_all("Passed:", counter.m_pass)
          << format_all("Failed:", counter.m_fail)
          << format_all("Moot:", counter.m_moot)
          << format_count("TOTAL:", counter.total_observations());
        ::uvm::uvm_report_info("SCORE_BOARD", message.str());

        //--end of compilation unit with implicit return
      }

    private:    // methods
    private:    // embedded classes and types
    private:    // attributes
      score_board_import_t m_import;
  }; // class ::score_board::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::score_board::EXAMPLE::single_cycle_t::data_values_t
  //--checks data output values from the example pipeline development
  //--  class ::EXAMPLE::single_cycle_t.
  //--NB: provides consistent interface naming.
  //--NB: Verifies the output data values of the single_cycle_t module.
  //--NB: Tracks (pass/fail/moot) transactions.

  class score_board::EXAMPLE::single_cycle_t::data_values_t final
    : public ::score_board::base_t<::transaction::single_cycle_t>
  { // score board to check data output values from the example
    // pipeliine development class ::EXAMPLE::single_cycle_t.
    public:     // UVM evil
      UVM_COMPONENT_UTILS(
        ::score_board::EXAMPLE::single_cycle_t::data_values_t);

    public:     // c'tors and d'tor
      data_values_t(::uvm::uvm_component_name name)
        : base_t{name}
        , m_count{}
      { /* noOp */ }

      virtual ~data_values_t() override
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      virtual bool is_pass() const override
      { // verify that all pass criteria are met.
        return  m_count.is_pass();
      }

    protected:  // methods
    private:    // methods
      virtual void report_phase(::uvm::uvm_phase &) override
      {
        report_summary("DATA VALUE PASS/FAIL COUNT", m_count);
      }

      virtual void write(transaction_t const &item) override
      { // check the given transaction for correctness
        // NB: only tests the (singular) data value.
        // NB: assumes all control signals are correct.
        //--local variables and definitions
        using ::predictor::EXAMPLE::single_cycle_t::predict_state;

        auto const state_1  {item.value().state_1};
        auto const predicted{predict_state(item)};

        auto const value_out      {state_1.data.value};
        auto const value_predicted{predicted.value};

        //--track the outcome
        if  (not state_1.ctl.is_valid)
          m_count.moot();
        else if  (value_out == value_predicted)
          m_count.pass();
        else
          m_count.fail();

        //--report when a failure is observed
        if (state_1.ctl.is_valid and
            (value_out != value_predicted))
        {
          ::std::ostringstream error{};
          error
            << ::std::showbase << ::std::hex
            << "\n"
            << "  DATA VALUE OUTPUT TEST FAILED: \n"
            << "    Observed: " << value_out << ". \n"
            << "    Expected: " << value_predicted << ". \n";
          uvm_report_error("SCORE_BOARD", error.str());
        }

        //--end of compilation unit with implicit return
      }

    private:    // embedded classes and types
    private:    // attributes
      counter::pass_fail_count_t  m_count;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::score_board::EXAMPLE::single_cycle_t::control_bits_t
  //--verifies computed (output) control bits for the development example
  //--  class ::EXAMPLE::single_cycle_t

  class score_board::EXAMPLE::single_cycle_t::control_bits_t final
    : public ::score_board::base_t<::transaction::single_cycle_t>
  { // scoreboard to verify control signal input to outputs
    // for each sampled transaction.
    public: // UVM_evil
      UVM_COMPONENT_UTILS(
        ::score_board::EXAMPLE::single_cycle_t::control_bits_t);

    public: // c'tors and d'tor
      control_bits_t(::uvm::uvm_component_name name)
        : base_t{name}
        , m_count{}
      { /* noOp */ }

      virtual ~control_bits_t() override
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      virtual bool is_pass() const override
      { // All transaction should pass with no failures
        return  m_count.is_pass();
      }

    protected:  // methods
    private:    // methods
      virtual void report_phase(::uvm::uvm_phase &) override
      { // report this score board's interesting statistics as
        // UVM output information, should manual evaluation of
        // the observations be required.
        //--local variables and definitions
        report_summary("CONTROL BIT PASS/FAIL COUNT", m_count);
      }

      virtual void write(transaction_t const &item) override
      { // write a transaction to this score-board.
        // verifies written transaction signals and predicted match.
        // counts observed transaction passes and failures.
        // NB: verifies that the EXAMPLE/single_cycle class computes
        //     its output control signals correctly.

        //--collect the necessary signals expected and output
        using ::predictor::EXAMPLE::single_cycle_t::predict_controls;
        auto const expected{predict_controls(item)};
        auto const observed{item.value().state_1.ctl};

        //--verify that output and predicted match
        //--NB: control bit outputs are never moot.
        auto const is_equal
        {
          (observed.is_advance == expected.is_advance) and
          (observed.is_error   == expected.is_error  ) and
          (observed.is_stall   == expected.is_stall  ) and
          (observed.is_valid   == expected.is_valid  )
        };

        if (is_equal)
          m_count.pass();
        else
          m_count.fail();

        //--report when a failure is observed
        if (not is_equal)
        {
          ::std::ostringstream error{};
          error
            << "\n"
            << "  CONTROL OUTPUT VALUE TEST FAILED: \n"
            << "    Observed [A/E/S/V]: "
              << observed.is_advance << '/'
              << observed.is_error << '/'
              << observed.is_stall << '/'
              << observed.is_valid << ". \n"
            << "    Expected [A/E/S/V]: "
              << expected.is_advance << '/'
              << expected.is_error << '/'
              << expected.is_stall << '/'
              << expected.is_valid << ". \n";
          uvm_report_error("SCORE_BOARD", error.str());
        }

        //--end of compilation unit with implicit return
      }

    private:    // embedded classes and types
    private:    // attributes
      counter::pass_fail_count_t  m_count;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::score_board::EXAMPLE::single_cycle_t::control_bit_pairs_t
  //--verifies that all possible pairs of control signal inputs
  //--  occur across the course of the test.

  class score_board::EXAMPLE::single_cycle_t::control_bit_pairs_t final
    : public ::score_board::base_t<::transaction::single_cycle_t>
  { // scoreboard to verify that all possible pairs of control signal inputs
    // occur across the course of the test.
    public:     // useful declarations and definitions
      static unsigned constexpr N_INPUT_CONTROL_BITS{5};

    public:     // UVM evil
      UVM_COMPONENT_UTILS(
        ::score_board::EXAMPLE::single_cycle_t::control_bit_pairs_t);

    public:     // c'tors and d'tor
      control_bit_pairs_t(::uvm::uvm_component_name name)
        : base_t{name}
        , m_count{}
      { /* noOp */ }

      virtual ~control_bit_pairs_t() override
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      virtual bool is_pass() const override
      {
        return m_count.is_coverage_complete();
      }

    protected:  // methods
    private:    // methods
      virtual void report_phase(::uvm::uvm_phase &) override
      { // report this score board's interesting statistics, should
        // manual evaluation be required.
        report_summary("CONTROL INPUT PAIR COVERAGE", m_count);
      }

      void write(transaction_t const& item) override
      { // process a transaction written to this score-board
        //--join the control signals and build a single value from them
        auto stimuli{item.value().stimuli};
        unsigned bits{0};
        for (bool bit :
              {stimuli.ctl.reset,      stimuli.ctl.is_advance,
               stimuli.debug.is_error, stimuli.ctl.is_flush,
               stimuli.ctl.is_valid,})
          bits = ((bits << 1) + bit);

        //--updating measured statistics
        m_count.observe(bits);

        //--end of compilation unit with implicit return
      }

    private:    // embedded classes and types
    private:    // attributes
      counter::control_bit_pairs_t<N_INPUT_CONTROL_BITS>  m_count;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace score_board
  {
    // <none>
  } // namespace score_board

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__SCORE_BOARD_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
