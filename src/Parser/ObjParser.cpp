/*
** EPITECH PROJECT, 2023
** Project
** File description:
** Parser.cpp
*/

#include "ObjParser.hpp"

namespace Parser {
    int parseObj(const std::string& filePath)
    {
        try {
            Parser::ObjParser test;
            test.parse(filePath);
        } catch (std::exception &e) {
            std::cerr << "raytracer: " << e.what() << "." << std::endl;
            return 84;
        }
        return 0;
    }

    void command_mtllib(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() != 2)
            throw Parser::ParseFailureException("invalid number of arguments");
    }

    void command_v(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() < 4 || argv.size() > 5)
            throw Parser::ParseFailureException("invalid number of arguments");
        Parser::ObjParserData::Vertices vertices = {
            std::stod(argv[1]), std::stod(argv[2]), std::stod(argv[3])};
        if (argv.size() == 5)
            vertices.w = std::stod(argv[4]);
        data.objects[data.obj_index].vertices.push_back(vertices);
    }

    void command_o(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() != 2)
            throw Parser::ParseFailureException("invalid number of arguments");
        Parser::ObjParserData::Object obj{.name = argv[1]};
        data.objects.push_back(obj);
    }

    ObjParser::ObjParser()
    {
        _accumulator.objects.push_back({});

        addCommand("mtllib", &command_mtllib);
        addCommand("v", &command_v);
        addCommand("o", &command_o);
    }
}


