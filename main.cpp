#include <bits/stdc++.h>
#include "crow.h"

#define fast_io std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
constexpr size_t MAX_FILE_SIZE = 6 * 1024 * 1024; // 6 MB

// 🔧 CORS middleware - FIXED
struct CORS {
    struct context {};
    void before_handle(crow::request&, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PATCH");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Content-Length");
        res.add_header("Access-Control-Max-Age", "86400");
    }
    void after_handle(crow::request&, crow::response& res, context&) {
        // Ensure CORS headers are always present
        if (res.get_header_value("Access-Control-Allow-Origin").empty()) {
            res.add_header("Access-Control-Allow-Origin", "*");
        }
    }
};

// 🔑 id generator
std::string generate_id(size_t len = 12) {
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_int_distribution<> dist(0, sizeof(alphanum) - 2);
    std::string id;
    id.reserve(len);
    for (size_t i = 0; i < len; ++i) id += alphanum[dist(gen)];
    return id;
}

std::map<std::string, std::chrono::system_clock::time_point> files;

int main() {
    fast_io;

    if (!std::filesystem::exists("uploads"))
        std::filesystem::create_directory("uploads");

    crow::App<CORS> app;

    CROW_ROUTE(app, "/")([] {
        return "server online\n";
    });

    CROW_ROUTE(app, "/add/<int>/<int>").methods(crow::HTTPMethod::GET, crow::HTTPMethod::PATCH)
    ([](int a, int b) {
        return std::to_string(a + b) + "\n";
    });

    // ✅ OPTIONS preflight - Add explicit CORS headers
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::OPTIONS)
    ([](const crow::request&, crow::response& res){
        res.code = 204;
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PATCH");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Content-Length");
        res.add_header("Access-Control-Max-Age", "86400");
        res.end();
    });

    // ✅ File upload - Add explicit CORS headers
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto content_length_header = req.get_header_value("Content-Length");
        if (content_length_header.empty()) {
            crow::response res(411, "No data received\n");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }

        size_t content_length = std::stoul(content_length_header);
        if (content_length > MAX_FILE_SIZE) {
            crow::response res(413, "File too large, max file size is 6MB\n");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }

        std::string id = generate_id();
        std::string path = "uploads/" + id;

        std::ofstream file(path, std::ios::binary);
        file.write(req.body.c_str(), req.body.size());
        file.close();

        auto current_time = std::chrono::system_clock::now();
        files[id] = current_time + std::chrono::hours(1);

        std::string base_url = req.get_header_value("Host");
        if (base_url.empty()) base_url = "localhost:18080";

        std::string full_url = "https://" + base_url + "/uploads/" + id + "\n";
        crow::response res(200, full_url);
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    // ✅ File serve - Add explicit CORS headers
    CROW_ROUTE(app, "/uploads/<string>").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req, std::string id) {
        auto it = files.find(id);
        if (it == files.end() || it->second < std::chrono::system_clock::now()) {
            crow::response res(404, "file not found or expired\n");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }

        std::string path = "uploads/" + id;
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            crow::response res(404, "file not found\n");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();

        crow::response res(200);
        res.body = content;
        res.add_header("Content-Type", "application/octet-stream");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    // ✅ Expired file cleanup
    CROW_ROUTE(app, "/checker").methods(crow::HTTPMethod::GET)
    ([](const crow::request&) {
        auto now = std::chrono::system_clock::now();
        for (auto it = files.begin(); it != files.end(); ) {
            if (it->second < now) {
                std::filesystem::remove("uploads/" + it->first);
                it = files.erase(it);
            } else {
                ++it;
            }
        }
        return crow::response(200, "cleanup done\n");
    });

    app.port(18080).multithreaded().run();
}
