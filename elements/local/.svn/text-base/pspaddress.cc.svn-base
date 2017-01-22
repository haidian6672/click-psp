// -*- c-basic-offset: 4; related-file-name: "../include/click/ipaddress.hh" -*-
/*
 * ipaddress.{cc,hh} -- an IP address class
 * Robert Morris / John Jannotti / Eddie Kohler
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
#include <click/glue.hh>
#include <click/pspaddress.hh>
#include <click/args.hh>
#include <click/straccum.hh>
#include <click/integers.hh>
#if !CLICK_TOOL
# include <click/nameinfo.hh>
# include <click/standard/addressinfo.hh>
#elif HAVE_NETDB_H
# include <netdb.h>
#endif
CLICK_DECLS


PSPAddress::PSPAddress(const String &str)
{
    
}


StringAccum &
operator<<(StringAccum &sa, PSPAddress ipa)
{
    const unsigned char *p = ipa.data();
    char buf[20];
    int amt;
    amt = sprintf(buf, "%d.%d.%d.%d.%d.%d.%d.%d", p[0], p[1], p[2], p[3],p[4], p[5], p[6], p[7]);
    sa.append(buf, amt);
    return sa;
}


CLICK_ENDDECLS
