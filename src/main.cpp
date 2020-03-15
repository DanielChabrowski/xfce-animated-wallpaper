#include "Application.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    try
    {
        aw::Application app{ argc, argv };
        return app.run();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
