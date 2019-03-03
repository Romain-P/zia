//
// Created by romain on 3/2/19.
//

#ifndef ZIA_HTTP_H
#define ZIA_HTTP_H


namespace http {
    inline const std::string protocol = "HTTP/1.1";

    enum code {
        internal_error = 500,
        resource_not_found = 404
    };

    namespace responses {
        inline const Response bad_request = {
                .protocol = protocol,
                .status_code = 400,
                .status_message = "Bad Request"
        };
    }
}

#endif //ZIA_HTTP_H
