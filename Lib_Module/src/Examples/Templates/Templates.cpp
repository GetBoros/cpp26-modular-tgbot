//------------------------------------------------------------------------------------------------------------
module;

module Templates;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Templates_Preview()
{
    int user_input;
    int result;
    ACTAD_Example ctad_example { "Hello world" };  // CTAD example

    user_input = 0;
    result = 0;

    result = TSum<double>(5, 10.5);
    std::println("result {}", result);

    result = TMultiply_Value<3>(result);
    std::println("showcase NTTP result is {}", result);

    std::println("text {}", ctad_example);
}
//------------------------------------------------------------------------------------------------------------
