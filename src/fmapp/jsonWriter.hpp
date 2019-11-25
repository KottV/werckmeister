#ifndef FMAPP_JSON_WRITER
#define FMAPP_JSON_WRITER

#include <fm/common.hpp>
#include <forward.hpp>
#include <memory>
#include <vector>
#include <compiler/error.hpp>

namespace sheet {
    class Document;
}

namespace fmapp {
    struct EventInfo;
    /**
     * writes sheet info as json format
     **/
    class JsonWriter {
        public:
            JsonWriter() = default;
            JsonWriter(const JsonWriter&) = delete;
            JsonWriter& operator= (const JsonWriter&) = delete;
            std::string funkfeuerToJSON(fm::Ticks elapsedTime, unsigned long lastUpdateTimestamp);
            std::string funkfeuerToJSON(fm::Ticks elapsedTime, unsigned long lastUpdateTimestamp, const std::vector<EventInfo> &eventInfos);
            std::string documentInfosToJSON(const sheet::Document &document, fm::Ticks duration, const sheet::Warnings &warnings);
            std::string exceptionToJSON(const std::exception &ex);
    };
}

#endif