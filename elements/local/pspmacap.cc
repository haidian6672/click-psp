/*
 * fastudpsource.{cc,hh} -- fast udp source, a benchmark tool
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include <clicknet/psp.h>
#include "pspmacap.hh"
#include <click/args.hh>
#include <click/etheraddress.hh>
#include <click/error.hh>
#include <click/straccum.hh>
#include <click/glue.hh>
#include <click/standard/alignmentinfo.hh>

CLICK_DECLS


PSPMacap::PSPMacap()
  : _packet(0)
{

}

PSPMacap::~PSPMacap()
{
}

int
PSPMacap::configure(Vector<String> &conf, ErrorHandler *errh)
{
  
  if (Args(conf, this, errh)
      .read_mp_with("DSTETH", EtherAddressArg(), _ethh.ether_dhost)
      .complete() < 0)
    return -1;
  _ethh.ether_type = htons(0x0600);

  return 0;
}

/*
int
FastPSPSource::initialize(ErrorHandler *)
{
  _count = 0;
  _incr = 0;
  WritablePacket *q = Packet::make(_len);
  _packet = q;
  memcpy(q->data(), &_ethh, 14);
  click_psp *psp = reinterpret_cast<click_psp *>(q->data()+14);
  click_udp *udp = reinterpret_cast<click_udp *>(psp + 1);
  _psphdr = psp;
  // set up PSP header
  psp->psp_v = 4;
  psp->psp_resv = 0;
  psp->psp_id = 0;
  psp->psp_len = htons(_len-14-sizeof(click_psp));
  psp->psp_next = PSP_UDP;
  psp->psp_ttl=8;
  psp->psp_src=_sipaddr;
  psp->psp_dst=_dipaddr;
  _packet->set_dst_psp_anno(_dipaddr);
  _packet->set_psp_header(psp, sizeof(click_psp));

  // set up UDP header
  udp->uh_sport = htons(_sport);
  udp->uh_dport = htons(_dport);
  udp->uh_sum = 0;
  unsigned short len = _len-14-sizeof(click_psp);
  udp->uh_ulen = htons(len);
  udp->uh_sum = 0;

  return 0;
}
*/
void PSPMacap::push(int port, Packet *p)
{
     
      WritablePacket *q = Packet::make(p->length());
      _packet = q;

      click_ether dest_ethh;
      memcpy(&dest_ethh,p->data(), 14);
      memcpy(&(dest_ethh.ether_dhost),&(_ethh.ether_dhost), 6);

      dest_ethh.ether_type = _ethh.ether_type; 

      memcpy(q->data(), &dest_ethh, 14);
      memcpy((q->data()+14),(p->data()+14),(p->length()-14));
     
         unsigned bytes = 64;
    StringAccum sa( 2 // label:
		   + 6		// (processor)
		   + 28		// timestamp:
		   + 9		// length |
		   + 84      // packet info
		   + 17 // (h[headroom] t[tailroom])
		   + 8 // annotations |
		   + 3 * bytes);
    if (sa.out_of_memory()) {
		click_chatter("no memory for PSPPrint");
		
    }

    int len;
    	
    const click_ether *peth = reinterpret_cast<const click_ether *>(p->data()) ;
    const click_psp *psp = reinterpret_cast<const click_psp *>(p->data()+14);
    const click_udp *udp = reinterpret_cast<const click_udp *>(psp + 1);
    unsigned char *p_pspaddr = 0;
    

	// print psp seq(8B) , src addr(32B), des addr, udp srcport(6B) , desport
	p_pspaddr = (unsigned char *)&(psp->psp_src);
	len = sprintf(sa.reserve(32), " src %d.%d.%d.%d.%d.%d.%d.%d",p_pspaddr[0],p_pspaddr[1],p_pspaddr[2],p_pspaddr[3],
		p_pspaddr[4],p_pspaddr[5],p_pspaddr[6],p_pspaddr[7]);
	sa.adjust_length(len);
	p_pspaddr = (unsigned char *)&(psp->psp_dst);
	len = sprintf(sa.reserve(32), " des %d.%d.%d.%d.%d.%d.%d.%d",p_pspaddr[0],p_pspaddr[1],p_pspaddr[2],p_pspaddr[3],
		p_pspaddr[4],p_pspaddr[5],p_pspaddr[6],p_pspaddr[7]);
	sa.adjust_length(len);

  if (bytes) {
		sa << " | ";
                int _contents =1;
		char *buf = sa.data() + sa.length();
		const unsigned char *data = p->data();
		if (_contents == 1) {
		    for (int i = 0; i < bytes; i++, data++) {
			if (i && (i % 4) == 0)
			    *buf++ = ' ';
			sprintf(buf, "%02x", *data & 0xff);
			buf += 2;
		    }
		} else if (_contents == 2) {
		    for (int i = 0; i < bytes; i++, data++) {
			if ((i % 8) == 0)
			    *buf++ = ' ';
			if (*data < 32 || *data > 130)
			    *buf++ = '.';
			else
			    *buf++ = *data;
		    }
		}
		sa.adjust_length(buf - (sa.data() + sa.length()));
    }
   
   
   click_chatter("%s", sa.c_str());

  
  output(0).push(q);
}



CLICK_ENDDECLS
EXPORT_ELEMENT(PSPMacap)
ELEMENT_MT_SAFE(PSPMacap)
