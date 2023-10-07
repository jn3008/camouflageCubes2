#pragma once

#include <vector>

//--------------------------------------------------------------
// time interval class with circular topology [0, 1], 0~1
// that is, if t0 < t1 then we consider the time interval [t0, t1]
// otherwise then we consider [t0, 1) U [0, t1]
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

    //--------------------------------------------------------------
    // check whether this time interval contains the timepoint q
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

    //--------------------------------------------------------------
    // check whether this interval overlaps at all with 'i'
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

    //--------------------------------------------------------------
    // return a copy of this interval with t0 set to x
    Interval set0(float x)
    {
        return Interval(x, t1);
    }

    //--------------------------------------------------------------
    // return a copy of this interval with t1 set to x
    Interval set1(float x)
    {
        return Interval(t0, x);
    }
};

//--------------------------------------------------------------
// class for storing a list of time intervals
class Occupation
{
public:
    std::vector<Interval> intervals;

public:
    Occupation(){};

    //--------------------------------------------------------------
    // check whether interval 'i' overlaps with any in 'intervals'
    bool clashes(Interval i)
    {
        for (Interval interval : intervals)
        {
            if (interval.clashes(i))
                return true;
        }
        return false;
    }

    //--------------------------------------------------------------
    // check whether any intervals in 'intervals' overlaps
    // the timepoint q
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