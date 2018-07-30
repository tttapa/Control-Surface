#include <Selectors/Selectable.hpp>
#include <gmock/gmock.h>

class MockSelectable : public Selectable {
  public:
    MOCK_METHOD1(select, void(setting_t));
};