#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <cmath>
#include <set>

using namespace std;

class HeatFlow {
private:
    array<double, 15> temperatures;
    int num_sections;
    double rod_length;
    array<double, 15> kappa;
    double dx;
    double dt;
    set<pair<int, double>> sources_sinks;  

    void update_temperatures() {
        array<double, 15> new_temperatures = temperatures;

        for (int i = 1; i < num_sections - 1; ++i) {
            double d2T_dx2 = (temperatures[i+1] - 2*temperatures[i] + temperatures[i-1]) / (dx * dx);
            new_temperatures[i] += kappa[i] * d2T_dx2 * dt;
        }

        for (const auto& [pos, strength] : sources_sinks) {
            new_temperatures[pos] += strength * dt;
        }

        temperatures = new_temperatures;
    }

    void print_border() const {
        const int width = 10;
        const char top_bottom = '-';
        const char corner = '+';
        cout << corner << string(num_sections * (width + 1) - 1, top_bottom) << corner << '\n';
       }

    void print_temperatures() const {
        const int width = 10;
        const char vertical = '|';
        cout << vertical;
        for (double temp : temperatures) {
            cout << setw(width);
            if (abs(temp) < 1000 && abs(temp) >= 0.01) {
                cout << fixed << setprecision(1) << temp;
            } else {
                cout << scientific << setprecision(2) << temp;
            }
            cout << vertical;
        }
        cout << '\n';
    }

public:
    HeatFlow(double initial_temp, int num_sections, const array<double, 15>& kappa_values, 
             double delta_t)
        : num_sections(num_sections), rod_length(1.0), kappa(kappa_values), dt(delta_t) {
        temperatures.fill(initial_temp);
        dx = rod_length / (num_sections - 1);
    }

    void add_source_sink(int position, double strength) {
        sources_sinks.insert({position, strength});
    }

    void tick() {
        update_temperatures();
    }

    void pretty_print() const {
        print_border();
        print_temperatures();
        print_border();
    }
};

int main() {
    double initial_temperature = 10;
    int number_of_sections = 15;
    double dt = 0.1;

    array<double, 15> kappa_values = {0.1, 0.1, 0.1, 0.1, 0.1, 0.01, 0.01, 0.01, 0.01, 0.01, 0.1, 0.1, 0.1, 0.1, 0.1}; 
    
    HeatFlow h(initial_temperature, number_of_sections, kappa_values, dt);
    
    h.add_source_sink(0, 100.0);
    
    cout << "Initial state:" << endl;
    h.pretty_print();

    for (int i = 0; i < 100; ++i) {
        h.tick();
        if (i % 10 == 9) {
            cout << "After " << (i + 1) * dt << " seconds:" << endl;
            h.pretty_print();
        }
    }

    return 0;
}
