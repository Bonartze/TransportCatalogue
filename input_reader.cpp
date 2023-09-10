#include "input_reader.h"

namespace InputData {
    using namespace std;

    RouteImitation::TransportCatalogue &Parse::Parser::GetTransportCatalogue() {
        return transportCatalogue;
    }

    void Parse::Parser::CutSpaces(std::string &str) {
        auto pos = str.find_first_not_of(' ');
        str = str.substr(pos, str.size() - pos + 1);
        pos = str.find_last_not_of(' ');
        str = str.substr(0, pos + 1);
    }

    RouteImitation::TransportCatalogue &FillTransportManual::GetTransportCataloguePars() {
        return parser.GetTransportCatalogue();
    }

    void Parse::Parser::CoordinatesAndStopName(stringstream &line) {
        string name_from, name_to;
        double x, y;
        getline(line, name_from, ':');
        string name;
        line.ignore(1);
        line >> x;
        line.ignore(1);
        line >> y;
        line.ignore(1);
        size_t distance_to;
        transportCatalogue.AddStop(name_from, {x, y});
        while (line >> distance_to) {
            line.ignore(1);
            std::string to;
            line >> to;
            line.ignore(1);
            getline(line, name_to, ',');
            transportCatalogue.AddStopDistances(name_from, name_to, distance_to);
            line.ignore(1);
        }
        transportCatalogue.AddStop(name, {x, y});
    }

    void Parse::Parser::BusNumberAndRoute(std::stringstream &line) {
        size_t number;
        line >> number;
        if (line.str().find('-') != std::string::npos) {
            transportCatalogue.AddTag(number, 'u'); //uncycle route
            string name;
            line.ignore(2);
            while (getline(line, name, '-')) {
                CutSpaces(name);
                transportCatalogue.AddBusRouteStop(number, name);
            }
        } else {
            transportCatalogue.AddTag(number, 'c'); //cycle route
            string name;
            line.ignore(2);
            while (getline(line, name, '>')) {
                CutSpaces(name);
                transportCatalogue.AddBusRouteStop(number, name);
            }
        }

    }

    void FillTransportManual::GetData() {
        size_t n, i = 0;
        cin >> n;
        string line;
        getline(cin, line);
        while (i++ < n) {
            getline(cin, line, '\n');
            data += line + '\n';
        }
    }

    void FillTransportManual::CallTransportManual() {
        string key_word;
        stringstream data_stream(data);
        while (data_stream >> key_word) {
            data_stream.ignore(1);
            if (key_word == "Stop") {
                string line;
                getline(data_stream, line, '\n');
                stringstream line_stream(line);
                parser.CoordinatesAndStopName(line_stream);
            } else {
                string line;
                getline(data_stream, line);
            }
        }
        stringstream new_data_stream(data);
        key_word.clear();
        while (new_data_stream >> key_word) {
            string line;
            if (key_word == "Bus") {
                new_data_stream.ignore(1);
                getline(new_data_stream, line, '\n');
                stringstream line_stream(line);
                parser.BusNumberAndRoute(line_stream);
            } else
                getline(new_data_stream, line, '\n');
        }
    }
}