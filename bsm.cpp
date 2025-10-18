// Black 0+Scholes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <chrono>

const double PI = 3.14159265358979323846;
const double SQRT_2PI = std::sqrt(2.0 * PI);
const double INV_SQRT_2 = 0.70710678118654752440; // 1/sqrt(2)

struct OptionParameters {
    double S, K, T, r, sigma;
};
std::ostream& operator<<(std::ostream& os, const OptionParameters& p) {
    os
        << "S: " << p.S
        << ", K: " << p.K
        << ", T: " << p.T
        << ", r: " << p.r
        << ", sigma: " << p.sigma;
    return os;
}

struct Results {
    double call, put, delta, gamma, vega, theta, rho;
};

double norm_cdf(double x) {
    return 0.5 * std::erfc(-x * INV_SQRT_2);
}

Results black_scholes(const OptionParameters& p) {
    double d1 = (log(p.S / p.K) + (p.r + 0.5 * p.sigma * p.sigma) * p.T) / (p.sigma * sqrt(p.T));
    double d2 = d1 - p.sigma * sqrt(p.T);

    double call = p.S * norm_cdf(d1) - p.K * exp(-p.r * p.T) * norm_cdf(d2);
    double put = p.K * exp(-p.r * p.T) * norm_cdf(-d2) - p.S * norm_cdf(-d1);

    // Greeks (simplified)
    double delta = norm_cdf(d1);
    double gamma = std::exp(-0.5 * d1 * d1) / (p.S * p.sigma * std::sqrt(p.T) * SQRT_2PI);
    double vega = p.S * std::exp(-0.5 * d1 * d1) * std::sqrt(p.T) / SQRT_2PI;
    double theta = -(p.S * p.sigma * std::exp(-0.5 * d1 * d1)) / (2 * std::sqrt(p.T) * SQRT_2PI) - p.r * p.K * std::exp(-p.r * p.T) * norm_cdf(d2);
    double rho = p.K * p.T * exp(-p.r * p.T) * norm_cdf(d2);

    return { call, put, delta, gamma, vega, theta, rho };
};

int main()
{
    std::cout << "Hello World!\n";

    std::ifstream inf{ "data.csv" };
    std::ofstream ouf{ "results.csv" };

    if (!inf) {
        std::cout << "Uh oh, Sample.txt could not be opened for writing!\n";
        return 1;

    }

    std::string strInput{};
    double total;



    std::vector<OptionParameters> params;


    std::getline(inf, strInput);


    while (std::getline(inf, strInput))
    {
        OptionParameters p;
        char comma;
        
        std::stringstream lineStream(strInput);
        std::string cell;
        total = 0;
        lineStream >> p.S >> comma >> p.K >> comma >> p.T >> comma >> p.r >> comma >> p.sigma;
        /*
        while (std::getline(lineStream, cell, ',')) {
            std::cout << cell << '\n';
            total += stod(cell);
        }
        */

        params.push_back(p);
        
        /*
        for (int i = 0; i < params.size(); i++)
            std::cout << params[i] << '\n';
        */
        //std::cout << strInput << " - Total: " << total << '\n';
    }
    auto start = std::chrono::high_resolution_clock::now();

    ouf << "Call,Put,Delta,Gamma,Vega,Theta,Rho\n";
    for (auto& p : params) {
        Results r = black_scholes(p);
        ouf << r.call << "," << r.put << "," << r.delta << "," << r.gamma
            << "," << r.vega << "," << r.theta << "," << r.rho << "\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Execution time: " << diff.count() << " s\n";

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
