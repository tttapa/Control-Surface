#include <gmock-wrapper.h>
// Import GMock first because of F macro collisions
#include <Selectors/Selectable.hpp>

W_SUGGEST_OVERRIDE_OFF

template <CS::setting_t N>
class MockSelectable : public CS::Selectable<N> {
  public:
    MOCK_METHOD1(select, void(CS::setting_t));
};

W_SUGGEST_OVERRIDE_ON