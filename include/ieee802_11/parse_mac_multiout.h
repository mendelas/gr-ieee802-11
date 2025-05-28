
#ifndef INCLUDED_IEEE802_11_PARSE_MAC_MULTIOUT_H
#define INCLUDED_IEEE802_11_PARSE_MAC_MULTIOUT_H

#include <gnuradio/block.h>
#include <ieee802_11/api.h>

namespace gr {
namespace ieee802_11 {

class IEEE802_11_API parse_mac_multiout : virtual public block
{
public:
    typedef std::shared_ptr<parse_mac_multiout> sptr;
    static sptr make(bool log = false, bool debug = false);
};

} // namespace ieee802_11
} // namespace gr

#endif /* INCLUDED_IEEE802_11_PARSE_MAC_MULTIOUT_H */
