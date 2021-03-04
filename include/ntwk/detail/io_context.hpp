#ifndef NTWK_DETAIL_IO_CONTEXT_HPP
#define NTWK_DETAIL_IO_CONTEXT_HPP
#include <boost/asio/io_context.hpp>

namespace ntwk::detail {

/// Return the global io_context object.
inline auto io_context() -> boost::asio::io_context&
{
    static auto ctx = boost::asio::io_context{};
    return ctx;
}

}  // namespace ntwk::detail
#endif  // NTWK_DETAIL_IO_CONTEXT_HPP
