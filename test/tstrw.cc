// This file is part of the iff library
//
// Copyright (c) 2018 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "stdmain.h"

class Player {
public:
    enum {
	cfmt_Player = IFF_FMT('P','L','A','Y'),
	cfmt_Stats = IFF_FMT('S','T','A','T'),
	cfmt_Inventory = IFF_FMT('I','N','V','T'),
	cfmt_Quests = IFF_FMT('Q','E','S','T'),
    };
    class Stats {
    public:
			Stats (void)			:_hp(20),_maxhp(25),_mana(12),_maxmana(42) {}
	void		read (istream& is)		{ is >> _hp >> _maxhp >> _mana >> _maxmana; }
	void		write (ostream& os) const	{ os << _hp << _maxhp << _mana << _maxmana; }
	auto		stream_size (void) const	// This evaluates at compile time to 8,
			    { return (stream_size_of (_hp) + stream_size_of (_maxhp) + stream_size_of (_mana) + stream_size_of (_maxmana)); }
	void		Print (void) const		{ printf ("Hp: %hu/%hu, Mana: %hu/%hu\n", _hp,_maxhp, _mana,_maxmana); }
    private:
	uint16_t	_hp;
	uint16_t	_maxhp;
	uint16_t	_mana;
	uint16_t	_maxmana;
    };
    class Item {
    public:
			Item (void)			: _name(),_value(),_weight(),_dam() {}
			Item (const char* name, uint32_t value, uint16_t weight, uint16_t dam) : _name(name),_value(value),_weight(weight),_dam(dam) {}
	void		read (istream& is)		{ is >> _name >> ios::align(4) >> _value >> _weight >> _dam; }
	void		write (ostream& os) const	{ os << _name << ios::align(4) << _value << _weight << _dam; }
	auto		stream_size (void) const	{ return Align(stream_size_of(_name),4)+stream_size_of(_value)+stream_size_of(_weight)+stream_size_of(_dam); }
	void		Print (void) const		{ printf ("\t%s\t$%u, %hu lb, Dam %hu\n", _name.c_str(), _value, _weight, _dam); }
    private:
	string		_name;
	uint32_t	_value;
	uint16_t	_weight;
	uint16_t	_dam;
    };
public:
			Player (void) : _name("Player1"),_stats(),_inventory(),_quests() {
			    _inventory.emplace_back ("long sword", 500, 4, 10);
			    _inventory.emplace_back ("leather armor", 230, 14, 0);
			    _inventory.emplace_back ("food ration", 5, 1, 4);
			    _inventory.emplace_back ("green potion", 50, 0, 0);
			    _quests.emplace_back ("Find the evil sourceror");
			    _quests.emplace_back ("Rescue the princess");
			    _quests.emplace_back ("Have lunch");
			}
    void		read (istream& is) {
			    iff::ReadChunk (is, _name, iff::cfmt_Name);
			    iff::ReadChunk (is, _stats, cfmt_Stats);
			    iff::ReadVector (is, _inventory, cfmt_Inventory);
			    iff::ReadLIST (is, _quests, cfmt_Quests);
			}
    void		write (ostream& os) const {
			    iff::WriteChunk (os, _name, iff::cfmt_Name);
			    iff::WriteChunk (os, _stats, cfmt_Stats);
			    iff::WriteVector (os, _inventory, cfmt_Inventory);
			    iff::WriteLIST (os, _quests, cfmt_Quests);
			}
    size_t		stream_size (void) const {
			    return iff::chunk_size_of(_name)
				+ iff::chunk_size_of(_stats)
				+ iff::vector_size_of(_inventory)
				+ iff::list_size_of(_quests);
			}
    void		Print (void) const {
			    printf ("Name: %s\n", _name.c_str());
			    _stats.Print();
			    printf ("Inventory:\n");
			    for (auto& i : _inventory)
				i.Print();
			    printf ("Quests:\n");
			    for (auto& q : _quests)
				printf ("\t%s\n", q.c_str());
			}
private:
    string		_name;
    Stats		_stats;
    vector<Item>	_inventory;
    vector<string>	_quests;
};

//----------------------------------------------------------------------

class TestApp {
public:
    static auto&	Instance (void)	{ static TestApp s_App; return s_App; }
    void		Run (void);
private:
			TestApp (void) {}
private:
    Player		_player;
};

void TestApp::Run (void)
{
    memblock buf (iff::form_size_of (_player));
    ostream os (buf);
    iff::WriteFORM (os, _player, Player::cfmt_Player);

    Player rp;
    istream is (buf);
    iff::ReadFORM (is, _player, Player::cfmt_Player);

    rp.Print();
    if (isatty (STDIN_FILENO))
	buf.write_file ("/tmp/player.sav");
}

StdTestMain (TestApp)
