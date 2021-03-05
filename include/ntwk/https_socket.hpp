#ifndef NTWK_HTTPS_SOCKET_HPP
#define NTWK_HTTPS_SOCKET_HPP
#include <memory>
#include <string>

#include <boost/beast/core/flat_buffer.hpp>

#include <ntwk/detail/ssl.hpp>

namespace ntwk {

/// Connect to endpoint, send GET requests. Hardcoded port 443.
class HTTPS_socket {
   public:
    /// HTTP Response, includes return code and string contents of body.
    struct Response {
        unsigned code;
        std::string body;
    };

   public:
    /// Make Connection to \p host. \p host should have 'www.' prefix.
    /** Throws ntwk::Error if fails. */
    void connect(std::string const& host);

    /// Disconnect from endpoint. No-op if already disconnected.
    /** Throws ntwk::Error if fails. */
    void disconnect();

    /// Reconnects to host given in last connect() call.
    /** Throws ntwk::Error if fails. */
    void reconnect();

    /// Return true if the connect() has been called, without disconnect.
    /** Does not know if the connection is actually still good. */
    [[nodiscard]] auto is_connected() const -> bool
    {
        return socket_ != nullptr;
    }

   public:
    /// Send GET HTTP message to \p resource at endpoint. Return body.
    /** throws ntwk::Error if fails. Auto reconnects up to three times if fails
     *  to send. */
    auto get(std::string const& resource) -> Response;

   private:
    detail::SSL_context_t ssl_ctx_{detail::make_ssl_context()};

    std::unique_ptr<detail::SSL_socket_t> socket_{nullptr};

    boost::beast::flat_buffer buffer_;
    std::string host_;
};

}  // namespace ntwk
#endif  // NTWK_HTTPS_SOCKET_HPP
