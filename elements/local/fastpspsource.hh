#ifndef FASTPSPSOURCE_HH
#define FASTPSPSOURCE_HH

/*
 * =c
 * FASTPSPSOURCE(RATE, LIMIT, LENGTH, SRCETH, SRCIP, SPORT, DSTETH, DSTIP, DPORT [, CHECKSUM, INTERVAL, ACTIVE])
 * =s udp
 * creates packets with static UDP/IP/Ethernet headers
 * =d
 * FastPSPSource is a benchmark tool. At initialization
 * time, FastPSPSource creates a UDP/IP packet of length
 * LENGTH (min 60), with source ethernet address SRCETH,
 * source IP address SRCIP, source port SPORT,
 * destination ethernet address DSTETH, destination IP
 * address DSTIP, and destination port DPORT. The UDP
 * checksum is calculated if CHECKSUM is true; it is
 * true by default. Each time the FastPSPSource element
 * is called, it increments the reference count on the
 * skbuff created and returns the skbuff object w/o
 * copying or cloning. Therefore, the packet returned by
 * FastPSPSource should not be modified.
 *
 * FastPSPSource sents packets at RATE packets per
 * second. It will send LIMIT number of packets in
 * total.
 *
 * After FastPSPSource has sent LIMIT packets, it will
 * calculate the average send rate (packets per second)
 * between the first and last packets sent and make that
 * available in the rate handler.
 *
 * By default FastPSPSource is ACTIVE.
 *
 * INTERVAL is zero by default. If it is not 0, after
 * INTERVAL number of packets, both sport and dport will
 * be incremented by 1. Checksum will be recomputed.
 *
 * =h count read-only
 * Returns the total number of packets that have been generated.
 * =h rate read/write
 * Returns or sets the RATE parameter.
 * =h reset write
 * Reset and restart.
 * =h active write
 * Change ACTIVE
 *
 * =e
 *  FastPSPSource(100000, 500000, 60, 0:0:0:0:0:0, 1.0.0.1, 1234,
 *                                    1:1:1:1:1:1, 2.0.0.2, 1234)
 *    -> ToDevice;
 */

#include <click/element.hh>
#include <click/glue.hh>
#include <click/gaprate.hh>
#include <click/packet.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>

class FastPSPSource : public Element {

  bool _rate_limited; // obey _rate? rather than as fast as possible.
  unsigned _len;
  click_ether _ethh;
  uint64_t _sipaddr;
  uint64_t _dipaddr;
  uint16_t _sport;
  uint16_t _dport;
  unsigned short _incr;
  unsigned int _interval;
  Packet *_packet;
  click_jiffies_t _first;
  click_jiffies_t _last;
  click_psp *_psphdr;
  void incr_ports();

 public:

  static const unsigned NO_LIMIT = 0xFFFFFFFFU;

  GapRate _rate;
  unsigned _count;
  unsigned _limit;
  bool _active;

  FastPSPSource();
  ~FastPSPSource();

  const char *class_name() const	{ return "FastPSPSource"; }
  const char *port_count() const	{ return PORTS_0_1; }
  const char *processing() const	{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void cleanup(CleanupStage);
  Packet *push(int);

  void add_handlers();
  void reset();
  unsigned count() { return _count; }
  click_jiffies_t first() { return _first; }
  click_jiffies_t last() { return _last; }

};

#endif
