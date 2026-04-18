
#include "Result.hpp"

namespace ElCalculator::data {

Result::Result(Status status, const std::string &message)
    : mStatus(status), mMessage(message) {}

bool Result::isSuccess() const { return mStatus == Status::Success; }

const std::string &Result::message() const { return mMessage; }

} // namespace ElCalculator::data
