//------------------------------------------------------------------------------------------------------------
module;

export module TGB_Deserializer;
//------------------------------------------------------------------------------------------------------------
import TGB_Data;
//------------------------------------------------------------------------------------------------------------
export class ATGB_Deserializer
{
public:
    void Deserialize_Event(const AString &response_text, STelegram_Event &telegram_event);
    
    void Deserialize_NBU_USD_Rate(const AString &response_text, SCurrency_Rate &currency_rate) const;

private:
    void Print_Json(const AString &response_text) const;

};
//------------------------------------------------------------------------------------------------------------
