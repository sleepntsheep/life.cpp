#include "Life.hpp"
#include <algorithm>
#include <fstream>

bool Life::get(Point p)
{
    return points.count(p);
}

void Life::set(Point p)
{
    points.insert(p);
}

void Life::erase(Point p)
{
    points.erase(p);
}

std::vector<Point> Life::all_neighbor(Point p)
{
    std::vector<Point> v;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            v.push_back({p.x + i, p.y + j});
        }
    }
    return v;
}

std::vector<Point> Life::alive_neighbor(Point p)
{
    const auto all = all_neighbor(p);
    std::vector<Point> alive;
    std::copy_if(all.begin(), all.end(), std::back_inserter(alive),
            [this](const Point &p){ return get(p); } );
    return alive;
}

void Life::do_generation()
{
    std::set<Point> updated_points;
    for (const Point &p: points)
    {
        auto an = alive_neighbor(p);
        if (an.size() == 3 || an.size() == 2)
            updated_points.insert(p);

        for (const auto &p2: all_neighbor(p)) {
            auto an2 = alive_neighbor(p2);
            if (an2.size() == 3)
                updated_points.insert(p2);
        }
    }
    points = updated_points;
    generation++;
}

void Life::iterate_cells(std::function<void(const Point &p)> f)
{
    std::for_each(points.begin(), points.end(), f);
}

void Life::save_to_file(std::string path)
{
    std::ofstream file;
    file.open(path);
    file << generation << '\n';
    std::for_each(points.begin(), points.end(), [&file](const Point &p) {
        file << p.x << ' ' << p.y << '\n';
    });
    file.close();
}

void Life::load_from_file(std::string path)
{
    std::ifstream file;
    file.open(path);
    file >> generation;
    Point p {0, 0};
    while (file)
    {
        file >> p.x >> p.y;
        set(p);
    }
    file.close();
}

