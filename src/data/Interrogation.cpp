
#include "Interrogation.hpp"

namespace ElCalculator::data {

Interrogation::Interrogation(
    const std::string &question,
    const std::vector<std::pair<std::string, Response>> &proposedAnswers)
    : mQuestion(question), mProposedAnswers(proposedAnswers) {}

const std::string &Interrogation::question() const { return mQuestion; }

const std::vector<std::pair<std::string, Response>> &
Interrogation::availableAnswers() const {
  return mProposedAnswers;
}

} // namespace ElCalculator::data
