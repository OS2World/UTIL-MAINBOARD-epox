/*   Compile with Visualage C++ for OS/2   */

/*
   Options:
   /Q+  :  No logo
   /Gm+ :  Link with multithread runtime libraries.
   /Gd- :  Use the version of the runtime library that is statically linked.


   Wall+: Enable all compilier messages.
   Wext-: Disable External not used messages.
   Wppt-: Disable  preprocessor trace messages.
   Wgen-: Disable General messages.

   Gd-: Static link of libraries.
   Ss : Source code options - allows two slashes (//) for comments
   Se : Allow all IBM C and C++ language extensions.
   Ti-: No debugger info.
*/


STDCCSW  = "/Q+ /Gd- /Gm+ /Ss /Se /Wall+ /Wext- /Wgen- /Wppt- /Wtrd- /Ti- "

'icc /B" /de /NOLOGO " ' STDCCSW ' EPoX_8RDA6.c EPoX_8RDA6.def PORTIO.obj '



