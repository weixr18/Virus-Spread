#include "game.h"

Game *game;
Grid **city = new Grid *[MAP_H]; // x:[0, 699]-MAP_H   y:[0, 799]-MAP-W
Person **const pPerson = new Person *[TOTAL_POPULATION];

Game::Game(
    const int HOSPITAL_CAPACITY,
    const double P_MOVE,
    const double P_INFECT,
    const double P_VACCINATION,
    const double P_PROTECTION,
    const double P_OBSERVE) : HOSPITAL_CAPACITY_(HOSPITAL_CAPACITY),
                              d_(P_MOVE,P_INFECT, P_VACCINATION, P_PROTECTION, P_OBSERVE)
{
    step_count_ = 0;
    std::cout << "P_MOVE: " << d_.P_MOVE_ << " P_INFECT_: " << d_.P_INFECT_;
    std::cout << " P_VACCINATION: " << d_.P_VACCINATION_ << " P_PROTECTION: " << d_.P_PROTECTION_;
    std::cout << " P_OBSERVE" << d_.P_OBSERVE_ << std::endl;
    
    // city grid initialization
    for (int i = 0; i < MAP_H; i++)
    {
        city[i] = new Grid[MAP_W];
    }
    for (int i = 0; i < MAP_H; i++)
    {
        for (int j = 0; j < MAP_W; j++)
        {
            city[i][j].type_ = kCity;
            city[i][j].position_ = Point(i, j);
            if (i > 0)
            {
                city[i][j].neighbor_grids_.push_back(&(city[i - 1][j]));
                if (j > 0)
                {
                    city[i][j].neighbor_grids_.push_back(&(city[i - 1][j - 1]));
                }
                if (j < MAP_W - 1)
                {
                    city[i][j].neighbor_grids_.push_back(&(city[i - 1][j + 1]));
                }
            }
            if (i < MAP_H - 1)
            {
                city[i][j].neighbor_grids_.push_back(&(city[i + 1][j]));
                if (j > 0)
                {
                    city[i][j].neighbor_grids_.push_back(&(city[i + 1][j - 1]));
                }
                if (j < MAP_W - 1)
                {
                    city[i][j].neighbor_grids_.push_back(&(city[i + 1][j + 1]));
                }
            }
            if (j > 0)
            {
                city[i][j].neighbor_grids_.push_back(&(city[i][j - 1]));
            }
            if (j < MAP_W - 1)
            {
                city[i][j].neighbor_grids_.push_back(&(city[i][j + 1]));
            }
        }
    }

    // pPerson initializion
    HealthyPerson **const tmp_pointers = new HealthyPerson *[TOTAL_POPULATION];
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        tmp_pointers[i] = new HealthyPerson;
        pPerson[i] = tmp_pointers[i];
    }
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        // basic info
        Person &cur_person = *(pPerson[i]);
        cur_person.id_ = i;
        cur_person.vaccinated_ = d_.IsVaccinated();

        // grid allocate
        int x = d_.GetInitX();
        int y = d_.GetInitY();
        x = std::max(std::min(x, MAP_H - 1), 0);
        y = std::max(std::min(y, MAP_W - 1), 0);
        cur_person.belonging_grid_ = &(city[x][y]);
        cur_person.destination_ = city[x][y].position_;

        // infect cast
        if (i < INITIAL_INFECT_NUM)
        {
            int incubete_time = d_.GetIncubateTime();
            incubete_time = std::max(incubete_time, 0);
            InfectedPerson *pInfected = new InfectedPerson(*(tmp_pointers[i]), incubete_time);
            pPerson[i] = pInfected;
        }
    }
    delete tmp_pointers;
}

Game::~Game()
{
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        delete pPerson[i];
    }
    delete pPerson;

    for (int i = 0; i < MAP_H; i++)
    {
        delete city[i];
    }
    delete city;
}

void Game::MoveStep()
{
    // persons judgements
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        // destination reset
        Person &cur_person = *(pPerson[i]);
        int cur_x = cur_person.belonging_grid_->position_.x_;
        int cur_y = cur_person.belonging_grid_->position_.y_;
        if (cur_x + cur_y < 0)
        {
            // the person is dead or hospitalized. Do not judge.
            continue;
        }

        // move judge
        cur_person.stay_time--;
        if (cur_person.stay_time == 0 || step_count_ % STEP_PER_DAY == NIGHT_FALL_STEP)
        {
            Point nxt_pos = d_.GetNextPosition(step_count_, cur_person.area_);
            int new_x = nxt_pos.x_;
            int new_y = nxt_pos.y_;
            cur_person.belonging_grid_ = &(city[new_x][new_y]);
        }
    }

    // grid infomation update
    for (int i = 0; i < MAP_H; i++)
    {
        for (int j = 0; j < MAP_W; j++)
        {
            city[i][j].persons_.clear();
        }
    }
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        Person &cur_person = *(pPerson[i]);
        int cur_x = cur_person.belonging_grid_->position_.x_;
        int cur_y = cur_person.belonging_grid_->position_.y_;
        if (cur_x + cur_y < 0)
        {
            // the person is dead or hospitalized.
            continue;
        }
        cur_person.belonging_grid_->persons_.push_back(pPerson[i]);
    }

    // neighborhood update
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        Person &cur_person = *(pPerson[i]);
        cur_person.neighborhood_.clear();
        int cur_x = cur_person.belonging_grid_->position_.x_;
        int cur_y = cur_person.belonging_grid_->position_.y_;
        if (cur_x + cur_y < 0)
        {
            // the person is dead or hospitalized.
            continue;
        }
        auto tmp = cur_person.belonging_grid_->persons_;
        for (int j = 0; j < tmp.size(); j++)
        {
            if (tmp[j] != pPerson[i])
            {
                cur_person.neighborhood_.push_back(tmp[j]);
            }
        }
        auto neighbor_gs = cur_person.belonging_grid_->neighbor_grids_;
        for (int j = 0; j < neighbor_gs.size(); j++)
        {
            auto neighbor_ps = neighbor_gs[j]->persons_;
            for (int k = 0; k < neighbor_ps.size(); k++)
            {
                cur_person.neighborhood_.push_back(neighbor_ps[k]);
            }
        }
    }
}

void Game::SwitchStateStep()
{
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        // if (step_count_ == 64 || step_count_ == 76)
        // {
        //     printf("%d\n", i);
        //     if (i == 2718)
        //     {
        //         printf("state: %d, (%d, %d)\n", pPerson[i]->status_);
        //     }
        // }
        pPerson[i]->StateChange(i);
    }
}

void Game::SaveStep()
{
    // output to file
    if (step_count_ == 0)
    {
        std::stringstream fmt;
        fmt << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        fmt << "./data/"
            << "C" << std::setw(3) << std::setfill('0') << HOSPITAL_CAPACITY_
            << "_M" << d_.P_MOVE_ << "_I" << d_.P_INFECT_ << "_V" << d_.P_VACCINATION_
            << "_P" << d_.P_PROTECTION_ << "_O" << d_.P_OBSERVE_ << ".txt";
        std::string file_name = fmt.str();
        std::cout << file_name << std::endl;
        logger_.Start(file_name);
    }

    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        const Person &cur_p = *(pPerson[i]);
        std::stringstream tmp;
        tmp << cur_p.id_ << '\t';
        tmp << cur_p.status_ << '\t';
        tmp << cur_p.belonging_grid_->position_.x_ << '\t';
        tmp << cur_p.belonging_grid_->position_.y_ << '\n';
        logger_.Log(tmp.str());
    }

    if (step_count_ == TOTAL_STEPS)
    {
        logger_.Close();
    }

    // statistics
    if (step_count_ % STEP_PER_DAY == 0)
    {
        int healthy_count = 0;
        int infected_count = 0;
        int confirmed_count = 0;
        int hospitalized_count = 0;
        int healed_count = 0;
        int dead_count = 0;
        int observed_count = 0;
        for (int i = 0; i < TOTAL_POPULATION; i++)
        {
            switch (pPerson[i]->status_)
            {
            case kHealthy:
                healthy_count++;
                break;
            case kInfected:
                infected_count++;
                break;
            case kConfirmd:
                confirmed_count++;
                break;
            case kHospitalized:
                hospitalized_count++;
                break;
            case kHealed:
                healed_count++;
                break;
            case kDead:
                dead_count++;
                break;
            case kObserved:
                observed_count++;
                break;
            default:
                break;
            }
        }

        printf("Day %3d: healthy %4d, infected %4d, confirmed %4d, hospital %4d, healed %4d, dead %4d, observed %4d\n",
               step_count_ / STEP_PER_DAY, healthy_count, infected_count, confirmed_count,
               hospitalized_count, healed_count, dead_count, observed_count);
    }

    step_count_ += 1;
}

void Game::Step()
{
    SaveStep();
    MoveStep();
    SwitchStateStep();
}
