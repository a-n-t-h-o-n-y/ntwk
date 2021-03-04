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
    ~HTTPS_socket() { this->disconnect(); }

   public:
    /// Make Connection to \p host. \p host should have 'www.' prefix.
    /** Throws Error if fails. */
    void connect(std::string const& host);

    /// Disconnect from endpoint. No-op if already disconnected.
    /** Throws Error if fails. */
    void disconnect();

    /// Reconnects to host given in last connect() call.
    /** Throws Error if fails. */
    void reconnect();

    /// HTTP Response, includes return code and string contents of body.
    struct Response {
        unsigned code;
        std::string body;
    };

    /// Send GET HTTP message to \p resource at endpoint. Return body.
    auto get(std::string const& resource) -> Response;

    /// Return true if the connect() has been called, without disconnect.
    /** Does not know if the connection is actually still good. */
    [[nodiscard]] auto is_connected() const -> bool
    {
        return socket_ != nullptr;
    }

   private:
    detail::Context_t ssl_ctx_{detail::make_context()};

    std::unique_ptr<detail::SSL_socket_t> socket_{nullptr};

    boost::beast::flat_buffer buffer_;
    std::string host_;
};

}  // namespace ntwk
#endif  // NTWK_HTTPS_SOCKET_HPP
