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
//  utility.h
//  - some basic utility classes.
//
//  NOTES:
//  2023-10-22: Peimann: created.
//  2023-10-22: Peimann: added class utility::ring_buffer_t.
//  2024-08-23: Peimann: added enum uvm_return_code_t.
//  2024-09-26: Peimann: added namespace utility::format &c.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--invocation guard
#if not defined(__UTILITY_H__)
# define __UTILITY_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <array>
# include <climits>
# include <cstddef>
# include <ostream>
# include <ranges>
# include <sstream>
# include <stdexcept>
# include <type_traits>

  //--required library includes
  // <none>

  //--required project includes
  // <none>

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace utility
  {
    class activity_indicator_t;
    template<typename element_t, std::size_t N>
    class ring_buffer_t;
    class test_bits_t;
    enum class uvm_return_code_t;
  } // namespace ::utility

  namespace utility::format
  {
    enum class options_t;
    template<typename INTEGER_T>
    std::string binary(INTEGER_T const value, unsigned const count);
  } // namespace ::utility::format

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
#if false
#error "Hoist this into the ::utility namespace."
  REQUIREMENTS:
  a) print a tic mark every n cycles
  b) print a tic mark at final cycle followed by total number of cycles.
  c) print a maximum of "length" tic marks per line
  c.1) this implies that a newline must be inserted when condition is met
  d) allow prefix or indent at start of each new line of tic marks
  what are the arguments need to display - thoughts
    1) Cycle per tic.
    2) tics per line.
#endif

  class utility::activity_indicator_t
  {
    public:
      activity_indicator_t():
        activity_indicator_t(std::cout)
      { /* noOp */ }

      activity_indicator_t(std::ostream &stream)
        : m_cycle_iteration{0}
        , m_cycle_size{1}
        , m_line_tic{0}
        , m_line_size{72}
        , m_is_complete{false}
        , m_iteration{0}
        , m_cycles{0}
        , m_tic{'.'}
        , m_stream{stream.rdbuf()}
      { /* noOp */ }

      activity_indicator_t(unsigned const cycle_per_tic,
                           unsigned const cycle_per_line)
        : m_cycle_iteration{0}
        , m_cycle_size{cycle_per_tic}
        , m_line_tic{0}
        , m_line_size{cycle_per_line}
        , m_is_complete{false}
        , m_iteration{0}
        , m_cycles{0}
        , m_tic{'.'}
        , m_stream{std::cout.rdbuf()}
      { /* noOp */ }

      activity_indicator_t& tic()
      {
        if  (m_is_complete)
          throw  std::logic_error("activity_indicator_t: in complete state");

        if  (m_iteration == m_line_size)         // Need to Start of a line
        {
          m_iteration = 0;                      // restart number of characters per line (Like 80)
          m_stream << std::endl;
        }

        if  (m_cycle_iteration == 0)             // The number of number of iteratations per tic hit
          m_stream << m_tic;

        m_cycle_iteration++;
        if  (m_cycle_iteration == m_cycle_size)  // Are the number of number of iteratations per tic hit
        {
          m_cycle_iteration = 0;                // restart number of iteratations per tic
          m_iteration++;
        }

        m_cycles++;

        return  *this;
      }

      activity_indicator_t& complete()
      {
        if (m_is_complete)
          throw  std::logic_error("activity_indicator_t: why complete twice?");
        else if  (m_iteration < 2)
          m_stream << " Total Cycles: " << m_cycles << std::endl;
        else
          m_stream << m_tic << " Total Cycles: " << m_cycles << std::endl;
        m_is_complete = true;
        return  *this;
      }

      activity_indicator_t& reset()
      {
        if  (not m_is_complete)
          throw  std::logic_error("reset(): not complete.");
        m_is_complete = false;
        m_cycle_iteration = 0;
        m_iteration = 0;
        return  *this;
      }

    private:
      unsigned m_cycle_iteration;    // Tracks number of tics done  
      unsigned m_cycle_size;         // number of iteratations per tic
      unsigned m_line_tic;
      unsigned m_line_size;          // number of characters per line (Like 80)
                                   //  (len(prefix) + tics)
      bool     m_is_complete;
      unsigned m_iteration;          // Track number of iteratations per tic
      unsigned m_cycles;             // Track cycles completed
      char     m_tic;                // Character to display 

      std::ostream  m_stream;        // output goes here
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  template<typename element_t, std::size_t N>
  class utility::ring_buffer_t
  { // ring buffer which works backwards.
    // NB: implements interface methods consistent with std::queue.
    private:
      static_assert(N > 0);

    public:
      using container_type  = std::array<element_t, N>;
      using const_reference = typename container_type::const_reference;
      using reference       = typename container_type::reference;
      using size_type       = typename container_type::size_type;
      using value_type      = typename container_type::value_type;

    public:
      ring_buffer_t(): m_body{}, m_first{0}, m_size{0}
      { /* noOp - naught to do  right now */ }

      ring_buffer_t(ring_buffer_t const &that) = default;
      ring_buffer_t(ring_buffer_t &&that) = default;

      ~ring_buffer_t()
      { // d'tor: scramble attributes
        m_first = N;
        m_size = 0;
      }

      ring_buffer_t &operator=(ring_buffer_t const &that) = default;
      ring_buffer_t &operator=(ring_buffer_t &&that) = default;

    public:
      reference back()
      { // std::queue interface support
        throw  std::exception("ring_buffer_t::back(): unimplemented.");
      }

      const_reference back() const
      { // std::queue interface support
        throw  std::exception("ring_buffer_t::back() const: unimplemented.");
      }

      void clear()
      {
        m_size = 0;
      }

      bool empty() const
      { // std::queue interface support
        return  is_empty();
      }

      reference front()
      { // std::queue interface support
        return  peek();
      }

      const_reference front() const
      { // std::queue interface support
        return  peek();
      }

      value_type get()
      {
        if  (is_empty())
          throw  std::underflow_error("ring_buffer_t::get(): empty.");
        auto result{m_body.at(m_first)};
        m_first = next_first();
        m_size--;
        return  result;
      }

      bool is_empty() const noexcept
      {
        return  (m_size == 0);
      }

      bool is_full() const noexcept
      {
        return  (m_size == N);
      }

      size_type constexpr max_size() const noexcept
      {
        return  N;
      }

      reference peek()
      { // peek at first element in the buffer
        if  (is_empty())
          throw  std::underflow_error("ring_buffer_t::peek(): empty.");
        return  m_body.at(m_first);
      }

      const_reference peek() const
      { // peek at first element in the buffer
        if  (is_empty())
          throw  std::underflow_error("ring_buffer_t::peek() const: empty.");
        return  m_body.at(m_first);
      }

      value_type pop()
      { // std::queue interface support
        return  get();
      }

      void push(value_type const &value)
      { // std::queue interface support
        put(value);
      }

      void push(value_type &&value)
      { // std::queue interface support
        put(std::move(value));
      }

      void put(value_type const &value)
      {
        if  (is_full())
          throw  std::overflow_error("ring_buffer_t::put() [copy]: full.");
        m_body.at(first_free()) = value;
        m_size++;
      }

      void put(value_type &&value)
      {
        if  (is_full())
          throw  std::overflow_error("ring_buffer_t::put() [move]: full.");
        m_body.at(first_free()) = std::move(value);
        m_size++;
      }

      size_type size() const noexcept
      { // std::queue interface support
        return  m_size;
      }

      void swap(ring_buffer_t &that)
      { // std::queue interface support
        std::swap(this->m_body,  that.m_body );
        std::swap(this->m_first, that.m_first);
        std::swap(this->m_size,  that.m_size );
      }

    private:
      size_type first_free() const
      { // first free element beyond used buffer elements.
        // NB: no overrun error check.
        // NB: mathematics are designed to avoid unsigned integer wrap.
        size_type result;

        if  (m_first  <  (N - m_size))  // free follows first
          result = (m_first + m_size);
        else                            // free preceeds first
          result = (m_size - (N - m_first));

        return  result;
      }

      size_type next_first() const
      { // increment to next first element index, wrapping at end.
        return  ((m_first==(N-1)) ? 0 : (m_first+1));
      }

    private:
      container_type m_body;   // buffer of elements
      size_type      m_first;  // location of first available element
      size_type      m_size;   // number of elements in the buffer
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

  class utility::test_bits_t
  {
    public:
      test_bits_t() : test_bits_t{compl 0u}
      { /* noOp */ }

      test_bits_t(unsigned const value) : m_bits{value}
      { /* noOp */ }

      void increment()
      {
        m_bits++;
      }

      bool boolean(unsigned nn) const
      {
        return  ((m_bits bitand (1u << nn)) != 0);
      }

      std::string string() const
      {
        std::ostringstream ss;
        ss << "0x" << std::hex << m_bits;
        return  ss.str();
      }

    private:
      unsigned m_bits;
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // enum class ::utility::uvm_return_code:t
  // return codes for use with UVM tests.

  enum class utility::uvm_return_code_t : int
  {
    success   = 0,  // unqualified success.
    uvm_fatal = 1,  // uvm fatality; implicit return code of one (1).
    test_fail = 2,  // the test failed.
    not_set   = 3,  // the test did not set a return code.
  };

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  // ::utility::format::binary(value, count)
  // returns a string containing the binary representation of the
  // count low-order bits of value.
  // e.g. bits(0x71C, 4) --> "0b1100"
  // NB: this function solely exists because GNU 12.2 does not
  //     support std::format.

  template<typename INTEGER_T>
  std::string utility::format::binary(INTEGER_T const value,
                                      unsigned const  count)
  {
    static_assert(std::is_integral<INTEGER_T>::value,
                  "format::to_bits(...) only supports integral types.");

    unsigned constexpr bits_max{sizeof(INTEGER_T) * CHAR_BIT};
    unsigned const     bits    {((0 < count) and (count <= bits_max))
                                 ? count : bits_max};
    std::string        result  {"0b", (2+bits)}; // init full length

    for  (unsigned const bit: std::ranges::views::iota(0u, bits))
      result[2 + bit] =
        static_cast<char>('0' + ((value >> (bits-bit-1)) bitand 1));

    return  result;
  }

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//  end of reentrance guard

#endif // not defined(__UTILITY_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
