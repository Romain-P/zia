//
// Created by romain on 3/2/19.
//

#ifndef ZIA_HTTP_H
#define ZIA_HTTP_H

#include <utility>
#include <regex>
#include "Zia/API.hpp"

using headers_end_offset = ssizet;
using body_start_offset = ssizet;

namespace http {
    inline const std::string protocol = "HTTP/1.1";

    enum code {
        internal_error = 500,
        bad_request = 400,
        resource_not_found = 404,
    };

    /** returns true if error >= 400 **/
    inline bool is_error_code(uint16_t code) {
        return (code >= 400);
    }

    namespace headers {
        inline const std::string content_length = "Content-Length";
    }

    inline const std::string headers_end_delimiter = "\r\n\r\n";

    namespace request {
        namespace parser {
            inline const auto offsets_not_found = std::make_pair<headers_end_offset, body_start_offset >(-1, -1);

            inline std::pair<headers_end_offset, body_start_offset> requestOffsets(char const *buffer) {
                std::string str(buffer);

                auto offset = str.find(headers_end_delimiter);

                if (offset != std::string::npos)
                    return std::make_pair(offset, offset + headers_end_delimiter.size());
                else
                    return offsets_not_found;
            }

            inline bool parseRequest(char const *buffer, sizet size, Request &request) {
                static const std::regex regex_request("([A-Z]+)\\s+([^\\s]+)\\s+(.+)");
                static const std::regex regex_header("([^:\n]+):\\s?(.+)");

                std::smatch matcher;
                std::string str(buffer, buffer + size);
                if (!std::regex_search(str , matcher, regex_request))
                    return false;

                request.method = matcher[1];
                request.uri = matcher[2];
                request.protocol = matcher[3];

                str = matcher.suffix();

                while (std::regex_search(str, matcher, regex_header)) {
                    request.headers[matcher[1]] = matcher[2];
                    str = matcher.suffix();
                }

                return true;
            }
        }
    }

    namespace responses {
        namespace serializer {
            inline void serialize(Response &response, std::vector<char> &buffer) {
                std::string data(http::protocol + " ");
                data += std::to_string(response.status_code) + " ";
                data += response.status_message + "\n";

                for (auto &it: response.headers)
                    data += (it.first + ": " + it.second + "\n");

                data += headers_end_delimiter;

                if (!response.body.empty())
                    data.insert(data.end(), response.body.begin(), response.body.end());

                buffer.insert(buffer.end(), data.begin(), data.end());
            }
        }

        inline const Response bad_request = {
                protocol,
                code::bad_request,
                "Bad Request",
                std::unordered_map<std::string, std::string>(),
                std::vector<char>()
        };

        inline const Response not_found = {
                protocol,
                code::resource_not_found,
                "Not Found",
                std::unordered_map<std::string, std::string>(),
                std::vector<char>()
        };

        inline const Response internal_error = {
                protocol,
                code::internal_error,
                "Internal Error",
                std::unordered_map<std::string, std::string>(),
                std::vector<char>()
        };
    }
}

#endif //ZIA_HTTP_H
