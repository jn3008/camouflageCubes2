#pragma once

#include <vector>

class Interval
{
public:
    float t0, t1;

public:
    Interval(float t0_ = 0, float t1_ = 0)
    {
        t0 = t0_;
        t1 = t1_;
    };
    bool contains(float q)
    {
        if (t1 > t0)
        {
            if (q > t0 && q <= t1)
                return true;
        }
        else
        {
            if (q > t0 || q <= t1)
                return true;
        }
        return false;
    }
    bool clashes(Interval i)
    {
        if (contains(i.t0))
            return true;
        else if (contains(i.t1))
            return true;
        else if (i.contains(t0))
            return true;
        else if (i.contains(t1))
            return true;
        else
            return false;
    }
    Interval set0(float x)
    {
        return Interval(x, t1);
    }
    Interval set1(float x)
    {
        return Interval(t0, x);
    }
};

class Occupation
{
public:
    std::vector<Interval> intervals;

public:
    Occupation(){};
    bool clashes(Interval i)
    {
        for (Interval interval : intervals)
        {
            if (interval.clashes(i))
                return true;
        }
        return false;
    }
    bool contains(float q)
    {
        for (Interval interval : intervals)
        {
            if (interval.contains(q))
                return true;
        }
        return false;
    }
};