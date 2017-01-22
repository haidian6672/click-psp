#ifndef CLICK_ARPRESPONDERABC_HH
#define CLICK_ARPRESPONDERABC_HH
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <click/pspaddress.hh>
#include <click/vector.hh>
CLICK_DECLS

/*
 * =c
 *
 * ARPResponderABC(IP/MASK [IP/MASK...] ETH, IP2/MASK2 ETH2, ...)
 *
 * =s arp
 *
 * generates responses to ARP queries
 *
 * =d
 *
 * Input should be ARP request packets, including the Ethernet header.
 * Forwards an ARP reply if we know the answer -- that is, if one of the
 * IPPREFIX arguments matches the requested IP address, then it outputs an ARP
 * reply giving the corresponding ETH address, otherwise the ARP request
 * packet is pushed out of output 1 (if it exists). Could be used for proxy
 * ARP as well as producing replies for a host's own address.
 *
 * The IP/MASK arguments are IP network addresses (IP address/netmask pairs).
 * The netmask can be specified in CIDR form (`C<18.26.7.0/24>') or dotted
 * decimal form (`C<18.26.7.0/255.255.255.0>').
 *
 * ARPResponderABC sets the device and VLAN TCI annotations on generated ARP
 * responses to the corresponding annotations from the queries.
 *
 * =n
 *
 * AddressInfo elements can simplify the arguments to ARPResponderABC. In
 * particular, if C<NAME> is shorthand for both an IP network address (or IP
 * address) C<IP> and an Ethernet address C<ETH>, then C<ARPResponderABC(NAME)> is
 * equivalent to C<ARPResponderABC(IP ETH)>. If C<NAME> is short for both an IP
 * address and an IP network address, then ARPResponderABC will prefer the IP
 * address. (You can say C<NAME:ipnet> to use the IP network address.)
 *
 * =h table r
 *
 * Return the ARPResponderABC's current table, with one IP network entry per
 * line.
 *
 * =h lookup "read with parameters"
 *
 * Takes an IP address as a parameter and returns the corresponding Ethernet
 * address, if any.
 *
 * =h add w
 *
 * Add new entries.  Takes a string in "IP/MASK [IP/MASK...] ETH" form.
 *
 * =h remove w
 *
 * Delete a single entry.  Takes a string in "IP/MASK" form; deletes any
 * entry with the same IP and MASK.
 *
 * =e
 *
 * Produce ARP replies for the local machine (18.26.4.24)
 * as well as proxy ARP for all machines on net 18.26.7
 * directing their packets to the local machine:
 *
 *   c :: Classifier(12/0806 20/0001, ...);
 *   ar :: ARPResponderABC(18.26.4.24 18.26.7.0/24 00-00-C0-AE-67-EF);
 *   c[0] -> ar;
 *   ar -> ToDevice(eth0);
 *
 * =a
 *
 * ARPQuerier, ARPFaker, AddressInfo */

class ARPResponderABC : public Element { public:

    ARPResponderABC();
    ~ARPResponderABC();

    const char *class_name() const		{ return "ARPResponderABC"; }
    const char *port_count() const		{ return PORTS_1_1X2; }
    const char *processing() const		{ return PROCESSING_A_AH; }

    int configure(Vector<String> &, ErrorHandler *);
    bool can_live_reconfigure() const		{ return true; }
    void add_handlers();

    Packet *simple_action(Packet *);

    static Packet *make_response(const uint8_t target_eth[6],
				 const uint8_t target_ip_abc[8],
				 const uint8_t src_eth[6],
				 const uint8_t src_ip_abc[8],
				 const Packet *p = 0);

    const EtherAddress *lookup(PSPAddress addr) const {
        printf("lookup(%s).\n", addr.unparse().data());	
        Vector<Entry>::const_iterator end = _v.end();
	for (Vector<Entry>::const_iterator it = _v.begin(); it != end; ++it)
	    if ((addr & it->mask) == it->dst)
            {
                printf("hit.\n");
		return &it->ena;
            }
	return 0;
    }

    bool lookup(PSPAddress addr, EtherAddress &result) const {
	if (const EtherAddress *x = lookup(addr)) {
	    result = *x;
	    return true;
	} else
	    return false;
    }

  private:

    struct Entry {
	PSPAddress dst;
	PSPAddress mask;
	EtherAddress ena;
    };
    Vector<Entry> _v;

    static int entry_compare(const void *a, const void *b, void *user_data);
    int add(Vector<Entry> &v, const String &arg, ErrorHandler *errh) const;
    static void normalize(Vector<Entry> &v, bool warn, ErrorHandler *errh);

    static String read_handler(Element *, void *);
    static int lookup_handler(int op, String &str, Element *e, const Handler *h, ErrorHandler *errh);
    static int add_handler(const String &s, Element *e, void *, ErrorHandler *errh);
    static int remove_handler(const String &s, Element *e, void *, ErrorHandler *errh);

};

CLICK_ENDDECLS
#endif
