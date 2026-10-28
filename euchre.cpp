#include "Pack.hpp"
#include "Card.hpp"
#include "Player.hpp"

#include <algorithm> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Game {
    public: 
    Game(istream &pack_filename, const string &type_shuffle, int points_to_win, 
         const vector<string> &player_name, const vector<string> &player_type);
    void play();

private:
    vector<Player*> players;
    Pack pack;
    bool shuffle_mode; 
    int winning_points;
    int hand_count;
    int team1_score;
    int team2_score;

    //Helper functions to simplify play()
    void shuffle();
    void deal(int dealer_index);
    void deal_round(int start_player, int how_many[4]);
    void make_trump(int dealer_index, Card upcard, Suit &trump_suit, bool &team1_is_maker);
    bool make_trump_round1(int dealer_index, Card upcard, Suit &trump_suit, bool &team1_is_maker);
    bool make_trump_round2(int dealer_index, Card upcard, Suit &trump_suit, bool &team1_is_maker);
    int play_trick(int led_index, Suit trump);
    void play_hand(int dealer_index, Suit trump, bool &team1_is_maker);
    void score_hand(int trump_caller, int trump_caller_tricks);
    void print_round_winner(int winning_team);
    void print_scores();
    void print_game_winner();
};

Game::Game(istream &pack_filename, const string &type_shuffle, int points_to_win, 
        const vector<string> &player_name, const vector<string> &player_type) {

        pack = Pack(pack_filename);
        if (type_shuffle == "shuffle") {
        shuffle_mode = true;
        } 
        else {
        shuffle_mode = false;
        }

        winning_points = points_to_win;
        team1_score = 0;
        team2_score = 0;
        hand_count = 0;

  for (size_t i = 0; i < 4; ++i) {
    Player *player = Player_factory(player_name[i], player_type[i]);
    players.push_back(player);
  }
}

        
void Game::shuffle() {
    if (shuffle_mode == true) {
        pack.shuffle();
        return;
    }
    else {
        pack.reset();
    }
}

void Game::deal(int dealer_index) {
    int starting_player = (dealer_index + 1) % 4;
    int first_round[4] = {3,2,3,2};
    int second_round[4] = {2,3,2,3};

    deal_round(starting_player, first_round);
    deal_round(starting_player, second_round);
}

void Game::deal_round(int starting_player, int card_to_deal[4]) {
    for(int i =0; i < 4; i++) {
        int player_index = (starting_player + i) % 4;
            for(int j =0; j < card_to_deal[i]; j++) {
                players[player_index]->add_card(pack.deal_one());
            }
    }
}

void Game::make_trump(int dealer_index, Card upcard, Suit &trump_suit, bool &team1_order_up) {
    if (make_trump_round1(dealer_index, upcard, trump_suit, team1_order_up)) {
        return;
    }
    else {
        make_trump_round2(dealer_index, upcard, trump_suit, team1_order_up);
    }
}

bool Game::make_trump_round1(int dealer_index, Card upcard, Suit &trump_suit, bool &team1_order_up) {
    int curr_player = (dealer_index + 1) % 4;
    Suit order_up_suit;

    for(int i = 0; i < 4; i++) {
        bool curr_dealer = (curr_player == dealer_index);
        bool need_trump = players[curr_player]-> make_trump(upcard, curr_dealer, 1, order_up_suit);

        if (need_trump == true) {
            cout << players[curr_player]-> get_name() << " orders up " << order_up_suit << endl;
            trump_suit = order_up_suit;
            team1_order_up = (curr_player == 0 || curr_player == 2);
            players[dealer_index]-> add_and_discard(upcard);
            return true;
        }
        else {
            cout << players[curr_player]-> get_name() << " passes" << endl;
        }
        curr_player = (curr_player + 1) % 4;
    }
  return false;
}
    
bool Game::make_trump_round2(int dealer_index, Card upcard, Suit &trump_suit, bool &team1_order_up) {
    int curr_player = (dealer_index + 1) % 4;
    Suit order_up_suit;

    for(int i = 0; i < 4; i++) {
        bool curr_dealer = (curr_player == dealer_index);
        bool need_trump = players[curr_player]-> make_trump(upcard, curr_dealer, 2, order_up_suit);

        if (need_trump == true) {
            cout << players[curr_player]-> get_name() << " orders up " << order_up_suit << endl;
            trump_suit = order_up_suit;
            team1_order_up = (curr_player == 0 || curr_player == 2);
            return true;
        }
        else {
            cout << players[curr_player]-> get_name() << " passes" << endl;
        }
        curr_player = (curr_player + 1) % 4;
    }
  return false;
}

int Game::play_trick(int led_index, Suit trump) {
    int winner_index = led_index;
    Card led_card;
    Card won_card;

    for (int i=0; i < 4; i++) {
        int curr_player = (led_index + i) % 4;
        string curr_name = players[curr_player]->get_name();
        if(i==0) {
            led_card = players[curr_player]->lead_card(trump);
            cout << led_card << " led by " << curr_name << endl;
            won_card = led_card;
            winner_index = curr_player;
        }
        else {
            Card played_card = players[curr_player]->play_card(led_card, trump);
            cout << played_card << " played by " << curr_name << endl;

            if (Card_less(won_card, played_card, led_card, trump)) {
                won_card = played_card;
                winner_index = curr_player;
            }
        }
    }
   string trick_winner = players[winner_index]->get_name();
    cout << trick_winner << " takes the trick" << endl;
    return winner_index;
}

void Game::play_hand(int dealer_index, Suit trump, bool &team1_is_maker){
    int team1_won_tricks = 0;
    int team2_won_tricks =0;
    int led_index = (dealer_index + 1) % 4;

    for (int i = 0; i < 5; i++) {
        int trick_winner = play_trick(led_index, trump);
        led_index = trick_winner;

        if (trick_winner == 0 || trick_winner == 2) {
            team1_won_tricks += 1;
        }
        else {
            team2_won_tricks +=1;
        }
    }
    if (team1_is_maker == true) {
        score_hand(1,team1_won_tricks);
    }
    else {
        score_hand(2,team2_won_tricks);
    }
}

void Game::score_hand(int trump_caller, int trump_caller_trick){
    bool march;
    bool euchred;

    if (trump_caller_trick == 5) {
         march = true;
    }
    else {
        march = false;
    }

    if (trump_caller_trick < 3) {
         euchred = true;
    }
    else{
         euchred = false;
    }

    //figuiring out who won the hand 
    int winning_team = 0;
    if (euchred == true) {
        if (trump_caller == 1) {
            winning_team = 2;
        }
        else {
            winning_team = 1;
        }
    }
    else {
        winning_team= trump_caller;
    }

    //how many points are gain for winning 
    int points_gained = 0;
    if (march == true) {
        points_gained = 2;
    } 
    else if (euchred == true) {
        points_gained = 2;
    } 
    else {
        points_gained = 1;
  }

  //updating the score after a team wins 
  if (winning_team == 1){
    team1_score = team1_score + points_gained;
  }
  else {
    team2_score = team2_score + points_gained;
  }

  print_round_winner(winning_team);
  if (march == true) {
    cout << "march!" << endl;
  }
  else if (euchred == true) {
    cout << "euchred!" << endl;
  }
print_scores();
}

void Game::print_round_winner(int winning_team) {
    if (winning_team == 1) {
        cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
    }
    else {
        cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
  }
}

void Game::print_scores() {
    cout << players[0]->get_name() << " and " << players[2]->get_name() << " have " << team1_score << " points" << endl;
    cout << players[1]->get_name() << " and " << players[3]->get_name() << " have " << team2_score << " points" << endl;
}

void Game::print_game_winner() {
    if (team1_score >= winning_points) {
        cout << players[0]->get_name() << " and " << players[2]->get_name() << " win!" << endl;
    } 
    else {
        cout << players[1]->get_name() << " and " << players[3]->get_name() << " win!" << endl;
    }
}

void Game::play() {
    while (team1_score < winning_points && team2_score < winning_points) {
    
    int dealer_index = hand_count % 4;
    cout << "Hand " << hand_count << endl;
    cout << players[dealer_index]->get_name() << " deals" << endl;

    shuffle();
    deal(dealer_index);

    Card upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;

    Suit trump;
    bool team1_is_maker;
    make_trump(dealer_index, upcard, trump, team1_is_maker);

    play_hand(dealer_index, trump, team1_is_maker);

    hand_count = hand_count + 1;
    }

    print_game_winner();

    for (size_t i = 0; i < players.size(); ++i) {
    delete players[i];
  }
  players.clear();
}

int main(int argc, char *val[]) {
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
    return 1;
    }

    string pack_filename = val[1];
    string type_shuffle = val[2];
    int points_to_win = stoi(val[3]);

    if (points_to_win < 1 || points_to_win > 100) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
    return 1;
    }

    if (!(type_shuffle == "shuffle" || type_shuffle == "noshuffle")) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
    return 1;
    }
    vector<string> types;
    vector<string> names;
    for (int i = 4; i < 12; i+=2) {
        string type = val[i+1];
        string name = val[i];

        if (!(type == "Simple" || type == "Human")) {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
        }

        types.push_back(type);
        names.push_back(name);
    }
    
    ifstream pack_file_in(pack_filename);
    if(!pack_file_in.is_open()) {
        cout << "Error opening " << pack_filename << endl;
    return 1;
    }

    for (int i = 0; i < argc; i++) {
        cout << val[i] << " " << endl;
    }

    Game game(pack_file_in, type_shuffle, points_to_win, names, types);
        game.play();
    return 0;
}

    for (size_t i = 0; i < players.size(); ++i) {
    delete players[i];
    }
}
