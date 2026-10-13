#include <iostream>
#include <memory>

class ITemperatureSensor {
public:
    virtual ~ITemperatureSensor() = default;

    [[nodiscard]]
    virtual double getCelsius() const = 0;
};

class LegacyTemperatureSensor {
public:
    double readFahrenheit() const {
        return 77.0;
    }
};

class TemperatureAdapter final : public ITemperatureSensor {
private:
    const LegacyTemperatureSensor& sensor_;
public:
    explicit TemperatureAdapter(const LegacyTemperatureSensor& sensor) : sensor_(sensor) {}

    double getCelsius() const override {
        const double fahrenheit = sensor_.readFahrenheit();
        return (fahrenheit - 32.0) * 5.0 / 9.0;
    }
};

void displayTemperature(const std::unique_ptr<ITemperatureSensor>& sensor) {
    std::cout << "Temperature: " << sensor->getCelsius()<< " C\n";
}

int main() {
    std::unique_ptr<LegacyTemperatureSensor> legacy = std::make_unique<LegacyTemperatureSensor>();
    std::unique_ptr<ITemperatureSensor> adapter = std::make_unique<TemperatureAdapter>(*legacy);
    displayTemperature(adapter);
    return 0;
}
