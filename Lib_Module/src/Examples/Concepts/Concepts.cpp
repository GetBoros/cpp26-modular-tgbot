//------------------------------------------------------------------------------------------------------------
module;

module Concepts;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Concepts_Preview()
{
    int example_add;
    std::string_view example_string_view;
    int numbers[] = { 1, 2, 3 };
    APoint point_a { 25, 25 };
    APoint point_b { 35, 35 };
    APoint point_c = Example_Sum(point_a, point_b);

    example_add = static_cast<int>(Example_Sum_All(2, 5.0f, 6, 8, 12) );  // 33
    example_string_view = Example_String_View("Hello world", 5);
    example_add = Example_Add(25, 32);
    Example_Print_Info(point_a);
    Example_Print_Range(numbers);

    std::println("result Example_Add: {}", example_add);
    std::println("result Example_String_View: {}", example_string_view);
    std::println("result Example_Sum: {}, {}", point_c.X, point_c.Y);

}
//------------------------------------------------------------------------------------------------------------
