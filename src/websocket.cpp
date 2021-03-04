#include <ntwk/websocket.hpp>

#include <exception>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/multi_buffer.hpp>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/websocket/rfc6455.hpp>  // websocket::close_code
#include <boost/beast/websocket/ssl.hpp>
#include <boost/system/system_error.hpp>

#include <ntwk/detail/io_context.hpp>
#include <ntwk/detail/ssl.hpp>
#include <ntwk/error.hpp>

namespace {
using namespace ntwk;

using Socket_t = boost::beast::websocket::stream<detail::SSL_socket_t>;

/// Connect the lowest layer socket to \p host, \p port.
/** throws Error if fails. */
void make_connection(Socket_t& socket,
                     std::string const& host,
                     std::string const& port)
{
    try {
        auto resolver = boost::asio::ip::tcp::resolver{detail::io_context()};
        auto const result = resolver.resolve(host, port);
        boost::asio::connect(socket.next_layer().next_layer(),
                             std::begin(result), std::end(result));
    }
    catch (std::exception const& e) {
        throw Error{"websocket.cpp anon::make_connection()" +
                    std::string{e.what()}};
    }
}

/// Perform websocket handshake.
/** throws Error if fails. */
void ws_handshake(Socket_t& socket,
                  std::string const& host,
                  std::string const& URI)
{
    try {
        socket.handshake(host, URI);
    }
    catch (std::exception const& e) {
        throw Error{"websocket.cpp anon::ws_handshake()" +
                    std::string{e.what()}};
    }
}
}  // namespace

namespace ntwk {

void Websocket::connect(std::string const& host,
                        std::string const& URI,
                        std::string const& port)
{
    detail::set_hostname(socket_->next_layer(), host);
    make_connection(*socket_, host, port);
    detail::handshake(socket_->next_layer());
    ws_handshake(*socket_, host, URI);
    connected_ = true;
}

void Websocket::disconnect()
{
    try {
        socket_->close(boost::beast::websocket::close_code::normal);
    }
    catch (boost::system::system_error const&) {
        // Expected to throw, for some reason this is always going to happen.
    }
    // Replace socket with new socket. Clean disconnect is impossible.
    socket_    = std::make_unique<Socket_t>(detail::io_context(), ssl_ctx_);
    connected_ = false;
}

void Websocket::write(std::string const& request)
{
    try {
        socket_->write(boost::asio::buffer(request));
    }
    catch (std::exception const& e) {
        throw Error{"Websocket::write()" + std::string{e.what()}};
    }
}

auto Websocket::read() -> std::string
{
    try {
        auto buf = boost::beast::multi_buffer{};
        socket_->read(buf);
        auto oss = std::ostringstream{};
        oss << boost::beast::buffers_to_string(buf.data());
        return oss.str();
    }
    catch (std::exception const& e) {
        throw Error{"Websocket::read()" + std::string{e.what()}};
    }
}

}  // namespace ntwk
