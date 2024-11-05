
Installation Procedure for Linux
===============================================================================

The authors elected to build SystemC and UVM/SystemC reference
implementations in-place, linking the example code against the
generated static link libraries.  This was done to meet the project
goals of:

1. Simplicity of first-time project use.

2. Project use of C++20 features.

3. Latent bug illumination and obviation.

Simplicity is achieved by "cookbooking" the build sequence in a
reliable, repeatable fashion.  The build sequence is designed to
isolate the project code and libraries from system peculiarities as
much as possible.

The C\+\+20 standard was selected, as the most-recent, viable language
standard available.  The C\+\+23 standard was obviated, as the SystemC
reference implementation fails to build using the standard.  Both
SystemC and UVM/SystemC appear to function correctly using the C\+\+20
standard.

Latent bugs in the authors' work are addressed using the industry
best practice of enabling all appropriate warnings and treating them
as compilation errors.  The `-Wunused-const-variable` warning
threshold was reduced to two (2), as the highest level threshold of
three (3) yielded superfluous warnings.


Caveats
-------------------------------------------------------------------------------

This procedure assumes that the user is fluent with the BASH shell,
as the examples given are based on the default configuration used by
the author's Debian/bookworm installation.

Hereinafter we assume that the target directory for the project is
`$DEMO`.  In the specific examples, the `DEMO` environment symbol
will be set to the directory `/home/user/uvm-demo`.  This is one
means of setting and verifying the symbol `DEMO`:

```bash
user@verify:~$ export DEMO=~/uvm-demo
user@verify:~$ echo $DEMO
/home/user/uvm-demo
user@verify:~$
```

__NOTA BENE__: In the specific examples shown in this procedure, the
following values apply:


| __DESCRIPTION__              | __SYMBOL__  | __VALUE__              |
|------------------------------|-------------|------------------------|
| System Name                  | HOSTNAME    | `verify`               |
| User's home directory        | HOME        | `/home/user`           |
| UVM demonstration directory  | DEMO        | `/home/user/uvm-demo`  |

__Specific library versions__ are called for in this procedure, as
incompatibilities were identified between the various software
reference versions when this UVM/SystemC Demonstration Project
commenced.  As this project progresses, it is likely that updated
reference libraries will be incorporated.  When these updates are
emplaced, the documentaiton will be updated, reflecting the changes.

Should you decide, a priori, to use alternate versions of the
reference libraries, the authors would appreciate a note detailing
your observations, trials, tribulations, and solutions.


General Outline
-------------------------------------------------------------------------------

This is a general outline of process required to download and build
the UVM/SystemC Demonstration Project using GNU/Linux.  Specific
details for each step of the outline are detailed in the respective
sections, below.


### Installation Steps

1. [Download This Repository from GitHub](#Download-This-Repository-from-GitHub)

2. [Download SystemC and UVM/SystemC Archives](#Download-SystemC-and-UVMSystemC-Archives)

3. [Unpack the SystemC and UVM/SystemC Archives](#Unpack-the-SystemC-and-UVMSystemC-Archives)

4. [Configure and Build SystemC](#Configure-and-Build-SystemC)

5. [Configure and Build UVM/SystemC](#Configure-and-Build-UVMSystemC)

6. [Build and Test the Demonstration Project Executables](#Build-and-Test-the-Demonstration-Project-Executables)

7. [Debian (Bookworm) Configuration Issues](#Debian-Bookworm-Configuration-Issues)



Download This Repository from GitHub
===============================================================================

The goal of this step is to download and verify correct installation
of the demonstration project into a well-defined directory structure.

Download and unpack, or clone, this repository into the `$DEMO`
directory.

After unpacking or cloning, you should see the the file 
`$DEMO/README.md`, amongst others.  Following is an example listing
which you might expect to see:

```bash
user@verify:~$ cd $DEMO
user@verify:~/uvm-demo$ ls -la
total 80
drwxr-xr-x 6 user user  4096 Oct  1 13:29 .
drwx------ 3 user user  4096 Oct  1 14:57 ..
drwxr-xr-x 2 user user  4096 Oct  1 13:28 build-linux
drwxr-xr-x 2 user user  4096 Sep 28 14:39 build-msvc
drwxr-xr-x 2 user user  4096 Sep 23 16:45 documents
-rwxr-xr-x 1 user user 11462 Sep 24 12:54 INSTALL_LINUX.md
-rwxr-xr-x 1 user user  2076 Sep 18 16:41 INSTALL.md
-rwxr-xr-x 1 user user 12193 Sep 21 15:55 INSTALL_WINDOWS.md
-rwxr-xr-x 1 user user 11357 Mar 15  2023 LICENSE
-rwxr-xr-x 1 user user   621 Sep 10 13:01 NOTICE
-rwxr-xr-x 1 user user  4129 Sep 21 14:45 README.md
-rwxr-xr-x 1 user user  1329 Sep 23 15:29 RELEASE_NOTES.md
drwxr-xr-x 5 user user  4096 Oct  1 12:45 source
user@verify:~/uvm-demo$
```


Download SystemC and UVM/SystemC Archives
===============================================================================

Specific library versions are called for in this step, as various
incompatibilities were identified between the software reference
versions when this UVM/SystemC Demonstration Project was commenced.  

The authors note that a newer UVM/SystemC beta version were released
prior to UVM/SystemC Demonstration Project being released.  


SystemC Reference Version 2.3.4
-------------------------------------------------------------------------------

Download one of the two available distribution files of the SystemC
reference version 2.3.4. This can be accomplished by the 'wget' command to 
downloaded to a 'know' directory.

Links to the SystemC-2.3.4 reference:

1. [SystemC Tarball](https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.tar.gz)

2. [SystemC Zip File](https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.zip)

An example that will download the file from a given URL (Uniform Resource 
Locator) and save it in the current directory. 

```bash
$ wget https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.tar.gz
```

Alternately you can use GIT to clone the SystemC repository:

```bash
user@verify:~$ cd $DEMO
user@verify:~/uvm-demo/$ git clone --branch=2.3.4 \
> https://github.com/accellera-official/systemc.git \
> systemc-2.3.4
```


UVM/SystemC Reference Version 1.0 beta5
-------------------------------------------------------------------------------

Download the archived beta5 version of the reference UVM/SystemC 1.0
library.

Link to the
[UVM/SystemC-1.0-beta5](https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-1.0-beta5.tar.gz)
reference archive.


Unpack the SystemC and UVM/SystemC Archives
===============================================================================

In this step you will unpack the archived SystemC and UVM/SystemC
library source code into directories which are version numbered.
This is done to avoid future confusion and grief.

As this project progresses, it is likely that updated reference
libraries will be incorporated.  When these updates are emplaced,
the documentaiton will be updated to reflect the changes.

Should you decide to use alternate versions of the reference
libraries a priori, the authors would appreciate a note detailing
your observations, trials, and tribulations.


SystemC
-------------------------------------------------------------------------------

If you did not clone the SystemC repository, you will need to unpack
the downloaded reference release of SystemC 2.3.4 into into the
directory `$DEMO/systemc-2.3.4`.

As a sanity check, you ought verify that the SystemC `README` file
`$DEMO/systemc-2.3.4/README.md` was correctly emplaced, amongst
others.

For example, using the BASH command shell, you should see something
similar to the following when you do a directory listing of the files
in `$DEMO/systemc-2.3.4`:

```bash
user@verify:~$ cd $DEMO/systemc-2.3.4
user@verify:~/uvm-demo/systemc-2.3.4$ ls -la
total 824
drwxr-xr-x 8 user user   4096 Dec  2  2022 .
drwxr-xr-x 7 user user   4096 Oct  2 12:32 ..
-rw-r--r-- 1 user user   1971 Dec  2  2022 AUTHORS.md
drwxr-xr-x 2 user user   4096 Dec  2  2022 cmake
-rw-r--r-- 1 user user  39369 Dec  2  2022 CMakeLists.txt
drwxr-xr-x 2 user user   4096 Dec  2  2022 config
-rwxr-xr-x 1 user user 600409 Dec  2  2022 configure
-rw-r--r-- 1 user user  22873 Dec  2  2022 configure.ac
-rw-r--r-- 1 user user   5640 Dec  2  2022 CONTRIBUTING.md
drwxr-xr-x 4 user user   4096 Dec  2  2022 docs
drwxr-xr-x 6 user user   4096 Dec  2  2022 examples
-rw-r--r-- 1 user user  30718 Dec  2  2022 INSTALL.md
-rw-r--r-- 1 user user  11358 Dec  2  2022 LICENSE
-rw-r--r-- 1 user user   2589 Dec  2  2022 Makefile.am
-rw-r--r-- 1 user user  29258 Dec  2  2022 Makefile.in
drwxr-xr-x 3 user user   4096 Dec  2  2022 msvc10
-rw-r--r-- 1 user user   3495 Dec  2  2022 NOTICE
-rw-r--r-- 1 user user   4631 Dec  2  2022 README.md
-rw-r--r-- 1 user user  34564 Dec  2  2022 RELEASENOTES
drwxr-xr-x 5 user user   4096 Dec  2  2022 src
user@verify:~/uvm-demo/systemc-2.3.4$
```


UVM/SystemC
-------------------------------------------------------------------------------

Unpack the downloaded beta release of UVM/SystemC 1.0 into the
directory `$DEMO/uvm-systemc-1.0-beta5`.

After unpacking, you should see the file
`$DEMO/uvm-systemc-1.0-beta5/README.md`, amongst others.

For example, using the BASH command shell, you should see something
similar to the following when you do a directory listing of the files
in `$DEMO/uvm-systemc-1.0-beta5`:

```bash
user@verify:~$ cd $DEMO/uvm-systemc-1.0-beta5
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5$ ls -la
total 832
drwxr-xr-x 7 user user   4096 Mar 15  2023 .
drwxr-xr-x 8 user user   4096 Oct  2 12:38 ..
-rw-r--r-- 1 user user  52779 Mar 15  2023 aclocal.m4
-rw-r--r-- 1 user user    586 Mar 15  2023 AUTHORS
-rw-r--r-- 1 user user   2629 Mar 15  2023 ChangeLog
drwxr-xr-x 2 user user   4096 Mar 15  2023 config
-rwxr-xr-x 1 user user 645906 Mar 15  2023 configure
-rw-r--r-- 1 user user  20240 Mar 15  2023 configure.ac
drwxr-xr-x 3 user user   4096 Mar 15  2023 docs
drwxr-xr-x 4 user user   4096 Mar 15  2023 examples
-rw-r--r-- 1 user user   1047 Mar 15  2023 .gitignore
-rw-r--r-- 1 user user  14580 Mar 15  2023 INSTALL
-rw-r--r-- 1 user user  11357 Mar 15  2023 LICENSE
-rw-r--r-- 1 user user    724 Mar 15  2023 Makefile.am
-rw-r--r-- 1 user user  30520 Mar 15  2023 Makefile.in
drwxr-xr-x 3 user user   4096 Mar 15  2023 msvc14
-rw-r--r-- 1 user user   1003 Mar 15  2023 NEWS
-rw-r--r-- 1 user user   5058 Mar 15  2023 NOTICE
-rw-r--r-- 1 user user   3433 Mar 15  2023 README.md
-rw-r--r-- 1 user user   8000 Mar 15  2023 RELEASENOTES
drwxr-xr-x 3 user user   4096 Mar 15  2023 src
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5$
```


Configure and Build SystemC
===============================================================================

As noted, SystemC is built and installed locally in the `$DEMO`
directory structure.

This step builds and installs SystemC using the default build
conventions of the reference implementation.  The build conventions
are documented in the SystemC `INSTALL.md` file.

The default build configuration is used with one exception.  That
being the use of the C\+\+20 standard.


### SystemC Configuration and Build Steps

1. [Bootstrap the SystemC Configuration](#Bootstrap-the-SystemC-Configuration)

2. [Create the Build Directory](#Create-the-Build-Directory)

3. [Configure the SystemC Build](#Configure-the-SystemC-Build)

4. [Build, Check, and Install SystemC](#Build-Check-and-Install-SystemC)


### Quick Command Summary

A summary of the required commands is as follows:

```bash
$ cd $DEMO/systemc-2.3.4
$ ./config/bootstrap
$ mkdir objdir
$ cd    objdir
$ ../configure  CXXFLAGS='-std=c++20'
$ make
$ make check
$ make install
```

Bootstrap the SystemC Configuration
-------------------------------------------------------------------------------

__This is normally a ONE-TIME process__.  Following bootstrap, the
SystemC installation can be reconfigured as desired.

SystemC requires a bootstrap of the configuration process.

Bootstraping is documented in the SystemC `INSTALL.md` file.  Note
that there is some ambiguity in the documentation regarding this
requirement.  In the authors' experience, the SystemC-2.3.4 bootstrap
procedure __must__ be performed for installation from an archive or
from the `git` repository.

From the SystemC directory, the bootstrap command is:

`./config/bootstrap`

This is an example of the expected output:

```bash
user@verify:~$ cd $DEMO/systemc-2.3.4
user@verify:~/uvm-demo/systemc-2.3.4$ ./config/bootstrap
configure.ac:95: installing 'config/compile'
configure.ac:59: installing 'config/missing'
examples/sysc/Makefile.am: installing 'config/depcomp'
configure.ac:58: warning: The macro `AC_CANONICAL_SYSTEM' is obsolete.
configure.ac:58: You should run autoupdate.
./lib/autoconf/general.m4:2081: AC_CANONICAL_SYSTEM is expanded from...
configure.ac:58: the top level
configure.ac:270: warning: The macro `AC_PROG_LIBTOOL' is obsolete.
configure.ac:270: You should run autoupdate.
config/libtool.m4:100: AC_PROG_LIBTOOL is expanded from...
configure.ac:270: the top level
configure.ac:582: warning: AC_OUTPUT should be used without arguments.
configure.ac:582: You should run autoupdate.
user@verify:~/uvm-demo/systemc-2.3.4$
```


Create the Build Directory
-------------------------------------------------------------------------------

From the SystemC directory, create the build directory using:

`mkdir objdir`

This is an example of the expected output:

```bash
user@verify:~$ cd $DEMO/systemc-2.3.4
user@verify:~/uvm-demo/systemc-2.3.4$ mkdir objdir
user@verify:~/uvm-demo/systemc-2.3.4$ ls -la objdir
total 8
drwxr-xr-x  2 user user 4096 Oct  2 14:09 .
drwxr-xr-x 10 user user 4096 Oct  2 14:09 ..
user@verify:~/uvm-demo/systemc-2.3.4$
```


Configure the SystemC Build
-------------------------------------------------------------------------------

From the SystemC build directory, configure SystemC to build using
the C++20 standard.

The command which configures SystemC to build using the defaults,
with the exception of the C++20 standard.

`../configure  CXXFLAGS='-std=c++20'`

This is an __abridged__ example of the expected output:

```bash
user@verify:~$ cd $DEMO/systemc-2.3.4/objdir
user@verify:~/uvm-demo/systemc-2.3.4/objdir$ ../configure CXXFLAGS='-std=c++20'
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu

[snip]

---------------------------------------------------------------------
Configuration summary of SystemC 2.3.4 for x86_64-unknown-linux-gnu
---------------------------------------------------------------------

 Directory setup (based on classic layout):
   Installation prefix (aka SYSTEMC_HOME):
      /home/user/uvm-demo/systemc-2.3.4
   Header files  : <SYSTEMC_HOME>/include
   Libraries     : <SYSTEMC_HOME>/lib-linux64
   Documentation : <SYSTEMC_HOME>/docs
   Examples      : <SYSTEMC_HOME>/examples

 Architecture    : linux64
 Compiler        : g++-12 (C/C++)

 User-provided command-line flags :
   C++ compiler flags (CXXFLAGS)  : -std=c++20

 Build settings:
   Enable compiler optimizations  : yes
   Include debugging symbols      : no
   Coroutine package for processes: QuickThreads
   Enable VCD scopes by default   : yes
   Disable async_request_update   : no
   Enable legacy mem. mangement   : no
   Phase callbacks (experimental) : no

---------------------------------------------------------------------
user@verify:~/uvm-demo/systemc-2.3.4/objdir$
```


Build, Check, and Install SystemC
-------------------------------------------------------------------------------

From the SystemC build directory, building, checking, and installing
SystemC requires three commands.  No errors ought arise.

To build SystemC, use the command:

`make`

Check that the build works using the command:

`make check`

After running the tests, inspect the output and verify that no
failures occurred.

Finally, install SystemC locally (per the configuration) with the
command:

`make install`

The build/check/install sequence is sumarised as:

```bash
$ cd $DEMO/systemc-2.3.4/objdir
$ make
$ make check
$ make install
```

For the brave, singular use of the `make install` command builds and
installs SystemC without checking.

For those familiar with `make`, the author suggests use of the `-j`
(alternately `--jobs`) command-line switch to reduce the wait whilst
executing the build and check steps.


Configure and Build UVM/SystemC
===============================================================================

As noted, UVM/SystemC is built and installed locally in the `$DEMO`
directory structure.

This step builds and installs UVM/SystemC using the conventions of the
reference implementation.  These conventions are expected throughout
this demonstration project.

The default build configuration is used with two exceptions, the first
of which is mandatory, the second simply for convenience:

1. Use of the C++20 standard.

2. Use of the GNU complier's `-Wno-use-after-free` flag to suppress
   a large number of warnings inherent in the library build.


### UVM/SystemC Configuration and Build Steps

1. [Create the Build Directory for UVM/SystemC](#Create-the-Build-Directory-for-UVMSystemC)

2. [Configure the UVM/SystemC Build](#Configure-the-UVMSystemC-Build)

3. [Build, Check, and Install UVM/SystemC](#Build-Check-and-Install-UVMSystemC)


### Quick Command Summary

A quick summary of the required commands is as follows.  Note that the
option `-Wno-use-after-free` is included to suppress a large number of
warnings inherent in the library build, it is not required.

```bash
$ cd $DEMO/uvm-systemc-1.0-beta5
$ mkdir objdir
$ cd    objdir
$ ../configure  CXXFLAGS='-std=c++20 -Wno-use-after-free' \
  --with-systemc="$(realpath ../../systemc-2.3.4)"
$ make
$ make check
$ make install
```

Create the Build Directory for UVM/SystemC
-------------------------------------------------------------------------------

From the UVM/SystemC directory, create the build directory using:

`mkdir objdir`

This is an example of the expected output:

```bash
user@verify:~$ cd $DEMO/uvm-systemc-1.0-beta5
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5$ mkdir objdir
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5$ ls -la objdir
total 8
drwxr-xr-x  2 user user 4096 Oct  2 14:09 .
drwxr-xr-x 10 user user 4096 Oct  2 14:09 ..
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5$
```


Configure the UVM/SystemC Build
-------------------------------------------------------------------------------

From the UVM/SystemC build directory, configure UVM/SystemC to build
using the C++20 standard, referencing the SystemC library built (see
above).

The basic command which configures UVM/SystemC to build using the
C++20 standard is:

```
../configure CXXFLAGS='-std=c++20' \
  --with-systemc="$(realpath ../../systemc-2.3.4)"
```

Alternately (see the summary, above), you can suppress a large number
of warnings by adding `-Wno-use-after-free` to `CXXFLAGS`:

```
../configure  CXXFLAGS='-std=c++20 -Wno-use-after-free' \
  --with-systemc="$(realpath ../../systemc-2.3.4)"
```

This is an abridged example of the expected output:

```bash
user@verify:~$ cd $DEMO/uvm-systemc-1.0-beta5/objdir
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5/objdir$ ../configure CXXFLAGS='-std=c++20' --with-systemc="$(realpath ../../systemc-2.3.4)"
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu

[snip]

------------------------------------------------------------------------------
Configuration summary of UVM-SystemC 1.0-beta5 for x86_64-unknown-linux-gnu
------------------------------------------------------------------------------

 Directory setup (based on SystemC layout):
   Installation prefix: /home/user/uvm-demo/uvm-systemc-1.0-beta5
   Header files:        /home/user/uvm-demo/uvm-systemc-1.0-beta5/include
   Libraries:           /home/user/uvm-demo/uvm-systemc-1.0-beta5/lib
   Documentation:       /home/user/uvm-demo/uvm-systemc-1.0-beta5/docs
   Examples:            /home/user/uvm-demo/uvm-systemc-1.0-beta5/examples

Target architecture:   linux64

Build settings:
   Compiler (flags):    g++-12
                        -std=c++20
   Preprocessor flags:
   SystemC CFLAGS:      -I/home/user/uvm-demo/systemc-2.3.4/include
   SystemC LIBS:        -L/home/user/uvm-demo/systemc-2.3.4/lib-linux64 -lsystemc
   Additional LIBS:
   Enable compiler optimizations:   yes
   Include debug symbols:           no
   Generate instrumentation calls:  no
   Regular expression library:      POSIX
---------------------------------------------------------------------
user@verify:~/uvm-demo/uvm-systemc-1.0-beta5/objdir$
```


Build, Check, and Install UVM/SystemC
-------------------------------------------------------------------------------

From the UVM/SystemC build directory, building, checking, and
installing UVM/SystemC requires three commands.  

The C++ compiler will issue a significant number of warnings while
building the library and tests.  As noted above, most of these
warnings can be suppressed by adding `-Wno-use-after-free` to the
`CXXFLAGS`.  These warnings are issued regardless of the language
standard used.

The tests have __one__ failure regardless of the language standard
used.  The failing test is "`sinple/phases/timeout`".  This failure
has no apparent affect on execution of the demonstration code between
Linux and Windows.  Ergo, the authors have elected to ignore this
error, as this project is intended to demonstrate original work, not
debugging of the reference libraries.

To build UVM/SystemC, use the following command:

`make`

Check that the build works using the command:

`make check`

Install locally (per the configuration) with:

`make install`

For the brave, singular use of the `make install` command builds and
installs UVM/SystemC without checking.

For those familiar with `make`, the author suggests use of the `-j`
(alternately `--jobs`) command-line switch to reduce the wait whilst
executing the build and check steps.


Build and Test the Demonstration Project Executables
===============================================================================

The demonstration code is built and executed from the
`$DEMO/build-linux` directory.

After building the reference libraries, build and execute the
demonstration project `$DEMO/build-linux` directory.  No warnings or
errors ought arise.


To build and test the 'uvm-EXAMPLE-single_cycle' Project, use the
commands:

```bash
$ cd $DEMO/build-linux
$ make
$ ./uvm-EXAMPLE-single_cycle
```

This is an example of the expected test output:

```bash
        SystemC 2.3.4-Accellera --- Sep 30 2024 08:24:39
        Copyright (c) 1996-2022 by all Contributors,
        ALL RIGHTS RESERVED
Create top-level signals.
Create module under test.
Collect exported signals from module.
Create VIF, binds clock and exports from MUT.
Bind input signals to ports in the MUT


   Universal Verification Methodology for SystemC (UVM-SystemC)
              Version: 1.0-beta5  Date: 2023-03-15
          Copyright (c) 2006 - 2023 by all Contributors
            See NOTICE file for all Contributors
                    ALL RIGHTS RESERVED
         Licensed under the Apache License, Version 2.0


UVM_INFO @ 0 s: reporter [RNTST] Running test ::test::EXAMPLE::single_cycle_t...
UVM_INFO @ 0 s: ::test::EXAMPLE::single_cycle_t [TOPOLOGY] Test topology :
---------------------------------------------------------------------------------------------------------
Name                             Type                                                         Size  Value
---------------------------------------------------------------------------------------------------------
::test::EXAMPLE::single_cycle_t  ::test::EXAMPLE::single_cycle_t                              -     @078
  test_bench                     ::test_bench::EXAMPLE::single_cycle_t                        -     @130
    m_environment                ::environment::EXAMPLE::single_cycle_t                       -     @132
      m_agent                    ::agent::EXAMPLE::single_cycle_t                             -     @143
        m_driver                 ::driver::EXAMPLE::single_cycle_t                            -     @151
        m_monitor                ::monitor::EXAMPLE::single_cycle_t                           -     @147
        m_sequencer              ::sequencer::EXAMPLE::single_cycle_t                         -     @149
          arbitration_queue      array                                                        0     -
          lock_queue             array                                                        0     -
      m_monitor                  ::monitor::EXAMPLE::single_cycle_t                           -     @141
    m_sb_controls                ::score_board::EXAMPLE::single_cycle_t::control_bits_t       -     @134
    m_sb_pairs                   ::score_board::EXAMPLE::single_cycle_t::control_bit_pairs_t  -     @138
    m_sb_values                  ::score_board::EXAMPLE::single_cycle_t::data_values_t        -     @136
---------------------------------------------------------------------------------------------------------

UVM_INFO @ 0 s: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::repeat_t

Info: (I702) default timescale unit used for tracing: 1 ps (/tmp/trace_file.vcd)
UVM_INFO @ 10 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::repeat_t
UVM_INFO @ 11 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::repeat_t
UVM_INFO @ 16 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::EXAMPLE::single_cycle_t::all_control_pairs_t
UVM_INFO @ 20498 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::EXAMPLE::single_cycle_t::random_weighted_t
UVM_INFO @ 40502 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::EXAMPLE::single_cycle_t::random_loop_t
UVM_INFO @ 60506 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::EXAMPLE::single_cycle_t::flush_w_advance_never_advances_t
UVM_INFO @ 60522 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::EXAMPLE::single_cycle_t::error_w_advance_valid_t
UVM_INFO @ 60528 ns: ::test::EXAMPLE::single_cycle_t [SEQUENCE_START] ::sequence::EXAMPLE::single_cycle_t::reset_advance_with_out_valid_t
UVM_INFO @ 60541 ns: reporter [SCORE_BOARD]
  CONTROL BIT PASS/FAIL COUNT:
    Passed:  60540  (100.00%).
    Failed:      0  (  0.00%).
    Moot:        0  (  0.00%).
    TOTAL:   60540.

UVM_INFO @ 60541 ns: reporter [SCORE_BOARD]
  CONTROL INPUT PAIR COVERAGE:
    Pair coverage is complete.
    Observations:          1024  (100.00%).
    Minimum observation:     19.
    Maximum observation:  12078.
    TOTAL PAIRS:           1024.

UVM_INFO @ 60541 ns: reporter [SCORE_BOARD]
  DATA VALUE PASS/FAIL COUNT:
    Passed:  33987  ( 56.14%).
    Failed:      0  (  0.00%).
    Moot:    26553  ( 43.86%).
    TOTAL:   60540.

UVM_INFO @ 60541 ns: ::test::EXAMPLE::single_cycle_t [PASS/FAIL] ** UVM TEST PASSED **
UVM_INFO C:\tmp\uvm-demo\uvm-systemc-1.0-beta5\src\uvmsc\report\uvm_default_report_server.cpp(667) @ 60541 ns: reporter [UVM/REPORT/SERVER]
--- UVM Report Summary ---

** Report counts by severity
UVM_INFO      :  15
UVM_WARNING   :   0
UVM_ERROR     :   0
UVM_FATAL     :   0
** Report counts by id
[PASS/FAIL]             1
[RNTST]                 1
[SCORE_BOARD]           3
[SEQUENCE_START]        9
[TOPOLOGY]              1

UVM_INFO @ 60541 ns: reporter [FINISH] UVM-SystemC phasing completed; simulation finished

C:\tmp\uvm-demo\build-msvc\x64\Debug\uvm-EXAMPLE-single_cycle.exe (process 19940) exited with code 0 (0x0).
Press any key to close this window . . .
```


Debian (Bookworm) Configuration Issues
===============================================================================

Whilst building the SystemC libraries, system configuration issues
were observed.  Note that the authors used a minimal Debian
(bookworm) virtual machine as their starting point.

The authors solutions to these issues are
discussed below:

1. [Bootstrap Failure due to Missing `aclocal` Command](#Bootstrap-Failure-due-to-Missing-aclocal-Command)

2. [Bootstrap Failure due to Missing `libtoolize` Command](#bootstrap-failure-due-to-missing-libtoolize-command)

3. [Configuration Failure due to Missing C++ Compiler](#configuration-failure-due-to-missing-c-compiler)

4. [Configuration Failure due to Missing `make` Command](#configuration-failure-due-to-missing-make-command)

Whether you encounter these issues, or others, will depend on how
your system is configured.  The authors installed the following five
packages in the order given:

1. automake 1.16.5

2. libtool 2.4.7

3. g++ 12.2.0

4. make 4.3

5. git 2.39.5


Bootstrap Failure due to Missing `aclocal` Command
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `automake`
package.  The following was observed whilst executing the bootstrap
process:

```bash
user@verify:~/uvm-demo/systemc-2.3.4$ ./config/bootstrap
./config/bootstrap: 8: aclocal: not found
user@verify:~/uvm-demo/systemc-2.3.4$
```


Bootstrap Failure due to Missing `libtoolize` Command
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `libtool`
package.  The following was observed whilst executing the bootstrap
process:

```bash
user@verify:~/uvm-demo/systemc-2.3.4$ ./config/bootstrap
./config/bootstrap: 9: libtoolize: not found
user@verify:~/uvm-demo/systemc-2.3.4$
```


Configuration Failure due to Missing C++ Compiler
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `g++`
package.  The following (abridged output) was observed whilst
executing the configuration of SystemC; observe that the
configuration script detects an error, but message is somewhat
misleading:

```bash
user@verify:~/uvm-demo/systemc-2.3.4/objdir$ ../configure CXXFLAGS='-std=c++20'
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu
checking target system type... x86_64-unknown-linux-gnu
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes

[snip]

checking for g++... no
checking for c++... no
checking for gpp... no
checking for aCC... no
checking for CC... no
checking for cxx... no
checking for cc++... no
checking for cl.exe... no
checking for FCC... no
checking for KCC... no
checking for RCC... no
checking for xlC_r... no
checking for xlC... no
checking for clang++... no
checking whether the C++ compiler works... no
configure: error: in `/home/user/uvm-demo/systemc-2.3.4/objdir':
configure: error: C++ compiler cannot create executables
See `config.log' for more details
user@verify:~/uvm-demo/systemc-2.3.4/objdir$
```


Configuration Failure due to Missing `make` Command
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `make`
package.  The following (abridged output) was observed whilst
executing the configuration of SystemC; observe that the
configuration script was able to identify the problem and suggested
a potential solution:

```bash
user@verify:~/uvm-demo/systemc-2.3.4/objdir$ ../configure CXXFLAGS='-std=c++20'
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu
checking target system type... x86_64-unknown-linux-gnu
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes

[snip]

config.status: executing depfiles commands
config.status: error: in `/home/user/uvm-demo/systemc-2.3.4/objdir':
config.status: error: Something went wrong bootstrapping makefile fragments
    for automatic dependency tracking.  If GNU make was not used, consider
    re-running the configure script with MAKE="gmake" (or whatever is
    necessary).  You can also try re-running configure with the
    '--disable-dependency-tracking' option to at least be able to build
    the package (albeit without support for automatic dependency tracking).
See `config.log' for more details
user@verify:~/uvm-demo/systemc-2.3.4/objdir$
```


---

End of file.