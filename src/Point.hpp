#pragma once

class Point {
    public:
        int x, y;

        auto operator<=>(const Point&) const = default;

        auto operator+(const Point&o) const {
            return Point{x + o.x, y + o.y};
        }

        auto operator-() const {
            return Point{-x, -y};
        }

        auto operator+=(const Point&o) {
            x += o.x; y += o.y;
        }

        Point(int x, int y) : x{x}, y{y} {}
    private:
};

