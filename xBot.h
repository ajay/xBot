#ifndef XBOT_H
#define XBOT_H

#include <armadillo>
#include "serial.h"
// #include "baserobot.h"

class xBot
{
	public:
		xBot(void);
		~xBot(void);
		int numconnected(void);
		void send(const arma::vec &motion); // [topleft topright botleft botright arm]
		arma::vec recv(void);
		void reset(void);
		int id(void);
		bool connect(void);
		bool connected(void);
		void disconnect(void);
		// virtual void send(const arma::vec &motion);
		// virtual arma::vec recv(void);
		// virtual void reset(void);

	private:
		arma::vec prev_motion;
		arma::vec motion_const;
		int robotid;
		std::vector<serial_t *> connections;
		std::vector<int> ids;
		std::vector<char *> pports;
};

#endif