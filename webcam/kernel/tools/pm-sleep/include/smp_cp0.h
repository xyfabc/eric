#ifndef __SMP_CP0_H__
#define __SMP_CP0_H__
#include <mipsregs.h>

#define get_smp_ctrl()		__read_32bit_c0_register($12, 2)
#define set_smp_ctrl(val)	__write_32bit_c0_register($12, 2, val)
#define get_smp_status()	__read_32bit_c0_register($12, 3)
#define set_smp_status(val)	__write_32bit_c0_register($12, 3, val)
#define get_smp_reim()		__read_32bit_c0_register($12, 4)
#define set_smp_reim(val)	__write_32bit_c0_register($12, 4, val)
#define get_smp_lock()		__read_32bit_c0_register($12, 5)
#define set_smp_lock(val)	__write_32bit_c0_register($12, 5, val)
#define get_smp_val()		__read_32bit_c0_register($12, 6)
#define set_smp_val(val)	__write_32bit_c0_register($12, 6, val)

#define get_smp_mbox0()		__read_32bit_c0_register($20, 0)
#define set_smp_mbox0(val)	__write_32bit_c0_register($20, 0, val)
#define get_smp_mbox1()		__read_32bit_c0_register($20, 1)
#define set_smp_mbox1(val)	__write_32bit_c0_register($20, 1, val)
#define get_smp_mbox2()		__read_32bit_c0_register($20, 2)
#define set_smp_mbox2(val)	__write_32bit_c0_register($20, 2, val)
#define get_smp_mbox3()		__read_32bit_c0_register($20, 3)
#define set_smp_mbox3(val)	__write_32bit_c0_register($20, 3, val)


#define smp_ipi_unmask(mask) do {		\
		unsigned int reim;		\
		reim = get_smp_reim();		\
		reim |= (mask) & 0xff;		\
		set_smp_reim(reim);		\
	} while(0)
#define smp_ipi_mask(mask) do {			\
		unsigned int reim;		\
		reim = get_smp_reim();		\
		reim &= ~((mask) & 0xff);	\
		set_smp_reim(reim);		\
	} while(0)

#define smp_clr_pending(mask) do {			\
		unsigned int stat;			\
		stat = get_smp_status();		\
		stat &= ~((mask) & 0xff);		\
		set_smp_status(stat);			\
	} while(0)

#endif
