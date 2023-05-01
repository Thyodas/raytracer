/*
** EPITECH PROJECT, 2023
** Project
** File description:
** Parser.cpp
*/

#include "ObjParser.hpp"
#include "../Primitives/MeshTriangle/MeshTriangle.hpp"
#include <limits>
#include <memory>

namespace Parser {
    inline void printObjects(Parser::ObjParser &parser)
    {
        Parser::ObjParserData::Data data = parser.getData();
        for (int i = 0; i < data.objects.size(); ++i) {
            std::cout << "Objet name: " << data.objects[i].name << std::endl;
            std::cout << "Vertex array : " << std::endl;
            for (int j = 0; j < data.objects[i].vertexArray.size(); ++j)
                std::cout << data.objects[i].vertexArray[j] << std::endl;
            std::cout << std::endl;
            std::cout << "Face index array:" << std::endl;
            for (int j = 0; j < data.objects[i].faceIndex.size(); ++j)
                std::cout << data.objects[i].faceIndex[j] << std::endl;
            std::cout << std::endl;
            std::cout << "Vertex index array" << std::endl;
            for (int j = 0; j < data.objects[i].vertexIndex.size(); ++j)
                std::cout << data.objects[i].vertexIndex[j] << std::endl;
            std::cout << std::endl;
            std::cout << "Normals array" << std::endl;
            for (int j = 0; j < data.objects[i].normals.size(); ++j)
                std::cout << data.objects[i].normals[j] << std::endl;
            std::cout << std::endl;
            std::cout << "St array" << std::endl;
            for (int j = 0; j < data.objects[i].st.size(); ++j)
                std::cout << data.objects[i].st[j] << std::endl;
        }
    }
    int parseObj(raytracer::Core &core, const std::string& filePath, bool debug)
    {
        try {
            Parser::ObjParser test;
            test.parse(filePath);
            if (debug)
                printObjects(test);
            Parser::ObjParserData::Data data = test.getData();
            for (size_t i = 0; i < data.objects.size(); ++i) {
                primitive::MeshTriangle *mesh =
                    new primitive::MeshTriangle(data.objects[i].faceIndex,
                                                data.objects[i].vertexIndex,
                                                data.objects[i].vertexArray,
                                                data.objects[i].normals,
                                                data.objects[i].st);
                core.addObject(std::shared_ptr<primitive::MeshTriangle>(mesh));
            }
        } catch (std::exception &e) {
            std::cerr << "raytracer: " << e.what() << "." << std::endl;
            return {};
        }
    }

    void command_mtllib(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() != 2)
            throw Parser::ParseFailureException("invalid number of arguments");
    }

    void command_f(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() < 4)
            throw Parser::ParseFailureException("invalid number of arguments");

        for (int i = 1; i < argv.size(); ++i) {
            std::vector<std::string> indices;
            std::stringstream ss(argv[i]);
            std::string index;
            while (std::getline(ss, index, '/'))
                indices.push_back(index);
            int vertexIndex = std::stoi(indices[0]) - 1;
            int texCoordIndex = std::numeric_limits<int>::max();
            if (indices.size() > 1 && !indices[1].empty())
                texCoordIndex = std::stoi(indices[1]) - 1;
            int normalIndex = std::numeric_limits<int>::max();
            if (indices.size() > 2 && !indices[2].empty())
                normalIndex = std::stoi(indices[2]) - 1;
            data.objects[data.obj_index].vertexIndex.push_back(vertexIndex);
            if (texCoordIndex != std::numeric_limits<int>::max() && std::abs(texCoordIndex) > data.tmpSt.size())
                throw Parser::ParseFailureException("Texture coordinates invalid");
            if (texCoordIndex != std::numeric_limits<int>::max())
                data.objects[data.obj_index].st.push_back(data.tmpSt[texCoordIndex]);
            if (normalIndex != std::numeric_limits<int>::max() && std::abs(normalIndex) > data.tmpNormals.size())
                throw Parser::ParseFailureException("Normal coordinates invalid");
            if (normalIndex != std::numeric_limits<int>::max())
                data.objects[data.obj_index].normals.push_back(data.tmpNormals[normalIndex]);
        }
        data.objects[data.obj_index].faceIndex.push_back(argv.size() - 1);
    }

    void command_vt(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() != 3)
            throw Parser::ParseFailureException("invalid number of arguments");
        Vec2f txtCoord(std::stof(argv[1]), std::stof(argv[2]));
        data.tmpSt.push_back(txtCoord);
    }

    void command_vn(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() != 4)
            throw Parser::ParseFailureException("invalid number of arguments");
        Vec3f normalCoord(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]));
        data.tmpNormals.push_back(normalCoord);
    }

    void command_v(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() < 4 || argv.size() > 5)
            throw Parser::ParseFailureException("invalid number of arguments");
        Vec3f vertice(std::stod(argv[1]), std::stod(argv[2]), -std::fabs(std::stod(argv[3])));
        data.objects[data.obj_index].vertexArray.push_back(vertice);
    }

    void command_o(Parser::ObjParserData::Data &data, std::vector<std::string> &argv)
    {
        if (argv.size() != 2)
            throw Parser::ParseFailureException("invalid number of arguments");
        if (data.obj_index == 0 && data.objects[0].name == "Default Object") {
            data.objects[0].name = argv[1];
            return;
        }
        Parser::ObjParserData::Object obj{.name = argv[1]};
        data.objects.push_back(obj);
        data.obj_index++;
        data.tmpNormals.clear();
        data.tmpSt.clear();
        data.tmpVertices.clear();
    }

    ObjParser::ObjParser()
    {
        _accumulator.objects.push_back({});

        addCommand("mtllib", &command_mtllib);
        addCommand("v", &command_v);
        addCommand("o", &command_o);
        addCommand("vt", &command_vt);
        addCommand("vn", &command_vn);
        addCommand("f", &command_f);
    }
}


