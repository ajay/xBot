// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <ctime>
// #include <cmath>
// #include <unistd.h>
// #include <sys/types.h>
#include <dirent.h>
#include <termios.h>
// #include <assert.h>
// #include <vector>
// #include <iostream>
#include "xBot.h"
#include <pthread.h>

// #include <cstdio>
// #include <cstring>
// #include <ctime>

#define WBUFSIZE 128
#define DEV_BAUD  B57600
#define SYNC_NSEC 100000000

using namespace arma;

static double limitf(double x, double min, double max);
static void *commHandler(void *args);

bool xBot::connect(void)
{
	DIR *device_dir = opendir("/dev/");
	struct dirent *entry;
	// iterate through all the filenames in the directory,
	// add all the possible connections to the list
	while ((entry = readdir(device_dir)))
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strstr(entry->d_name, "ttyACM"))
		{
			char *pport = new char[strlen("/dev/") + strlen(entry->d_name) + 1];
			sprintf(pport, "/dev/%s", entry->d_name);
			this->pports.push_back(pport);
		}
	}
	closedir(device_dir);
	if (this->pports.size() == 0)
	{
		this->disconnect();
		return -1;
	}

	// when finished adding all the possible filenames,
	// try to connect to a couple of them (NUM_DEV)
	// and identify their ids
	struct timespec synctime;
	synctime.tv_nsec = SYNC_NSEC % 1000000000;
	synctime.tv_sec = SYNC_NSEC / 1000000000;
	for (char *pport : this->pports)
	{
		// connect device
		serial_t *connection = new serial_t;
		serial_connect(connection, pport, DEV_BAUD);
		if (!connection->connected)
		{
			continue;
		}
		else
		{
			this->connections.push_back(connection);
		}
	}

	// read a message from each device
	nanosleep(&synctime, NULL);
	char *msg;
	for (serial_t *connection : this->connections)
	{
		do
		{
			msg = serial_read(connection);
		} while (!msg || strlen(msg) == 0);
	}

	// read another one in case that one was garbage
	nanosleep(&synctime, NULL);
	for (size_t i = 0; i < this->connections.size(); i++)
	{
		serial_t *connection = this->connections[i];
		do
		{
			msg = serial_read(connection);
		} while (!msg || strlen(msg) == 0);

		// if a valid device, add as connected, otherwise disconnect
		int id;
		sscanf(msg, "[%d ", &id);
		if (id > 0)
		{ // make sure the id is not 0
			this->ids.push_back(id);
		}
		else
		{
			serial_disconnect(connection);
			this->connections.erase(this->connections.begin() + i);
			delete connection;
		}
	}

	// disconnect if number of devices is not enough, or there are too many
	if (!this->connected())
	{
		this->disconnect();
		return false;
	}
	else
	{
		printf("connected to all\n");

    // create thread locks and thread
		this->update_thread = new pthread_t;
		this->commSendLock = new pthread_mutex_t;
		this->commRecvLock = new pthread_mutex_t;
		pthread_mutex_init(this->commSendLock, NULL);
		pthread_mutex_init(this->commRecvLock, NULL);

    // start the thread
		pthread_create(this->update_thread, NULL, commHandler, this);

		return true;
	}
}

static void *commHandler(void *args)
{
  xBot *bot = (xBot *)args;

  while (!(bot->startStop))
  {
	vec tempSendVec;
	pthread_mutex_lock(bot->commSendLock);
	tempSendVec = bot->commSend;
	pthread_mutex_unlock(bot->commSendLock);
	bot->threadSend(tempSendVec);

	vec tempRecvVec = bot->threadRecv();
	pthread_mutex_lock(bot->commRecvLock);
	bot->commRecv = tempRecvVec;
	pthread_mutex_unlock(bot->commRecvLock);

	}

  return NULL;
}


/** Default connect detection method
 *  @return true if connected, false otherwise
 */
bool xBot::connected(void)
{
	return this->connections.size() > 0;
}

void xBot::disconnect(void)
{
	if (this->connections.size() > 0)
	{
		for (serial_t *connection : this->connections)
		{
			serial_disconnect(connection);
			delete connection;
		}

		this->connections.clear();
		this->ids.clear();
	}

	if (this->pports.size() > 0)
	{
		for (char *pport : this->pports)
		{
			delete pport;
		}

		this->pports.clear();
	}
	this->robotid = 0;
}

xBot::xBot(void)
{
	this->prev_motion = zeros<vec>(5);
	this->motion_const = ones<vec>(5) * 255.0;
	if (this->connect())
	{
		this->reset();
		this->send(zeros<vec>(5));
	}
}

xBot::~xBot(void)
{
	if (this->connected())
	{
		this->send(zeros<vec>(5));
		this->reset();
		this->disconnect();
	}
}

void xBot::readClear()
{
  /* Idea is here:
	struct timespec synctime;
	synctime.tv_nsec = SYNC_NSEC % 1000000000;
	synctime.tv_sec = SYNC_NSEC / 1000000000;

	nanosleep(&synctime, NULL);
	char *msg;
	for (serial_t *connection : this->connections)
	{
		do
		{
			msg = serial_read(connection);
			printf("Read message was: %s\n", msg);
		} while (!msg || strlen(msg) == 0);
	}
  */

  int devid;

  // Go through every device
  for (size_t i = 0; i < this->connections.size(); i++) {
    switch ((devid = this->ids[i])) {
      case 1:
      case 2:
        serial_read(this->connections[i]); // just read everything, do nothing with it
        break;
      default:
        break;
    }
  }

	return;
}

int xBot::numconnected(void)
{
	return this->connections.size();
}

void xBot::reset(void)
{
	this->prev_motion.zeros();
}

void xBot::send(const vec &motion)
{
  // lock the data before setting it...avoids the thread from read the motion vector
  // before it finishes copying over
	pthread_mutex_lock(this->commSendLock);
	this->commSend = motion;
	pthread_mutex_unlock(this->commSendLock);
}

void xBot::threadSend(const vec &motion)
{
	vec new_motion = motion;
	// safety check
	if (new_motion.n_elem != motion_const.n_elem)
	{
		new_motion = zeros<vec>(motion_const.n_elem);
	}

	// boundary check
	for (int i = 0; i < (int)new_motion.n_elem; i++)
	{
		new_motion(i) = limitf(new_motion(i), -1.0, 1.0);
	}

	new_motion %= motion_const;

	char msg[WBUFSIZE];
	for (int i = 0; i < (int)this->connections.size(); i++)
	{
		switch (this->ids[i])
		{
			case 1: // Drive base
				// dont send dup speeds
				if (new_motion(0) == this->prev_motion(0) &&
						new_motion(1) == this->prev_motion(1) &&
						new_motion(2) == this->prev_motion(2) &&
						new_motion(3) == this->prev_motion(3))
				{
	//          if (new_motion(0) != 0 || new_motion(1) != 0 || new_motion(2) != 0 || new_motion(3) != 0)
	//            break;
				}
				else
				{
					this->prev_motion(0) = new_motion(0);
					this->prev_motion(1) = new_motion(1);
					this->prev_motion(2) = new_motion(2);
					this->prev_motion(3) = new_motion(3);

					sprintf(msg, "[%d %d %d %d]\n",
							(int)new_motion(0),
							(int)new_motion(1),
							(int)new_motion(2),
							(int)new_motion(3));
					serial_write(this->connections[i], msg);
				}
				break;
			case 2: // Arduino #2
				// new_motion(4) == this->prev_motion(4);
				serial_write(this->connections[i], msg);
				sprintf(msg, "[%d]\n", (int)new_motion(4));
				serial_write(this->connections[i], msg);
			default:
				break;
		}
	}
}

vec xBot::threadRecv(void)
{
	return zeros<vec>(5);
}

vec xBot::recv(void)
{
  // add a lock to wait until the commthread is done setting the vector
  vec tempVec;
  pthread_mutex_lock(this->commRecvLock);
	tempVec = this->commRecv;
  pthread_mutex_unlock(this->commRecvLock);
  return tempVec;
}

static double limitf(double x, double min, double max)
{
	if (x < min)
	{
		return min;
	}
	else if (x > max)
	{
		return max;
	}
	else
	{
		return x;
	}
}
