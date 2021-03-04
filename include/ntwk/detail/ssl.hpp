#ifndef NTWK_DETAIL_SSL_HPP
#define NTWK_DETAIL_SSL_HPP
#include <exception>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/context_base.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/stream_base.hpp>
#include <boost/asio/ssl/verify_mode.hpp>

#include "../error.hpp"

namespace ntwk::detail {

using Context_t    = boost::asio::ssl::context;
using SSL_socket_t = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

inline auto set_hostname(SSL_socket_t& socket, std::string const& host) -> bool
{
    return SSL_set_tlsext_host_name(socket.native_handle(), host.c_str());
}

/// Create a new SSL_context object, with verification method set.
inline auto make_context() -> Context_t
{
    auto context = Context_t{Context_t::sslv23_client};
    context.set_options(Context_t::default_workarounds | Context_t::no_sslv3);
    context.set_verify_mode(boost::asio::ssl::verify_peer);
    context.set_default_verify_paths();
    return context;
}

/// Perform SSL handshake as client.
/** throws Crab_error on failure. */
inline void handshake(SSL_socket_t& socket)
{
    try {
        socket.handshake(boost::asio::ssl::stream_base::client);
    }
    catch (std::exception const& e) {
        throw Crab_error{std::string{"network::ssl::handshake: "} + e.what()};
    }
}

}  // namespace ntwk::detail
#endif  // NTWK_DETAIL_SSL_HPP