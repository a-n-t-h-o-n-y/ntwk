#ifndef NTWK_CHECK_RESPONSE_HPP
#define NTWK_CHECK_RESPONSE_HPP
#include <string>
#include <utility>

#include <ntwk/error.hpp>
#include <ntwk/https_socket.hpp>

namespace ntwk {

/// Throws ntwk::Error if code != 200, includes \p about in thrown error.what().
void inline check_response(HTTPS_socket::Response const& message,
                           std::string const& about)
{
    if (message.code == 200)
        return;
    throw Error{"HTTPS Response Error; Response Status Code: " +
                std::to_string(message.code) + '\n' + about +
                "\nResponse Body: " + message.body};
}

}  // namespace ntwk
#endif  // NTWK_CHECK_RESPONSE_HPP
