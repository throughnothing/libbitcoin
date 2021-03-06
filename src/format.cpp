#include <bitcoin/format.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <bitcoin/primitives.hpp>
#include <bitcoin/utility/assert.hpp>

namespace libbitcoin {

std::ostream& operator<<(std::ostream& stream, const data_chunk& data)
{
    stream << encode_hex(data);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const hash_digest& hash)
{
    stream << encode_hex(hash);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const short_hash& hash)
{
    stream << encode_hex(hash);
    return stream;
}

template <typename Point>
std::ostream& concat_point(std::ostream& stream, const Point& point)
{
    stream << point.hash << ":" << point.index;
    return stream;
}
std::ostream& operator<<(std::ostream& stream, const output_point& point)
{
    return concat_point(stream, point);
}

data_chunk decode_hex(std::string hex_str)
{
    // Trim the fat.
    boost::algorithm::trim(hex_str);
    data_chunk result(hex_str.size() / 2);
    for (size_t i = 0; i + 1 < hex_str.size(); i += 2)
    {
        BITCOIN_ASSERT(hex_str.size() - i >= 2);
        auto byte_begin = hex_str.begin() + i;
        auto byte_end = hex_str.begin() + i + 2;
        // Perform conversion.
        int val = -1;
        std::stringstream converter;
        converter << std::hex << std::string(byte_begin, byte_end);
        converter >> val;
        if (val == -1)
            return data_chunk();
        BITCOIN_ASSERT(val <= 0xff);
        // Set byte.
        result[i / 2] = val;
    }
    return result;
}

std::string satoshi_to_btc(uint64_t value)
{
    uint64_t major = value / coin_price(1);
    std::string result = boost::lexical_cast<std::string>(major);
    BITCOIN_ASSERT(value >= major);
    uint64_t minor = value - (major * coin_price(1));
    BITCOIN_ASSERT(minor < coin_price(1));
    if (minor > 0)
    {
        std::string minor_str = boost::lexical_cast<std::string>(minor);
        std::string padded_minor(8 - minor_str.size(), '0');
        padded_minor += minor_str;
        boost::algorithm::trim_right_if(padded_minor,
            boost::is_any_of("0"));
        result = result + "." + padded_minor;
    }
    return result;
}

} // namespace libbitcoin

