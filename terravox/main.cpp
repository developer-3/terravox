//
//  main.cpp
//  ecs_game
//
//  Created by Adam Anderson on 4/5/23.
//

#include "src/application.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    vpr::Application app{};

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
