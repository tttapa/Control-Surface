#pragma once

#ifdef ESP32
#include <esp_pthread.h>
#endif

#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

struct ScopedThreadConfig {
#ifdef ESP32
    ScopedThreadConfig(size_t stack_size, size_t priority, bool inherit_cfg,
                       const char *thread_name,
                       int pin_to_core = tskNO_AFFINITY) {
        memset(&previousConfig, 0, sizeof(previousConfig));
        hadPreviousConfig = esp_pthread_get_cfg(&previousConfig) == ESP_OK;
        esp_pthread_cfg_t cfg = previousConfig;
        cfg.stack_size = stack_size;
        cfg.prio = priority;
        cfg.inherit_cfg = inherit_cfg;
        cfg.thread_name = thread_name;
        cfg.pin_to_core = pin_to_core;
        esp_pthread_set_cfg(&cfg);
    }

    ~ScopedThreadConfig() {
        if (hadPreviousConfig)
            esp_pthread_set_cfg(&previousConfig);
    }

  private:
    bool hadPreviousConfig;
    esp_pthread_cfg_t previousConfig;
#else
    ScopedThreadConfig(size_t stack_size, size_t priority, bool inherit_cfg,
                       const char *thread_name, int pin_to_core = -1) {
        (void)stack_size;
        (void)priority;
        (void)inherit_cfg;
        (void)thread_name;
        (void)pin_to_core;
    }
#endif
};

END_CS_NAMESPACE
