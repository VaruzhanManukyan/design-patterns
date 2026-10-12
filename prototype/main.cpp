#include <iostream>
#include <memory>
#include <string>
#include <cassert>
#include <utility>
#include <vector>

class Shape {
protected:
    std::string color_;

    explicit Shape(std::string color) : color_(std::move(color)) {}
public:
    virtual ~Shape() = default;

    virtual std::unique_ptr<Shape> clone() = 0;
    virtual void print() const = 0;

    void set_color(std::string color) {
        color_ = std::move(color);
    }

    const std::string& get_color() {
        return color_;
    }
};

class Circle final : public Shape {
private:
    double radius_;
public:
    Circle(double radius, std::string color) : Shape(color), radius_(radius) {}

    std::unique_ptr<Shape> clone() override {
        return std::make_unique<Circle>(*this);
    }

    void print() const override {
        std::cout << "Circle: radius=" << radius_
                  << ", color=" << color_ << '\n';
    }
};

class Rectangle final : public Shape {
private:
    double width_;
    double height_;
public:
    Rectangle(double width, double height, std::string color) :
        Shape(std::move(color)),
        width_(width),
        height_(height) {}

    std::unique_ptr<Shape> clone() override {
        return std::make_unique<Rectangle>(*this);
    }

    void print() const override {
        std::cout << "Rectangle: " << width_
                  << "x" << height_
                  << ", color=" << color_ << '\n';
    }
};


int main() {
    Circle circle(10.0, "Red");
    Rectangle rectangle(20.0, 30.0, "Blue");

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(circle.clone());
    shapes.push_back(rectangle.clone());

    auto clonedCircle = shapes[0]->clone();
    clonedCircle->set_color("Green");

    assert(circle.get_color() == "Red");
    assert(shapes[0]->get_color() == "Red");
    assert(clonedCircle->get_color() == "Green");

    assert(clonedCircle.get() != shapes[0].get());

    for (const auto& shape : shapes) {
        shape->print();
    }

    clonedCircle->print();

    std::cout << "Prototype test passed!\n";
}