#pragma once

/// Helper for the Curiously Recurring Template Pattern.
#define CRTP(Derived) (*static_cast<Derived *>(this))