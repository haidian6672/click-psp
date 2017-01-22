#ifndef CLICK_PSPADDRESS_HH
#define CLICK_PSPADDRESS_HH
#include <click/string.hh>
#include <click/glue.hh>
#include <clicknet/psp.h>
CLICK_DECLS
class StringAccum;
template <typename T> class Vector;

class PSPAddress { public:

    /** @brief Construct an IPAddress equal to 0.0.0.0. */
    inline PSPAddress()
	: _addr(0) {
    }

    /** @brief Construct an IPAddress from an integer in network byte order. */
   inline PSPAddress(uint64_t  x)
	: _addr(x) {
    }
   
    
    explicit PSPAddress(const unsigned char *data) {
#if HAVE_INDIFFERENT_ALIGNMENT
	_addr = *(reinterpret_cast<const unsigned *>(data));
#else
	memcpy(&_addr, data, 8);
#endif
    }


    /** @brief Test if the address is 0.0.0.0. */
    inline bool empty() const {
	return !_addr;
    }

    /** @brief Return the address as a uint32_t in network byte order. */
    inline uint64_t addr() const {
	return _addr;
    }


    inline operator uint64_t() const {
	return _addr;
    }

    inline unsigned char* data();
    //inline const unsigned char* data() const;
   inline bool matches_prefix(PSPAddress addr) const;

  private:

    uint64_t _addr;

};


inline bool
PSPAddress::matches_prefix(PSPAddress addr) const
{
    return ((this->addr() ^ addr.addr()) == 0);
}


/** @relates IPAddress
    @brief Compare two IPAddress objects for equality. */
inline bool
operator==(PSPAddress a, PSPAddress b)
{
    return a.addr() == b.addr();
}

/** @relates IPAddress
    @brief Compare an IPAddress with a network-byte-order address value for
    equality.
    @param a an address
    @param b an address value in network byte order */
inline bool
operator==(PSPAddress a, uint64_t b)
{
    return a.addr() == b;
}

/** @relates IPAddress
    @brief Compare two IPAddress objects for inequality. */
inline bool
operator!=(PSPAddress a, PSPAddress b)
{
    return a.addr() != b.addr();
}

/** @relates IPAddress
    @brief Compare an IPAddress with a network-byte-order address value for
    inequality.
    @param a an address
    @param b an address value in network byte order */
inline bool
operator!=(PSPAddress a, uint64_t b)
{
    return a.addr() != b;
}

inline PSPAddress
operator&(PSPAddress a, PSPAddress b)
{
    return PSPAddress(a.addr() & b.addr());
}

inline PSPAddress
operator^(PSPAddress a, PSPAddress b)
{
    return PSPAddress(a.addr() ^ b.addr());
}

/** @brief Return a pointer to the address data.

    Since the address is stored in network byte order, data()[0] is the top 8
    bits of the address, data()[1] the next 8 bits, and so forth. */


/** @brief Return a pointer to the address data.

    Since the address is stored in network byte order, data()[0] is the top 8
    bits of the address, data()[1] the next 8 bits, and so forth. */
inline unsigned char*
PSPAddress::data()
{
    return reinterpret_cast<unsigned char*>(&_addr);
}


StringAccum& operator<<(StringAccum&, PSPAddress);


CLICK_ENDDECLS
#endif
