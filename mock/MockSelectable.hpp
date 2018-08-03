#include <gmock/gmock.h>
// Import GMock first because of F macro collisions
#include <Selectors/Selectable.hpp>

template <setting_t N>
class MockSelectable : public Selectable<N> {
  public:
    MOCK_METHOD1(select, void(setting_t));
};