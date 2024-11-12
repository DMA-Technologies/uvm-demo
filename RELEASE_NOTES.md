
Demo-UVM-SystemC-C++ Release Notes
==================================

Douglas John Moore and Scott Meyer Peimann, November 12, 2024


License
-------

This project is licensed under the Apache Software Foundations Apache
License, Version 2.0, January 2004.  A copy of the full license is
available in the [LICENSE](LICENSE) file.


Acknowledgements
----------------

Moore, Douglas John.

Peimann, Scott Meyer.

Sheill, Jon.


What's new in the releases?
---------------------------

Initial release "demo.1.0.0".

1. Basic, single-cycle processor pipeline module example. Uses 
   Accellera SystemC Reference Version 2.3.4" and UVM/SystemC 
   Reference Version 1.0 beta5


Updated release "demo.2.0.0".

1. Basic, single-cycle processor pipeline module example. Uses 
   Accellera SystemC Reference Version 3.0.1" and UVM/SystemC 
   Reference Version 1.0 beta6


Known bugs for Releases
-----------------------

Initial release "demo.1.0.0".

1. There is a memory leak that is yet to be debugged.
2. The "SystemC 2.3.4" had to be gotten by a git clone.
   git clone --branch 2.3.4 https://github.com/accellera-official/systemc.git systemc-2.3.4
3. The "SystemC 2.3.4" build will show several types of warning.
4. The "UVM/SystemC-1.0-beta5"  build will show several types of warning.
5. Under Linux the example the test [simple/phases/timeout/test] fails
 
 
Initial release "demo.2.0.0".

1. There is a memory leak that is yet to be debugged.
2. The "SystemC 3.0.1" had to be gotten by a git clone.
   git clone --branch 3.0.1 https://github.com/accellera-official/systemc.git systemc-3.0.1
3. The "SystemC 3.0.1" build will show several types of warning.
4. The "UVM/SystemC-1.0-beta6"  build will show several types of warning.
5. Under Linux the example the test [simple/phases/timeout/test] fails
 

Bugs and Suggestions
--------------------

Please report bugs and suggestions about this work to:

* https://github.com/DMA-Technologies/uvm-demo/issues


Project Status
--------------

Tag Release demo.1.0.0 [Complete]
Tag Release demo.2.0.0 [Complete]
Further work in progress.


---

End of file.
