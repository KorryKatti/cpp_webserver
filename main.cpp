#include <bits/stdc++.h>
#include "crow.h"

#define fast_io std::ios::sync_with_stdio(false); std::cin.tie(nullptr);

constexpr size_t MAX_FILE_SIZE = 6 * 1024 * 1024; // 6 MB

std::string generate_id(size_t len = 12) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::uniform_int_distribution<> dist(0, sizeof(alphanum) - 2);

    std::string id;
    id.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        id += alphanum[dist(generator)];
    }
    return id;
}

std::map<std::string, std::chrono::system_clock::time_point> files;

int main() {
    fast_io;

    if (!std::filesystem::exists("uploads")) {
        std::filesystem::create_directory("uploads");
    }

    crow::SimpleApp app;

    // test route
    CROW_ROUTE(app, "/")([]() {
        return "testing a very small change i or did ?\n";
    });

    // add route
    CROW_ROUTE(app, "/add/<int>/<int>")
    .methods(crow::HTTPMethod::GET, crow::HTTPMethod::PATCH)
    ([](int a, int b) {
        return std::to_string(a + b) + "\n";
    });

    // file upload
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto content_length_header = req.get_header_value("Content-Length");
        if (content_length_header.empty()) {
            return crow::response(411, "No data received\n");
        }

        size_t content_length = std::stoul(content_length_header);
        if (content_length > MAX_FILE_SIZE) {
            return crow::response(413, "File too large, max file size is 6MB\n");
        }

        std::string id = generate_id();
        std::string path = "uploads/" + id;

        std::ofstream file(path, std::ios::binary);
        file.write(req.body.c_str(), req.body.size());
        file.close();

        auto current_time = std::chrono::system_clock::now();
        auto expiry_time = current_time + std::chrono::hours(1);
        files[id] = expiry_time;

        std::string base_url = req.get_header_value("Host");
        if (base_url.empty()) base_url = "localhost:18080";

        std::string full_url = "http://" + base_url + "/uploads/" + id + "\n";
        return crow::response(200, full_url);
    });

    // serve file route
    CROW_ROUTE(app, "/uploads/<string>").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req, std::string id) {
        auto it = files.find(id);
        if (it == files.end() || it->second < std::chrono::system_clock::now()) {
            return crow::response(404, "file not found or expired\n");
        }

        std::string path = "uploads/" + id;
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            return crow::response(404, "file not found\n");
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();

        crow::response res(200);
        res.body = content;
        res.add_header("Content-Type", "application/octet-stream");
        return res;
    });

    // cleanup expired files
    CROW_ROUTE(app, "/checker").methods(crow::HTTPMethod::GET)
    ([](const crow::request&) {
        auto now = std::chrono::system_clock::now();
        for (auto it = files.begin(); it != files.end(); ) {
            if (it->second < now) {
                std::string path = "uploads/" + it->first;
                std::filesystem::remove(path);
                it = files.erase(it);
            } else {
                ++it;
            }
        }
        return crow::response(200, "cleanup done\n");
    });

    app.port(18080).multithreaded().run();
}
