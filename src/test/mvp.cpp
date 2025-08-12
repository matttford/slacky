// #include <iostream>
// #include <curl/curl.h>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// std::string slack_token = "xoxp-your-token";

// // Write callback for curl to store response
// static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
//     s->append((char*)contents, size * nmemb);
//     return size * nmemb;
// }

// // Simulate mapping emoji to RGB
// std::string mapEmojiToColor(const std::string& emoji) {
//     if (emoji == ":house:") return "Green";
//     if (emoji == ":palm_tree:") return "Blue";
//     if (emoji == ":spiral_calendar_pad:") return "Orange";
//     if (emoji == ":zzz:") return "Purple";
//     return "Red"; // Default/error
// }

// // Parse emoji from JSON response
// std::string parseEmojiFromSlackProfile(const std::string& jsonStr) {
//     auto j = json::parse(jsonStr);
//     return j["profile"]["status_emoji"].get<std::string>();
// }

// int main() {
//     CURL* curl;
//     CURLcode res;
//     std::string readBuffer;

//     curl_global_init(CURL_GLOBAL_DEFAULT);
//     curl = curl_easy_init();

//     if (curl) {
//         struct curl_slist* headers = nullptr;
//         headers = curl_slist_append(headers, ("Authorization: Bearer " + slack_token).c_str());

//         curl_easy_setopt(curl, CURLOPT_URL, "https://slack.com/api/users.profile.get");
//         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

//         res = curl_easy_perform(curl);

//         if (res == CURLE_OK) {
//             std::string emoji = parseEmojiFromSlackProfile(readBuffer);
//             std::string color = mapEmojiToColor(emoji);
//             std::cout << "Slack emoji: " << emoji << "\n";
//             std::cout << "Simulated LED Color: " << color << "\n";
//         } else {
//             std::cerr << "CURL failed: " << curl_easy_strerror(res) << "\n";
//         }

//         curl_easy_cleanup(curl);
//     }

//     curl_global_cleanup();
//     return 0;
// }
