/* Small hardware monitor for the EpoX mainboard
 *        OS/2
 *
 *  bjorn@mork.no
 *     updated to add CPU temp for Winbond 83782 chips, Trevor-Hemsley@dial.pipex.com
 *
 *  06/2007 Modified by bart@together.net
 *          Target EPoX 8RDA6+Pro mainboard.
 *          Target IBM CSet compilier, Command line Native OS/2 (no EMX).
 *          Added test for fan speed count to avoid a divide by 0 trap
 *          Use the PORTIO external port access functions (included in zip)
 *          Changed some of the C macros to C functions
 *          Original (V0.05) code is on Hobbes.nmsu.edu as p2l97v05.zip in /pub/os2/util/mboard
 */

#include <builtin.h>
#include <stdio.h>
#define  INCL_DOSPROCESS      /* needed for DosSleep etc*/
#include <os2.h>
#include "PORTIO.H"

#define VERSION "0.06 ("__DATE__")"


#define LM78_BASE      0x290
#define LM78_ADDRESS   (LM78_BASE+0x05)
#define LM78_DATA      (LM78_BASE+0x06)


/* operations - must be changed for other operating systems! */
/*  Not used for the cset command line version,  left in as a comment
#ifdef __EMX__
    #define LM78_INIT             _portaccess(LM78_BASE,LM78_DATA)      // enabling access to the range of ports
    #define LM78_EXIST            ({_outp8(LM78_ADDRESS,LM78_IN0);_inp8(LM78_DATA);_inp8(LM78_ADDRESS) == LM78_IN0;})
    #define LM78_BUSY             (_inp8(LM78_ADDRESS)>>7)
    #define LM78_READ(addr)       ({_outp8(LM78_ADDRESS,addr);_inp8(LM78_DATA);})
    #define LM78_WRITE(addr,data) _outp8(LM78_ADDRESS,addr);_outp8(LM78_DATA,data)
#endif
*/

/* internal addresses */
#define LM78_IN0           0x20
#define LM78_IN1           0x21
#define LM78_IN2           0x22
#define LM78_IN3           0x23
#define LM78_IN4           0x24
#define LM78_IN5           0x25    // was neg
#define LM78_IN6           0x26    // was neg
#define LM78_TEMP          0x27
#define LM78_FAN1          0x28
#define LM78_FAN2          0x29
#define LM78_FAN3          0x2A
#define LM78_IN0_HIGH      0x2B
#define LM78_IN0_LOW       0x2C
#define LM78_IN1_HIGH      0x2D
#define LM78_IN1_LOW       0x2E
#define LM78_IN2_HIGH      0x2F
#define LM78_IN2_LOW       0x30
#define LM78_IN3_HIGH      0x31
#define LM78_IN3_LOW       0x32
#define LM78_IN4_HIGH      0x33
#define LM78_IN4_LOW       0x34
#define LM78_IN5_HIGH      0x35
#define LM78_IN5_LOW       0x36
#define LM78_IN6_HIGH      0x37
#define LM78_IN6_LOW       0x38
#define LM78_TEMP_HIGH     0x39
#define LM78_TEMP_LOW      0x3A
#define LM78_FAN1_LIMIT    0x3B
#define LM78_FAN2_LIMIT    0x3C
#define LM78_FAN3_LIMIT    0x3D
#define LM78_LM87_ID       0x3E
/* reserved 0x3F */
#define LM78_CONFIG        0x40  /* bit mapped register */
#define LM78_STATUS1       0x41  /* read only bit mapped register */
#define LM78_STATUS2       0x42  /* read only bit mapped register */
#define LM78_SMI_MASK1     0x43  /* bit mapped register */
#define LM78_SMI_MASK2     0x44  /* bit mapped register */
#define LM78_NMI_MASK1     0x45  /* bit mapped register */
#define LM78_NMI_MASK2     0x46  /* bit mapped register */
#define LM78_FANDIV        0x47  /* bit mapped register */
#define LM78_FANDIVA       0x4B  /* bit mapped register */
#define LM78_SERIAL        0x48  /* bit mapped register */
#define LM78_RESET         0x49  /* bit mapped register */
#define LM78_4A            0x4a
#define LM78_BANKSEL       0x4E
#define LM78_BANK0         0x80
#define LM78_BANK1         0x81
#define LM78_BANK2         0x82
#define LM78_BANK3         0x84
#define LM78_VENDOR        0x4F
#define LM78_EXT_TEMP1     0x50
#define LM78_EXT_TEMP2     0x50
#define LM78_TEMP_CFG      0x52
#define LM78_CHIPID        0x58
#define LM78_DIODE         0x59

/* conversion routines */
#define LM78_FANDIV1       (1<<((LM78_READ(LM78_FANDIV)&0x30)>>4))   /* bits 4 and 5 */
#define LM78_FANDIV2       (1<<(LM78_READ(LM78_FANDIV)>>6)) /* bits 6 and 7 */
#define LM78_FANDIV3       (1<<(LM78_READ(LM78_FANDIVA)>>6)) /* bits 6 and 7 */


// #define P2L97_M12V  ((double)LM78_READ(LM78_IN5_NEG) * -0.0564)
// #define P2L97_M5V   ((double)LM78_READ(LM78_IN6_NEG) * -0.0242)

// #define LM78_RPM(count,fandiv) (1350000/(count*fandiv))
double LM78_RPM(int count, int fandiv)
    {
    if (count != 0.0)
        {
        return(1.350e6 / (count * fandiv));
        }
    else
        {
        return(0.0);
        }
    }
// #endif /* LM78 */

//  BEG Added these for icc compiler no EMX
int LM78_INIT(void)
    {
    return(0);
    }


int LM78_EXIST(void)
    {
    outp(LM78_ADDRESS,LM78_IN0);
    inp(LM78_DATA);
    return(inp(LM78_ADDRESS) == LM78_IN0);
    }


int LM78_BUSY(void)
    {
    // return(inp(LM78_ADDRESS)>>7);
    return( (inp(LM78_ADDRESS) & 0x80) != 0);
    }


// #define LM78_READ(addr)       ({outp(LM78_ADDRESS,addr);inp(LM78_DATA);})
int LM78_READ(int addr)
    {
    outp(LM78_ADDRESS,addr);
    return(inp(LM78_DATA));
    }


// #define LM78_WRITE(addr,data) outp(LM78_ADDRESS,addr);outp(LM78_DATA,data)
void LM78_WRITE(int addr, int data)
    {
    outp(LM78_ADDRESS, addr);
    outp(LM78_DATA, data);
    }


void fanprint(char *title, short tick, short fandiv)
    {
    if (tick!=255)
        {
        printf("%s [%3d,%d] => %7.2lf RPM\n", title, tick, fandiv, LM78_RPM(tick, fandiv));
        }
    }


// --------------------------------------
//         MAIN
// --------------------------------------
int main(int argc, char* argv[])
    {
    int  i = 0;
    int  lread = 0;
    int  cfan = 0;

    // If any parameters are passed on the command line print error.  (This also eliminates compilier informational message)
    if (argc != 1)
        {
        fprintf(stderr, "\n No Arguments used in %s", argv[1]);
        }

    printf("LMxx/Super-IO W83627hf monitor for EPoX 8RDA6+Pro mainboard\n");
    printf("Version %s\n\n", VERSION);
    LM78_INIT();
    if (LM78_EXIST())
        {
        // The chasis fan is a bit finicky,  try a few times
        for (i=0; i<10; i++)
            {
            cfan = LM78_READ(LM78_FAN1);
            if (cfan > 10 )
                {
                break;
                }
            }
        fanprint("\tChassis fan\t:", cfan, LM78_FANDIV1);

        fanprint("\tCPU fan    \t:", LM78_READ(LM78_FAN2), LM78_FANDIV2);
        fanprint("\tPower fan  \t:", LM78_READ(LM78_FAN3), LM78_FANDIV3);

        i =      LM78_READ(LM78_TEMP);
        printf("\n\tMB temperature\t:%3døC / %3døF\n", i, (i * 9 / 5 + 32) );

        LM78_WRITE(LM78_BANKSEL, LM78_BANK0);                   // switch RAM bank
        lread = LM78_READ(LM78_VENDOR);
        if (lread == 0x5c)                                      // if a Winbond chip
            {
            LM78_WRITE(LM78_BANKSEL, (LM78_BANK0 &      0x7f)); // switch register halves
            lread = LM78_READ(LM78_VENDOR);
            if (lread      == 0xa3)                             // and check other half of sig
                {
                LM78_WRITE(LM78_BANKSEL, LM78_BANK1);           // switch RAM bank
                lread = LM78_READ(LM78_TEMP_CFG);
                if ((lread & 0x01) == 0)                        // if cpu monitor not stopped
                    {
                    i =      LM78_READ(LM78_EXT_TEMP1);         // read his temp
                    if (i > 127)                                // cater for 2's complement
                        i = (256 - i)*(-1);
                    printf("\tCPU0 temp.\t:%3døC / %3døF\n", i, (i * 9 / 5 + 32) );
                    }

                LM78_WRITE(LM78_BANKSEL, LM78_BANK2);           //  switch to other bank
                lread = LM78_READ(LM78_TEMP_CFG);
                if ((lread & 0x01) == 0)                        // to check if 2nd cpu installed
                    {
                    i =      LM78_READ(LM78_EXT_TEMP2);
                    if (i > 127)
                        i = (256 - i)*(-1);
                    printf("\tCPU1 temp.\t:%3døC / %3døF\n", i, (i * 9 / 5 + 32) );
                    }

                LM78_WRITE(LM78_BANKSEL, LM78_BANK0);
                }
            }

        // These Divisors were selected to match the EPoX USDM Windows application (Best guess at resistor divider)
        printf("\n");
        printf("\tVcore voltage  \t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN0), (double)(LM78_READ(LM78_IN0) * 0.0163) );  // nom 1.700
        printf("\tAGP voltage    \t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN1), (double)(LM78_READ(LM78_IN1) * 0.0160) );  // nom 1.536
        printf("\tChipset voltage\t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN2), (double)(LM78_READ(LM78_IN2) * 0.0163) );  // nom 1.632
        printf("\t+5V voltage    \t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN3), (double)(LM78_READ(LM78_IN3) * 0.0269) );  // nom 4.995
        printf("\tDIMM voltage   \t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN4), (double)(LM78_READ(LM78_IN4) * 0.0160) );  // nom 2.768
        printf("\tBatt voltage   \t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN5), (double)(LM78_READ(LM78_IN5) * 0.0123) );  // nom 2.976
        printf("\tStandby voltage\t: [%3d] => %6.3f V\n", LM78_READ(LM78_IN6), (double)(LM78_READ(LM78_IN6) * 0.0260) );  // nom 4.995  .0265
        }
    else
        {
        printf("\nERROR: Cannot find an LMxx/Winbond at port %#hx\n", LM78_BASE);
        }
    return(0);
    }

