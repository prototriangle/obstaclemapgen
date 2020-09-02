#ifndef OBSTACLE_MAP_GEN_UTILITY_H
#define OBSTACLE_MAP_GEN_UTILITY_H

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <tuple>
#include <cfloat>


namespace ObstacleMapGen {


    typedef std::vector<std::vector<double>> Matrix2D;

    struct MapParams {
        float maxX = 16.f;
        float minX = 0.f;
        float maxZ = 16.f;
        float minZ = 0.f;
        float maxS = 4.16f;
        float minS = 2.08f;
        float height = 4.f;
        float spacing = 0.96f;
        float padding = 0.96f;
        float margin = 0.96f;
    };

    struct Vec2D {
        Vec2D(double x, double y) : x(x), y(y) {}

        Vec2D() : x(0), y(0) {}

        double x;
        double y;

        Vec2D operator+(const Vec2D &right) const {
            return {x + right.x, y + right.y};
        }

        Vec2D operator-(const Vec2D &right) const {
            return {x - right.x, y - right.y};
        }

        Vec2D operator*(double k) const {
            return {x * k, y * k};
        }

        Vec2D operator/(double k) const {
            return {x / k, y / k};
        }

        bool operator==(const Vec2D &right) const {
            return x == right.x && y == right.y;
        }

        inline std::string to_string() const {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        friend std::ostream &operator<<(std::ostream &stream, const Vec2D &vec) {
            stream << "(" << vec.x << ", " << vec.y << ")";
            return stream;
        }

        friend std::istream &operator>>(std::istream &input, Vec2D &vec) {
            char c = static_cast<char>(input.peek());
            while (c != '(') {
                input.ignore();
                if (input.eof() || !input.good()) return input;
                c = static_cast<char>(input.peek());
            }
            input.ignore();
            input >> vec.x;
            c = static_cast<char>(input.peek());
            while (c != ',') {
                input.ignore();
                if (input.eof() || !input.good()) return input;
                c = static_cast<char>(input.peek());
            }
            input.ignore();
            input >> vec.y;
            c = static_cast<char>(input.peek());
            while (c != ')') {
                input.ignore();
                if (input.eof() || !input.good()) return input;
                c = static_cast<char>(input.peek());
            }
            input.ignore();
            return input;

        }
    };

    inline double Dot(Vec2D v, Vec2D w) {
        const double x = w.x * v.x;
        const double y = w.y * v.y;
        return x + y;
    }

    inline double LengthSquared(Vec2D v, Vec2D w) {
        const double x = w.x - v.x;
        const double y = w.y - v.y;
        return x * x + y * y;
    }

    inline double Distance(Vec2D v, Vec2D w) {
        return sqrt(LengthSquared(v, w));
    }

    inline Vec2D Rotate(Vec2D v, double theta) {
        double cs = std::cos(theta);
        double sn = std::sin(theta);
        double px = v.x * cs - v.y * sn;
        double py = v.x * sn + v.y * cs;
        return {px, py};
    }

    struct Obstacle {
        float originX;
        float originZ;
        float sizeX;
        float sizeZ;
        float rot;

        Vec2D c1;
        Vec2D c2;
        Vec2D c3;
        Vec2D c4;

        void calculateCorners() {
            Vec2D o1 = {sizeX / 2, sizeZ / 2};
            Vec2D o2 = {sizeX / 2, -sizeZ / 2};
            Vec2D o3 = {-sizeX / 2, -sizeZ / 2};
            Vec2D o4 = {-sizeX / 2, sizeZ / 2};
            Vec2D c = {originX, originZ};
            c1 = c + Rotate(o1, rot);
            c2 = c + Rotate(o2, rot);
            c3 = c + Rotate(o3, rot);
            c4 = c + Rotate(o4, rot);
        }

        Obstacle(float originX, float originZ, float sizeX, float sizeZ, float rot)
                : originX(originX), originZ(originZ), sizeX(sizeX), sizeZ(sizeZ), rot(rot) {
            calculateCorners();
        }

        Obstacle() : originX(0.f), originZ(0.f), sizeX(0.f), sizeZ(0.f), rot(0.f) {}

        friend std::ostream &operator<<(std::ostream &stream, const Obstacle &o) {
            stream << o.originX << " " << o.originZ << " "
                   << o.sizeX << " " << o.sizeZ << " " << o.rot;
            return stream;
        }

        std::tuple<Vec2D, Vec2D, Vec2D, Vec2D> GetCorners() const {
            return std::make_tuple(c1, c2, c3, c4);
        }

    };

    inline double MinDistanceBetweenObstacles(const Obstacle &o1, const Obstacle &o2) {
        double a1 = std::abs(o1.c1.x - o2.c3.x);
        double a2 = std::abs(o1.c1.y - o2.c3.y);
        double a3 = std::abs(o1.c3.x - o2.c1.x);
        double a4 = std::abs(o1.c3.y - o2.c1.y);
        double min = std::min(std::min(std::min(a1, a2), a3), a4);
        return min;
    }


    inline bool ObstacleCornerMinDistnaceLessThan(const Obstacle &o1, const Obstacle &o2, double d) {
        Vec2D o1Points[] = {o1.c1, o1.c2, o1.c3, o1.c4};
        Vec2D o2Points[] = {o2.c1, o2.c2, o2.c3, o2.c4};
        for (auto o1Point : o1Points) {
            for (auto o2Point : o2Points) {
                if (Distance(o1Point, o2Point) < d)
                    return true;
            }
        }
        return false;
    }

    inline double ShortestDistanceBetweenPointAndLineSegment(Vec2D v, Vec2D w, Vec2D p) {

        if (v == w) return Distance(v, p); // 0 length line segment
        double d_sq = LengthSquared(v, w);
        double m = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / d_sq;
        Vec2D diff = w - v;
        if (m <= 0.0f || m >= 1.0f) {
            // point outside of line segment
            double dist = std::min(Distance(v, p), Distance(w, p));
            return dist;
        } else {
            double dist = Distance(Vec2D{v.x + m * diff.x, v.y + m * diff.y}, p);
            return dist;
        }

    }

    inline bool LineSegmentIntersectsCircle(
            double ax, double ay,
            double bx, double by,
            double cx, double cy, double r) {

        bool intersects = ShortestDistanceBetweenPointAndLineSegment({ax, ay}, {bx, by}, {cx, cy}) <= r;

        return intersects;
    }

    inline bool PointInObstacle(const Vec2D &point, const Obstacle &o) {
        ;
        const Vec2D AB = o.c2 - o.c1;
        const Vec2D AP = point - o.c1;
        const Vec2D BC = o.c3 - o.c2;
        const Vec2D BP = point - o.c2;
        const double ABdotAP = Dot(AB, AP);
        const double ABdotAB = Dot(AB, AB);
        const double BCdotBP = Dot(BC, BP);
        const double BCdotBC = Dot(BC, BC);
        return 0 <= ABdotAP && ABdotAP <= ABdotAB && 0 <= BCdotBP && BCdotBP <= BCdotBC;
    }

    inline bool PointInAnyObstacle(Vec2D point, std::vector<Obstacle> &os) {
        for (const auto &o : os) {
            if (PointInObstacle(point, o))
                return true;
        }
        return false;
    }

    inline bool ObstaclesIntersect(const Obstacle &o1, const Obstacle &o2) {
        Vec2D o1Points[] = {o1.c1, o1.c2, o1.c3, o1.c4};
        Vec2D o2Points[] = {o2.c1, o2.c2, o2.c3, o2.c4};
        for (int i = 0; i < 4; ++i) {
            if (PointInObstacle(o1Points[i], o2) || PointInObstacle(o2Points[i], o1))
                return true;
        }
        return false;
    }

    inline bool CircleIntersectsObstacle(const Vec2D &centre, const double &radius, const Obstacle &obstacle) {
        return PointInObstacle(centre, obstacle)
               || LineSegmentIntersectsCircle(
                obstacle.c1.x, obstacle.c1.y, obstacle.c2.x, obstacle.c2.y,
                centre.x, centre.y,
                radius)
               || LineSegmentIntersectsCircle(
                obstacle.c2.x, obstacle.c2.y, obstacle.c3.x, obstacle.c3.y,
                centre.x, centre.y,
                radius)
               || LineSegmentIntersectsCircle(
                obstacle.c3.x, obstacle.c3.y, obstacle.c4.x, obstacle.c4.y,
                centre.x, centre.y,
                radius)
               || LineSegmentIntersectsCircle(
                obstacle.c4.x, obstacle.c4.y, obstacle.c1.x, obstacle.c1.y,
                centre.x, centre.y,
                radius);
    }

}
#endif //OBSTACLE_MAP_GEN_UTILITY_H
