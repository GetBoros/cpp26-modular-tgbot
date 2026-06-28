//------------------------------------------------------------------------------------------------------------
module;

export module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
import std;
import TGB_Bot_API;
//------------------------------------------------------------------------------------------------------------
export class ATGB_Manager
{
public:
    ATGB_Manager();
    
    void Initialize();

private:
    void Tick();
    
    void Send_NBU_USD_Rate();

    // int Lst_Processed_Event_Id;

    // AString Response;
    // ATGB_Deserializer Deserializer;
    ATGB_Bot_API Bot_API;
};
//------------------------------------------------------------------------------------------------------------
