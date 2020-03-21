#pragma once

/// Helper for the Curiously Recurring Template Pattern.
#define CRTP(Derived) (*static_cast<Derived *>(this))
#define CRTP_INST(Derived, el) (static_cast<Derived &>(el))
