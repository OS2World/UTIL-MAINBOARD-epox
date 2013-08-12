#if !defined(PORTIO_H)
#define PORTIO_H

#if defined(__IBMCPP__)
   extern "C" {
#endif

#if defined(inp)
#undef inp
#endif

#if defined(inpw)
#undef inpw
#endif

#if defined(outp)
#undef outp
#endif

#if defined(outpw)
#undef outpw
#endif

extern short _Far16 _Pascal inp(unsigned short port);
extern unsigned short _Far16 _Pascal inpw(unsigned short port);
extern void _Far16 _Pascal outp(unsigned short port,
                                short data);
extern void _Far16 _Pascal outpw(unsigned short port,
                                 unsigned short data);

#if defined(__IBMCPP__)
   }
#endif

#endif
