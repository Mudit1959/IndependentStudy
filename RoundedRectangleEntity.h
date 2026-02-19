#pragma once
class RoundedRectangleEntity
{
// Would need three rectangle entities with four circles - Seven Rectangle Entities in total
// Could implement semi-circle shader as well
// Three have RectVS+PixelShader, four have RectVS+CirclePS
// Render the circles first, then the above rectangles

// Would be nice to find a graph, and then simply have two rectangle entities reflected vertically

// If radius is 0 - render a single rectangle, the one who has the center transform
// Once radius is larger than 1, subtract radius from width, to get the two bordering rectangles, whose height will also be reduced by the radius both from the top and the bottom

// When rendering, there needs to be a render boolean/counter for different situations (1 entities vs 7 entities);

// One transform to define the center, then a width, height and radius

public:
private:

};

