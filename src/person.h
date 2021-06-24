#ifndef _PERSON_H_
#define _PERSON_H_

#include <vector>

#include "utils.h"
#include "global.h"
#include "definations.h"

enum InfectType
{
    kHealthy = 0,
    kInfected = 1,
    kConfirmd = 2,
    kHospitalized = 3,
    kHealed = 4,
    kDead = 5,
    kObserved = 6,
};

class Person
{
public:
    int id_ = -1;
    size_t stay_time = 1;
    InfectType status_;
    AreaType area_ = kDomitory;
    bool vaccinated_;
    std::vector<Person *> neighborhood_;
    Grid *belonging_grid_;
    Point destination_;
    Person();
    Person(const Person &p);
    virtual ~Person();

    virtual void StateChange(int index) = 0;
};

class HealthyPerson : public Person
{
public:
    void StateChange(int index);
    HealthyPerson();
    HealthyPerson(const Person &p);
    ~HealthyPerson();
};

class InfectedPerson : public Person
{
public:
    int incubate_time_ = -1;
    InfectedPerson(const HealthyPerson &p, int incubate_time);
    void StateChange(int index);
    ~InfectedPerson();
};

class ObservedPerson : public Person
{
public:
    int incubate_time_ = -1;
    void StateChange(int index);
    ObservedPerson(const InfectedPerson &p);
    ~ObservedPerson();
};

class ConfirmedPerson : public Person
{
public:
    void StateChange(int index);
    ConfirmedPerson(const InfectedPerson &p);
    ConfirmedPerson(const ObservedPerson &p);
    ~ConfirmedPerson();
};

class HospitalizedPerson : public Person
{
public:
    int hospitalize_time_ = -1;
    int home_x = -1;
    int home_y = -1;
    void StateChange(int index);
    HospitalizedPerson(const ConfirmedPerson &p, int hospitalize_time);
    ~HospitalizedPerson();
};

class HealedPerson : public Person
{
public:
    void StateChange(int index);
    HealedPerson(const HospitalizedPerson &p);
    HealedPerson(const ConfirmedPerson &p);
    ~HealedPerson();
};

class DeadPerson : public Person
{
public:
    void StateChange(int index);
    DeadPerson(const HospitalizedPerson &p);
    DeadPerson(const ConfirmedPerson &p);
    ~DeadPerson();
};

#endif //_PERSON_H_