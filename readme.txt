
LM78, Windbond, SuperIO chip system monitor program.
I modified it for the Cset compilier (EMX not required) and targeted the 
readout for my EPox 8RDA6+Pro mainboard.  Please see the original package 
on Hobbes for the earlier mainboards and pm version.  I only made updates 
to the command line version.  The readout titles were updated to match the 
EPoX USDM monitor program supplied with the mainboard (windows).  The voltage
divisors were also selected to match the EPoX USDM Windows application 
(Best guess at resistor divider values.)

  06/2007 Modified by   "bart at together dot net"  (fix this to email me)
          Target EPoX 8RDA6+Pro mainboard.
          Target IBM CSet compilier, Command line Native OS/2 (no EMX).
          Added test for fan speed count to avoid a divide by 0 trap.
          Use the PORTIO external port access functions (included in zip)
          Changed some of the C macros to C functions

  Original (V0.05) code is on Hobbes.nmsu.edu as p2l97v05.zip in /pub/os2/util/mboard

** Please follow the ORIGINAL Licence and Q & A below.  **

----------------------------------------------------------
Original readme:

This is a very simple temp/fan/voltage monitor for mainboards with the
National Semiconductor LM78 chip. It has been tested on an Asus P2L97-S.

LICENSE: 
   This program, with source, is freely available to anyone
          interested. Change it, sell it, copy it, and do whatever
          you want with it.


Q1: Why doesn't the CPU temperature show up?
A1: Because I haven't written the necessary code. See Q4.

Q2: Why haven't you written the code for CPU temperature measurements?
A2: Because I don't have a board to test it on. See Q4.

Q3: Why is this program released as freeware? I would have paid several cents for it.
A3: Because it's unsupported. If you find a bug, see Q4.

Q4: Why is the program released with source?
A4: So you can fix it yourself instead of bothering me :-)

Q5: Where is the documentation?
A5: Writing docs is boring. The right mouse button will bring up the menu. Do you need to know more?

Q6: Why have you used this stupid technique to do xxxxx?
A6: Because this was my first attempt on a PM program. Comments are welcome.


Have fun,
Bj›rn <bjorn@mork.no>
