#pragma once

#include <string>

class Game : public GameObject {
    std::set<GameObject *> game_objects;    // http://www.cplusplus.com/reference/set/set/

    std::set<BackgroundObject *> background_objects;

    AvancezLib *engine;

    ObjectPool<Rocket> rockets_pool;

    ObjectPool<Bomb> bomb_pool;

    ObjectPool<Mine> mines_pool;

    ObjectPool<Alien> aliens_pool;

    ObjectPool<Baiter> baiters_pool;

    ObjectPool<Swarmer> swarmer_pool;

    ObjectPool<MotherShip> mother_ship_pool;

    ObjectPool<Human> humans_pool;

    ObjectPool<Bomber> bombers_pool;

    ObjectPool<Scanner> scanners_pool;

    Player *player;

    Background *background;

    Sprite *life_sprite;
    bool game_over = false;

    unsigned int score = 0;
    unsigned int wave = 1;
    unsigned int update = 1;
    unsigned int alien_kill = 0;
    unsigned int alien_number = 25;
    unsigned int lives = 3;

public:

    virtual void Create(AvancezLib *engine) {
        this->engine = engine;
        player = new Player();
        player->player = true;
        background = new Background();

        //create background
        auto *background_behaviour = new BackgroundBehaviourComponent();
        background_behaviour->Create(engine, background, &game_objects);
        auto *background_render = new RenderComponent();
        background_render->Create(engine, background, &game_objects,
                                  "data/background.bmp");

        background->Create();
        background->AddComponent(background_behaviour);
        background->AddComponent(background_render);
        background_objects.insert(reinterpret_cast<BackgroundObject *const>(background));

        //create player
        auto *player_behaviour = new PlayerBehaviourComponent();
        player_behaviour->Create(engine, player, &game_objects, &rockets_pool, &humans_pool);
        auto *player_render = new RenderComponent();
        player_render->Create(engine, player, &game_objects, "data/player.bmp");

        //create collide components for player
        auto *playerCollideBombComponent = new CollideComponent();
        auto *playerCollideMineComponent = new CollideComponent();
        auto *playerCollideAlienComponent = new CollideComponent();
        auto *playerCollideBaiterComponent = new CollideComponent();
        auto *playerCollideBomberComponent = new CollideComponent();
        auto *playerCollideMotherShipComponent = new CollideComponent();
        auto *playerCollideSwarmerComponent = new CollideComponent();

        playerCollideBombComponent->Create(engine, player, &game_objects, (ObjectPool<GameObject> *) &bomb_pool);
        playerCollideMineComponent->Create(engine, player, &game_objects, (ObjectPool<GameObject> *) &mines_pool);
        playerCollideAlienComponent->Create(engine, player, &game_objects, (ObjectPool<GameObject> *) &aliens_pool);
        playerCollideBaiterComponent->Create(engine, player, &game_objects, (ObjectPool<GameObject> *) &baiters_pool);
        playerCollideBomberComponent->Create(engine, player, &game_objects, (ObjectPool<GameObject> *) &bombers_pool);
        playerCollideMotherShipComponent->Create(engine, player, &game_objects,
                                                 (ObjectPool<GameObject> *) &mother_ship_pool);
        playerCollideSwarmerComponent->Create(engine, player, &game_objects, (ObjectPool<GameObject> *) &swarmer_pool);

        player->Create();
        player->AddComponent(playerCollideBombComponent);
        player->AddComponent(playerCollideMineComponent);
        player->AddComponent(playerCollideAlienComponent);
        player->AddComponent(playerCollideBaiterComponent);
        player->AddComponent(playerCollideBomberComponent);
        player->AddComponent(playerCollideMotherShipComponent);
        player->AddComponent(playerCollideSwarmerComponent);
        player->AddComponent(player_behaviour);
        player->AddComponent(player_render);
        player->AddReceiver(this);
        game_objects.insert(player);

        //create humans object pool
        humans_pool.Create(3);
        for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++) {
            HumanBehaviourComponent *human_behaviour = new HumanBehaviourComponent();
            human_behaviour->Create(engine, *human, &game_objects);
            RenderComponent *human_render = new RenderComponent();
            human_render->Create(engine, *human, &game_objects, "data/human.bmp");
            CollideComponent *human_collide = new CollideComponent();
            human_collide->Create(engine, *human, &game_objects, (ObjectPool<GameObject> *) &rockets_pool);

            (*human)->Create();
            (*human)->human = true;
            (*human)->AddComponent(human_behaviour);
            (*human)->AddComponent(human_render);
            (*human)->AddComponent(human_collide);
            (*human)->AddReceiver(this);
            game_objects.insert(*human);
        }

        //create aliens object pool
        aliens_pool.Create(10);
        for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++) {
            auto *alien_behaviour = new AlienBehaviourComponent();
            alien_behaviour->Create(engine, *alien, player, &game_objects, &bomb_pool, &humans_pool);
            auto *alien_render = new RenderComponent();
            alien_render->Create(engine, *alien, &game_objects, "data/enemy_0.bmp");
            auto *alien_collide = new CollideComponent();
            alien_collide->Create(engine, *alien, &game_objects, (ObjectPool<GameObject> *) &rockets_pool);

            (*alien)->Create();
            (*alien)->AddComponent(alien_behaviour);
            (*alien)->AddComponent(alien_render);
            (*alien)->AddComponent(alien_collide);
            (*alien)->AddReceiver(this);
            game_objects.insert(*alien);
        }

        //create bombers object pool
        bombers_pool.Create(5);
        for (auto bomber = bombers_pool.pool.begin(); bomber != bombers_pool.pool.end(); bomber++) {
            auto *bomber_behaviour = new BomberBehaviourComponent();
            bomber_behaviour->Create(engine, *bomber, &game_objects, &mines_pool);
            auto *bomber_render = new RenderComponent();
            bomber_render->Create(engine, *bomber, &game_objects, "data/bomber.bmp");
            auto *bomber_collide = new CollideComponent();
            bomber_collide->Create(engine, *bomber, &game_objects, (ObjectPool<GameObject> *) &rockets_pool);

            (*bomber)->Create();
            (*bomber)->AddComponent(bomber_behaviour);
            (*bomber)->AddComponent(bomber_render);
            (*bomber)->AddComponent(bomber_collide);
            (*bomber)->AddReceiver(this);
            game_objects.insert(*bomber);
        }

        //create baiters object pool
        baiters_pool.Create(2);
        for (auto baiter = baiters_pool.pool.begin(); baiter != baiters_pool.pool.end(); baiter++) {
            auto *baiter_behaviour = new BaiterBehaviourComponent();
            baiter_behaviour->Create(engine, *baiter, player, &game_objects, &bomb_pool, 150, 100);
            auto *baiter_render = new RenderComponent();
            baiter_render->Create(engine, *baiter, &game_objects, "data/baiter.bmp");
            auto *baiter_collide = new CollideComponent();
            baiter_collide->Create(engine, *baiter, &game_objects, (ObjectPool<GameObject> *) &rockets_pool);

            (*baiter)->Create();
            (*baiter)->AddComponent(baiter_behaviour);
            (*baiter)->AddComponent(baiter_render);
            (*baiter)->AddComponent(baiter_collide);
            (*baiter)->AddReceiver(this);
        }

        //create mother ship object pool
        mother_ship_pool.Create(2);
        for (auto mother_ship = mother_ship_pool.pool.begin();
             mother_ship != mother_ship_pool.pool.end(); mother_ship++) {
            auto *mother_ship_behaviour = new MotherShipBehaviourComponent();
            mother_ship_behaviour->Create(engine, *mother_ship, &game_objects);
            auto *mother_ship_render = new RenderComponent();
            mother_ship_render->Create(engine, *mother_ship, &game_objects,
                                       "data/mothership.bmp");
            auto *mother_ship_collide = new CollideComponent();
            mother_ship_collide->Create(engine, *mother_ship, &game_objects, (ObjectPool<GameObject> *) &rockets_pool);

            (*mother_ship)->Create();
            (*mother_ship)->AddComponent(mother_ship_behaviour);
            (*mother_ship)->AddComponent(mother_ship_render);
            (*mother_ship)->AddComponent(mother_ship_collide);
            (*mother_ship)->AddReceiver(this);
        }

        //create swarmer object pool
        swarmer_pool.Create(4);
        for (auto swarmer = swarmer_pool.pool.begin(); swarmer != swarmer_pool.pool.end(); swarmer++) {
            auto *swarmer_behaviour = new SwarmerBehaviourComponent();
            swarmer_behaviour->Create(engine, *swarmer, player, &game_objects);
            auto *swarmer_render = new RenderComponent();
            swarmer_render->Create(engine, *swarmer, &game_objects,
                                   "data/swarmer.bmp");
            auto *swarmer_collide = new CollideComponent();
            swarmer_collide->Create(engine, *swarmer, &game_objects, (ObjectPool<GameObject> *) &rockets_pool);

            (*swarmer)->Create();
            (*swarmer)->AddComponent(swarmer_behaviour);
            (*swarmer)->AddComponent(swarmer_render);
            (*swarmer)->AddComponent(swarmer_collide);
            (*swarmer)->AddReceiver(this);
        }

        //create rockets object pool for player
        rockets_pool.Create(30);
        for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++) {
            auto *behaviour = new RocketBehaviourComponent();
            behaviour->Create(engine, *rocket, &game_objects);
            auto *render = new RenderComponent();
            render->Create(engine, *rocket, &game_objects, "data/rocket.bmp");
            (*rocket)->Create();
            (*rocket)->AddComponent(behaviour);
            (*rocket)->AddComponent(render);
        }

        //create bombs object pool for alien enemies
        bomb_pool.Create(30);
        for (auto bomb = bomb_pool.pool.begin(); bomb != bomb_pool.pool.end(); bomb++) {
            auto *behaviour = new BombBehaviourComponent();
            behaviour->Create(engine, *bomb, &game_objects);
            auto *render = new RenderComponent();
            render->Create(engine, *bomb, &game_objects, "data/bomb.bmp");
            (*bomb)->Create();
            (*bomb)->AddComponent(behaviour);
            (*bomb)->AddComponent(render);
        }

        //create mines object pool for bomber enemies
        mines_pool.Create(30);
        for (auto mine = mines_pool.pool.begin(); mine != mines_pool.pool.end(); mine++) {
            auto *behaviour = new MineBehaviourComponent();
            behaviour->Create(engine, *mine, &game_objects);
            auto *render = new RenderComponent();
            render->Create(engine, *mine, &game_objects, "data/mine.bmp");
            (*mine)->Create();
            (*mine)->AddComponent(behaviour);
            (*mine)->AddComponent(render);
        }

        //create scanners object pool
        scanners_pool.Create(game_objects.size());
        int i = 0;
        for (auto go = game_objects.begin(); go != game_objects.end(); go++) {
            Scanner *scanner = scanners_pool.pool[i];
            auto *scanner_behaviour = new ScannerBehaviourComponent();
            scanner_behaviour->Create(engine, scanner, (*go));
            auto *scanner_render = new RenderComponent();
            //render different scanners depending on the entity
            if ((*go)->player) {
                scanner_render->Create(engine, scanner, &game_objects,
                                       "data/scanner_p.bmp");
            }
            if ((*go)->human) {
                scanner_render->Create(engine, scanner, &game_objects,
                                       "data/scanner_h.bmp");
            } else if (!(*go)->human && !(*go)->player) {
                scanner_render->Create(engine, scanner, &game_objects,
                                       "data/scanner_a.bmp");
            }

            scanner->Create();
            scanner->AddComponent(scanner_behaviour);
            scanner->AddComponent(scanner_render);
            scanner->AddReceiver(this);
            i++;
        }
        //representation of players life
        life_sprite = engine->createSprite("data/player.bmp");
    }

    virtual void Init() {
        //init scanner
        for (auto scanner = scanners_pool.pool.begin(); scanner != scanners_pool.pool.end(); scanner++) {
            if ((*scanner) != NULL) {
                game_objects.insert(*scanner);
                (*scanner)->Init();
            }
        }
        //srand(time(0));

        //spawning of aliens
        NewAttack();

        background->Init();
        player->Init();

        SDL_Delay(32);
        enabled = true;
    }

    virtual void NewAttack() {
        //init the object pools of aliens
        SpawnAliens();
        SpawnBombers();
        SpawnHumans();
        SpawnMotherShips();
    }

    virtual void SpawnMotherShips() {
        //placement of xPos for enemy
        float mother_ship_pos = 1000;
        for (auto mothership = mother_ship_pool.pool.begin(); mothership != mother_ship_pool.pool.end(); mothership++) {
            if ((*mothership) != NULL)
            {
                game_objects.insert(*mothership);
                (*mothership)->Init(mother_ship_pos);
                (*mothership)->enabled = true;
                (*mothership)->dead = false;
            }
            mother_ship_pos += 1000;
        }
    }

    virtual void SpawnBaiters() {
        float baiter_pos = 0;
        for (auto baiter = baiters_pool.pool.begin(); baiter != baiters_pool.pool.end(); baiter++) {
            if ((*baiter) != NULL) {
                game_objects.insert(*baiter);
                (*baiter)->Init(baiter_pos);
                (*baiter)->enabled = true;
                (*baiter)->dead = false;
            }
            baiter_pos += 6400 / 2;
        }
    }

    virtual void SpawnHumans() {
        float humans_pos = 0;
        for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++) {
            if ((*human) != NULL)
            {
                game_objects.insert(*human);
                (*human)->Init(humans_pos);
                (*human)->enabled = true;
                (*human)->dead = false;
            }
            humans_pos += 6400 / 3;
        }
    }

    virtual void SpawnBombers() {
        float bomber_pos = 0;
        for (auto bomber = bombers_pool.pool.begin(); bomber != bombers_pool.pool.end(); bomber++) {
            if ((*bomber) != NULL)
            {
                game_objects.insert(*bomber);
                (*bomber)->Init(bomber_pos);
                (*bomber)->enabled = true;
                (*bomber)->dead = false;
            }
            bomber_pos += 6400 / 5;
        }
    }

    virtual void SpawnAliens() {
        float alien_pos = 0;
        int i = 0;
        for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++) {
            if ((*alien) != NULL)
            {
                game_objects.insert(*alien);
                (*alien)->Init(alien_pos);
                (*alien)->enabled = true;
                (*alien)->dead = false;
            }
            alien_pos += 6400 / 10;
            i++;
        }
    }

    virtual void SpawnSwarmers(float xPos, float yPos) {
        float swarm_pos = 0;
        for (auto swarmer = swarmer_pool.pool.begin(); swarmer != swarmer_pool.pool.end(); swarmer++) {
            swarm_pos += 32;
            if ((*swarmer) != NULL)
            {
                game_objects.insert(*swarmer);
                (*swarmer)->Init(xPos, yPos + swarm_pos);
                (*swarmer)->enabled = true;
                (*swarmer)->dead = false;
            }
        }

    }

    virtual void Update(float dt) {
        AvancezLib::KeyStatus keys;
        engine->getKeyStatus(keys);
        //turn off game by pressing escape
        if (keys.esc) {
            Destroy();
            engine->quit();
        }
        //stop game when game over
        if (game_over)
            return;

        //render background before game objects
        for (auto go = background_objects.begin(); go != background_objects.end(); go++) {
            (*go)->Update(dt);
        }

        for (auto go = game_objects.begin(); go != game_objects.end(); go++) {
            //change sprite of alien if it mutates
            if ((*go)->mutate) {
                auto *alien_render = new RenderComponent();
                alien_render->Create(engine, (*go), &game_objects,
                                     "data/mutant.bmp");
                (*go)->AddComponent(alien_render);
                (*go)->mutate = false;
            }
            //spawn swarmers in the position of mother ship if it's killed
            if ((*go)->mother_ship_killed) {
                SpawnSwarmers((*go)->xPos, (*go)->yPos);
                (*go)->mother_ship_killed = false;
            }
            //spawn baiter enemies for each new enemy wave
            if (alien_kill == alien_number && update == wave)
                SpawnBaiters();
            //spawn new aliens if all enemies have been killed
            if (alien_kill == alien_number) {
                //only spawn new aliens when wave is not complete
                if (update < wave) {
                    alien_kill = 0;
                    update++;
                    NewAttack();
                } else {
                    //update wave to the next wave if all enemies for that enemy wave has been killed
                    lives = 0;
                    alien_kill = 0;
                    update = 0;
                    wave++;
                    lives = 3;
                    player->lives = 3;
                    NewAttack();
                }
            }
            (*go)->Update(dt);
        }
    }

    //used to print on screen
    char *toPoints(int points) {
        static char string[10];
        sprintf(string, "%06d", points);
        return string;
    }

    char *toWave(int wave) {
        static char string[10];
        sprintf(string, "%06d", wave);
        return string;
    }

    //drawing of different information during gameplay
    virtual void Draw() {
        if (game_over)
            engine->drawText(640 / 2 - 70, 550 / 2 - 40, "GAME OVER");
        else {
            engine->drawText(10, 10, "LIVES:");
            for (int i = 0; i < player->lives + 1; i++)
                life_sprite->draw(90 + 34 * i, 3, false);
            engine->drawText(10, 30, toPoints(score));
            engine->drawText(550, 10, toWave(wave));
        }
        engine->swapBuffers();
        engine->clearWindow();
    }

    virtual void Receive(Message m) {
        if (m == GAME_OVER) {
            game_over = true;
        }

        if (m == HIT) {
            lives--;
        }

        if (m == ALIEN_HIT) {
            alien_kill++;
            score += POINTS_PER_ALIEN * game_speed;
        }
    }

    //destroy all game objects and object pools when quitting the game
    virtual void Destroy() {
        SDL_Log("Game::Destroy");

        for (auto bo = background_objects.begin(); bo != background_objects.end(); bo++)
            (*bo)->Destroy();

        for (auto go = game_objects.begin(); go != game_objects.end(); go++)
            (*go)->Destroy();

        life_sprite->destroy();
        rockets_pool.Destroy();
        bomb_pool.Destroy();
        mines_pool.Destroy();
        aliens_pool.Destroy();
        swarmer_pool.Destroy();
        baiters_pool.Destroy();
        mother_ship_pool.Destroy();
        bombers_pool.Destroy();
        humans_pool.Destroy();
        scanners_pool.Destroy();
        delete player;
    }
};