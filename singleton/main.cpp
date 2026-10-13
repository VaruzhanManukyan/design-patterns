#include <cassert>
#include <iostream>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <vector>
#include <thread>

class ConfigManager final {
private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> config_;

    ConfigManager() = default;
    ~ConfigManager() = default;
public:
    static ConfigManager& instance() {
        static ConfigManager config;
        return config;
    }

    ConfigManager(ConfigManager const&) = delete;
    ConfigManager& operator=(ConfigManager const&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    void set(std::string key, std::string value) {
        std::lock_guard<std::mutex> lock(mutex_);
        config_.insert_or_assign(std::move(key), std::move(value));
    }

    std::optional<std::string> get(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::unordered_map<std::string, std::string>::iterator it = config_.find(key);
        if (it == config_.end()) {
            return std::nullopt;
        }
        return it->second;
    }
};

static_assert(!std::is_copy_constructible_v<ConfigManager>);
static_assert(!std::is_copy_assignable_v<ConfigManager>);
static_assert(!std::is_move_constructible_v<ConfigManager>);
static_assert(!std::is_move_assignable_v<ConfigManager>);

int main() {
    ConfigManager& config = ConfigManager::instance();
    config.set("mode", "debug");
    assert(config.get("mode") == std::optional<std::string>{"debug"});
    assert(&config == &ConfigManager::instance());

    std::vector<std::thread> workers;
    workers.reserve(4);

    for (std::size_t i = 0; i < 4; ++i) {
        workers.emplace_back([i, &config] {
            assert(&config == &ConfigManager::instance());

            config.set("worker_" + std::to_string(i), "ready");
        });
    }

    for (std::thread& worker : workers) {
        worker.join();
    }

    for (int i = 0; i < 4; ++i) {
        assert(config.get("worker_" + std::to_string(i)) == "ready");
    }

    return 0;
}