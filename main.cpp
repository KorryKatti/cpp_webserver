#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "testing a very small change i or did ?";
    });

    app.port(18080).multithreaded().run();
}
