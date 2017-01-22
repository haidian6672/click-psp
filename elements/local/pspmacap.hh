#ifndef PSPMACAP_HH
#define PSPMACAP_HH

/*
 *  PSPMacap( 00:0C:29:F6:ED:69 )
 *    -> ToDevice;
 */

#include <click/element.hh>
#include <click/glue.hh>
#include <click/gaprate.hh>
#include <click/packet.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>

class PSPMacap : public Element {

  //unsigned _len;
  click_ether _ethh;
  Packet *_packet;

 public:

  PSPMacap();
  ~PSPMacap();

  const char *class_name() const	{ return "PSPMacap"; }
  const char *port_count() const	{ return PORTS_1_1; }
  const char *processing() const	{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  //void cleanup(CleanupStage);
  void push(int port, Packet *p);

  //void add_handlers();
  
};

#endif
