
#ifndef CLICK_PSPROUTETABLE_HH
#define CLICK_PSPROUTETABLE_HH
#include <click/glue.hh>
#include <click/element.hh>
#include <click/vector.hh>
#include <clicknet/psp.h>
#include <click/pspaddress.hh>
CLICK_DECLS

// PSP routing table.
// Lookup by exact match.
// Each entry contains a gateway and an output index.

struct PSPRoute {
    PSPAddress addr;
    int32_t port;
   
    PSPRoute()			: port(-1) { }
    PSPRoute(PSPAddress a, int p)
				: addr(a),  port(p) { }

    inline bool real() const	{ return port > (int32_t) -0x80000000; }
    inline void kill()		{ addr = 0; port = -0x80000000; }
    inline bool contains(PSPAddress a) const;
    //inline bool contains(const click_psp& r) const;

    inline bool match(const PSPRoute& r) const;

    /*
    StringAccum &unparse(StringAccum&, bool tabs) const;
    String unparse() const;
    String unparse_addr() const	{ return addr.unparse_with_mask(mask); }
    */
};

class PSPRouteTable: public Element { public:

    //void* cast(const char*);
    int configure(Vector<String>&, ErrorHandler*);
    void add_handlers();

    virtual int add_route(const PSPRoute& route, bool allow_replace, PSPRoute* replaced_route, ErrorHandler* errh);
    virtual int remove_route(const PSPRoute& route, PSPRoute* removed_route, ErrorHandler* errh);
    virtual int lookup_route(PSPAddress addr) const = 0;
    //virtual String dump_routes();

    void push(int port, Packet* p);

    static int add_route_handler(const String&, Element*, void*, ErrorHandler*);
    static int remove_route_handler(const String&, Element*, void*, ErrorHandler*);

  private:

    //enum { CMD_ADD, CMD_SET, CMD_REMOVE };
    //int run_command(int command, const String &, Vector<PSPRoute>* old_routes, ErrorHandler*);
  };

inline bool
PSPRoute::contains(PSPAddress a) const
{
    return a.matches_prefix(addr);
}

inline bool
PSPRoute::match(const PSPRoute& r) const
{
    return addr == r.addr &&  port == r.port;
}


CLICK_ENDDECLS
#endif
