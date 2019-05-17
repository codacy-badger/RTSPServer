#include <sstream>
#include "skeleton/exception.h"

namespace exception {
RuntimeError::RuntimeError(std::string& reason)
        :std::runtime_error(reason),
         mCode(ErrorCode::internalError) { }

RuntimeError::RuntimeError(const char* reason)
        :std::runtime_error(std::string(reason)),
         mCode(ErrorCode::internalError) { }

RuntimeError::RuntimeError(std::string& reason, ErrorCode& code)
        :std::runtime_error(reason),
         mCode(code) { }

RuntimeError::RuntimeError(const char* reason, ErrorCode& code)
        :std::runtime_error(std::string(reason)),
         mCode(code) { }

RuntimeError::~RuntimeError() throw() { }

inline ErrorCode RuntimeError::whatCode() const throw()
{
    return mCode;
}

void runtimeAssert(std::string reason, ErrorCode code, bool isOkay)
{
    if (!isOkay) {
        runtimeAssert(reason.c_str(), code, isOkay);
    }
}

void runtimeAssert(const char* reason, ErrorCode code, bool isOkay)
{
    if (!isOkay && code!=ErrorCode::ok) {
        throw RuntimeError(reason, code);
    }
}

void runtimeAssert(std::string reason, ErrorCode code)
{
    runtimeAssert(reason.c_str(), code);
}

void runtimeAssert(const char* reason, ErrorCode code)
{
    if (ErrorCode::ok!=code) {
        throw RuntimeError(reason, code);
    }
}

void runtimeAssert(std::string reason, bool isOkay)
{
    runtimeAssert(reason.c_str(), isOkay);
}

void runtimeAssert(const char* reason, bool isOkay)
{
    if (!isOkay) {
        throw RuntimeError(reason);
    }
}

void runtimeAssert(std::string reason, int32_t rc)
{
    if (0!=rc) {
        reason += decimal_to_hexstring(rc);
        runtimeAssert(reason, false);
    }
}

void runtimeAssert(const char* reason, int32_t rc)
{
    if (nullptr==reason) {
        return;
    }

    auto msg = std::string(reason);
    runtimeAssert(msg, rc);
}

template<typename T>
std::string decimal_to_hexstring(T t)
{
    std::stringstream ss;
    ss << std::hex << t;

    return ss.str();
}
} // namespace exception
