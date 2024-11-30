
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
isolate the project code and libraries from system peculiarities
as much as possible.

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


Synopsis
-------------------------------------------------------------------------------
Leaving out descriptive details for each step, the following is a
(relatively) quick synopsis of the linux installation process:

```bash
$ git clone https://github.com/DMA-Technologies/uvm-demo.git
$ DEMO="$(realpath ./uvm-demo)"
$
$ cd $DEMO
$ git clone https://github.com/accellera-official/systemc.git --branch 3.0.1 systemc-3.0.1
$ cd systemc-3.0.1
$ ./config/bootstrap
$ mkdir objdir
$ cd objdir
$ ../configure  CXXFLAGS='-std=c++20'
$ make
$ make check
$ make install
$
$ cd $DEMO
$ wget https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-1.0-beta6.tar.gz
$ tar xf uvm-systemc-1.0-beta6.tar.gz
$ cd uvm-systemc-1.0-beta6
$ mkdir objdir
$ cd objdir
$ ../configure 'CXXFLAGS=-std=c++20 -Wno-use-after-free' --with-systemc=$DEMO/systemc-3.0.1
$ make
$ make check
$ make install
$
$ cd $DEMO/build-linux
$ make
```


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
emplaced, the documentation will be updated, reflecting the changes.

__NOTA_BENE__: The following library combinations were checked for
compatibility:

| __SYSTEMC VERSION__ | __UVM/SYSTEMC VERSION__ | __COMPATIBILITY__ |
|:-------------------:|:-----------------------:|:------------------|
| 2.3.4               | 1.0-beta5               | Compatible        |
| 2.3.4               | 1.0-beta6               | Compatible        |
| 3.0.0               | 1.0-beta5               | Incompatible      |
| 3.0.0               | 1.0-beta6               | compatible        |
| 3.0.1               | 1.0-beta5               | Compatible        |
| 3.0.1               | 1.0-beta6               | Compatible        |


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

After unpacking or cloning, you should see the file 
`$DEMO/README.md`, amongst others.  Following is an example listing
which you might expect to see:

```bash
user@verify:~$ cd $DEMO
user@verify:~/uvm-demo$ ls -la
total 112
drwxr-xr-x 7 user user  4096 Nov 30 09:19 .
drwx------ 8 user user  4096 Nov 30 09:19 ..
drwxr-xr-x 2 user user  4096 Nov 30 09:19 build-linux
drwxr-xr-x 2 user user  4096 Nov 30 09:19 build-msvc
drwxr-xr-x 2 user user  4096 Nov 30 09:19 documents
drwxr-xr-x 8 user user  4096 Nov 30 09:19 .git
-rw-r--r-- 1 user user   391 Nov 30 09:19 .gitignore
-rw-r--r-- 1 user user 33702 Nov 30 09:19 INSTALL_LINUX.md
-rw-r--r-- 1 user user  1751 Nov 30 09:19 INSTALL.md
-rw-r--r-- 1 user user 12037 Nov 30 09:19 INSTALL_WINDOWS.md
-rw-r--r-- 1 user user 11357 Nov 30 09:19 LICENSE
-rw-r--r-- 1 user user   621 Nov 30 09:19 NOTICE
-rw-r--r-- 1 user user  4115 Nov 30 09:19 README.md
-rw-r--r-- 1 user user  1233 Nov 30 09:19 RELEASE_NOTES.md
drwxr-xr-x 5 user user  4096 Nov 30 09:19 source
user@verify:~/uvm-demo$
```


Download SystemC and UVM/SystemC Archives
===============================================================================

Specific library versions are called for in this step, as various
incompatibilities were identified between the software reference
versions when this UVM/SystemC Demonstration Project was commenced.  

The authors note that a newer UVM/SystemC beta version were released
prior to UVM/SystemC Demonstration Project being released.  


SystemC Reference Version 3.0.1
-------------------------------------------------------------------------------

Download one of the two available distribution files of the SystemC
reference version 3.0.1. This can be accomplished by the 'wget' command to 
downloaded to a 'known' directory.

Links to the SystemC-3.0.1 reference:

1. [SystemC Tarball](https://github.com/accellera-official/systemc/archive/refs/tags/3.0.1.tar.gz)

2. [SystemC Zip File](https://github.com/accellera-official/systemc/archive/refs/tags/3.0.1.zip)

An example that will download the file from a given URL (Uniform Resource 
Locator) and save it in the current directory. 

```bash
$ wget https://github.com/accellera-official/systemc/archive/refs/tags/3.0.1.tar.gz
```

Alternately you can use GIT to clone the SystemC repository:

```bash
user@verify:~$ cd $DEMO
user@verify:~/uvm-demo/$ git clone --branch=3.0.1 \
> https://github.com/accellera-official/systemc.git \
> systemc-3.0.1
```


UVM/SystemC Reference Version 1.0 beta6
-------------------------------------------------------------------------------

Download the archived beta6 version of the reference UVM/SystemC 1.0
library.

Link to the
[UVM/SystemC-1.0-beta6](https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-1.0-beta6.tar.gz)
reference archive.


Unpack the SystemC and UVM/SystemC Archives
===============================================================================

In this step you will unpack the archived SystemC and UVM/SystemC
library source code into directories which are version numbered.
This is done to avoid future confusion and grief.


SystemC
-------------------------------------------------------------------------------

If you did not clone the SystemC repository, you will need to unpack
the downloaded reference release of SystemC 3.0.1 into the directory
`$DEMO/systemc-3.0.1`.

As a sanity check, you ought verify that the SystemC `README` file
`$DEMO/systemc-3.0.1/README.md` was correctly emplaced, amongst
others.

For example, using the BASH command shell, you should see something
similar to the following when you do a directory listing of the files
in `$DEMO/systemc-3.0.1`:

```bash
user@verify:~$ cd $DEMO/systemc-3.0.1
user@verify:~/uvm-demo/systemc-3.0.1$ ls -la
total 856
drwxr-xr-x 10 user user   4096 Oct 15 09:47 .
drwxr-xr-x  8 user user   4096 Nov 30 09:31 ..
-rw-r--r--  1 user user  44849 Oct 15 09:47 aclocal.m4
-rw-r--r--  1 user user   2109 Oct 15 09:47 AUTHORS.md
drwxr-xr-x  2 user user   4096 Oct 15 09:47 cmake
-rw-r--r--  1 user user  34880 Oct 15 09:47 CMakeLists.txt
drwxr-xr-x  2 user user   4096 Oct 15 09:47 config
-rwxr-xr-x  1 user user 617381 Oct 15 09:47 configure
-rw-r--r--  1 user user  17575 Oct 15 09:47 configure.ac
-rw-r--r--  1 user user   5076 Oct 15 09:47 CONTRIBUTING.md
drwxr-xr-x  2 user user   4096 Oct 15 09:47 docker
drwxr-xr-x  4 user user   4096 Oct 15 09:47 docs
drwxr-xr-x  6 user user   4096 Oct 15 09:47 examples
-rw-r--r--  1 user user  26148 Oct 15 09:47 INSTALL.md
-rw-r--r--  1 user user  11358 Oct 15 09:47 LICENSE
-rw-r--r--  1 user user   2619 Oct 15 09:47 Makefile.am
-rw-r--r--  1 user user  29986 Oct 15 09:47 Makefile.in
drwxr-xr-x  3 user user   4096 Oct 15 09:47 msvc16
-rw-r--r--  1 user user   3988 Oct 15 09:47 NOTICE
-rw-r--r--  1 user user   4178 Oct 15 09:47 README.md
-rw-r--r--  1 user user   8614 Oct 15 09:47 RELEASENOTES.md
drwxr-xr-x  5 user user   4096 Oct 15 09:47 src
drwxr-xr-x  6 user user   4096 Oct 15 09:47 tests
user@verify:~/uvm-demo/systemc-3.0.1$
```


UVM/SystemC
-------------------------------------------------------------------------------

Unpack the downloaded beta release of UVM/SystemC 1.0 into the
directory `$DEMO/uvm-systemc-1.0-beta6`.

After unpacking, you should see the file
`$DEMO/uvm-systemc-1.0-beta6/README.md`, amongst others.

For example, using the BASH command shell, you should see something
similar to the following when you do a directory listing of the files
in `$DEMO/uvm-systemc-1.0-beta6`:

```bash
user@verify:~$ cd $DEMO/uvm-systemc-1.0-beta6

user@verify:~/uvm-demo/uvm-systemc-1.0-beta6$ ls -la
total 832
drwxr-xr-x 7 user user   4096 Jul  1 03:48 .
drwxr-xr-x 9 user user   4096 Nov 30 09:36 ..
-rw-r--r-- 1 user user  52779 Jul  1 03:48 aclocal.m4
-rw-r--r-- 1 user user    586 Jul  1 03:48 AUTHORS
-rw-r--r-- 1 user user   2831 Jul  1 03:48 ChangeLog
drwxr-xr-x 2 user user   4096 Jul  1 03:48 config
-rwxr-xr-x 1 user user 645906 Jul  1 03:48 configure
-rw-r--r-- 1 user user  20240 Jul  1 03:48 configure.ac
drwxr-xr-x 3 user user   4096 Jul  1 03:48 docs
drwxr-xr-x 4 user user   4096 Jul  1 03:48 examples
-rw-r--r-- 1 user user   1047 Jul  1 03:48 .gitignore
-rw-r--r-- 1 user user  15076 Jul  1 03:48 INSTALL
-rw-r--r-- 1 user user  11357 Jul  1 03:48 LICENSE
-rw-r--r-- 1 user user    724 Jul  1 03:48 Makefile.am
-rw-r--r-- 1 user user  30520 Jul  1 03:48 Makefile.in
drwxr-xr-x 3 user user   4096 Jul  1 03:48 msvc14
-rw-r--r-- 1 user user   1107 Jul  1 03:48 NEWS
-rw-r--r-- 1 user user   5059 Jul  1 03:48 NOTICE
-rw-r--r-- 1 user user   3588 Jul  1 03:48 README.md
-rw-r--r-- 1 user user   7999 Jul  1 03:48 RELEASENOTES
drwxr-xr-x 3 user user   4096 Jul  1 03:48 src
user@verify:~/uvm-demo/uvm-systemc-1.0-beta6$
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
$ cd $DEMO/systemc-3.0.1
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

SystemC requires a ONE-TIME bootstrap of the build process, regardless
of how the files were installed.

__This is normally a ONE-TIME process__.  Following bootstrap, the
SystemC installation can be configured as often as necessary.

Bootstrapping is documented in the SystemC `INSTALL.md` file.

From the SystemC directory, the bootstrap command is:

`./config/bootstrap`

This is an example of the expected output:

```bash
user@verify:~$ cd $DEMO/systemc-3.0.1/
user@verify:~/uvm-demo/systemc-3.0.1$ ./config/bootstrap
configure.ac:95: installing 'config/compile'
configure.ac:59: installing 'config/missing'
examples/sysc/Makefile.am: installing 'config/depcomp'
configure.ac:58: warning: The macro `AC_CANONICAL_SYSTEM' is obsolete.
configure.ac:58: You should run autoupdate.
./lib/autoconf/general.m4:2081: AC_CANONICAL_SYSTEM is expanded from...
configure.ac:58: the top level
configure.ac:232: warning: The macro `AC_PROG_LIBTOOL' is obsolete.
configure.ac:232: You should run autoupdate.
config/libtool.m4:100: AC_PROG_LIBTOOL is expanded from...
configure.ac:232: the top level
configure.ac:462: warning: AC_OUTPUT should be used without arguments.
configure.ac:462: You should run autoupdate.
user@verify:~/uvm-demo/systemc-3.0.1$
```


Create the Build Directory
-------------------------------------------------------------------------------

From the SystemC directory, create the build directory using:

`mkdir objdir`

This is an example of the expected output:

```bash
user@verify:~$ cd $DEMO/systemc-3.0.1/
user@verify:~/uvm-demo/systemc-3.0.1$ mkdir objdir
user@verify:~/uvm-demo/systemc-3.0.1$ ls -la objdir
total 8
drwxr-xr-x  2 user user 4096 Nov 30 09:48 .
drwxr-xr-x 12 user user 4096 Nov 30 09:48 ..
user@verify:~/uvm-demo/systemc-3.0.1$
```


Configure the SystemC Build
-------------------------------------------------------------------------------

From the SystemC build directory, configure SystemC to build using
the C++20 standard.

This command configures SystemC to build using the default settings,
with the exception of the C++20 standard:

`../configure  CXXFLAGS='-std=c++20'`

This is an __abridged__ example of the expected output:

```bash
user@verify:~$ cd $DEMO/systemc-3.0.1/objdir
user@verify:~/uvm-demo/systemc-3.0.1/objdir$ ../configure CXXFLAGS='-std=c++20'
checking build system type... x86_64-pc-linux-gnu
checking host system type... x86_64-pc-linux-gnu

[snip]

---------------------------------------------------------------------
Configuration summary of SystemC 3.0.0 for x86_64-pc-linux-gnu
---------------------------------------------------------------------

 Directory setup (based on classic layout):
   Installation prefix (aka SYSTEMC_HOME):
      /home/user/uvm-demo/systemc-3.0.1
   Header files  : <SYSTEMC_HOME>/include
   Libraries     : <SYSTEMC_HOME>/lib-linux64
   Documentation : <SYSTEMC_HOME>/docs
   Examples      : <SYSTEMC_HOME>/examples

 Architecture    : linux64
 Compiler        : g++ (C/C++)

 User-provided command-line flags :
   C++ compiler flags (CXXFLAGS)  : -std=c++20

 Build settings:
   Enable compiler optimizations  : yes
   Include debugging symbols      : no
   Coroutine package for processes: QuickThreads
   Enable VCD scopes by default   : yes

---------------------------------------------------------------------
user@verify:~/uvm-demo/systemc-3.0.1/objdir$
```


Build, Check, and Install SystemC
-------------------------------------------------------------------------------

From the SystemC build directory, building, checking, and installing
SystemC requires three commands.  No errors ought to arise.

To build SystemC, use the command:

`make`

Check that the build works using the command:

`make check`

Inspect the output and verify that no failures occurred prior to
installing SystemC locally (per the configuration) with the command:

`make install`

A synopsis of the build/check/install sequence is:

```bash
$ cd $DEMO/systemc-3.0.1/objdir
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

A synopsis of the required commands is as follows.  Note that the
option `-Wno-use-after-free` is included to suppress a large number of
warnings inherent in the library build, it is not required.

```bash
$ cd $DEMO/uvm-systemc-1.0-beta6
$ mkdir objdir
$ cd    objdir
$ ../configure  CXXFLAGS='-std=c++20 -Wno-use-after-free' \
  --with-systemc="$(realpath ../../systemc-3.0.1)"
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
user@verify:~$ cd $DEMO/uvm-systemc-1.0-beta6
user@verify:~/uvm-demo/uvm-systemc-1.0-beta6$ mkdir objdir
user@verify:~/uvm-demo/uvm-systemc-1.0-beta6$ ls -la objdir
total 8
drwxr-xr-x  2 user user 4096 Oct  2 14:09 .
drwxr-xr-x 10 user user 4096 Oct  2 14:09 ..
user@verify:~/uvm-demo/uvm-systemc-1.0-beta6$
```


Configure the UVM/SystemC Build
-------------------------------------------------------------------------------

From the UVM/SystemC build directory, configure UVM/SystemC to build
using the C++20 standard, referencing the SystemC library which was
previously built, using this command:

```
../configure CXXFLAGS='-std=c++20' \
  --with-systemc="$(realpath ../../systemc-3.0.1)"
```

Alternately (see the summary, above), you can suppress a large number
of warnings by adding `-Wno-use-after-free` to `CXXFLAGS`:

```
../configure  CXXFLAGS='-std=c++20 -Wno-use-after-free' \
  --with-systemc="$(realpath ../../systemc-3.0.1)"
```

This is an abridged example of the expected output:

```bash
user@verify:~$ cd $DEMO/uvm-systemc-1.0-beta6/objdir
user@verify:~/uvm-demo/uvm-systemc-1.0-beta6/objdir$ ../configure CXXFLAGS='-std=c++20' --with-systemc="$(realpath ../../systemc-3.0.1)"
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu

[snip]

------------------------------------------------------------------------------
Configuration summary of UVM-SystemC 1.0-beta6 for x86_64-unknown-linux-gnu
------------------------------------------------------------------------------

 Directory setup (based on SystemC layout):
   Installation prefix: /home/user/uvm-demo/uvm-systemc-1.0-beta6
   Header files:        /home/user/uvm-demo/uvm-systemc-1.0-beta6/include
   Libraries:           /home/user/uvm-demo/uvm-systemc-1.0-beta6/lib
   Documentation:       /home/user/uvm-demo/uvm-systemc-1.0-beta6/docs
   Examples:            /home/user/uvm-demo/uvm-systemc-1.0-beta6/examples

 Target architecture:   linux64

 Build settings:
   Compiler (flags):    g++
                        -std=c++20
   Preprocessor flags:
   SystemC CFLAGS:      -I/home/user/uvm-demo/systemc-3.0.1/include
   SystemC LIBS:        -L/home/user/uvm-demo/systemc-3.0.1/lib-linux64 -lsystemc
   Additional LIBS:
   Enable compiler optimizations:   yes
   Include debug symbols:           no
   Generate instrumentation calls:  no
   Regular expression library:      POSIX
------------------------------------------------------------------------------
user@verify:~/uvm-demo/uvm-systemc-1.0-beta6/objdir$
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
used.  The failing test is "`simple/phases/timeout`".  This failure
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
`$DEMO/build-linux` directory.  Two programs are created, the first
demonstrates test of the example module using pure SystemC, and the
second demonstrates test of the example module using UVM/SystemC.

A synopsis of the required commands follows.  No warnings or errors
ought occur.  Two message pragmas are displayed during the build
process.

```bash
$ cd $DEMO/build-linux
$ make
$ ./test-EXAMPLE-single_cycle
$ ./uvm-EXAMPLE-single_cycle
```

This is an example of the expected output from the pure SystemC
test implementation:

```bash
user@verify:~/uvm-demo/build-linux$ ./test-EXAMPLE-single_cycle

        SystemC 3.0.1-Accellera --- Nov 30 2024 10:10:22
        Copyright (c) 1996-2024 by all Contributors,
        ALL RIGHTS RESERVED
Create signals.
Create module instances.
Bind signals to EXAMPLE/single_cycle_0.
Run the simulation.
spin: start of execution: 0 s
2 s do_test_evil

Info: /OSCI/SystemC: Simulation stopped by user.
Simulation is complete.
Diagnostics of various sorts.
All test results:
  m_results.size(): 1.
  Test group evil: random stimulous test.

    Summary:
      pass 100000 of 100000.
      fail 0 of 100000.

  Overall Summary:
    Test Results:
      pass 100000 of 100000.
      fail 0 of 100000.

    Test Group "evil" Results:
      pass 100000 of 100000.
      fail 0 of 100000.

  Outcome: PASS
user@verify:~/uvm-demo/build-linux$
```

This is an example of the expected output from the UVM/SystemC
test implementation:

```bash
user@verify:~/uvm-demo/build-linux$ ./uvm-EXAMPLE-single_cycle

        SystemC 3.0.1-Accellera --- Nov 30 2024 10:10:22
        Copyright (c) 1996-2024 by all Contributors,
        ALL RIGHTS RESERVED
Create top-level signals.
Create module under test.
Collect exported signals from module.
Create VIF, binds clock and exports from MUT.
Bind input signals to ports in the MUT


   Universal Verification Methodology for SystemC (UVM-SystemC)
              Version: 1.0-beta6  Date: 2024-07-01
          Copyright (c) 2006 - 2024 by all Contributors
            See NOTICE file for all Contributors
                    ALL RIGHTS RESERVED
         Licensed under the Apache License, Version 2.0


UVM_INFO @ 0 s: reporter [RNTST] Running test ::test::EXAMPLE::single_cycle_t...
UVM_INFO @ 0 s: ::test::EXAMPLE::single_cycle_t [TOPOLOGY] Test topology :
---------------------------------------------------------------------------------------------------------
Name                             Type                                                         Size  Value
---------------------------------------------------------------------------------------------------------
::test::EXAMPLE::single_cycle_t  ::test::EXAMPLE::single_cycle_t                              -     @075
  test_bench                     ::test_bench::EXAMPLE::single_cycle_t                        -     @127
    m_environment                ::environment::EXAMPLE::single_cycle_t                       -     @129
      m_agent                    ::agent::EXAMPLE::single_cycle_t                             -     @140
        m_driver                 ::driver::EXAMPLE::single_cycle_t                            -     @148
        m_monitor                ::monitor::EXAMPLE::single_cycle_t                           -     @144
        m_sequencer              ::sequencer::EXAMPLE::single_cycle_t                         -     @146
          arbitration_queue      array                                                        0     -
          lock_queue             array                                                        0     -
      m_monitor                  ::monitor::EXAMPLE::single_cycle_t                           -     @138
    m_sb_controls                ::score_board::EXAMPLE::single_cycle_t::control_bits_t       -     @131
    m_sb_pairs                   ::score_board::EXAMPLE::single_cycle_t::control_bit_pairs_t  -     @135
    m_sb_values                  ::score_board::EXAMPLE::single_cycle_t::data_values_t        -     @133
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
UVM_INFO ../../../src/uvmsc/report/uvm_default_report_server.cpp(667) @ 60541 ns: reporter [UVM/REPORT/SERVER]
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
user@verify:~/uvm-demo/build-linux$
```


Debian (Bookworm) Configuration Issues
===============================================================================

Whilst building the SystemC libraries, system configuration issues
were observed.  Note that the authors used a minimal Debian
(bookworm) virtual machine as their starting point.

The authors' solutions to these issues are
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
user@verify:~/uvm-demo/systemc-3.0.1$ ./config/bootstrap
./config/bootstrap: 8: aclocal: not found
user@verify:~/uvm-demo/systemc-3.0.1$
```


Bootstrap Failure due to Missing `libtoolize` Command
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `libtool`
package.  The following was observed whilst executing the bootstrap
process:

```bash
user@verify:~/uvm-demo/systemc-3.0.1$ ./config/bootstrap
./config/bootstrap: 9: libtoolize: not found
user@verify:~/uvm-demo/systemc-3.0.1$
```


Configuration Failure due to Missing C++ Compiler
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `g++`
package.  The following (abridged output) was observed whilst
executing the configuration of SystemC; observe that the
configuration script detects an error, but message is somewhat
misleading:

```bash
user@verify:~/uvm-demo/systemc-3.0.1/objdir$ ../configure CXXFLAGS='-std=c++20'
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
configure: error: in `/home/user/uvm-demo/systemc-3.0.1/objdir':
configure: error: C++ compiler cannot create executables
See `config.log' for more details
user@verify:~/uvm-demo/systemc-3.0.1/objdir$
```


Configuration Failure due to Missing `make` Command
-------------------------------------------------------------------------------

The authors' solution to this problem was to install the `make`
package.  The following (abridged output) was observed whilst
executing the configuration of SystemC; observe that the
configuration script was able to identify the problem and suggested
a potential solution:

```bash
user@verify:~/uvm-demo/systemc-3.0.1/objdir$ ../configure CXXFLAGS='-std=c++20'
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu
checking target system type... x86_64-unknown-linux-gnu
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes

[snip]

config.status: executing depfiles commands
config.status: error: in `/home/user/uvm-demo/systemc-3.0.1/objdir':
config.status: error: Something went wrong bootstrapping makefile fragments
    for automatic dependency tracking.  If GNU make was not used, consider
    re-running the configure script with MAKE="gmake" (or whatever is
    necessary).  You can also try re-running configure with the
    '--disable-dependency-tracking' option to at least be able to build
    the package (albeit without support for automatic dependency tracking).
See `config.log' for more details
user@verify:~/uvm-demo/systemc-3.0.1/objdir$
```


---

End of file.
