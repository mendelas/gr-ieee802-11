#include "utils.h"
#include <ieee802_11/parse_mac_multiout.h>

#include <gnuradio/block_detail.h>
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

using namespace gr::ieee802_11;

class parse_mac_multiout_impl : public parse_mac_multiout
{

public:
    parse_mac_multiout_impl(bool log,
                            bool debug,
                            const std::string& mac_1,
                            const std::string& mac_2)
        : block("parse_mac_multiout",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
          d_log(log),
          d_last_seq_no(-1),
          d_debug(debug),
          d_mac_1(mac_1),
          d_mac_2(mac_2)
    {
        message_port_register_in(pmt::mp("in"));
        set_msg_handler(pmt::mp("in"),
                        boost::bind(&parse_mac_multiout_impl::classify_by_mac,
                                    this,
                                    boost::placeholders::_1));

        message_port_register_out(pmt::mp("mac_1"));
        message_port_register_out(pmt::mp("mac_2"));
        message_port_register_out(pmt::mp("mac_other"));
    }

    ~parse_mac_multiout_impl() {}

    void classify_by_mac(pmt::pmt_t pdu)
    {
        if (pmt::is_eof_object(pdu)) {
            detail().get()->set_done(true);
            return;
        } else if (pmt::is_symbol(pdu)) {
            return;
        }

        d_meta = pmt::car(pdu);
        d_msg = pmt::cdr(pdu);

        int frame_len = pmt::blob_length(d_msg); // check frame length
        const uint8_t* header_data =
            reinterpret_cast<const uint8_t*>(pmt::blob_data(d_msg));

        dout << std::endl << "new mac frame  (length " << frame_len << ")" << std::endl;
        dout << "=========================================" << std::endl;

        if (frame_len < 20) {
            dout << "frame too short to parse (<20)" << std::endl;
            return;
        }

        const uint8_t* mac_addr = header_data + 10; // 10 is offset
        std::string src_mac = format_mac_address(mac_addr);
        // debug
        if (d_debug) {
            std::cout << "[DEBUG] src_mac: " << src_mac << std::endl;
            return;
        }
        pmt::pmt_t out_pdu = pmt::cons(d_meta, d_msg);

        dout << "Comparing MACs:" << std::endl;
        dout << "  d_mac_1     = [" << d_mac_1 << "]" << std::endl;
        dout << "  d_mac_2     = [" << d_mac_2 << "]" << std::endl;

        if (src_mac == d_mac_1) {
            message_port_pub(pmt::mp("mac_1"), out_pdu);
        } else if (src_mac == d_mac_2) {
            message_port_pub(pmt::mp("mac_2"), out_pdu);
        } else {
            message_port_pub(pmt::mp("mac_other"), out_pdu);
        }
    }


    std::string format_mac_address(const uint8_t* addr)
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');

        for (int i = 0; i < 6; ++i) {
            if (i > 0)
                oss << ":";
            oss << std::setw(2) << static_cast<int>(addr[i]);
        }

        return oss.str();
    }

    void print_ascii(char* buf, int length)
    {

        for (int i = 0; i < length; i++) {
            if ((buf[i] > 31) && (buf[i] < 127)) {
                dout << buf[i];
            } else {
                dout << ".";
            }
        }
        dout << std::endl;
    }

private:
    bool d_log;
    bool d_debug;
    std::string d_mac_1;
    std::string d_mac_2;
    int d_last_seq_no;
    pmt::pmt_t d_meta;
    pmt::pmt_t d_msg;
};

// parse_mac::sptr parse_mac::make(bool log, bool debug)
// {
//     return gnuradio::get_initial_sptr(new parse_mac_multiout_impl(log, debug));
// }

parse_mac_multiout::sptr parse_mac_multiout::make(bool log,
                                                  bool debug,
                                                  const std::string& mac_1,
                                                  const std::string& mac_2)
{
    return gnuradio::get_initial_sptr(
        new parse_mac_multiout_impl(log, debug, mac_1, mac_2));
}