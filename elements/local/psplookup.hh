// -*- c-basic-offset: 4 -*-
#ifndef CLICK_PSPLOOKUP_HH
#define CLICK_PSPLOOKUP_HH
#include "psproutetable.hh"
CLICK_DECLS

#define IP_RT_CACHE2 1

class PSPLookup : public PSPRouteTable { public:

    PSPLookup();
    ~PSPLookup();

    const char *class_name() const	{ return "PSPLookup"; }
    const char *port_count() const	{ return "1/-"; }
    const char *processing() const	{ return PUSH; }

    int initialize(ErrorHandler *);

    void push(int port, Packet *p);

    int add_route(const PSPRoute&, bool, PSPRoute*, ErrorHandler *);
    int remove_route(const PSPRoute&, PSPRoute*, ErrorHandler *);  
    int lookup_route(PSPAddress) const;
   // String dump_routes();

    //bool check() const;

  protected:

    Vector<PSPRoute> _t;
    int _zero_route;

    PSPAddress _last_addr;
    int _last_entry;
	
#ifdef IP_RT_CACHE2
    IPAddress _last_addr2;
    int _last_entry2;
#endif

    int lookup_entry(PSPAddress) const;

};

CLICK_ENDDECLS
#endif
