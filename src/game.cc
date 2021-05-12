#include "game.h"

Game *game;
Grid **city = new Grid *[MAP_H]; // x:[0, 699]-MAP_H   y:[0, 799]-MAP-W
Person **const pPerson = new Person *[TOTAL_POPULATION];

Game::Game()
{
    step_count_ = 0;
    std::cout << "P_MOVE: " << P_MOVE << " P_INFECT: " << P_INFECT;
    std::cout << " P_VACCINATION: " << P_VACCINATION << " P_PROTECTION: " << P_PROTECTION << std::endl;
    std::normal_distribution<double> x_distribution(400, 100);
    std::normal_distribution<double> y_distribution(400, 100);

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
    HealthyPerson *tmp_persons = new HealthyPerson[TOTAL_POPULATION];
    for (int i = 0; i < TOTAL_POPULATION; i++)
    {
        // basic info
        pPerson[i] = tmp_persons + i;
        Person &cur_person = *(pPerson[i]);
        cur_person.id_ = i;
        cur_person.vaccinated_ = vaccinate_distribition_(random_generator_);

        // grid allocate
        int x = (int)x_distribution(random_generator_);
        int y = (int)y_distribution(random_generator_);
        x = std::max(std::min(x, MAP_H - 1), 0);
        y = std::max(std::min(y, MAP_W - 1), 0);
        cur_person.belonging_grid_ = &(city[x][y]);
        cur_person.destination_ = city[x][y].position_;

        // infect cast
        if (i < INITIAL_INFECT_NUM)
        {
            int incubete_time = (int)incubate_distribution_(random_generator_);
            incubete_time = std::max(incubete_time, 0);
            InfectedPerson *pInfected = new InfectedPerson(tmp_persons[i], incubete_time);
            pPerson[i] = pInfected;
        }
    }
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
            // the person is healed, dead or hospitalized.
            continue;
        }
        if (cur_person.destination_ == cur_person.belonging_grid_->position_)
        {

            std::normal_distribution<double> destination_x_dist(cur_x, 50);
            std::normal_distribution<double> destination_y_dist(cur_y, 50);
            int destination_x = (int)destination_x_dist(random_generator_);
            int destination_y = (int)destination_y_dist(random_generator_);
            if (destination_x > 0 && destination_x < MAP_H && destination_y > 0 && destination_y < MAP_W)
            {
                cur_person.destination_ = Point(destination_x, destination_y);
            }
        }

        // move judge
        bool is_move = move_distribution_(random_generator_);
        if (is_move)
        {
            int destination_x = cur_person.destination_.x_;
            int destination_y = cur_person.destination_.y_;
            int delta_x = std::abs(cur_x - destination_x);
            int delta_y = std::abs(cur_y - destination_y);
            int new_x = cur_x;
            int new_y = cur_y;
            if (delta_x + delta_y > 0)
            {
                std::bernoulli_distribution move_direction_x_dist(delta_x / (double)(delta_x + delta_y));
                bool is_moving_x = move_direction_x_dist(random_generator_);
                if (is_moving_x)
                {
                    new_x = cur_x + Sign(destination_x - cur_x);
                }
                else
                {
                    new_y = cur_y + Sign(destination_y - cur_y);
                }
            }
            cur_person.belonging_grid_ = &(city[new_x][new_y]);
        }
    }

    // grid persons update
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
            // the person is healed, dead or hospitalized.
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
            // the person is healed, dead or hospitalized.
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
            << "C" << std::setw(3) << std::setfill('0') << HOSPITAL_CAPACITY << "_M" << P_MOVE
            << "_I" << P_INFECT << "_V" << P_VACCINATION << "_P" << P_PROTECTION << ".txt";
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
    if (step_count_ % 10 == 0)
    {
        int healthy_count = 0;
        int infected_count = 0;
        int confirmed_count = 0;
        int hospitalized_count = 0;
        int healed_count = 0;
        int dead_count = 0;
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
            default:
                break;
            }
        }

        printf("Day %3d: healthy %4d, infected %4d, confirmed %4d, hospital %4d, healed %4d, dead %4d\n",
               step_count_ / 10, healthy_count, infected_count, confirmed_count,
               hospitalized_count, healed_count, dead_count);
    }

    step_count_ += 1;
}

void Game::Step()
{
    SaveStep();
    MoveStep();
    SwitchStateStep();
}