/*
 * psptable.{cc,hh} -- a stupid PSP routing table, best for small routing tables
 * Ricky Huang
 *
 */

#include <click/config.h>
#include <click/psptable.hh>
CLICK_DECLS

PSPTable::PSPTable()
{
}

PSPTable::~PSPTable()
{
}

bool
PSPTable::lookup(click_psp dst,  int &index) const
{
  int best = -1;

  // exactlly match
 // for (int i = 0; i < _v.size(); i++)
 //   if (dst.matches_prefix(_v[i].dst, _v[i].mask)) {
 //     if (best < 0 || _v[i].mask.mask_as_specific(_v[best].mask))
//	best = i;
//    }
  for (int i = 0; i < _v.size(); i++){
        click_chatter("This route is %d\n", _v[i].dst);
 	click_chatter("The route to match is: %d\n", dst);
        if(_v[i].dst == dst){
		best = i;
		click_chatter("Find a route\n");
        }

  }

  if (best < 0)
    return false;
  return true;
}


void
PSPTable::add(click_psp dst, int index)
{

  struct Entry e;
  e.dst = dst;
  //e.mask = mask;
 // e.gw = gw;
  e.index = index;

  _v.push_back(e);
}

void
PSPTable::del(click_psp dst)
{
  for (int i = 0; i < _v.size(); i++)
    if (_v[i].dst == dst ) {
      //_v[i].dst = 1;
      
    }
}

// must always generate the whole template instance! LookupIPRoute demands it
template class Vector<PSPTable::Entry>;
CLICK_ENDDECLS
