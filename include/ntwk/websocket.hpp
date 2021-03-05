#ifndef NTWK_WEBSOCKET_HPP
#define NTWK_WEBSOCKET_HPP
#include <memory>
#include <string>

#include <boost/beast/websocket/stream.hpp>

#include <ntwk/detail/io_context.hpp>
#include <ntwk/detail/ssl.hpp>

namespace ntwk {

/// Secure websocket.
class Websocket {
   public:
    /// Perform connect, ssl handshake and websocket handshake.
    /** throws ntwk::Error if fails. */
    void connect(std::string const& host,
                 std::string const& URI  = "/",
                 std::string const& port = "443");

    /// Safely disconnect the socket.
    /** throws ntwk::Error if fails. */
    void disconnect();

    /// Reconnect to the previously connected endpoint.
    void reconnect();

    /// Return true if connect has been called and disconnect has not.
    [[nodiscard]] auto is_connected() const -> bool { return connected_; }

   public:
    /// Send \p request to the socket.
    /** throws ntwk::Error if fails. Auto Reconnects if websocket has closed. */
    void write(std::string const& request);

    /// Perform one read from the socket, returning the read bytes.
    /** throws ntwk::Error if fails. Auto Reconnects if websocket has closed. */
    [[nodiscard]] auto read() -> std::string;

   private:
    detail::SSL_context_t ssl_ctx_ = detail::make_ssl_context();

    using Socket_t = boost::beast::websocket::stream<detail::SSL_socket_t>;
    std::unique_ptr<Socket_t> socket_ =
        std::make_unique<Socket_t>(detail::io_context(), ssl_ctx_);

    std::string host_;
    std::string uri_;
    std::string port_;
    bool connected_ = false;
};

}  // namespace ntwk
#endif  // NTWK_WEBSOCKET_HPP
