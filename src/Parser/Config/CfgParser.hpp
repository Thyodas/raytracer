/*
** EPITECH PROJECT, 2023
** RayTracer
** File description:
** CfgParser.hpp
*/

#pragma once

#include <Parser.hpp>
#include <libconfig.h++>
#include <functional>
#include <vector>
#include <memory>

#include "math/Vectors/Vec3.hpp"
#include "../../Primitives/Sphere/Sphere.hpp"
#include "../../Camera/Camera.hpp"
#include "../Object/ObjParser.hpp"

namespace Parser {

    namespace CfgParserData {

        typedef raytracer::Camera Camera;

        typedef std::vector<std::shared_ptr<primitive::Object>> Primitives;

        typedef std::vector<std::shared_ptr<physics::Light>> Lights;

    }

    class CfgParser {
        public:
            CfgParser(std::string path);
            ~CfgParser() = default;

            CfgParserData::Camera getCamera();

            CfgParserData::Primitives getPrimitives();

            CfgParserData::Lights getLights();

        private:
            template<typename Func>
            auto _parser_wrapper(const std::string &scope, Func func)
            {
                return [scope, func, this](auto&&... args) {
                    try {
                        return func(std::forward<decltype(args)>(args)...);
                    } catch (libconfig::ParseException &e) {
                        throw Parser::ParseFailureException(
                            scope + ": couldn't parse '" + _path + "' line "
                            + std::to_string(e.getLine()) + ": " + e.getError());
                    } catch (libconfig::SettingNotFoundException &e) {
                        throw Parser::ParseFailureException(
                            scope + ": couldn't parse '" + _path + "' invalid setting '"
                            + e.getPath() + "'");
                    } catch (libconfig::SettingTypeException &e) {
                        throw Parser::ParseFailureException(
                            scope + ": couldn't parse '" + _path + "' invalid type for setting '"
                            + e.getPath() + "'");
                    } catch (std::exception &e) {
                        throw Parser::ParseFailureException(
                            scope + ": couldn't parse '" + _path + "': " + e.what());
                    }
                };
            }

            void _getObjectBase(primitive::Object &object, const libconfig::Setting &setting);
            std::shared_ptr<primitive::Sphere> _getSphere(const libconfig::Setting &setting);
            std::vector<std::shared_ptr<primitive::MeshTriangle>> _getObj(const libconfig::Setting &setting);

            std::shared_ptr<physics::DistantLight> _getDistantLight(const libconfig::Setting &setting);
            std::shared_ptr<physics::PointLight> _getPointLight(const libconfig::Setting &setting);

            std::string _path;
            libconfig::Config _config;
    };
} // Parser
