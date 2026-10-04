//------------------------------------------------------------------------------------------------------------
module;

module Concepts;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Test()
{
    int example_add;
    std::string_view example_string_view;
    APoint point_a { 25, 25 };
    APoint point_b { 35, 35 };
    APoint point_c = Example_Sum(point_a, point_b);

    example_string_view = Example_String_View("Hello world", 5);
    example_add = Example_Add(25, 32);

    std::println("result Example_Add {}", example_add);
    std::println("result Example_String_View {}", example_string_view);
    std::println("result Example_Sum {}", point_c.X, point_c.Y);

}
//------------------------------------------------------------------------------------------------------------
