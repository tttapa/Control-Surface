#include <gmock/gmock.h>
// Import GMock first because of F macro collisions
#include <Selectors/Selectable.hpp>

template <CS::setting_t N>
class MockSelectable : public CS::Selectable<N> {
  public:
    MOCK_METHOD(void, select, (CS::setting_t), (override));
};
