// table.h
#ifndef TABLE_H
#define TABLE_H

class Table {
private:
    int id;
    bool isReserved;
    int waiterId;

public:
    Table(int id);
    ~Table() = default;

    int getId() const;
    bool getIsReserved() const;
    void setIsReserved(bool isReserved);
    
    int getWaiterId() const;
    void setWaiterId(int waiterId);
};

#endif // TABLE_H