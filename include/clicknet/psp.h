/* -*- mode: c; c-basic-offset: 4 -*- */
#ifndef CLICKNET_PSP_H
#define CLICKNET_PSP_H
/* get struct in_addr */
#include <click/cxxprotect.h>
CLICK_CXX_PROTECT
#if CLICK_LINUXMODULE
# include <net/checksum.h>
# include <linux/in.h>
#else
# include <sys/types.h>
# include <netinet/in.h>
#endif

/*
 * <clicknet/psp.h> -- PSP header definitions.
 */

struct click_psp {
#if CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
    unsigned	psp_v : 4;		/* 0     version == 1		     */
    unsigned	psp_resv : 4;		/*      reserve field		     */
#elif CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
    unsigned	psp_resv : 4;		
    unsigned	psp_v : 4;		
#else
#   error "unknown byte order"
#endif
    unsigned	psp_id:24;			/* start from 1     packet sequence number	     */
    uint16_t	psp_len;			/*16 bits  psp packet data len, doesn't include psp header	     */
    uint8_t	psp_next;			/* protocol type in psp data field    */
    uint8_t	psp_ttl;			/* hops limits		     */
    uint64_t psp_src;		/* 12-15 source address		     */
    uint64_t psp_dst;		/* 16-19 destination address	     */
};

#define PSP_ICMP                 (1)
#define PSP_UDP                  (2)


CLICK_CXX_UNPROTECT
#include <click/cxxunprotect.h>
#endif
