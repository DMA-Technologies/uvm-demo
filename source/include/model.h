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
//  model.h
//  - the basic processor data model.
//  - use these types and objects in preference to open coding.
//
//  NOTES:
//  2023-06-20: Moore and Peimann: first cut.
//  2023-06-20: NB: all model data types must be synthesizable.
//  2023-08-10: Peimann: revised types to "generic" pipeline model.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

//--invocation guard
#if  not defined(__MODEL_H__)
# define __MODEL_H__

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--required system includes
# include <array>
# include <compare>
# include <cstdint>

  //--required library includes
  // <none>

  //--required project includes
# include "SC.h"

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace model
  {
    using address_t      = std::uint32_t;    // general machine address
    using word_t         = std::uint32_t;    // general machine word
    using bit_t          = bool;             // a single bit, cannot "tristate"

    address_t constexpr pc_reset{0x100};

    using reset_t        = bit_t;            // 0:reset; 1:otherwise
    using instruction_t  = word_t;           // instructions are one full-word

    using i_number_t     = address_t;        // instruction number (addr = (i_num<<2))
    using i_binary_t     = instruction_t;    // instruction binary (32 bits)
    using function_t     = ::SC::uint<3>;    // functional unit function number
    using value_t        = word_t;           // 
    using carry_t        = bit_t;            // 0:0-carry-value; 1:1-carry-value
    using gpr_number_t   = ::SC::uint<5>;    // 32 registers
    using is_valid_t     = bit_t;            // output "value(s)" are valid
    using is_advance_t   = bit_t;            // pipeline advance
    using is_flush_t     = bit_t;            // pipeline flush
    using is_stall_t     = bit_t;            // pipeline stall
    using is_error_t     = bit_t;            // an error was detected.
    using mem_line_t     = std::array<value_t, 4>;
    using mem_tag_t      = std::uint32_t;
    using io_port_t      = std::uint16_t;
    using timer_t        = std::uint64_t;    // system_timer
    using cc_t           = ::SC::uint<2>;    // condition code
    using is_valid_cc_t  = bit_t;            // CC write is valid
    using is_valid_reg_t = bit_t;            // Register value is valid for io_read
    using read_write_t   = bit_t;            // read/write bit (memory access)
                                             //   0/false:read  1/true:write

    read_write_t constexpr read {false};     // read/write signal values
    read_write_t constexpr write{true};
    
    using value_mask_t   = ::SC::uint<4>;    // byte mask for 32 bit value
    using is_complete_t  = bit_t;            // transaction complete flag
                                             //   0/false: incomplete or no transaction
                                             //   1/true:  transaction complete
    using mem_enable_t   = bit_t;            // memory enable bit
                                             //   0/false: not enabled
                                             //   1/true:  enabled
    using mem_mask_t     = ::SC::uint<16>;   // byte mask for 128-bit mem-line

    enum class unit_t: unsigned
    { // functional unit designations
      system = 5,
      //fxu    = 3,
      // &c;
    };
  } // namespace model

  // data passed betwixt modules
  namespace model
  {
    struct i_decoded_t
    { // decoder to register-read data
      // "DECODED instruction information"
      using ordering = ::std::partial_ordering;

      ::std::uint32_t unit      :  3;  // functional unit
                                       //   - decoded from full opcode
      ::std::uint32_t function  :  3;  // unit specific function
                                       //   - decoded from full opcode
      ::std::uint32_t rS_rT     :  5;  // inst[25:21]: register number
      ::std::uint32_t rA        :  5;  // inst[20:16]: register number
      ::std::uint32_t rB_rU     :  5;  // inst[15:11]: register number
      ::std::uint32_t use_rS    :  2;  // use of rS_rT; decoded from full opcode
                                       //   - 0: rS value
                                       //   - 1: zero
                                       //   - 2: <tbd>
                                       //   - 3: imm1
      ::std::uint32_t use_rA    :  2;  // use of rA; decoded from full opcode
                                       //   - 0: rA value
                                       //   - 1: zero
                                       //   - 2: rS value
                                       //   - 3: PC of instruction
      ::std::uint32_t use_rB    :  2;  // use of rB; decoded from full opcode
                                       //   - 0: rB value
                                       //   - 1: zero
                                       //   - 2: rB value inverted
                                       //   - 3: imm1
      ::std::uint32_t use_cy    :  2;  // use of carry bit (might go away)
                                       //   - 0: zero
                                       //   - 1: one
                                       //   - 2: (future) PSW[carry]
                                       //   - 4: <tbd>
      ::std::uint32_t imm0      :  8;  // immediate value
                                       //   - adder fn4
                                       //   - bool3 table8
                                       //   - branch bitmap4
                                       //   - shifter fn4
                                       //   - load/store size2
      ::std::uint32_t imm1      : 32;  // immediate value

      auto constexpr operator<=>(i_decoded_t const &that) const
      { // general comparison operator
        // NB: implicitly causes deletion of specific comparison operators.
        // NB: define comparison operators (e.g. operator==), as required.
        //--local variables and definitions
        ordering result{ordering::equivalent}; // NB: GNU requires a value

        //--detailed test based on unit type
        switch  (unit_t{this->unit})
        {
          case  unit_t::system:
            {
              std::uint32_t aa{this->unit};
              std::uint32_t bb{that.unit};
              result = (aa <=> bb);
            }
            if  (result == 0)
            {
              std::uint32_t aa{this->function};
              std::uint32_t bb{that.function};
              result = (aa <=> bb);
            }
            break;
          default:
            result = ordering::unordered;
        }

        //--end of compilation unit with explicit return
        return  result;
      }

      bool constexpr operator==(i_decoded_t const &that) const
      {
        return  ((*this <=> that) == ordering::equivalent);
      }
    };

    struct i_raw_t final
    { // prefetch to decoder data
      // "RAW instruction information"
      i_binary_t i_binary;
      i_number_t i_number;

      auto operator<=>(i_raw_t const &) const = default;
    };

    struct i_tbd_t final
    { // register-read to functional-unit data
      // "DECODED instruction information and register values"
      std::uint32_t system     :  1;  //  1 bit
      std::uint32_t unit       :  4;  //  4 bits
      std::uint32_t function   :  2;  //  2 bits
      std::uint32_t fn         :  4;  //  4 bits - fn/bitmap4
      std::uint32_t rT_rS      :  5;  //  5 bits
      std::uint32_t rT_value   : 32;  // 32 bits
      std::uint32_t rA         :  5;  //  5 bits
      std::uint32_t rA_value   : 32;  // 32 bits
      std::uint32_t rB_rU      :  5;  //  5 bits
      std::uint32_t rB_value   : 32;  // 32 bits - either rB value or imm1 (see i_decoded_t.use_rB_imm)
      std::uint32_t imm0       :  5;  //  5 bits
      std::uint32_t imm1       : 32;  // 32 bits

      auto operator<=>(i_tbd_t const &) const = default;
    };

  } // namespace model

  namespace model::input_output
  { // functional aspects of the input_output module.
    namespace cc
    {
      unsigned /*cc_t*/ constexpr success {0};
      unsigned /*cc_t*/ constexpr fail    {1};
      unsigned /*cc_t*/ constexpr bad_port{2};
      unsigned /*cc_t*/ constexpr disaster{3};
    } // namespace model::input_output_cc

    namespace function
    { // functions which are supported (see: model::function_t)
      unsigned constexpr io_read {0};
      unsigned constexpr io_write{1};
    } // namespace model::input_output::function

    namespace port
    { // I/O port numbers
      io_port_t constexpr timer     {0x0000};
      io_port_t constexpr uart_read {0x1000};
      io_port_t constexpr uart_write{0x1010};
    } // namespace model::input_output::port
  } // namespace model::input_output

  namespace model::instruction
  { // instruction specific information
    //--instruction mechanic numbers and helpers
    unsigned constexpr bits_per_byte{8};

    //--instruction address structure of a memory line
    unsigned constexpr mbz_bits {2};
    unsigned constexpr addr_bits{30};

    //--base offsets within an address
    unsigned constexpr mbz_bit0 {0};
    unsigned constexpr addr_bit0{mbz_bit0 + mbz_bits};

    //--useful address mask(s)
    i_number_t constexpr mbz_mask {(((1u << mbz_bits ) - 1) << mbz_bit0 )};  // 0x00000003
    i_number_t constexpr addr_mask{(((1u << addr_bits) - 1) << addr_bit0)};  // 0xFFFFFFFC

    //--derived values
    unsigned constexpr size{(1 << addr_bit0)};

    //--sanity check ourselves
    static_assert((sizeof(i_number_t) == (1 << mbz_bits)),
                  "instruction sizing is not consistent in the model.");

    //--support methods
    i_number_t constexpr inline next(address_t const address)
    { // given an instruction address, returns the next instruction number.
      return  ((address bitand addr_mask) + size);
    }
  } // namespace model::instruction

  namespace model::memory
  { // memory specifc values
    unsigned constexpr read {0};
    unsigned constexpr write{1};

    // memory mechanic numbers and helpers
    unsigned constexpr bits_per_byte{8};
    unsigned constexpr bits_per_word{sizeof(word_t) * bits_per_byte};

    namespace line
    { // memory line structure
      //--instruction address structure of a memory line
      unsigned constexpr byte_bits{2};   // index of a byte in a word
      unsigned constexpr word_bits{2};   // index of a word in a memory line
      unsigned constexpr tag_bits {28};  // memory tag/physical address bits

      //--base offsets within an address
      unsigned constexpr byte_bit0{0};                      // word[1:0]
      unsigned constexpr word_bit0{byte_bit0 + byte_bits};  // word[3:2]
      unsigned constexpr tag_bit0 {word_bit0 + word_bits};  // word[31:4]

      //--useful address masks
      address_t constexpr byte_mask{(((1u << byte_bits) - 1) << byte_bit0)};  // 0x00000003
      address_t constexpr word_mask{(((1u << word_bits) - 1) << word_bit0)};  // 0x0000000C
      address_t constexpr tag_mask {(((1u << tag_bits)  - 1) << tag_bit0 )};  // 0xFFFFFFF0

      //--derived values
      address_t constexpr size{(1 << tag_bit0)};

      //--sanity check ourselves
      static_assert(((1 << byte_bits) == sizeof(mem_line_t::value_type)),
                    "The number of bits allocated to indexing a byte within "
                    "a word from a memory line is incorrect.");

      static_assert(((1 << word_bits) == mem_line_t{}.size()),
                    "The number of bits allocated to indexing a word within "
                    "a memory line is incorrect.");

      static_assert((byte_bits + word_bits + tag_bits) == bits_per_word,
                    "The memory line address structure does not correctly "
                    "account for all bits in an address.");

      static_assert(sizeof(mem_line_t) == (1 << (byte_bits + word_bits)),
                    "The memory line address structure is not consistent "
                    "with the memory line size.");

      static_assert((sizeof(mem_line_t) == (1 << tag_bit0)),
                    "The memory line tag structure is not consistent "
                    "with the memory line size.");

      //--support methods
      unsigned constexpr inline instruction_index(address_t const address)
      { // given an instruction address, returns the index of that instruction
        // within its memory line.
        return  ((address bitand word_mask) >> word_bit0);
      }

      mem_tag_t constexpr inline tag(address_t const address)
      { // given an instruction address, returns the memory line tag.
        return  (address bitand tag_mask);
      }

      i_number_t constexpr inline tag_next(address_t const address)
      { // given an instruction address, returns the memory line tag of the
        // next memory line.
        return  ((address bitand tag_mask) + size);
      }
    } // namespace model::memory::line
  } // namespace model::memory

  namespace model::memory_controller
  { // memory controller values and helper methods
    // memory specifc signal values
    model::read_write_t constexpr read {model::read};
    model::read_write_t constexpr write{model::write};
  } // namespace model::memory_controller

  namespace model::prefetch
  { // prefetch unit (PFU) values and helper methods
    // number of buffer entries must be a power of two (2).
    unsigned constexpr entry_bits {2};
    unsigned constexpr entry_count{(1 << entry_bits)};
    unsigned constexpr entry_mask {((1 << entry_bits) - 1)};

  } // namespace model::prefetch

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--SystemC++ signal and port type definitions.

  namespace model::signal
  { // (SystemC++) signal type definitions
    using clock_t       = ::SC::clock;
    using reset_t       = ::SC::signal<model::reset_t>;
    using address_t     = ::SC::signal<model::address_t>;
    using instruction_t = ::SC::signal<model::instruction_t>;
    using i_number_t    = ::SC::signal<model::i_number_t>;
    using i_binary_t    = ::SC::signal<model::i_binary_t>;
    using function_t    = ::SC::signal<model::function_t>;
    using value_t       = ::SC::signal<model::value_t>;
    using word_t        = ::SC::signal<model::word_t>;
    using carry_t       = ::SC::signal<model::carry_t>;
    using gpr_number_t  = ::SC::signal<model::gpr_number_t>;
    using is_valid_t    = ::SC::signal<model::is_valid_t>;
    using is_advance_t  = ::SC::signal<model::is_advance_t>;
    using is_flush_t    = ::SC::signal<model::is_flush_t>;
    using is_stall_t    = ::SC::signal<model::is_stall_t>;
    using is_error_t    = ::SC::signal<model::is_error_t>;
    using mem_line_t    = ::SC::signal<model::mem_line_t>;
    using mem_tag_t     = ::SC::signal<model::mem_tag_t>;
    using io_port_t     = ::SC::signal<model::io_port_t>;
    using cc_t          = ::SC::signal<model::cc_t>;
    using is_valid_cc_t = ::SC::signal<model::is_valid_cc_t>;     // CC write is valid
    using is_valid_reg_t= ::SC::signal<model::is_valid_reg_t>;    // Register value is valid for io_read
    using read_write_t  = ::SC::signal<model::read_write_t>;
    using value_mask_t  = ::SC::signal<model::value_mask_t>;
    using is_complete_t = ::SC::signal<model::is_complete_t>;
    using mem_enable_t  = ::SC::signal<model::mem_enable_t>;
    using mem_mask_t    = ::SC::signal<model::mem_mask_t>;

    using i_raw_t       = ::SC::signal<i_raw_t>;
    using i_decoded_t   = ::SC::signal<i_decoded_t>;
  } // namespace model::signal

  namespace model::exprt
  { // (SystemC++) exported output port type definitions
    using clock_t       = ::SC::exprt<model::signal::clock_t>;
    using reset_t       = ::SC::exprt<model::signal::reset_t>;
    using address_t     = ::SC::exprt<model::signal::address_t>;
    using instruction_t = ::SC::exprt<model::signal::instruction_t>;
    using i_number_t    = ::SC::exprt<model::signal::i_number_t>;
    using i_binary_t    = ::SC::exprt<model::signal::i_binary_t>;
    using function_t    = ::SC::exprt<model::signal::function_t>;
    using value_t       = ::SC::exprt<model::signal::value_t>;
    using word_t        = ::SC::exprt<model::signal::word_t>;
    using carry_t       = ::SC::exprt<model::signal::carry_t>;
    using gpr_number_t  = ::SC::exprt<model::signal::gpr_number_t>;
    using is_valid_t    = ::SC::exprt<model::signal::is_valid_t>;
    using is_advance_t  = ::SC::exprt<model::signal::is_advance_t>;
    using is_flush_t    = ::SC::exprt<model::signal::is_flush_t>;
    using is_stall_t    = ::SC::exprt<model::signal::is_stall_t>;
    using is_error_t    = ::SC::exprt<model::signal::is_error_t>;
    using mem_line_t    = ::SC::exprt<model::signal::mem_line_t>;
    using mem_tag_t     = ::SC::exprt<model::signal::mem_tag_t>;
    using io_port_t     = ::SC::exprt<model::signal::io_port_t>;
    using cc_t          = ::SC::exprt<model::signal::cc_t>;
    using is_valid_cc_t = ::SC::exprt<model::signal::is_valid_cc_t>;  // CC write is valid
    using is_valid_reg_t= ::SC::exprt<model::signal::is_valid_reg_t>; // Register value is valid for io_read
    using read_write_t  = ::SC::exprt<model::signal::read_write_t>;
    using value_mask_t  = ::SC::exprt<model::signal::value_mask_t>;
    using is_complete_t = ::SC::exprt<model::signal::is_complete_t>;
    using mem_enable_t  = ::SC::exprt<model::signal::mem_enable_t>;
    using mem_mask_t    = ::SC::exprt<model::signal::mem_mask_t>;

    using i_raw_t       = ::SC::exprt<model::signal::i_raw_t>;
    using i_decoded_t   = ::SC::exprt<model::signal::i_decoded_t>;
  } // namespace model::exprt

  namespace model::port
  { // (SystemC++) port type definitions
    // NB: input ports and output ports must be symmetrical
    namespace in
    { // namespace model::port::in
      using clock_t       = ::SC::in<bool>;
      using reset_t       = ::SC::in<model::reset_t>;
      using address_t     = ::SC::in<model::address_t>;
      using instruction_t = ::SC::in<model::instruction_t>;
      using i_number_t    = ::SC::in<model::i_number_t>;
      using i_binary_t    = ::SC::in<model::i_binary_t>;
      using function_t    = ::SC::in<model::function_t>;
      using value_t       = ::SC::in<model::value_t>;
      using word_t        = ::SC::in<model::word_t>;
      using carry_t       = ::SC::in<model::carry_t>;
      using gpr_number_t  = ::SC::in<model::gpr_number_t>;
      using is_valid_t    = ::SC::in<model::is_valid_t>;
      using is_advance_t  = ::SC::in<model::is_advance_t>;
      using is_flush_t    = ::SC::in<model::is_flush_t>;
      using is_stall_t    = ::SC::in<model::is_stall_t>;
      using is_error_t    = ::SC::in<model::is_error_t>;
      using mem_line_t    = ::SC::in<model::mem_line_t>;
      using mem_tag_t     = ::SC::in<model::mem_tag_t>;
      using io_port_t     = ::SC::in<model::io_port_t>;
      using cc_t          = ::SC::in<model::cc_t>;
      using is_valid_cc_t = ::SC::in<model::is_valid_cc_t>;  // CC write is valid
      using is_valid_reg_t= ::SC::in<model::is_valid_reg_t>; // Register value is valid for io_read
      using read_write_t  = ::SC::in<model::read_write_t>;
      using value_mask_t  = ::SC::in<model::value_mask_t>;
      using is_complete_t = ::SC::in<model::is_complete_t>;
      using mem_enable_t  = ::SC::in<model::mem_enable_t>;
      using mem_mask_t    = ::SC::in<model::mem_mask_t>;

      using i_raw_t       = ::SC::in<model::i_raw_t>;
      using i_decoded_t   = ::SC::in<model::i_decoded_t>;
    } // namespace model::port::in
    namespace out
    { // namespace model::port::out
      using clock_t       = ::SC::out<bool>;
      using reset_t       = ::SC::out<model::reset_t>;
      using address_t     = ::SC::out<model::address_t>;
      using instruction_t = ::SC::out<model::instruction_t>;
      using i_number_t    = ::SC::out<model::i_number_t>;
      using i_binary_t    = ::SC::out<model::i_binary_t>;
      using function_t    = ::SC::out<model::function_t>;
      using value_t       = ::SC::out<model::value_t>;
      using word_t        = ::SC::out<model::word_t>;
      using carry_t       = ::SC::out<model::carry_t>;
      using gpr_number_t  = ::SC::out<model::gpr_number_t>;
      using is_valid_t    = ::SC::out<model::is_valid_t>;
      using is_advance_t  = ::SC::out<model::is_advance_t>;
      using is_flush_t    = ::SC::out<model::is_flush_t>;
      using is_stall_t    = ::SC::out<model::is_stall_t>;
      using is_error_t    = ::SC::out<model::is_error_t>;
      using mem_line_t    = ::SC::out<model::mem_line_t>;
      using mem_tag_t     = ::SC::out<model::mem_tag_t>;
      using io_port_t     = ::SC::out<model::io_port_t>;
      using cc_t          = ::SC::out<model::cc_t>;
      using is_valid_cc_t = ::SC::out<model::is_valid_cc_t>;  // CC write is valid
      using is_valid_reg_t= ::SC::out<model::is_valid_reg_t>; // Register value is valid for io_read
      using read_write_t  = ::SC::out<model::read_write_t>;
      using value_mask_t  = ::SC::out<model::value_mask_t>;
      using is_complete_t = ::SC::out<model::is_complete_t>;
      using mem_enable_t  = ::SC::out<model::mem_enable_t>;
      using mem_mask_t    = ::SC::out<model::mem_mask_t>;

      using i_raw_t       = ::SC::out<model::i_raw_t>;
      using i_decoded_t   = ::SC::out<model::i_decoded_t>;
    } // namespace model::port::out
  } // namespace model::port

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions

  namespace model
  {
    unsigned constexpr inline instruction_index(address_t const address)
    { // given an instruction address, returns the index of that instruction
      // within its memory line.
      return  memory::line::instruction_index(address);
    }

    i_number_t constexpr inline next_instruction(address_t const address)
    { // given an instruction address returns the next instruction address
      return  instruction::next(address);
    }

    mem_tag_t constexpr inline next_tag(address_t const address)
    { // given an instruction address, returns the memory line tag of the
      // next line in sequence.  
      // NB: wraps at end of memory.  
      return  memory::line::tag_next(address);
    }

    mem_tag_t constexpr inline tag(address_t const address)
    { // given an instruction address, returns the memory line tag.
      return  memory::line::tag(address);
    }
  } // namespace model

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

#endif // not defined(__MODEL_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
