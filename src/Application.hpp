#pragma once

namespace aw
{
class Application final
{
public:
    Application(int argc, char *argv[]);

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    int run();
};
} // namespace aw
