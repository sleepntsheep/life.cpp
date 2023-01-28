#pragma once
#include <set>
#include <vector>
#include "Point.hpp"
#include <functional>

class Life {
    public:
        Life() = default;
        ~Life() = default;

        void do_generation();
        bool get(Point p);
        void set(Point p);
        void erase(Point p);
        int generation = 0;
        void iterate_cells(std::function<void(const Point &p)> f);

    private:
        std::set<Point> points;
        std::vector<Point> all_neighbor(Point p);
        std::vector<Point> alive_neighbor(Point p);
};

