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
    //destination_ = p.destination_;
    stay_time_ = p.stay_time_;
    area_ = p.area_;
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
            // do not count kObserved.
            ill_neighbor_count += 1;
        }
    }
    bool infected = game->d_.IsInfected(ill_neighbor_count, vaccinated_);
    if (infected)
    {
        int incubete_time = game->d_.GetIncubateTime();
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
    else
    {
        bool is_observed = game->d_.IsObserved();
        if (is_observed)
        {
            ObservedPerson *pObserved = new ObservedPerson(*this);
            pPerson[index] = pObserved;
            delete this;
        }
    }
}

/*ConfirmedPerson*/

ConfirmedPerson::ConfirmedPerson(const InfectedPerson &p) : Person(p)
{
    status_ = kConfirmd;
}

ConfirmedPerson::ConfirmedPerson(const ObservedPerson &p) : Person(p)
{
    status_ = kConfirmd;
}

ConfirmedPerson::~ConfirmedPerson()
{
}

void ConfirmedPerson::StateChange(int index)
{
    if (game->hospital_.persons_.size() < game->HOSPITAL_CAPACITY_)
    {
        // type cast
        int hospitalize_time = game->d_.GetHospitalizeTime();
        hospitalize_time = std::max(hospitalize_time, 0);
        HospitalizedPerson *pHospitalized = new HospitalizedPerson(*this, hospitalize_time);
        pPerson[index] = pHospitalized;
        // add to hospital
        game->hospital_.persons_.push_back(this);
        delete this;
    }
    else
    {
        bool dead = game->d_.IsCityPersonDead();
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
            bool healed = game->d_.IsCityPersonNotDeadButHealed();
            if (healed)
            {
                // type cast
                HealedPerson *pHealed = new HealedPerson(*this);
                pPerson[index] = pHealed;
                // no need to refresh grid->persons_
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
    home_x = p.belonging_grid_->position_.x_;
    home_y = p.belonging_grid_->position_.y_;
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
        bool dead = game->d_.IsHospitalPersonDead();
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
            // add to home grid

            delete this;
        }
    }
}

/*HealedPerson*/

HealedPerson::HealedPerson(const HospitalizedPerson &p) : Person(p)
{
    status_ = kHealed;
    belonging_grid_ = &(city[p.home_x][p.home_y]);
    stay_time_ = 1; // move in the next step
}

HealedPerson::HealedPerson(const ConfirmedPerson &p) : Person(p)
{
    status_ = kHealed;
    belonging_grid_ = p.belonging_grid_;
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

/*ObservedPerson*/

ObservedPerson::ObservedPerson(const InfectedPerson &p) : Person(p)
{
    incubate_time_ = p.incubate_time_;
    status_ = kObserved;
}

ObservedPerson::~ObservedPerson()
{
}

void ObservedPerson::StateChange(int index)
{
    this->incubate_time_ -= 1;
    if (this->incubate_time_ == 0)
    {
        // printf("Observe: incubate_time_ == 0!!!\n");
        ConfirmedPerson *pConfirmed = new ConfirmedPerson(*this);
        pPerson[index] = pConfirmed;
        delete this;
    }
}