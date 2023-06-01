#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace Param {
    static constexpr int MAP_WIDTH = 17600;
    static constexpr int MAP_HEIGHT = 9000;

    static constexpr int BASE_ATTRACTION_RADIUS = 5000;
    static constexpr int BASE_VIEW_RADIUS = 6000;
    static constexpr int BASE_MAX_HEALTH = 3;

    static constexpr int HERO_MOVE_SPEED = 800;
    static constexpr int HEROES_PER_PLAYER = 3;
    static constexpr int HERO_VIEW_RADIUS = 2200;
    static constexpr int HERO_ATTACK_RANGE = 800;
    static constexpr int HERO_ATTACK_DAMAGE = 2;

    static constexpr int MONS_MOVE_SPEED = 400;

    static constexpr int SPELL_COST = 10;
    static constexpr int SPELL_CONTROL_DURATION = 12;
    static constexpr int SPELL_CONTROL_RADIUS = 2200;
    static constexpr int SPELL_PUSH_DISTANCE = 2200;
    static constexpr int SPELL_PUSH_RADIUS = 1280;
}

enum class EntityType{
    Monster = 0,
    My_Hero = 1,
    Op_Hero = 2,
};

class Position{
private:
    int x_;
    int y_;

public:
    Position() :x_(0), y_(0) {}
    Position(int x, int y) :x_(x), y_(y) {}

    int x() const { return x_; }
    int y() const { return y_; }

    int distance(const Position& pos) const { return sqrt(pow(pos.x()-x_, 2) + pow(pos.y()-y_, 2)); }
    std::string toString() const { return std::to_string(x_) + " " + std::to_string(y_); }

    /* Operators */
    bool operator==(const Position& pos) const { return x_ == pos.x_ && y_ == pos.y_; }
    bool operator!=(const Position& pos) const { return x_ != pos.x_ || y_ != pos.y_; }

    const Position operator+(const Position& pos) const { return Position(x_ + pos.x_, y_ + pos.y_); }
    const Position operator-(const Position& pos) const { return Position(x_ - pos.x_, y_ - pos.y_); }
    Position& operator=(const Position& pos) {
        x_ = pos.x_;
        y_ = pos.y_;
        return *this;
    }

    friend std::istream& operator>>(std::istream& is, Position& s) { return is >> s.x_ >> s.y_; }
    friend std::ostream& operator<<(std::ostream& os, const Position& s) { return os << s.x_ << " " << s.y_; }
};


class Base {
private:
    Position pos_;
    int health_ = 0;
    int mana_ = 0;
    bool isBlue_;

public:
    Base(Position pos) : pos_(pos) { isBlue_ = (pos_.x() == 0); }

    /* Getters */
    Position getPos() const { return pos_; }
    int getHealth() const { return health_; }
    int getMana() const { return mana_; }
    bool isBlue() const { return isBlue_; }


    /* Public methods */
    bool isLand(const Position& p) const { return distance(p) < Param::BASE_ATTRACTION_RADIUS; }
    bool isView(const Position& p) const { return distance(p) < Param::BASE_VIEW_RADIUS; }
    bool isTopLeft() const { return pos_ == Position(0, 0); }

    int distance(Position pos) const { return pos_.distance(pos); }
    void statusUpdate(int health, int mana) {
        health_ = health;
        mana_ = mana;
    }

    /* Print Error */
    void printErr(string msg) const {
        fprintf(stderr, "================\n");
        fprintf(stderr, "Base: %s\n", pos_.toString().c_str());
        fprintf(stderr, "health: %d\n", health_);
        fprintf(stderr, "mana: %d\n", mana_);
        fprintf(stderr, "msg: %s\n", msg.c_str());
        fprintf(stderr, "================\n\n");
    }
};

class Entity {
protected:
	int id_;
	EntityType type_;
	Position pos_;
	int shieldLife_;
	int isControlled_;

public:
	// Entity() { id_ = -1; }
	Entity(int id, EntityType type) : id_(id), type_(type){}

    /* Getters */
    int getId() const { return id_; }
    Position getPos() const { return pos_; }

	int distance(Position pos) const{ return pos_.distance(pos); }
};

class Monster : public Entity {
private:
    int health_;
    Position vec_;
    int nearBase_;
    int threatFor_;
	/*
		if nearBase is 0:
			0: it will never reach a base.
            1: it will eventually reach your base.
			2: it will eventually reach your opponent's base.

		If nearBase is 1: 
			1: this monster is targeting your base.
			2: otherwise.
	*/

public:
    Monster(int id) : Entity(id, EntityType::Monster) {}

    /* Getters */
    bool isNearBase() const { return nearBase_ != 0; }
    bool isThreatFor(int side) const { return threatFor_ == side; }

    /* Input */
	friend std::istream& operator>>(std::istream& is, Monster& m) {
		return is >> m.pos_ >> m.shieldLife_ >> m.isControlled_ 
				  >> m.health_ >> m.vec_ >> m.nearBase_ >> m.threatFor_; 
	}

    /* Print Error */
	void printErr(std::string msg = "") const {
		fprintf(stderr, "================\n");
	
		fprintf(stderr, "Monster: %d\n", id_);
		fprintf(stderr, "x: %d   y: %d\n", pos_.x(), pos_.y());
		fprintf(stderr, "shield_life: %d\n", shieldLife_);
		fprintf(stderr, "is_controlled: %d\n", isControlled_);
		fprintf(stderr, "near_base: %d\n", nearBase_);
		fprintf(stderr, "threat_for: %d\n", threatFor_);
        fprintf(stderr, "msg: %s\n", msg.c_str());
		fprintf(stderr, "================\n\n");
	}
};

class Hero : public Entity {
private:
public:
    Hero(int id, EntityType type) : Entity(id, type) {}

    bool isInView(const Position& p) const { return distance(p) <= Param::HERO_VIEW_RADIUS; }

    /* Movement */
    void Move(Position pos, string msg = "") const {
        cout << "MOVE " << pos << " " << msg << endl;
    }

    void Wind(Position dir, string msg = "") const {
        cout << "SPELL WIND " << dir << " " << msg << endl;
    }
    void Control(Entity target, Position dir, string msg = "") const {
        cout << "SPELL CONTROL " << std::to_string(target.getId()) << " " << dir 
                  << " " << msg << endl; 
    }
    void Shield(Entity target, string msg = "") const {
        cout << "SPELL SHIELD " << std::to_string(target.getId()) << " " << msg << std::endl;
    }
    void Wait(std::string msg = "") const {
        cout << "WAIT " << msg << endl;
    }

    /* Input */
	friend std::istream& operator>>(std::istream& is, Hero& h) {
		int temp;
		Position tPos;
		return is >> h.pos_ >> h.shieldLife_ >> h.isControlled_ 
				  >> temp >> tPos >> temp >> temp; 
	}

    /* Print Error */
    void printErr(string msg) const {
		fprintf(stderr, "================\n");
	
		fprintf(stderr, "Hero: %d\n", id_);
		fprintf(stderr, "x: %d   y: %d\n", pos_.x(), pos_.y());
		fprintf(stderr, "shield_life: %d\n", shieldLife_);
		fprintf(stderr, "is_controlled: %d\n", isControlled_);
        fprintf(stderr, "msg: %s\n", msg.c_str());
		fprintf(stderr, "================\n\n");
    }
};

class GameManager {
private:
    int turn_ = 0;
    int patrol_pos_index[3] = {0, 0, 0};
    Position patrol_pos[3][2] = { // 0: atk, 1: def1, 2:def2
        { Position(13400, 2000), Position(9200, 8000) },
        { Position(6800, 1000), Position(8400, 1000), },
        { Position(3800, 7000), Position(5400, 7000), }
    };

public:

    void nextTurn() { turn_ += 1; }

    /* Getters */
    int getTurn() const { return turn_; }
    Position getPatrolPos(int id) { return patrol_pos[id][patrol_pos_index[id]]; }

    /* Setters */
    void switchPatrolPosIndex(int id) { patrol_pos_index[id] = (patrol_pos_index[id] + 1) % 2; }


    /* Update status */
    void readInBases(Base& my, Base& op) {
		int health, mana;

		std::cin >> health >> mana; std::cin.ignore();
		my.statusUpdate(health, mana);

		std::cin >> health >> mana; std::cin.ignore();
		op.statusUpdate(health, mana);
    }
    void readInEntities(vector<Monster>& mons, vector<Hero>& myHeros, vector<Hero>& opHeros){
        int entityCount;
		std::cin >> entityCount; std::cin.ignore();

        while(entityCount--){
			int id, type;
			std::cin >> id >> type;

			switch(type){
				case 0:{
					Monster temp(id);
					std::cin >> temp; std::cin.ignore();
					mons.push_back(temp);
					break;
				}
				case 1:{
					Hero temp(id, EntityType::My_Hero);
					std::cin >> temp; std::cin.ignore();
					myHeros.push_back(temp);
					break;
				}
				case 2:{
					Hero temp(id, EntityType::Op_Hero);
					std::cin >> temp; std::cin.ignore();
					opHeros.push_back(temp);
					break;
				}
			} // switch end
		} // while end

        return;
    }

    /* Tool func */
    Position getMirrorPos(const Position& pos) {
        Position map(Param::MAP_WIDTH, Param::MAP_HEIGHT);
        return map - pos;
    }

    Position getHuntPos(const vector<Monster>& mons) {
        int max = 0;
        Position huntPos;

        for (int i=0; i<mons.size(); ++i) {
            for (int j=i+1; j < mons.size(); ++j) {
                Position h1 = mons[i].getPos();
                Position h2 = mons[j].getPos();
                if ( h1.distance(h2) > Param::HERO_ATTACK_RANGE ) continue;

                // get center
                Position mid((h1.x() + h2.x()) / 2, (h1.y() + h2.y()) / 2);
                double angle = std::atan2(h2.y() - h1.y(), h2.x() - h1.x());
                double d = std::sqrt(Param::HERO_ATTACK_RANGE - h1.distance(mid) * h1.distance(mid));

                Position center(mid.x() + d * std::sin(angle), mid.y() - d * std::cos(angle));

                // print error
                std::cerr << "mid: " << mid << std::endl;
                std::cerr << "angle: " << angle << std::endl;
                std::cerr << "d: " << d << std::endl;
                std::cerr << "center: " << center << std::endl;

                // count number of monster in range
                int cnt = 0;
                for (int k=0; k<mons.size(); ++k) {
                    if (mons[k].getPos().distance(center) <= Param::HERO_ATTACK_RANGE) cnt++;
                }

                if (cnt >= max) {
                    max = cnt;
                    huntPos = center;
                }
            }
        }
        return huntPos;
    }

    void printErr(string msg) const {
        fprintf(stderr, "================\n");
        fprintf(stderr, "%s\n", msg.c_str());
        fprintf(stderr, "================\n\n");
    }

};





#endif // _GAME_H_
