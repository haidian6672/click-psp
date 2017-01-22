#ifndef CLICK_PSPADDRESS_HH
#define CLICK_PSPADDRESS_HH
#include <click/string.hh>
#include <click/glue.hh>
#include <clicknet/psp.h>
#include <click/args.hh>
CLICK_DECLS
class StringAccum;
class PSPAddressArg;
template <typename T> class Vector;

class PSPAddress { public:

    /** @brief Construct an PSPAddress equal to 0.0.0.0. */
    inline PSPAddress()
	: _addr(0) {
    }

    /** @brief Construct an PSPAddress from an integer in network byte order. */
   inline PSPAddress(uint64_t  x)
	: _addr(x) {
    }
   
    
    PSPAddress(const unsigned char *data) {
	memcpy(&_addr, data, 8);
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

    inline const unsigned char* data() const;
    inline unsigned char* print() const;
    //inline const unsigned char* data() const;
    inline uint32_t hashcode() const;
    inline bool matches_prefix(PSPAddress addr) const;

    String unparse() const;
    String unparse_mask() const;
    String unparse_with_mask(PSPAddress) const;

    inline PSPAddress& operator&=(PSPAddress);
    inline PSPAddress& operator=(uint64_t);

  private:

    uint64_t _addr;

};


inline bool
PSPAddress::matches_prefix(PSPAddress addr) const
{
    return ((this->addr() ^ addr.addr()) == 0);
}


/** @relates PSPAddress
    @brief Compare two PSPAddress objects for equality. */
inline bool
operator==(PSPAddress a, PSPAddress b)
{
    return a.addr() == b.addr();
}

inline PSPAddress&
PSPAddress::operator&=(PSPAddress a)
{
    _addr &= a._addr;
    return *this;
}

inline PSPAddress&
PSPAddress::operator=(uint64_t a)
{
    _addr = a;
    return *this;
}

/** @relates PSPAddress
    @brief Compare an PSPAddress with a network-byte-order address value for
    equality.
    @param a an address
    @param b an address value in network byte order */
inline bool
operator==(PSPAddress a, uint64_t b)
{
    return a.addr() == b;
}

/** @relates PSPAddress
    @brief Compare two PSPAddress objects for inequality. */
inline bool
operator!=(PSPAddress a, PSPAddress b)
{
    return a.addr() != b.addr();
}

/** @relates PSPAddress
    @brief Compare an PSPAddress with a network-byte-order address value for
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
inline const unsigned char*
PSPAddress::data() const
{
    return reinterpret_cast<const unsigned char*>(&_addr);
}

/** @brief Hash function.
 * @return The hash value of this PSPAddress.
 */
inline uint32_t
PSPAddress::hashcode() const
{
    char left[4];
    char right[4];
    uint32_t ret;
    memcpy(left, &_addr, 4);
    memcpy(right, ((char*)&_addr)+4, 4);
    int i;
    for (i=0; i != 4; i++)
        left[i] = left[i] ^ right[i];
    memcpy(&ret, left, 4);
    return ret;
}


StringAccum& operator<<(StringAccum&, PSPAddress);

/** @class PSPAddressArg
  @brief Parser class for PSP addresses. */
class PSPAddressArg { public:
    static const char *basic_parse(const char *begin, const char *end,
				   unsigned char value[8], int &nbytes);
    static bool parse(const String &str, PSPAddress &result,
		      const ArgContext &args);
//     static bool parse(const String &str, uint64_t &result,
// 		      const ArgContext &args = blank_args) {
// 	return parse(str, reinterpret_cast<PSPAddress &>(result), args);
//     }
//     static bool parse(const String &str, Vector<PSPAddress> &result,
// 		      const ArgContext &args = blank_args);

};

/** @class PSPPrefixArg
  @brief Parser class for PSP prefixes. */
class PSPPrefixArg { public:
    PSPPrefixArg(bool allow_bare_address_ = true)
	: allow_bare_address(allow_bare_address_) {
    }
    bool parse(const String &str,
	       PSPAddress &result_addr, PSPAddress &result_mask,
	       const ArgContext &args = blank_args) const;
//    bool parse(const String &str,
//	       struct in_addr &result_addr, struct in_addr &result_mask,
//	       const ArgContext &args = blank_args) const {
//	return parse(str, reinterpret_cast<PSPAddress &>(result_addr),
//		     reinterpret_cast<PSPAddress &>(result_mask), args);
//    }
    bool allow_bare_address;
};



template<> struct DefaultArg<PSPAddress> : public PSPAddressArg {};
// template<> struct DefaultArg<uint64_t> : public PSPAddressArg {};
template<> struct DefaultArg<Vector<PSPAddress> > : public PSPAddressArg {};


CLICK_ENDDECLS
#endif
