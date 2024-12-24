#include <gmock/gmock.h>
// Import GMock first because of F macro collisions
#include <Selectors/Selectable.hpp>

template <cs::setting_t N>
class MockSelectable : public cs::Selectable<N> {
  public:
    MOCK_METHOD(void, select, (cs::setting_t), (override));
};
