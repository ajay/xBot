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
		void readClear(void);
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

		pthread_t *update_thread;
		pthread_mutex_t *commSendLock;
		pthread_mutex_t *commRecvLock;
		arma::vec prev_motion;
		arma::vec motion_const;
		arma::vec commSend;
		arma::vec commRecv;
		int robotid;
		std::vector<serial_t *> connections;
		std::vector<int> ids;
		std::vector<char *> pports;
};

#endif