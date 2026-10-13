#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle final : public Shape {
private:
    double radius_;
public:
    explicit Circle(double radius) : radius_(radius) {}

    void draw() const override {
        std::cout << "Circle: " << radius_ << '\n';
    }
};

class Rectangle final : public Shape {
    double width_;
    double height_;
public:
    Rectangle(double width, double height)
        : width_(width), height_(height) {}

    void draw() const override {
        std::cout << "Rectangle: "<< width_ << "x" << height_ << '\n';
    }
};

class ShapeFactory {
public:
    virtual ~ShapeFactory() = default;

    [[nodiscard]]
    virtual std::unique_ptr<Shape> create() const = 0;

    void render() const {
        std::unique_ptr<Shape> shape = create();
        shape->draw();
    }
};

class CircleFactory final : public ShapeFactory {
    double radius_;
public:
    explicit CircleFactory(double radius)
        : radius_(radius) {}

    [[nodiscard]]
    std::unique_ptr<Shape> create() const override {
        return std::make_unique<Circle>(radius_);
    }
};

class RectangleFactory final : public ShapeFactory {
private:
    double width_;
    double height_;
public:
    RectangleFactory(double width, double height) : width_(width), height_(height) {}

    [[nodiscard]]
    std::unique_ptr<Shape> create() const override {
        return std::make_unique<Rectangle>(width_, height_);
    }
};

static_assert(std::is_abstract_v<Shape>);
static_assert(std::is_abstract_v<ShapeFactory>);

int main() {
    std::vector<std::unique_ptr<ShapeFactory>> factories;

    factories.push_back(std::make_unique<CircleFactory>(10.0));
    factories.push_back(std::make_unique<RectangleFactory>(20.0, 30.0));

    for (const std::unique_ptr<ShapeFactory>& factory : factories) {
        factory->render();
    }

    std::unique_ptr<Shape> shape = factories[0]->create();

    assert(dynamic_cast<Circle*>(shape.get()) != nullptr);

    std::cout << "Factory Method test passed!\n";
}