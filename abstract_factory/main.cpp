#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

class Button {
public:
    virtual ~Button() = default;
    virtual void render() const = 0;
};

class CheckButton {
public:
    virtual ~CheckButton() = default;
    virtual void render() const = 0;
};

class WindowsButton final : public Button {
public:
    void render() const override {
        std::cout << "windows button" << '\n';
    }
};

class WindowsCheckButton final : public CheckButton {
public:
    void render() const override {
        std::cout << "windows check button" << '\n';
    }
};

class MacButton final : public Button {
public:
    void render() const override {
        std::cout << "mac button" << '\n';
    }
};

class MacCheckButton final : public CheckButton {
public:
    void render() const override {
        std::cout << "mac check button" << '\n';
    }
};

class UIFactory {
public:
    virtual ~UIFactory() = default;
    [[nodiscard]]
    virtual std::unique_ptr<Button> createButton() const = 0;
    [[nodiscard]]
    virtual std::unique_ptr<CheckButton> createCheckButton() const = 0;
};

class WindowsFactory final : public UIFactory {
public:
    [[nodiscard]]
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<WindowsButton>();
    }

    [[nodiscard]]
    std::unique_ptr<CheckButton> createCheckButton() const override {
        return std::make_unique<WindowsCheckButton>();
    }
};

class MacFactory final : public UIFactory {
public:
    [[nodiscard]]
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<MacButton>();
    }

    [[nodiscard]]
    std::unique_ptr<CheckButton> createCheckButton() const override {
        return std::make_unique<MacCheckButton>();
    }
};

void renderUI(const UIFactory& factory) {
    std::unique_ptr<Button> button = factory.createButton();
    std::unique_ptr<CheckButton> checkButton = factory.createCheckButton();

    button->render();
    checkButton->render();
}

static_assert(std::is_abstract_v<Button>);
static_assert(std::is_abstract_v<CheckButton>);
static_assert(std::is_abstract_v<UIFactory>);

int main() {
    WindowsFactory windows;
    MacFactory mac;

    renderUI(windows);
    renderUI(mac);

    std::vector<std::unique_ptr<UIFactory>> factories;
    factories.push_back(std::make_unique<WindowsFactory>());
    factories.push_back(std::make_unique<MacFactory>());

    for (const std::unique_ptr<UIFactory>& factory : factories) {
        std::unique_ptr<Button> button = factory->createButton();
        assert(button != nullptr);
    }

    std::cout << "Abstract Factory test passed!\n";
}