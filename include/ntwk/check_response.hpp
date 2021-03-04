#ifndef NTWK_CHECK_RESPONSE_HPP
#define NTWK_CHECK_RESPONSE_HPP
#include <string>
#include <utility>

#include <ntwk/error.hpp>
#include <ntwk/https_socket.hpp>

namespace ntwk {

/// Throws error if code != 200, includes \p about in thrown error.what().
void inline check_response(HTTPS_socket::Response const& message,
                           std::string const& about)
{
    if (message.code == 200)
        return;
    auto what = "code: " + std::to_string(message.code) + '\n' + about + ": " +
                message.body;
    throw Error{std::move(what)};
}

}  // namespace ntwk
#endif  // NTWK_CHECK_RESPONSE_HPP
