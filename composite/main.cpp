#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Shape {
public:
    virtual ~Shape() {}

    virtual void draw(std::size_t depth = 0) const = 0;
    virtual std::size_t leaf_count() const noexcept = 0;
};

class Circle final : public Shape {
    double radius_;
public:
    Circle(double radius) : radius_(radius) {}

    void draw(std::size_t depth = 0) const override {
        std::cout << std::string(depth * 2, ' ')
                  << "Circle: " << radius_ << '\n';
    }

    [[nodiscard]]
    std::size_t leaf_count() const noexcept override {
        return 1;
    }
};

class Rectangle final : public Shape {
    double width_;
    double height_;
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}

    void draw(std::size_t depth = 0) const override {
        std::cout << std::string(depth * 2, ' ')
                  << "Rectangle: "
                  << width_ << "x" << height_ << '\n';
    }

    [[nodiscard]]
    std::size_t leaf_count() const noexcept override {
        return 1;
    }
};

class ShapeGroup final : public Shape {
private:
    std::string name_;
    std::vector<std::unique_ptr<Shape>> children_;
public:
    explicit ShapeGroup(std::string name) : name_(std::move(name)) {}

    void add(std::unique_ptr<Shape> shape) {
        if (!shape) {
            throw std::invalid_argument("Null shape");
        }

        children_.push_back(std::move(shape));
    }

    void draw(std::size_t depth = 0) const override {
        std::cout << std::string(depth * 2, ' ')
          << "Group: " << name_ << '\n';

        for (const auto& child : children_) {
            child->draw(depth + 1);
        }
    }

    [[nodiscard]]
    std::size_t leaf_count() const noexcept override {
        std::size_t count = 0;

        for (const std::unique_ptr<Shape>& child : children_) {
            count += child->leaf_count();
        }

        return count;
    }
};

static_assert(std::is_abstract_v<Shape>);
static_assert(std::is_base_of_v<Shape, ShapeGroup>);


int main() {
    ShapeGroup canvas("Canvas");

    canvas.add(std::make_unique<Circle>(10.0));

    auto group = std::make_unique<ShapeGroup>("Details");

    group->add(std::make_unique<Rectangle>(20.0, 30.0));
    group->add(std::make_unique<Circle>(5.0));

    canvas.add(std::move(group));

    assert(canvas.leaf_count() == 3);

    canvas.draw();

    std::cout << "Composite test passed!\n";
}