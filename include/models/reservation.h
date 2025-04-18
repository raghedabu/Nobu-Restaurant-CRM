// reservation.h
#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

class Reservation {
private:
    int id;
    int customerId;
    int tableId;
    std::string time;

public:
    Reservation(int id, int customerId, int tableId, const std::string& time);
    ~Reservation() = default;

    int getId() const;
    int getCustomerId() const;
    int getTableId() const;
    std::string getTime() const;
};

#endif // RESERVATION_H