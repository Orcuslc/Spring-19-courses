#include "1d_linear_hydrodynamics.h"
#include <valarray>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

valarray<double> lax_step(const valarray<double>& u, const valarray<double>& p, const double dx, const double dt) {
	valarray<double> u1(0., u.size()), dp(0., p.size());

	// compute the time derivative at each spatial point
	// du/dt = -dp;
	dp[slice(1, p.size()-2, 1)] = (p[slice(2, p.size()-1, 1)] - p[slice(0, p.size()-3, 1)])/(2*dx);
	dp[0] = (p[1] - p[p.size()-2])/(2*dx);
	dp[p.size()-1] = (p[1] - p[p.size()-2])/(2*dx);

	// update with Lax scheme
	u1[slice(1, u.size()-2, 1)] = 0.5*(u[slice(2, u.size()-1, 1)] + u[slice(0, u.size()-3, 1)]) - valarray<double>(dp[slice(1, p.size()-2, 1)])*dt;
	u1[0] = 0.5*(u[1]+u[u.size()-2]) - dt*dp[0];
	u1[u.size()-1] = 0.5*(u[1]+u[u.size()-2]) - dt*dp[p.size()-1];

	return u1; 
}

void lax(const valarray<double>& xspan, const valarray<double>& tspan, const int nx, const double dt) {
	double dx = (xspan[1]-xspan[0])/(double)nx;
	vector<double> nodes;
	for(int i = 0; i < nx; i++) {
		nodes.push_back(xspan[0]+(double)i*dx);
	}
	nodes.push_back(xspan[1]);

	for(auto c: nodes)
		cout << c << endl;
	// initialize
	valarray<double> x(nodes.data(), nodes.size());
	valarray<double> rho = rho_initial(x), u = u_initial(x), p = p_initial(x);
	write(save_path, rho);
	write(save_path, u);
	write(save_path, p);

	// update snapshots
	int nt = floor((tspan[1] - tspan[0])/dt);
	for(int n = 0; n < nt; n++) {
		cout << n << endl;
		rho = lax_step(rho, u, dx, dt);
		valarray<double> u_copy(u);
		u = lax_step(u, p, dx, dt);
		p = lax_step(p, u_copy, dx, dt);
		write(save_path, rho);
		write(save_path, u);
		write(save_path, p);
	}
}

void write(string path, valarray<double>& data) {
	ofstream ofs (path, ios::app);	
	for(auto c: data) {
		ofs << c << ",";
	}
	ofs << "\n";
}