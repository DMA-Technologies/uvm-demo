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
//  sequence_item.h
//  - sequence items for all DUTs/MUTs
//  - mostly based on a ::std::variant as the data container.
//
//  NOTES:
//  2024-07-18: Moore, Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if  not defined(__SEQUENCE_ITEM_H__)
# define __SEQUENCE_ITEM_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <variant>

  //--required library includes
  // <none>

  //--required project includes
# include "UVM_wrapper.h"

# include "model.h"
# include "request.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace sequence_item
  { // UVM sequence items for each DUT/MUT type
    // base class for all our sequence items.
    // NB: intentionally implemented as a direct parent class
    //     for all sequence items, as opposed to a mix-in.
    // NB: forces implementation of randomise() for all
    //     derived sequence items.
    class base_t;

    // an empty sequuence item, contains no data.
    // NB: intended for basic driver development purposes.
    class empty_t;
  } // namespace ::sequence_item

  namespace sequence_item::EXAMPLE
  { // sequence items for DUTs/MUTs in the ::EXAMPLE namespace
    //--common carrier object DUT/MUT ::EXAMPLE::single_cycle_t;
    class single_cycle_t;
  } // namespace ::sequence_item::EXAMPLE

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence_item::base_t
  //--NB: requires implementation of randomise()

  class sequence_item::base_t
    : public ::uvm::uvm_sequence_item
  {
    public:     // UVM evil
    public:     // c'tors and d'tor
    public:     // accessors
    public:     // methods
      virtual void randomise() = 0;

    protected:  // c'tors and d'tor
      base_t()
        : ::uvm::uvm_sequence_item("::sequence_item::base_t")
      { /* noOp */ }
      base_t(::uvm::uvm_object_name name)
        : ::uvm::uvm_sequence_item(name)
      { /* noOp */ }
      virtual ~base_t() override
      { /* noOp */ }
  }; // class ::sequence_item::base_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence_item::empty_t
  //--NB: empty of data, intended for base-line driver development.

  class sequence_item::empty_t final
    : public ::sequence_item::base_t
  {
    public:     // forward declarations and type aliases
    public:     // UVM evil
      UVM_OBJECT_UTILS(::sequence_item::empty_t);

    public:     // c'tors and d'tor
      empty_t()
        : base_t("::sequence_item::empty_t")
      { /* noOp */ }
      empty_t(::uvm::uvm_object_name name)
        : base_t(name)
      { /* noOp */ }
      virtual ~empty_t() override final
      { /* noOp */ }

    public:     // methods
      virtual void randomise() override final
      { /* noOp - there are zero bits of data in this class */ }

    private:    // attributes
      // <none>
  }; // class ::sequence_item::empty_t

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--class ::sequence_item::EXAMPLE::single_cycle_t
  //--container for all request items whcih will be sent to drivers for
  //--  the class ::EXAMPLE::single_cycle_t DUT/MUT.
  //--NB: this is a container for all possible request types.

  class sequence_item::EXAMPLE::single_cycle_t final
    : public ::sequence_item::base_t
  {
    public:     // forward declarations and type aliases
      using error_t = ::request::error_t;
      using flush_t = ::request::flush_t;
      using hold_t  = ::request::hold_t;
      using reset_t = ::request::reset_t;

      using random_weighted_t =
        ::request::EXAMPLE::single_cycle_t::random_weighted_t;
      using value_fixed_t =
        ::request::EXAMPLE::single_cycle_t::value_fixed_t;

    public:     // UVM evil
      UVM_OBJECT_UTILS(::sequence_item::EXAMPLE::single_cycle_t);

    public:     // c'tors and d'tor
      single_cycle_t()
        : single_cycle_t{"::sequence_item::EXAMPLE::single_cycle_t"}
      { /* noOp */ }

      single_cycle_t(::uvm::uvm_object_name name)
        : single_cycle_t{name, reset_t{}}
      { /* noOp */ }

      single_cycle_t(char const *const name)
        : single_cycle_t{name, reset_t{}}
      { /* noOp */ }

      template<typename REQUEST_T>
      single_cycle_t(REQUEST_T request)
        : single_cycle_t{
          "::sequence_item::EXAMPLE::single_cycle_t",
          request}
      { /* noOp */ }

      template<typename REQUEST_T>
      single_cycle_t(::uvm::uvm_object_name name,
                     REQUEST_T const        request)
        : base_t{name}
        , m_variant{request}
      { /* noOp */ }

      virtual ~single_cycle_t() override final
      { /* noOp */ }

    public:     // accessors
    public:     // methods
      template<typename TYPE_T>
      bool constexpr holds() const
      { // one possibility for contained type ascertation
        // E.g.: if  (variadic->holds<empty_t>())  do_something();
        return  ::std::holds_alternative<TYPE_T>(m_variant);
      }

      template<typename TYPE_T>
      TYPE_T &get_request()
      { // get a specific request type
        // NB: template to reduce typing
        return  ::std::get<TYPE_T>(m_variant);
      }

      template<typename TYPE_T>
      TYPE_T const constexpr &get_request() const
      { // get a specific request type
        // NB: template to reduce typing
        return  ::std::get<TYPE_T>(m_variant);
      }

      virtual void randomise() override final
      { // randomise the current contained request.
        // dispatch is by type, of course.
        if  (holds<error_t>())
          get_request<error_t>().randomise();
        else if  (holds<flush_t>())
          get_request<flush_t>().randomise();
        else if  (holds<hold_t>())
          get_request<hold_t>().randomise();
        else if  (holds<reset_t>())
          get_request<reset_t>().randomise();
        else if  (holds<random_weighted_t>())
          get_request<random_weighted_t>().randomise();
        else if  (holds<value_fixed_t>())
          get_request<value_fixed_t>().randomise();
        else
          ::uvm::uvm_report_fatal(
            "FAIL",
            "Apparently we failed to randomise a particular request.");
      }

      template<typename REQUEST_T>
      auto &select()
      { // select an arbitrary embedded type
        // e.g.: object.select<random_weighted_t>()
        m_variant = ::std::move(REQUEST_T{});
        return  *this;
      }

      auto &select_error_t()
      { return  select<error_t>(); }
      auto &select_flush_t()
      { return  select<flush_t>(); }
      auto  &select_hold_t()
      { return  select<hold_t>(); }
      auto &select_reset_t()
      { return  select<reset_t>(); }
      auto &select_random_weighted_t()
      { return  select<random_weighted_t>(); }

      template<typename REQUEST_T>
      auto &set(REQUEST_T &request)
      { // set the request to that given.
        return  ((m_variant = request), *this);
      }

    protected:  // types
    protected:  // methods
    protected:  // attributes
    private:    // types
      using variant_t =
        ::std::variant
        <
          error_t,
          flush_t,
          hold_t,
          reset_t,
          random_weighted_t,
          value_fixed_t
        >;

    private:    // methods
    private:    // attributes
      variant_t m_variant;
  }; // class ::sequence_item::EXAMPLE::single_cycle_t

  //..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--deferred declarations
  //--NB: see comments at top of file

  namespace sequence_item
  {
    // <none>
  } // namespace ::sequence_item

  //..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--end of invocation guard
#endif // not defined(__SEQUENCE_ITEM_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
