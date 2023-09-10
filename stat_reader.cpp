#include "stat_reader.h"

#define AllRoutes GetTransportCataloguePars().GetRoutes()
#define AllStops GetTransportCataloguePars().GetStops()

namespace Display {
    void Data::RequestData(InputData::FillTransportManual &input) {
        size_t n, i = 0;
        std::cin >> n;
        std::string data;
        std::string ln;
        getline(std::cin, ln, '\n');
        while (i++ < n) {
            std::string line;
            getline(std::cin, line, '\n');
            data += line + '\n';
        }
        std::stringstream data_stream(data), output;
        std::string line;
        while (getline(data_stream, line)) {
            size_t number;
            std::stringstream line_stream(line);
            std::string request;
            line_stream >> request;
            if (request == "Bus") {
                line_stream >> number;
                if (input.AllRoutes.find(number) !=
                    std::end(input.AllRoutes)) {
                    output << "Bus " << number << ": " << input.AllRoutes[number]->stops_number << " stops on route, "
                           << input.AllRoutes[number]->unique_stops.size() << " unique stops, "
                           << input.AllRoutes[number]->route_length_computed << " route length, "
                           << input.AllRoutes[number]->curvature << " curvature\n";
                } else
                    output << "Bus " << number << ": not found\n";
            } else {
                std::string stop_name;
                line_stream.ignore(1);
                getline(line_stream, stop_name, ':');
                output << "Stop " << stop_name << ": ";
                if (input.AllStops.find(stop_name) != end(input.AllStops)) {
                    if (input.GetTransportCataloguePars().GetBusesInStop(stop_name).empty())
                        output << "no buses\n";
                    else {
                        output << "buses ";
                        for (auto &numbers: input.GetTransportCataloguePars().GetBusesInStop(stop_name))
                            output << numbers << ' ';
                        output << '\n';
                    }
                } else {
                    output << "not found\n";
                }
            }
        }
        std::cout << output.str();
    }
}