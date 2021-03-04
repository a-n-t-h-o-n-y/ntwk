#ifndef NTWK_ERROR_HPP
#define NTWK_ERROR_HPP
#include <stdexcept>

namespace ntwk {

class Error : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
};

}  // namespace ntwk
#endif  // NTWK_ERROR_HPP
