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
//  sequencer.h
//  - 
//
//  NOTES:
//  2024-07-18: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__SEQUENCER_H__)
# define __SEQUENCER_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
  // <none>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "sequence_item.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace sequencer
  { // various sequencers
    //--base class for all project sequencers.
    //--NB: non-instantable.
    template<class REQUEST_SEQUENCE_ITEM_T,
             class RESPONSE_SEQUEMCE_ITEM_T = REQUEST_SEQUENCE_ITEM_T>
    class base_t;

    //--passes empty sequence items to drivers.
    //--NB: for basic driver development purposes.
    class empty_t;

    namespace EXAMPLE
    { // used for example modules
      class single_cycle_t;
    } // namespace ::sequencer::EXAMPLE
  } // namespace ::sequencer

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequencer::base_t
  //--base class for sequencers.
  //--NB: non-instantiable.
  //--NB: declares common type names for convenience.
  //--NB: nothing else particularly special here.

  template <class REQUEST_T, class RESPONSE_T>
  class sequencer::base_t
    : public ::uvm::uvm_sequencer<REQUEST_T, RESPONSE_T>
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
              = ::uvm::uvm_sequencer<request_t, response_t>;

    public:     // UVM evil
    public:     // c'tors and d'tor
    public:     // accessors
    public:     // methods
    protected:  // c'tors and d'tor
      base_t()
        : base_t{"::sequencer::base_t"}
      { /* noOp */ }
      base_t(::uvm::uvm_component_name name)
        : base_class_t{name}
      { /* noOp */ }
      virtual ~base_t() override
      { /* noOp */ }
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequencer::empty_t
  //--NB: used for basic driver development.
  //--NB: passes empty sequence items to drivers; expects no responses.

  class sequencer::empty_t final
    : public ::sequencer::base_t<::sequence_item::empty_t>
  {
    public:     // forward declarations and type aliases
    public:     // UVM evil
      UVM_COMPONENT_UTILS(::sequencer::empty_t);

    public:     // c'tors and d'tor
      empty_t()
        : empty_t{"::sequencer::empty_t"}
      { /* noOp */ }
      empty_t(::uvm::uvm_component_name name)
        : base_t{name}
      { /* noOp */ }
      virtual ~empty_t() override
      { /* noOp */ }
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequencer::EXAMPLE::single_cycle_t
  //--NB: used for basic driver development.
  //--NB: passes empty sequence items to drivers; expects no responses.

  class sequencer::EXAMPLE::single_cycle_t final
    : public ::sequencer::base_t<::sequence_item::EXAMPLE::single_cycle_t>
  {
    public:     // forward declarations and type aliases
    public:     // UVM evil
      UVM_COMPONENT_UTILS(::sequencer::EXAMPLE::single_cycle_t);

    public:     // c'tors and d'tor
      single_cycle_t()
        : single_cycle_t{"::sequencer::EXAMPLE::single_cycle_t"}
      { /* noOp */ }
      single_cycle_t(::uvm::uvm_component_name name)
        : base_t{name}
      { /* noOp */ }
      virtual ~single_cycle_t() override
      { /* noOp */ }
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace sequencer
  {
    // <none>
  } // namespace ::sequencer

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__SEQUENCER_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
