#pragma once
#include <iostream>

struct FullFWIOptions {
    std::string dir;
    std::string preprocess;
    std::string forward;
    std::string inversion;
    std::string stepdir;
    std::string stepsize;
    bool skippost;
    bool skippre;
    bool skipprocess;

    FullFWIOptions() = delete;

    FullFWIOptions(const std::vector<std::string>& arguments) :
        dir(""),
        preprocess("FiniteDifference"),
        forward("FiniteDifference"),
        inversion("StepAndDirection"),
        stepdir("ConjugateGradient"),
        stepsize("ConjugateGradient"),
        skippost(false),
        skippre(false),
        skipprocess(false)
    {
        for(auto it = arguments.begin(); it!=arguments.end(); ++it)
        {
            if(*it == "-d" or *it == "--dir")
                dir = * ++it;
            else if(*it == "-p" or *it == "--preprocess")
                preprocess = * ++it;
            else if(*it == "-f" or *it == "--forward")
                forward = * ++it;
            else if(*it == "-i" or *it == "--inversion")
                inversion = * ++it;
            else if(*it == "--stepdir")
                stepdir = * ++it;
            else if(*it == "--stepsize")
                stepsize = * ++it;
            else if(*it == "--skip-post")
                skippost = true;
            else if(*it == "--skip-pre")
                skippre = true;
            else if(*it == "--skip-process")
                skipprocess = true;
            else if (*it == "") 
                {}
            else
                throw std::invalid_argument("Illegal argument: " + *it);
        }

        if (dir == "")
            throw std::invalid_argument(
                "Program requires -d or --dir flag specifying the path to the input and output folders."
            );

        
    }
};
