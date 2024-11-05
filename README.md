
UVM/SystemC Demonstration Project
=================================

About The Project
-----------------

This UVM/SystemC Demonstration Project provides a demonstration of 
the UVM methodology and tools implemented using SystemC.

Existing, simple, verified modules implemented in SystemC were used
as the targets of this project.  The original tests used to verify
the target modules were constructed using an ad-hoc mechanism which
was composed atop SystemC.  In this project, the ad-hoc mechanism
used previously is replaced using the elements of the UVM/SystemC
beta reference implementaiton.

The Accellera SystemC and UVM/SystemC reference libraries are used
for this project.  These implementations were selected, as they
are 1) open source, and 2) compatable with both Windows and Linux.


Licensing and Copyright
-----------------------

See the separate [LICENSE](LICENSE) and [NOTICE](NOTICE)
files to determine your rights and responsibilities for using the
UVM/SystemC Demonstration Project.


Disclaimer
----------

THE CONTRIBUTORS AND THEIR LICENSORS MAKE NO WARRANTY OF ANY KIND
WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.

THIS SOFTWARE IS FOR DEMONSTRATION AND EXPERIMENTAL PURPOSES, ONLY.
USE IT AT YOUR OWN RISK.


Project Support Documentation
-----------------------------

Supporting documents regarding the modules and mechanisms used in
this project are located in the [documents](/documents/INDEX.md)
directory.


Installation
------------

See the [INSTALL.md](INSTALL.md) file for project installation
instructions.  

All known project dependencies and instructions are in the
file for both Window and Linux.

### Caveats

A "caveat" is a warning or a caution.  In this project several
choices were at the outset due to technology, compilation, and
tools' compatibility issues.  These include:

1. The SystemC 2.3.4 reference implementation was selected, as the
available reference implementation of UVM/SystemC (1.0-beta5) was not
compatible with the available SystemC 3.0.0 implementation.

2. The C++ 2020 standard was selected, as it is the most recent
standard compatible with the SystemC and UVM/SystemC reference
implementations.  The C++ 2023 standard deprecates, or alters,
several language features used by the reference implementations.
As the project goals do not include active maintenance of the
reference implementations, the most-recent, compatible standard
was used.

3. Several compilation issues were found in the Windows reference
implementations of SystemC and UVM/SystemC.  For example, some MSVC
library incompatibilities were found.  The issues identified during
the development of this project were resolved as part of the install
process.

4. This project's use of the C++ 2020 standard is incompatible with
most of the default SystemC library packages installed under the
contemporary Linux distributions.  Local builds and static linking
are used to avoid problems which would otherwise occur from
dynamic linking with nominal SystemC library installations.


Release Notes
-------------

See the separate [RELEASE_NOTES.md](RELEASE_NOTES.md) file, which
provides up-to-date information about this release of UVM/SystemC
Demonstration Project.


About The Project Authors
-------------------------

* Douglas John Moore: lives and works in the Raleigh, North Carolina.  
His skills include engineering for lower-level integration of 
hardware, software, and firmware for embedded designs.
Experienced in technical leadership of teams and projects for the
full lifecycle. Able to architect, code, develop interfaces, and
excells at debugging.

* Scott Meyer Peimann: software manager and engineer with a broad
range of skills in the software arena.  Experienced in handling
projects from concept and requirements collection through product
end-of-life.  Uses mentoring methods to bring continuous improvement
to his teams.


---

End of file.