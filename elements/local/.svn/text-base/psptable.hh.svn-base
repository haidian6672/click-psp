
#ifndef CLICK_PSPTABLE_HH
#define CLICK_PSPTABLE_HH
#include <click/glue.hh>
#include <click/vector.hh>
#include <clicknet/psp.h>
CLICK_DECLS

// PSP routing table.
// Lookup by exact match.
// Each entry contains a gateway and an output index.

class PSPTable { public:

  PSPTable();
  ~PSPTable();

  //bool lookup(IPAddress dst, IPAddress &gw, int &index) const;

  //void add(IPAddress dst, IPAddress mask, IPAddress gw, int index);
  //void del(IPAddress dst, IPAddress mask);
  //void clear()				{ _v.clear(); }

  /* modified by ricky */
    bool lookup(click_psp dst, int &index) const;
    void add(click_psp dst,  int index);
    void del(click_psp dst);
    void clear()				{ _v.clear(); }

 private:

  struct Entry {
    click_psp dst;
    //click_psp mask;
    //click_psp gw;
    int index;
    //bool valid() const			{ return mask || !dst; }
  };
  Vector<Entry> _v;

};

CLICK_ENDDECLS
#endif
