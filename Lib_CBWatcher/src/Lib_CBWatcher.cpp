//------------------------------------------------------------------------------------------------------------
module;

module Lib_CBWatcher;
//------------------------------------------------------------------------------------------------------------
import std;
import ACB_Fetcher;
import ACB_Room_Listener;
//------------------------------------------------------------------------------------------------------------





//------------------------------------------------------------------------------------------------------------
void Handle_CBWatcher()
{
    ACB_Room_Listener room_listener;

    // Need get from other source like playwrithe or else
    std::string token_str = "eyJhbGciOiJIUzI1NiIsImtpZCI6IktTS3cyZy5MMzZJU2ciLCJ0eXAiOiJKV1QifQ.eyJpYXQiOjE3ODk1NTk4NTQsImV4cCI6MTc4OTY0NjI1NC4wLCJ4LWFibHktY2FwYWJpbGl0eSI6IntcInJvb21fdXNlcjpncm91cGVkOkdLR1RKNFY6WkFFQkU1Qzo2XCI6IFtcInByZXNlbmNlXCIsIFwic3Vic2NyaWJlXCJdLCBcInVzZXI6Z3JvdXBlZDpaQUVCRTVDXCI6IFtcInN1YnNjcmliZVwiXSwgXCJnbG9iYWw6cHVzaF9zZXJ2aWNlXCI6IFtcInN1YnNjcmliZVwiXSwgXCJyb29tOmdyb3VwZWQ6R0tHVEo0Vjo2XCI6IFtcInN1YnNjcmliZVwiXSwgXCJyb29tOmZhbmNsdWI6R0tHVEo0VlwiOiBbXCJzdWJzY3JpYmVcIl0sIFwicm9vbTpzaG9ydGNvZGU6R0tHVEo0VlwiOiBbXCJzdWJzY3JpYmVcIl19IiwieC1hYmx5LWNsaWVudElkIjoiK3gxb3ZyMTFya2huLVpBRUJFNUMifQ._AJCbtAdEOknDH3zfZHyhMielWxp7CQx6k3F6Nf4iwQ&format=json&heartbeats=true&v=3&agent=ably-js/2.12.0 browser&remainPresentFor=0";
    std::string channel_name = "room:grouped:GKGTJ4V:6";

    room_listener.Start_Listening(token_str, token_str);  // work in background

    std::println("Listening to stream... Press Enter to stop.");
    std::cin.get();  // main thread wait when press enter

    room_listener.Stop();

}
//------------------------------------------------------------------------------------------------------------
