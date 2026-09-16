//------------------------------------------------------------------------------------------------------------
module;

export module ACB_Room_Listener;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------
export class ACB_Room_Listener
{
public:
    ~ACB_Room_Listener();
    ACB_Room_Listener();

    ACB_Room_Listener(const ACB_Room_Listener&) = delete;
    ACB_Room_Listener &operator=(const ACB_Room_Listener&) = delete;

    void Start_Listening(std::string_view access_token, std::string_view channel_name);
    void Stop();

private:
    struct SImpl;

    SImpl *Pimpl;
};
//------------------------------------------------------------------------------------------------------------

