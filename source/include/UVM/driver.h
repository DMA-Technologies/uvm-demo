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
//  driver.h
//  - UVM based drivers for all modules
//
//  NOTES:
//  2024-08-17: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__DRIVER_H__)
# define __DRIVER_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "request.h"
# include "sequence_item.h"
# include "sequencer.h"
# include "vif.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace driver
  {
    //--base class for all of our drivers.
    //--NB: cannot be directly instantiated.
    template <class VIRTUAL_INTERFACE_T,
              class REQUEST_SEQUENCE_ITEM_T,
              class RESPONSE_SEQUEMCE_ITEM_T = REQUEST_SEQUENCE_ITEM_T>
    class base_t;

    namespace EXAMPLE
    {
      //--driver for the DUT/MUT class ::EXAMPLE::single_cycle_t.
      class single_cycle_t;
    } // namespace driver::EXAMPLE
  } // namespace driver

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::driver::base_t
  //--base class for all drivers.
  //--NB: allows commonality of behaviour.
  //--NB: cannot be instantiated directly.

  template<class VIF_T, class REQUEST_T, class RESPONSE_T>
  class driver::base_t
    : public ::uvm::uvm_driver<REQUEST_T, RESPONSE_T>
  {
    public:     // forward declarations and type aliases
      using request_t       = REQUEST_T;
      using response_t      = RESPONSE_T;
      using sequencer_t     = ::sequencer::base_t<request_t, response_t>;
      using sequencer_ptr   = ::std::shared_ptr<sequencer_t>;
      using vif_t           = VIF_T;
      using vif_ptr         = VIF_T*;

      // FIXME: ought these only be declared using concepts, when
      //        the REQUEST_T and RESPONSE_T are of the same type?
      using item_t          = REQUEST_T;
      using sequence_item_t = REQUEST_T;

    private:    // forward declarations and type aliases
      using base_class_t    = ::uvm::uvm_driver<request_t, response_t>;

    public:     // UVM evil
    public:     // c'tors and d'tor
    public:     // accessors
    public:     // methods
      void connect_to(sequencer_ptr const sequencer)
      { // connect our input to the output of the given sequencer
        // NB: these ports are implemented implicitly in the UVM parent classes
        this->seq_item_port.connect(sequencer->seq_item_export);
      }

    protected:  // c'tors and d'tor
      base_t()
        : base_t{"::driver::base_t"}
      { /* noOp */ }
      base_t(::uvm::uvm_component_name name)
        : base_class_t{name}
        , m_vif{nullptr}
      { /* noOp */ }
      virtual ~base_t() override
      { // make unusable
        m_vif = nullptr;
      }

      base_t(base_t const &) = delete;
      base_t &operator=(base_t const &) = delete;

    protected:  // accessors
      vif_t &vif()
      { return  *m_vif; }

    protected:  // methods
      void build_phase(::uvm::uvm_phase &phase) override
      {
        //--parent class behaviour as a pre-method
        //--NB: the ::uvm_driver class method is not a strict pre-method,
        //--    which appears to be a bug in the design.
        base_class_t::build_phase(phase);

        //--implicitly connect to the DUT bus
        if (not ::uvm::uvm_config_db<vif_ptr>::get(this, "", "vif", m_vif))
          ::uvm::uvm_report_fatal(
            "NO_VIF",
            "Virtual interface not found in config_db.");

        //--end of compilation unit with implicit return
      }

    protected:  // type and class definitions
    protected:  // attributes
    private:    // methods
    private:    // type and class definitions
    private:    // attributes
      vif_ptr  m_vif;
  }; // class ::driver::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::driver::EXAMPLE::single_cycle_t
  //--driver for the ::EXAMPLE::single_cycle_t module.

  class driver::EXAMPLE::single_cycle_t final
    : public ::driver::base_t<::vif::EXAMPLE::single_cycle_t,
                              ::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
      using development_t     = ::request::development_t;
      using error_t           = ::request::error_t;
      using flush_t           = ::request::flush_t;
      using hold_t            = ::request::hold_t;
      using reset_t           = ::request::reset_t;
      using random_weighted_t =
        ::request::EXAMPLE::single_cycle_t::random_weighted_t;
      using value_fixed_t =
        ::request::EXAMPLE::single_cycle_t::value_fixed_t;

    public:     // UVM evil
      UVM_COMPONENT_UTILS(::driver::EXAMPLE::single_cycle_t);

    public:     // c'tors and d'tor
      single_cycle_t()
        : single_cycle_t("::driver::fixed_sequence_t")
      { /* noOp */ }
      single_cycle_t(::uvm::uvm_component_name name)
        : base_t(name)
      { /* noOp */ }
      virtual ~single_cycle_t() override
      { /* noOp */ }

    public:     // accessors
    public:     // methods
    protected:  // accessors
    protected:  // methods
      void build_phase(uvm::uvm_phase &phase) override
      { // get, or build, required bits, as necessary.
        //--parent class behaviour as a pre-method
        base_t::build_phase(phase);
        //--get or build!
        //--end of compilation unit with implicit return
      }

      void run_phase(uvm::uvm_phase &phase) override
      { // spawnn necessary tasks.
        //--parent class behaviour as a pre-method
        base_t::run_phase(phase);
        //--do the spawning
        SC_FORK
          sc_spawn(sc_bind(&single_cycle_t::do_sequence_items, this)),
        SC_JOIN
        //--end of compilation unit with implicit return
      }

    protected:  // type and class definitions
    protected:  // attributes
    private:    // methods
      void do_wait_for_clock()
      { // wait for the next cycle
        wait(vif().clock_posedge());
      }

      void do_sequence_items()
      { // work off all sequence items
        sequence_item_t item;

        while  (true)
        {
          seq_item_port.get_next_item(item);  // UVM SystemC
          item_dispatch(item);
          seq_item_port.item_done();
        }
      }

      void item_dispatch(sequence_item_t const &item)
      { // dispatch the given sequence item to the intended worker.
        if  (item.holds<error_t>())
          item_process(item.get_request<error_t>());
        else if  (item.holds<flush_t>())
          item_process(item.get_request<flush_t>());
        else if  (item.holds<hold_t>())
          item_process(item.get_request<hold_t>());
        else if  (item.holds<reset_t>())
          item_process(item.get_request<reset_t>());
        else if  (item.holds<random_weighted_t>())
          item_process(item.get_request<random_weighted_t>());
        else if (item.holds<value_fixed_t>())
          item_process(item.get_request<value_fixed_t>());
        else
          uvm_report_fatal(
            "DRIVER",
            "cannot dispatch based on item contained thing.");
      }

      void item_process(error_t const &)
      { // process a error request
        // fill in vif 
        auto i_binary = 0xE1000000;
        auto i_number = 0xE8000000;
        vif().set_control_inputs(1, 1, 0, 1); //reset, is_advance, is_flush, is_valid 
        vif().set_data_inputs(compl i_binary);
        vif().set_debug_inputs(i_binary, i_number, true, "ERROR");
        do_wait_for_clock();
      } // process a error request

      void item_process(flush_t const &)
      { // process a flush request

        // fill in vif 
        auto i_binary = 0xE2000000;
        auto i_number = 0xEA000000;
        vif().set_control_inputs(1, 1, 1, 1); //reset, is_advance, is_flush, is_valid 
        vif().set_data_inputs(compl i_binary);
        vif().set_debug_inputs(i_binary, i_number, false, "FLUSH");
        do_wait_for_clock();
      } // process a flush request

      void item_process(hold_t const &)
      { // process a hold request
        // fill in vif 
        auto i_binary = 0xE4000000;
        auto i_number = 0xEC000000;
        vif().set_control_inputs(1, 1, 0, 0); //reset, is_advance, is_flush, is_valid 
        vif().set_data_inputs(compl i_binary);
        vif().set_debug_inputs(i_binary, i_number, false, "HOLD");
        do_wait_for_clock();
      }

      void item_process(reset_t const &)
      { // process a reset request
        // NB: item has a reset_t request
        // fill in vif 
        auto i_binary = 0xD1000000;
        auto i_number = 0xD8000000;
        vif().set_control_inputs(0, 0, 0, 0);
        vif().set_data_inputs(compl i_binary);
        vif().set_debug_inputs(i_binary, i_number, false, "RESET");
        do_wait_for_clock();
      }

      void item_process(random_weighted_t const &request)
      { // process a randomly weighted request
        // NB: the request is randomised by the sequence
        auto is_advance = request.is_advance();
        auto is_error = request.is_error();
        auto is_flush = request.is_flush();
        auto is_valid = request.is_valid();
        auto reset = request.reset();
        auto i_binary = request.i_binary();
        auto i_number = request.i_number();
        vif().set_control_inputs(reset, is_advance, is_flush, is_valid);
        vif().set_data_inputs(compl i_binary);
        vif().set_debug_inputs(i_binary, i_number, is_error, "RANDOM_WEIGHTED");
        do_wait_for_clock();
      }

      void item_process(value_fixed_t const &request)
      { // process a request that feed in
        // send the given request to the DUT/MUT.
        auto is_advance = request.is_advance();
        auto is_error = request.is_error();
        auto is_flush = request.is_flush();
        auto is_valid = request.is_valid();
        auto reset = request.reset();
        auto i_binary = request.i_binary();
        auto i_number = request.i_number();
        auto prefix = request.prefix();
        if  (false and not prefix.empty())
          uvm_report_info("DEBUG", ("PREFIX[" + prefix + "]"));
        vif().set_control_inputs(reset, is_advance, is_flush, is_valid);
        vif().set_data_inputs(compl i_binary);
        vif().set_debug_inputs(i_binary, i_number, is_error, prefix);
        do_wait_for_clock();
      }

    private:    // type and class definitions
    private:    // attributes
  }; // class ::driver::EXAMPLE::single_cycle_t

  //..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__DRIVER_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
