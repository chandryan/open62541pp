#pragma once

#include "open62541pp/Config.h"
#include "open62541pp/detail/open62541/client.h"  // UA_ClientConfig

#include "CustomDataTypes.h"
#include "plugins/LoggerAdapter.h"
#include "plugins/PluginManager.h"

namespace opcua {

class ClientConfig {
public:
    ClientConfig(UA_ClientConfig& config)
        : config_(config) {}

    void setLogger(Logger logger) {
        if (logger) {
            logger_.assign(LoggerAdapter(std::move(logger)));
            // From version 1.4 the event loop gets created with a copy of the pointer to the
            // logger. It needs to be updated when we set a non-default logger.
#if UAPP_OPEN62541_VER_GE(1, 4)
            config_.eventLoop->logger = config_.logging;
#endif
        }
    }

    void setCustomDataTypes(std::vector<DataType> dataTypes) {
        customDataTypes_.assign(std::move(dataTypes));
    }

    constexpr UA_ClientConfig* operator->() noexcept {
        return &config_;
    }

    constexpr const UA_ClientConfig* operator->() const noexcept {
        return &config_;
    }

    constexpr UA_ClientConfig* handle() noexcept {
        return &config_;
    }

    constexpr const UA_ClientConfig* handle() const noexcept {
        return &config_;
    }

private:
    UA_ClientConfig& config_;
    CustomDataTypes customDataTypes_{config_.customDataTypes};
#if UAPP_OPEN62541_VER_GE(1, 4)
    PluginManager<UA_Logger*> logger_{config_.logging};
#else
    PluginManager<UA_Logger> logger_{config_.logger};
#endif
};

}  // namespace opcua
