#include <cmath>

#include "global.h"
#include "person.h"
#include "grid.h"
#include "game.h"

/*Person*/

Person::Person() : belonging_grid_(new Grid())
{
}

Person::Person(const Person &p)
{
    id_ = p.id_;
    status_ = p.status_;
    vaccinated_ = p.vaccinated_;
    neighborhood_ = p.neighborhood_;
    belonging_grid_ = p.belonging_grid_;
    destination_ = p.destination_;
}

Person::~Person()
{
}

/*HealthyPerson*/

HealthyPerson::HealthyPerson()
{
    status_ = kHealthy;
}
HealthyPerson::HealthyPerson(const Person &p) : Person(p)
{
    status_ = kHealthy;
}

HealthyPerson::~HealthyPerson()
{
}

void HealthyPerson::StateChange(int index)
{
    int ill_neighbor_count = 0;
    for (auto p : this->neighborhood_)
    {
        if (p->status_ == kInfected || p->status_ == kConfirmd)
        {
            ill_neighbor_count += 1;
        }
    }
    double p_infect = 1.0 - std::pow((1 - P_INFECT), ill_neighbor_count);
    std::bernoulli_distribution infect_dist(p_infect);
    bool infected = infect_dist(game->random_generator_);
    if (infected)
    {
        int incubete_time = (int)game->incubate_distribution_(game->random_generator_);
        incubete_time = std::max(incubete_time, 0);
        InfectedPerson *pInfected = new InfectedPerson(*this, incubete_time);
        pPerson[index] = pInfected;
        delete this;
    }
}

/*InfectedPerson*/

InfectedPerson::InfectedPerson(const HealthyPerson &p, int incubate_time) : Person(p)
{
    incubate_time_ = incubate_time;
    status_ = kInfected;
}

InfectedPerson::~InfectedPerson()
{
}

void InfectedPerson::StateChange(int index)
{
    this->incubate_time_ -= 1;
    if (this->incubate_time_ == 0)
    {
        ConfirmedPerson *pConfirmed = new ConfirmedPerson(*this);
        pPerson[index] = pConfirmed;
        delete this;
    }
}

/*ConfirmedPerson*/

ConfirmedPerson::ConfirmedPerson(const InfectedPerson &p) : Person(p)
{
    status_ = kConfirmd;
}

ConfirmedPerson::~ConfirmedPerson()
{
}

void ConfirmedPerson::StateChange(int index)
{
    if (game->hospital_.persons_.size() < HOSPITAL_CAPACITY)
    {
        // type cast
        int hospitalize_time = (int)game->hospitalize_distribution_(game->random_generator_);
        hospitalize_time = std::max(hospitalize_time, 0);
        HospitalizedPerson *pHospitalized = new HospitalizedPerson(*this, hospitalize_time);
        pPerson[index] = pHospitalized;
        // no need to refresh grid->persons_
        // add to hospital
        game->hospital_.persons_.push_back(this);
        delete this;
    }
    else
    {
        bool dead = game->city_dead_distribution_(game->random_generator_);
        if (dead)
        {
            // type cast
            DeadPerson *pDead = new DeadPerson(*this);
            pPerson[index] = pDead;
            // no need to refresh grid->persons_
            // add to tomb
            game->tomb_.persons_.push_back(pDead);
            delete this;
        }
        else
        {
            bool healed = game->city_not_dead_heal_distribution_(game->random_generator_);
            if (healed)
            {
                // type cast
                HealedPerson *pHealed = new HealedPerson(*this);
                pPerson[index] = pHealed;
                // no need to refresh grid->persons_
                // add to heaven
                game->heaven_.persons_.push_back(pHealed);
                delete this;
            }
        }
    }
}

/*HospitalizedPerson*/

HospitalizedPerson::HospitalizedPerson(const ConfirmedPerson &p, int hospitalize_time) : Person(p)
{
    hospitalize_time_ = hospitalize_time;
    status_ = kHospitalized;
    belonging_grid_ = &(game->hospital_);
}

HospitalizedPerson::~HospitalizedPerson()
{
}

void HospitalizedPerson::StateChange(int index)
{
    this->hospitalize_time_ -= 1;
    if (this->hospitalize_time_ == 0)
    {
        // delete from hospital
        auto hospital_list = game->hospital_.persons_;
        for (auto it = hospital_list.begin(); it != hospital_list.end();)
        {
            if (*it == this)
                it = hospital_list.erase(it);
            else
                it++;
        }
        // judge dead or healed
        bool dead = game->hospital_dead_distribution_(game->random_generator_);
        if (dead)
        {
            // type cast
            DeadPerson *pDead = new DeadPerson(*this);
            pPerson[index] = pDead;
            // add to tomb
            game->tomb_.persons_.push_back(pDead);
            delete this;
        }
        else
        {
            // type cast
            HealedPerson *pHealed = new HealedPerson(*this);
            pPerson[index] = pHealed;
            // add to tomb
            game->heaven_.persons_.push_back(pHealed);
            delete this;
        }
    }
}

/*HealedPerson*/

HealedPerson::HealedPerson(const HospitalizedPerson &p) : Person(p)
{
    status_ = kHealed;
    belonging_grid_ = &(game->heaven_);
}

HealedPerson::HealedPerson(const ConfirmedPerson &p) : Person(p)
{
    status_ = kHealed;
    belonging_grid_ = &(game->heaven_);
}

HealedPerson::~HealedPerson()
{
}

void HealedPerson::StateChange(int index)
{
}

/*DeadPerson*/

DeadPerson::DeadPerson(const HospitalizedPerson &p) : Person(p)
{
    status_ = kDead;
    belonging_grid_ = &(game->tomb_);
}

DeadPerson::DeadPerson(const ConfirmedPerson &p) : Person(p)
{
    status_ = kDead;
    belonging_grid_ = &(game->tomb_);
}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::StateChange(int index)
{
}