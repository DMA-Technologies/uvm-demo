
Installation Notes for Windows
===============================================================================

KISS - Keep It Simple Stupid.

This build/install of this UVM demonstration project is based on the
community release of Microsoft Visual Studio.  Nothing herein should
conflict with the use of the Microsoft professional tool versions.  To
quote elementary school teachers, 'caveat emptor'.

As the project, the goal is to demonstrate the use of SystemC and
UVM/SystemC, no changes to any reference library source code were
allowed.  Rather, the authors used the necessary, minimal changes of
the reference projects' default MSVS properties that are required to
bring the builds into consistency across configurations and platforms.

Major issues affecting project implementation included:
* UVM/SystemC release build incompatibility with SystemC.
* Use of the modern C++ standards.
* Use of warnings to expose latent software bugs.

__NOTA BENE__: hereinafter we use `<demo>` as a stand-in for the
project's base directory.  All code, object files, &c. are built
thereunder.

__NOTA BENE__: In some specific examples you will see the directory
name `C:\tmp\uvm-demo` used in place of `<demo>`.  This is a
consequence of the authors verifying that this install procedure
functions correctly.


General Outline
-------------------------------------------------------------------------------

This is a general outline of process required to download and build the
UVM/SystemC Demonstration Project using MSVC 2022.  Specific details for
each step of the outline are detailed in the respective sections, below.

Some MSVC configuration and platform combinations were observed to have
inconsistent properties, causing build issues.  These issues are resolved
is steps 6 and 7, which apply updates to the added projects.


### Installation Steps

1. [Download This Repository from GitHub](#Download-This-Repository-from-GitHub)

2. [Download SystemC and UVM/SystemC Archives](#Download-SystemC-and-UVMSystemC-Archives)

3. [Unpack the SystemC and UVM/SystemC Archives](#Unpack-the-SystemC-and-UVMSystemC-Archives)

4. [Open the MSVC Solution](#Open-the-MSVC-Solution)

5. [Add SystemC and UVM/SystemC into the MSVC Solution](#Add-SystemC-and-UVMSystemC-into-the-MSVC-Solution)

6. [Patch the Added SystemC and SystemC-core Projects](#Patch-the-Added-SystemC-and-SystemC-core-Projects)

7. [Patch the Added UVM/SystemC Project](#Patch-the-Added-UVMSystemC-Project)

8. [Build the 'uvm-EXAMPLE-single_cycle' Project](#Build-the-uvm-EXAMPLE-single_cycle-Project)





Download This Repository from GitHub
-------------------------------------------------------------------------------

The goal of this step is to download and verify correct installation of the
demonstration project into a well-defined directory structure.

Download and unpack or clone this repository into the `<demo>` directory.

After unpacking or cloning, you should see the the file `<demo>\README.md`,
amongst others.

For example, using the command shell, this is what you ought see:

```dos
C:\tmp\uvm-demo>dir /d /o
 Volume in drive C has no label.
 Volume Serial Number is DAED-0135

 Directory of C:\tmp\uvm-demo

[.]
[..]
[build-msvc]
[documents]
[source]
INSTALL.md
INSTALL_LINUX.md
INSTALL_WINDOWS.md
LICENSE
NOTICE
README.md
RELEASE_NOTES.md

               7 File(s)         39,657 bytes
               5 Dir(s)  259,910,402,048 bytes free

C:\tmp\uvm-demo>
```


Download SystemC and UVM/SystemC Archives
-------------------------------------------------------------------------------

Specific library versions are called for in this step, as various
incompatibilities were identified between the software reference
versions when this UVM/SystemC Demonstration Project was commenced.  

The authors note that a newer UVM/SystemC beta version were released
prior to UVM/SystemC Demonstration Project being released.  


### SystemC Reference Version 2.3.4

Download one of the two available distribution files of the SystemC
reference version 2.3.4.

Links to the SystemC-2.3.4 reference archives:

1. [SystemC Tarball](https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.tar.gz)

2. [SystemC Zip File](https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.zip)


### UVM/SystemC Reference Version 1.0 beta5

Download the archived beta5 version of the reference UVM/SystemC 1.0
library.

Link to the
[UVM/SystemC-1.0-beta5](https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-1.0-beta5.tar.gz)
reference archive.


Unpack the SystemC and UVM/SystemC Archives
-------------------------------------------------------------------------------

In this step you will unpack the archived SystemC and UVM/SystemC
library source code into directories which are version numbered.
This is done to avoid future confusion and grief.

As this project progresses, it is likely that updated reference
libraries will be incorporated.  When these updates are emplaced,
the documentaiton will be updated to reflect the changes.

Should you decide to use alternate versions of the reference
libraries a priori, the authors would appreciate a note detailing
your observations, trials, and tribulations.

__NOTA BENE__: Some paths in the SystemC archive are longer than
the historical 99 character limit, and several Windows archivers
(e.g. WinZip) have been reported to trip over this.  The open
source archiver [7-zip](http://7-zip.org/) is known to work.


### SystemC

Unpack the downloaded reference release of SystemC 2.3.4 into into
the directory `<demo>\systemc-2.3.4`.

After unpacking, as a sanity check, you should see the the file
`<demo>\systemc-2.3.4\README.md`, amongst others.

For example, using the Windows command shell, you should see something
similar to the following when you do a directory listing of the files
in `<demo>\systemc-2.3.4`:

```dos
C:\tmp\uvm-demo\systemc-2.3.4>dir /d /o
 Volume in drive C has no label.
 Volume Serial Number is DAED-0135

 Directory of C:\tmp\uvm-demo\systemc-2.3.4

[.]
[..]
[cmake]
[config]
[docs]
[examples]
[msvc10]
[src]
AUTHORS.md
CMakeLists.txt
configure
configure.ac
CONTRIBUTING.md
INSTALL.md
LICENSE
Makefile.am
Makefile.in
NOTICE
README.md
RELEASENOTES

              12 File(s)        786,875 bytes
               8 Dir(s)  259,851,776,000 bytes free

C:\tmp\uvm-demo\systemc-2.3.4>
```


### UVM/SystemC

Unpack the downloaded beta release of UVM/SystemC 1.0 into the
directory `<demo>\uvm-systemc-1.0-beta5`.

After unpacking, you should see the the file
`<demo>\uvm-systemc-1.0-beta5\README.md`, amongst others.

For example, using the Windows command shell, you should see something
similar to the following when you do a directory listing of the files
in `<demo>\uvm-systemc-1.0-beta5`:

```dos
C:\tmp\uvm-demo\uvm-systemc-1.0-beta5>dir /d /o
 Volume in drive C has no label.
 Volume Serial Number is DAED-0135

 Directory of C:\tmp\uvm-demo\uvm-systemc-1.0-beta5

[.]
[..]
[config]
[docs]
[examples]
[msvc14]
[src]
.gitignore
aclocal.m4
AUTHORS
ChangeLog
configure
configure.ac
INSTALL
LICENSE
Makefile.am
Makefile.in
NEWS
NOTICE
README.md
RELEASENOTES

              14 File(s)        797,862 bytes
               7 Dir(s)  259,850,612,736 bytes free

C:\tmp\uvm-demo\uvm-systemc-1.0-beta5>
```


Open the MSVC Solution
-------------------------------------------------------------------------------

Open the solution file located at `<demo>\build-msvc\build-msvc.sln`.

At this step you ought see only one project within the solution:
* `uvm-EXAMPLE-single_cycle`


Add SystemC and UVM/SystemC into the MSVC Solution
-------------------------------------------------------------------------------

1. From Visual Studio add the reference SystemC and UVM/SystemC projects
into the solution:

	
	| PROJECT       | LOCATION                                                        |
	|---------------|-----------------------------------------------------------------|
	| SystemC-core  | `<demo>\SystemC-2.3.4\msvc10\SystemC\SystemC-core.vcxproj`      |
	| SystemC       | `<demo>\SystemC-2.3.4\msvc10\SystemC\SystemC.vcxproj`           |
	| uvm-systemc   | `<demo>\uvm-systemc-1.0-beta5\msvc14\uvmsc\uvm-systemc.vcxproj` |

2. Retarget all projects to your MSVC installation.  

	1. Select the solution 'build-msvc' in the solution explorer.

	2. Retarget all projects (Project | Retarget solution).

	3. Verify that the retargeting is correctly configured.

	4. Click 'Ok' to complete retargeting.

3. Following addition, save the solution and all modified files
   (File | Save All).  This will abey madness.


Patch the Added SystemC and SystemC-core Projects
-------------------------------------------------------------------------------

These patches are repeated twice, once for the 'SystemC' project and
then again for the 'SystemC-core' project.  They allow compilation
using the C++20 language standard.


### Apply the Following 'Configuration Properties' Changes

The first change updates the language standard; the second obviates a
contradiction in compiler switches.

1. Set the Language Standard to C++20.

	* All Configurations
	* All Platforms
	* General Properties | C++ Language Standard
	* Select 'ISO C\++20 Standard (/stdc\++20)'

2. Restore Minimal Rebuild to the Default.

	* All Configurations
	* All Platforms
	* C/C++ | Code Generation | Enable Minimal Rebuild
	* Select '\<inherit from parent or project defaults\>'


Patch the Added UVM/SystemC Project
-------------------------------------------------------------------------------

These changes bring the `uvm-systemc` project into compatibility with
the SystemC properties which allows successful build of the
demonstration project using all configuration/platform combinations.


### Add the 'build-msvc\\uvm-systemc.props' property sheet to the project.

The properties set in the sheet are: 1) common to all of the
configuration/platform combinations and 2) defaulted in the project
specific properties.

Three properties are set:

1. Sets user macro SYSTEMC_HOME.
2. Selects language standard ISO C++20.
3. Selects warning Level 3 (/W3).


### Modify the Properties to Obviate Failures due to a deprecated option under C++20.

These changes allow the project to compile and repair a linking error
induced by an improperly set runtime library option.

1. Restore Minimal Rebuild to the Default.

	* All Configurations
	* All Platforms
	* C/C++ | Code Generation | Enable Minimal Rebuild
	* Select '\<inherit from parent or project defaults\>'

2. Fix the Runtime Library Requirement in UVM/SystemC.

	* __Debug__ Configuration
	* All Platforms
	* C/C++ | Code Generation | Runtime Library
	* Select Multi-Threaded Debug DLL (/MDd)

Following the changes to the properties, save the solution
and all other modified files (File|Save All).  This will help avoid
troubles and madness.


Set the Startup Project and References
-------------------------------------------------------------------------------

This step establishes proper build-order and library linkage.

1. Establish the project 'uvm-EXAMPLE-single_cycle' as the startup
   project.

2. Add references to the 'uvm-EXAMPLE-single_cycle' project.

	* SystemC
	* SystemC-core
	* uvm-systemc

Following the changes to the references, save the solution
and all other modified files (File|Save All).  This will help avoid
troubles and madness.


Build the 'uvm-EXAMPLE-single_cycle' Project
-------------------------------------------------------------------------------

Select the project 'uvm-EXAMPLE-single_cycle' and do a 'Ctrl+B'
or from Build menu click on Build 'uvm-EXAMPLE-single_cycle'


---

End of file.