
#pragma once

#include <string>

namespace NomCool::data {

class Result {
public:
  enum class Status { Success, Failure };

  Result(Status status, const std::string &message);

  bool isSuccess() const;
  const std::string &message() const;

private:
  Status mStatus;
  std::string mMessage;
};

} // namespace NomCool::data
