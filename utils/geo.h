//
// Some class templates for geometry primitives (point, interval, box)
//

#pragma once

#include <cassert>
#include <iostream>
#include <limits>

namespace utils {

// Point template
template <typename T>
class PointT {
public:
    T x, y;
    PointT(T xx = std::numeric_limits<T>::max(), T yy = std::numeric_limits<T>::max()) : x(xx), y(yy) {}
    bool IsValid() { return x != std::numeric_limits<T>::max() && y != std::numeric_limits<T>::max(); }

    // operators
    T& operator[](unsigned d) {
        assert(d == 0 || d == 1);
        return (d == 0 ? x : y);
    }
    PointT operator+(const PointT& rhs) { return PointT(x + rhs.x, y + rhs.y); }
    PointT operator/(T divisor) { return PointT(x / divisor, y / divisor); }
    PointT& operator+=(const PointT& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    PointT& operator-=(const PointT& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    bool operator==(const PointT& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const PointT& rhs) const { return !(*this == rhs); }
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const PointT<T>& pt) {
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}

// Manhattan distance between points
template <typename T>
inline T Dist(const PointT<T>& pt1, const PointT<T>& pt2) {
    return std::abs(pt1.x - pt2.x) + std::abs(pt1.y - pt2.y);
}

// L-inf distance between points
template <typename T>
inline T LInfDist(const PointT<T>& pt1, const PointT<T>& pt2) {
    return std::max(std::abs(pt1.x - pt2.x), std::abs(pt1.y - pt2.y));
}

// Interval template
template <typename T>
class IntervalT {
public:
    template<typename... Args>
    IntervalT(Args... params) { Set(params...); }

    // Setters
    T& low() { return low_; }
    T& high() { return high_; }
    void Set() {
        low_ = std::numeric_limits<T>::max();
        high_ = std::numeric_limits<T>::lowest();
    }
    void Set(T val) {
        low_ = val;
        high_ = val;
    }
    void Set(T low, T high) {
        low_ = low;
        high_ = high;
    }

    // Getters
    T low() const { return low_; }
    T high() const { return high_; }
    T center() const { return (high_ + low_) / 2; }
    T range() const { return high_ - low_; }

    // Update
    // Update() is always safe, FastUpdate() assumes existing values
    void Update(T newVal) {
        if (newVal < low_) low_ = newVal;
        if (newVal > high_) high_ = newVal;
    }
    void FastUpdate(T newVal) {
        if (newVal < low_)
            low_ = newVal;
        else if (newVal > high_)
            high_ = newVal;
    }

    // Two types of intervals: 1. normal, 2. degenerated (i.e., point)
    // is valid interval (i.e., valid closed interval)
    bool IsValid() const { return low_ <= high_; }
    // is strictly valid interval (excluding degenerated ones, i.e., valid open interval)
    bool IsStrictValid() const { return low_ < high_; }

    // Geometric Query/Update
    // interval/range of union (not union of intervals)
    IntervalT UnionWith(const IntervalT& rhs) const {
        if (!IsValid()) return *this;
        else if (!rhs.IsValid()) return rhs;
        else return IntervalT(std::min(low_, rhs.low_), std::max(high_, rhs.high_));
    }
    // may return an invalid interval (as empty intersection)
    IntervalT IntersectWith(const IntervalT& rhs) const {
        return IntervalT(std::max(low_, rhs.low_), std::min(high_, rhs.high_));
    }
    bool HasIntersectWith(const IntervalT& rhs) const { return IntersectWith(rhs).IsValid(); }
    bool HasStrictIntersectWith(const IntervalT& rhs) const { return IntersectWith(rhs).IsStrictValid(); }
    // get nearest point to val
    T GetNearestPoint(T val) const {
        if (val < low_)
            return low_;
        else if (val < high_)
            return val;
        else
            return high_;
    }

    bool operator==(const IntervalT& rhs) const {
        return (!IsValid() && !rhs.IsValid()) || (low_ == rhs.low_ && high_ == rhs.high_); 
    }
    bool operator!=(const IntervalT& rhs) const { return !(*this == rhs); }

private:
    T low_, high_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const IntervalT<T>& interval) {
    os << "(" << interval.low() << ", " << interval.high() << ")";
    return os;
}

// Box template
template <typename T>
class BoxT {
public:
    template<typename... Args>
    BoxT(Args... params) { Set(params...); }

    // Setters
    T& lx() { return x_.low(); }
    T& ly() { return y_.low(); }
    T& hy() { return y_.high(); }
    T& hx() { return x_.high(); }
    IntervalT<T>& operator[](unsigned i) {
        assert(i == 0 || i == 1);
        return (i == 0) ? x_ : y_;
    }
    void Set() {
        x_.Set();
        y_.Set();
    }
    void Set(T x, T y) {
        x_.Set(x);
        y_.Set(y);
    }
    void Set(const PointT<T>& pt) { Set(pt.x, pt.y); }
    void Set(T lx, T ly, T hx, T hy) {
        x_.Set(lx, hx);
        y_.Set(ly, hy);
    }
    void Set(const IntervalT<T>& x, const IntervalT<T>& y) {
        x_ = x;
        y_ = y;
    }
    void Set(const PointT<T>& low, const PointT<T>& high) { Set(low.x, low.y, high.x, high.y); }

    // Two types of boxes: normal & degenerated (line or point)
    // is valid box
    bool IsValid() const { return x_.IsValid() && y_.IsValid(); }
    // is strictly valid box (excluding degenerated ones)
    bool IsStrictValid() const { return x_.IsStrictValid() && y_.IsStrictValid(); }  // tighter

    // Getters
    T lx() const { return x_.low(); }
    T ly() const { return y_.low(); }
    T hy() const { return y_.high(); }
    T hx() const { return x_.high(); }
    T cx() const { return x_.center(); }
    T cy() const { return y_.center(); }
    T x() const { return x_.range(); }
    T y() const { return y_.range(); }
    T hp() const { return x() + y(); }  // half perimeter
    T area() const { return x() * y(); }
    const IntervalT<T>& operator[](unsigned i) const {
        assert(i == 0 || i == 1);
        return (i == 0) ? x_ : y_;
    }

    // Update() is always safe, FastUpdate() assumes existing values
    void Update(T x, T y) {
        x_.Update(x);
        y_.Update(y);
    }
    void FastUpdate(T x, T y) {
        x_.FastUpdate(x);
        y_.FastUpdate(y);
    }
    void Update(const PointT<T>& pt) { Update(pt.x, pt.y); }
    void FastUpdate(const PointT<T>& pt) { FastUpdate(pt.x, pt.y); }

    // Geometric Query/Update
    BoxT UnionWith(const BoxT& rhs) const { return BoxT(x_.UnionWith(rhs.x_), y_.UnionWith(rhs.y_)); }
    BoxT IntersectWith(const BoxT& rhs) const { return BoxT(x_.IntersectWith(rhs.x_), y_.IntersectWith(rhs.y_)); }
    bool HasIntersectWith(const BoxT& rhs) const { return IntersectWith(rhs).IsValid(); }
    bool HasStrictIntersectWith(const BoxT& rhs) const {  // tighter
        return IntersectWith(rhs).IsStrictValid();
    }
    PointT<T> GetNearestPoint(const PointT<T>& pt) {
        return PointT<T>(x_.GetNearestPoint(pt.x), y_.GetNearestPoint(pt.y));
    }

    bool operator==(const BoxT& rhs) const {
        return (x_ == rhs.x) && (y_ == rhs.y_); 
    }
    bool operator!=(const BoxT& rhs) const { return !(*this == rhs); }

private:
    using limits = std::numeric_limits<T>;
    IntervalT<T> x_, y_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const BoxT<T>& box) {
    os << "[x: " << box[0] << ", y: " << box[1] << "]";
    return os;
}

template <typename T>
class SegmentT : public BoxT<T> {
public:
    T length() const { return BoxT<T>::hp(); }
    bool IsRectilinear() const { return BoxT<T>::x() == 0 || BoxT<T>::y() == 0; }
};

}  // namespace utils