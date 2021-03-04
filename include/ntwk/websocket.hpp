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
    /** throws Error if fails. */
    void connect(std::string const& host,
                 std::string const& URI  = "/",
                 std::string const& port = "443");

    /// Safely disconnect the socket.
    /** throws Error if fails. */
    void disconnect();

    /// Send \p request to the socket.
    /** throws Error if fails. */
    void write(std::string const& request);

    /// Perform one read from the socket, returning the read bytes.
    /** throws Error if fails. */
    [[nodiscard]] auto read() -> std::string;

    /// Return true if connect has been called and disconnect has not.
    [[nodiscard]] auto is_connected() const -> bool { return connected_; }

   private:
    detail::Context_t ssl_ctx_ = detail::make_context();

    using Socket_t = boost::beast::websocket::stream<detail::SSL_socket_t>;
    std::unique_ptr<Socket_t> socket_ =
        std::make_unique<Socket_t>(detail::io_context(), ssl_ctx_);

    bool connected_ = false;
};

}  // namespace ntwk
#endif  // NTWK_WEBSOCKET_HPP
