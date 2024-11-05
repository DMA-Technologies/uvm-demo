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
//  UVM.h
//  - wrapper for <uvm> header file to suppress compiler warnings in
//    UVM only.
//  - provides namespace UVM to allow future wrappers, should they
//    be desirable.
//  - created so all the project code is subject to maximal warnings
//    which are then reported as errors.
//
//  NOTES:
//  2024-09-26: Moore and Peimann: created.
//
//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//--invocation guard
#if not defined(__UVM_H__)
# define __UVM_H__

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
#   pragma GCC diagnostic ignored "-Wstrict-overflow"
#   pragma GCC diagnostic ignored "-Wswitch-enum"
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Wuseless-cast"
# endif

# include <uvm>

# if  defined(__GNUC__)
#   pragma GCC diagnostic pop
# endif

  //--required project includes
  // <none>

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--declaratons

  namespace UVM
  {
    // <none-as-yet>
  } // namespace UVM

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--definitions

  //1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
  //--end of invocation guard

#endif // not defined(__UVM_H__)

//..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
//
//--end of file
//
