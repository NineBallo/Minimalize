#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "main.h"
#include "regex"
#include <unistd.h>
#include "stdlib.h"

int main(int argc, char** argv) {

    //----------------------------------------------------------------------------//
    //If command is passed as value use that, then run it.
    std::string command = argv[1];
    if (argv[2]){
        command = argv[2];
        command += " &";
    } else{
        command = argv[1];
        command += " &";
    }

    system(command.c_str());
    //----------------------------------------------------------------------------//



    //loop every X ms until window is found and minimized//
    int stopLoop = 0;
    int loops = 0;
    int loopsMax = 0;
    int loopTime = 0;

    argc >= 4 ? loopsMax = std::stoi(argv[3]) : loopsMax = 100;
    argc >= 5 ? loopTime = std::stoi(argv[4]) * 100 : loopTime = 100000;
    //----------------------------------------------------------------------------//
    while (stopLoop == 0){
        //----------------------------------------------------------------------------//
        // Split by line into array
        std::string openWindows = GetStdoutFromCommand("wmctrl -l");
        std::string program = argv[1];

        boost::algorithm::to_lower(openWindows);
        boost::algorithm::to_lower(program);

        std::string minimize;
        std::vector<std::string> strs;
        boost::split(strs,openWindows,boost::is_any_of("\n"));
        //----------------------------------------------------------------------------//

        //----------------------------------------------------------------------------//
        //loop through each string in array checking for argv[1]/window name
        for (int i = 0; i < strs.size(); i++){
            if (strs[i].find(program) != std::string::npos)
            {
                //----------------------------------------------------------------------------//
                //Regex search
                std::regex rgx ("^.{10}");
                std::smatch matches;
                if(std::regex_search(strs[i], matches, rgx)) {
                //----------------------------------------------------------------------------//
                    std::cout << "Match found\n";
                    //----------------------------------------------------------------------------//
                    // Minimalize program then stop loop
                    minimize = "xdotool windowminimize " + matches[0].str();
                    std::cout << minimize << "\n";
                    system(minimize.c_str());
                    stopLoop = 1;
                    //----------------------------------------------------------------------------//
                }
            }
        }
        //----------------------------------------------------------------------------//


        //----------------------------------------------------------------------------//
        //Ok this window dosen't exist, closing program to save resorces. Add to loop counter and sleep for x
        if (loops > loopsMax) {
            std::cout << "Window not found, loops compleated: " << loops;
            stopLoop = 1;
        }
        loops++;
        usleep(loopTime);
        //----------------------------------------------------------------------------//
    }
    return 0;
}

