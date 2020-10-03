
#include <UnitTest++/UnitTest++.h>
#include "../shared/Vector2D.h"

TEST(AddVectors)
{
    Vector2D v1 = Vector2D(2, 5);
    Vector2D v2 = Vector2D(3, 7);
    CHECK(v1 + v2 == Vector2D(5, 12));
}

TEST(SubtractVectors)
{
    Vector2D v1 = Vector2D(2, 5);
    Vector2D v2 = Vector2D(3, 7);
    CHECK(v1 - v2 == Vector2D(1, 2));
}

int main() {
    return UnitTest::RunAllTests();
}

