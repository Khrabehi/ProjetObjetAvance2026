
#pragma once

#include <string>
#include <utility>
#include <vector>

namespace NomCool::data {

using Response = std::string;

class Interrogation {
public:
  Interrogation(
      const std::string &question,
      const std::vector<std::pair<std::string, Response>> &proposedAnswers);

  // The question to ask
  const std::string &question() const;
  // The list of possible answer with the response to send back attached
  const std::vector<std::pair<std::string, Response>> &availableAnswers() const;

private:
  std::string mQuestion;
  std::vector<std::pair<std::string, Response>> mProposedAnswers;
};

} // namespace NomCool::data
