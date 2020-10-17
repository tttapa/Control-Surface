#pragma once

#include <AH/STL/utility> // std::forward
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Helper to turn a lambda function into a puller.
 * @ingroup MIDIParsers
 */
template <class Lambda>
class LambdaPuller_ {
  public:
    LambdaPuller_(Lambda &&lambda) : lambda(std::forward<Lambda>(lambda)) {}

    template <class T>
    bool pull(T &packet) {
        return lambda(packet);
    }

  private:
    Lambda lambda;
};

template <class Lambda>
LambdaPuller_<Lambda> LambdaPuller(Lambda &&lambda) {
    return std::forward<Lambda>(lambda);
}

END_CS_NAMESPACE