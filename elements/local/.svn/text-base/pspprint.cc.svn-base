/*
 * print.{cc,hh} -- element prints packet contents to system log
 * John Jannotti, Eddie Kohler
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 * Copyright (c) 2008 Regents of the University of California
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
#include "pspprint.hh"
#include <click/glue.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/straccum.hh>
#include <clicknet/psp.h>
#include <clicknet/udp.h>
#include <clicknet/ether.h>
#ifdef CLICK_LINUXMODULE
# include <click/cxxprotect.h>
CLICK_CXX_PROTECT
# include <linux/sched.h>
CLICK_CXX_UNPROTECT
# include <click/cxxunprotect.h>
#endif
CLICK_DECLS

PSPPrint::PSPPrint()
{
}

PSPPrint::~PSPPrint()
{
}

int PSPPrint::configure(Vector<String> &conf, ErrorHandler* errh)
{
  bool timestamp = false;
#ifdef CLICK_LINUXMODULE
  bool print_cpu = false;
#endif
  bool print_anno = false, headroom = false, bcontents;
  _active = true;
  String label, contents = "HEX";
  unsigned bytes = 24;

    if (Args(conf, this, errh)
	.read_p("LABEL", label)
	.read_p("MAXLENGTH", bytes)
	.read("LENGTH", Args::deprecated, bytes)
	.read("NBYTES", Args::deprecated, bytes)
	.read("CONTENTS", WordArg(), contents)
	.read("TIMESTAMP", timestamp)
	.read("PRINTANNO", print_anno)
	.read("ACTIVE", _active)
	.read("HEADROOM", headroom)
#if CLICK_LINUXMODULE
	.read("CPU", print_cpu)
#endif
	.complete() < 0)
	return -1;

    if (BoolArg().parse(contents, bcontents))
      _contents = bcontents;
  else if ((contents = contents.upper()), contents == "NONE")
      _contents = 0;
  else if (contents == "HEX")
      _contents = 1;
  else if (contents == "ASCII")
      _contents = 2;
  else
      return errh->error("bad contents value '%s'; should be 'NONE', 'HEX', or 'ASCII'", contents.c_str());

  _label = label;
  _bytes = bytes;
  _timestamp = timestamp;
  _headroom = headroom;
  _print_anno = print_anno;
#ifdef CLICK_LINUXMODULE
  _cpu = print_cpu;
#endif
  return 0;
}

Packet * PSPPrint::simple_action(Packet *p)
{
    if (!_active)
	return p;

    int bytes = (_contents ? _bytes : 0);
	const click_ether *peth = reinterpret_cast<const click_ether *>(p->data()) ;
	const click_psp *psp = reinterpret_cast<const click_psp *>(p->data()+14);
	const click_udp *udp = reinterpret_cast<const click_udp *>(psp + 1);
	unsigned char *p_pspaddr = 0;
	
    if (bytes < 0 || (int) p->length() < bytes)
	bytes = p->length();
	if(peth->ether_type !=  htons(0x0600)) return p;
	
    StringAccum sa(_label.length() + 2 // label:
		   + 6		// (processor)
		   + 28		// timestamp:
		   + 9		// length |
		   + 84      // packet info
		   + (_headroom ? 17 : 0) // (h[headroom] t[tailroom])
		   + Packet::anno_size*2 + 3 // annotations |
		   + 3 * bytes);
    if (sa.out_of_memory()) {
		click_chatter("no memory for PSPPrint");
		return p;
    }

    const char *sep = "";
    if (_label) {
		sa << _label;
		sep = ": ";
    }
    if (_timestamp) {
		sa << sep << p->timestamp_anno();
		sep = ": ";
    }

    // sa.reserve() must return non-null; we checked capacity above
    int len;
    len = sprintf(sa.reserve(11), "%s%4d", sep, p->length());
    sa.adjust_length(len);

    // headroom and tailroom
    if (_headroom) {
		len = sprintf(sa.reserve(16), " (h%d t%d)", p->headroom(), p->tailroom());
		sa.adjust_length(len);
    }
	// print psp seq(8B) , src addr(32B), des addr, udp srcport(6B) , desport
	len = sprintf(sa.reserve(16), "| seq %d",psp->psp_id);
	sa.adjust_length(len);
	p_pspaddr = (unsigned char *)&(psp->psp_src);
	len = sprintf(sa.reserve(32), " src %d.%d.%d.%d.%d.%d.%d.%d",p_pspaddr[0],p_pspaddr[1],p_pspaddr[2],p_pspaddr[3],
		p_pspaddr[4],p_pspaddr[5],p_pspaddr[6],p_pspaddr[7]);
	sa.adjust_length(len);
	p_pspaddr = (unsigned char *)&(psp->psp_dst);
	len = sprintf(sa.reserve(32), " des %d.%d.%d.%d.%d.%d.%d.%d",p_pspaddr[0],p_pspaddr[1],p_pspaddr[2],p_pspaddr[3],
		p_pspaddr[4],p_pspaddr[5],p_pspaddr[6],p_pspaddr[7]);
	sa.adjust_length(len);
	len = sprintf(sa.reserve(16), " srcport %d",udp->uh_sport);
	sa.adjust_length(len);
	len = sprintf(sa.reserve(16), " desport %d",udp->uh_dport);
	sa.adjust_length(len);

	
    if (_print_anno) {
		sa << " | ";
		char *buf = sa.reserve(Packet::anno_size * 2);
		int pos = 0;
		for (unsigned j = 0; j < Packet::anno_size; j++, pos += 2)
		    sprintf(buf + pos, "%02x", p->anno_u8(j));
		sa.adjust_length(pos);
    }
	
    if (bytes) {
		sa << " | ";
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

  return p;
}

void
PSPPrint::add_handlers()
{
    add_data_handlers("active", Handler::OP_READ | Handler::OP_WRITE | Handler::CHECKBOX | Handler::CALM, &_active);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PSPPrint)
ELEMENT_MT_SAFE(PSPPrint)
