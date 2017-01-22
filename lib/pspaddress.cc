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


// PSPAddress::PSPAddress(const String &str)
// {
//     _addr = 0;
// }


StringAccum &
operator<<(StringAccum &sa, PSPAddress ipa)
{
    const unsigned char *p = ipa.data();
    char buf[40];
    int amt;
    amt = sprintf(buf, "%x:%x:%x:%x:%x:%x:%x:%x", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
    sa.append(buf, amt);
    return sa;
}

String
PSPAddress::unparse() const
{
    const unsigned char *p = data();
    char buf[40];
    sprintf(buf, "%x:%x:%x:%x:%x:%x:%x:%x", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
    return String(buf);
}

String
PSPAddress::unparse_mask() const
{
    int prefix_len = 64;
    if (prefix_len >= 0)
	return String(prefix_len);
    else
	return unparse();
}

String
PSPAddress::unparse_with_mask(PSPAddress mask) const
{
    return unparse() + "/" + mask.unparse_mask();
}

// unsigned char* PSPAddress::print() const
// {
// unsigned char buf[40];
// 
// }
        
        
const char *
PSPAddressArg::basic_parse(const char *s, const char *end,
			  unsigned char value[8], int &nbytes)
{
    memset(value, 0, 8);
    int d = 0;
    while (d < 8 && s != end && (d == 0 || *s == '.')) {
	const char *t = s + !!d;
	if (t == end || !isdigit((unsigned char) *t))
	    break;
	int part = *t - '0';
	for (++t; t != end && isdigit((unsigned char) *t) && part <= 255; ++t)
	    part = part * 10 + *t - '0';
	if (part > 255)
	    break;
	s = t;
	value[d] = part;
	if (++d == 8)
	    break;
    }
    nbytes = d;
    return s;
}

bool
PSPAddressArg::parse(const String &str, PSPAddress &result, const ArgContext &args)
{
    unsigned char value[8];
    int nbytes;
    if (basic_parse(str.begin(), str.end(), value, nbytes) == str.end()
	&& nbytes == 8) {
//         printf("PSPAddressArg::parse().\n");
	memcpy(&result, value, 8);
        printf("PSPAddressArg::parse() get result：%s.\n", result.unparse().data());
        return true;
    }
// #if !CLICK_TOOL
//    return AddressInfo::query_ip(str, result.data(), args.context());
// #else
//    (void) args;
    return false;
// #endif
}

// bool
// PSPAddressArg::parse(const String &str, Vector<PSPAddress> &result, const ArgContext &args)
// {
//     Vector<PSPAddress> v;
//     String arg(str);
//     PSPAddress ip;
//     int nwords = 0;
//     while (String word = cp_shift_spacevec(arg)) {
// 	++nwords;
// 	if (parse(word, ip, args))
// 	    v.push_back(ip);
// 	else
// 	    return false;
//     }
//     if (nwords == v.size()) {
// 	v.swap(result);
// 	return true;
//     }
//     args.error("out of memory");
//     return false;
// }

bool
PSPPrefixArg::parse(const String &str,
		   PSPAddress &result_addr, PSPAddress &result_mask,
		   const ArgContext &args) const
{
    const char *begin = str.begin(), *slash = str.end(), *end = str.end();
    while (slash != begin)
	if (*--slash == '/')
	    break;

//    if (slash != begin && slash + 1 != end
//	&& prefix_with_slash(str, slash, result_addr, result_mask, args))
//	return true;

    if (allow_bare_address && PSPAddressArg::parse(str, result_addr, args)) {
	result_mask = 0xFFFFFFFFFFFFFFFFU;
	return true;
    }

// #if !CLICK_TOOL
//     return AddressInfo::query_ip_prefix(str, result_addr.data(),
// 					result_mask.data(), args.context());
// #else
   return false;
// #endif
}

// EXPORT_ELEMENT(PSPAddressArg)
// EXPORT_ELEMENT(PSPPrefixArg)
CLICK_ENDDECLS
