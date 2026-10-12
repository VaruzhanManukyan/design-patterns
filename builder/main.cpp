#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class HttpRequestBuilder;

class HttpRequest {
private:
    friend class HttpRequestBuilder;
    std::string url_;
    std::string method_;
    std::ptrdiff_t timeout_;
    std::vector<std::string> headers_;

    HttpRequest(
        std::string url,
        std::string method,
        std::ptrdiff_t timeout,
        std::vector<std::string> headers) :
            url_(std::move(url)),
            method_(std::move(method)),
            timeout_(timeout),
            headers_(std::move(headers)) {}
public:
    [[nodiscard]]
    const std::string& get_url() const {
        return url_;
    }

    [[nodiscard]]
    const std::string& get_method() const {
        return method_;
    }

    [[nodiscard]]
    std::ptrdiff_t get_timeout() const {
        return timeout_;
    }

    [[nodiscard]]
    std::vector<std::string> get_headers() const {
        return headers_;
    }

    void print() const {
        std::cout << method_ << " " << url_ << '\n';
        std::cout << "Timeout: " << timeout_ << '\n';

        for (const std::string& header : headers_) {
            std::cout << header << '\n';
        }
    }
};

class HttpRequestBuilder {
private:
    std::string url_ = "http://127.0.0.1:8080";
    std::string method_ = "GET";
    std::ptrdiff_t timeout_ = 30;
    std::vector<std::string> headers_;
public:
    HttpRequestBuilder& set_url(std::string url) {
        url_ = std::move(url);
        return *this;
    }

    HttpRequestBuilder& set_method(std::string method) {
        method_ = std::move(method);
        return *this;
    }

    HttpRequestBuilder& set_timeout(std::ptrdiff_t timeout) {
        timeout_ = timeout;
        return *this;
    }

    HttpRequestBuilder& set_headers(std::string headers) {
        headers_.push_back(std::move(headers));
        return *this;
    }

    HttpRequest build() {
        if (url_.empty()) {
            throw std::invalid_argument("URL is required");
        }

        if (timeout_ <= 0) {
            throw std::invalid_argument("Invalid timeout");
        }

        return {url_, method_, timeout_, headers_};
    }
};

int main() {
    auto request = HttpRequestBuilder{}
    .set_url("https://api.example.com")
    .set_method("POST")
    .set_timeout(15)
    .set_headers("Content-Type: application/json")
    .build();

    assert(request.get_url() == "https://api.example.com");

    request.print();
    return 0;
}