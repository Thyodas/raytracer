/*
** EPITECH PROJECT, 2023
** raytracer_obj
** File description:
** Parser.hpp
*/

#pragma once

#include "../src/Core.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <unordered_map>
#include <regex>

namespace Parser {

    namespace ObjParserData {
        struct TransformationsOptions;
    }

    class AException : public std::exception {
        public:
            [[nodiscard]] const char* what() const noexcept override {
                return _message.c_str();
            };
        protected:
            explicit AException(const std::string& message)
                : _message(message) {};
            std::string _message;
    };

    class OpenFailureException : public AException {
        public:
            explicit OpenFailureException(const std::string &message)
                : AException(message) {};
    };

    class ParseFailureException : public AException {
        public:
            explicit ParseFailureException(const std::string &message)
                : AException(message) {};
    };

    template<typename T>
    using Command = std::function<void(T &accumulator,
        std::vector<std::string> &argv)>;

    template<typename T>
    class AParser {
        public:
            virtual ~AParser();
            AParser() {};

            void addCommand(const std::string &name, Command<T> function);

            void parse(const std::string &path);

            T &getData();
            T _accumulator;
        protected:
            std::string _path;
            std::ifstream _file;
            std::unordered_map<std::string, Command<T>> _commands;
        private:
            void openFile(const std::string &path);
            void closeFile();
    };

    template<typename T>
    void AParser<T>::parse(const std::string &path)
    {
        openFile(path);

        std::string buffer;
        for (int l = 1; std::getline(_file, buffer); ++l) {
            std::regex token_regex("\\s+");
            std::sregex_token_iterator tokens(buffer.begin(), buffer.end(), token_regex, -1);
            std::sregex_token_iterator end;
            std::vector<std::string> argv;
            if (tokens == end || tokens->str().length() == 0
                || tokens->str()[0] == '#')
                continue;
            for (; tokens != end; ++tokens) {
                argv.push_back(*tokens);
            }
            try {
                if (!_commands.contains(argv.at(0))) {
                    throw ParseFailureException("unknown command '"
                        + argv.at(0) + "'");
                }
                _commands.at(argv.at(0))(_accumulator, argv);
            } catch (std::exception &e) {
                throw ParseFailureException(
                    "couldn't parse file '" + _path + "' command '"
                    + argv.at(0) + "' at line "
                    + std::to_string(l) + ": " + e.what()
                );
            }
        }

        closeFile();
    }

    template<typename T>
    T &AParser<T>::getData()
    {
        return _accumulator;
    }

    template<typename T>
    AParser<T>::~AParser()
    {
        AParser::closeFile();
    }

    template<typename T>
    void AParser<T>::openFile(const std::string &path)
    {
        if (_file.is_open())
            closeFile();
        _path = path;
        _file.open(_path, std::ios::in);

        if (_file.fail())
            throw Parser::OpenFailureException(
                "couldn't open '" + _path + "': " + strerror(errno));

    }

    template<typename T>
    void AParser<T>::closeFile()
    {
        if (!_file.is_open())
            return;
        _file.close();
    }

    template<typename T>
    void Parser::AParser<T>::addCommand(const std::string &name,
        Parser::Command<T> function)
    {
        _commands[name] = function;
    }

    int parseObj(raytracer::Core &core, Parser::ObjParserData::TransformationsOptions &opt,
                 const std::string& filePath, bool debug = false);

} // Parser
