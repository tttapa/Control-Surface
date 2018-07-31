#include <Selectors/Selectable.hpp>
#include <gmock/gmock.h>

template <setting_t N>
class MockSelectable : public Selectable<N> {
  public:
    MOCK_METHOD1(select, void(setting_t));
};