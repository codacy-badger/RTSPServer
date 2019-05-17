#ifndef PROJECT_EXCEPTION_H
#define PROJECT_EXCEPTION_H

#include <string>
#include <stdexcept>

#define throwException(str, errcode)     do{\
    exception::runtimeAssert(str, errcode);\
}while(0)

namespace exception {
  enum class ErrorCode {
      ok,
      parse,
      invalidParameter,
      methodNotSupport,
      notEnoughMemory,
      internalError,
      other
  };

  class RuntimeError : public std::runtime_error {
  public:
      RuntimeError(const char *reason);

      RuntimeError(std::string &reason);

      RuntimeError(const char *reason, ErrorCode &code);

      RuntimeError(std::string &reason, ErrorCode &code);

      virtual ~RuntimeError() throw();

  private:
      ErrorCode mCode;

      ErrorCode whatCode() const throw();
  };

  void runtimeAssert(std::string reason, ErrorCode code, bool isOkay);
  void runtimeAssert(const char *reason, ErrorCode code, bool isOkay);

  void runtimeAssert(std::string reason, ErrorCode code);
  void runtimeAssert(const char *reason, ErrorCode code);

  void runtimeAssert(std::string reason, bool isOkay);
  void runtimeAssert(const char *reason, bool isOkay);

  void runtimeAssert(std::string reason, int32_t rc);
  void runtimeAssert(const char *reason, int32_t rc);

  template<typename T>
  std::string decimal_to_hexstring(T t);
}

#endif //PROJECT_EXCEPTION_H
