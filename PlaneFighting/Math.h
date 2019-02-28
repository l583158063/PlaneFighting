#pragma once
class Math
{
public:
    static bool BoundingBox(int x1, int y1, int width1, int height1,
                            int x2, int y2, int width2, int height2)
    {
        return x1 < x2 + width2 &&
            x1 + width1 > x2 &&
            y1 < y2 + height2 &&
            height1 + y1 > y2;
    }
};
