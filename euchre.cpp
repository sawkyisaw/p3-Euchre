#include "Pack.cpp"
#include "Player.cpp"
#include "Card.cpp"

#include <iostream>

using namespace std;

/*
----- EECS280 EUCRE -----
6 Ranks: 9, 10, Jack, Queen, King and Ace
4 Suits: Spades, Hearts, Clubs, Diamonds 

Hand: A round consisting of: 
1) Set up table: dealer shuffles deck (using in shuffle) 
2) deal (player 0 then 1 player to the left) distribute 5 cards,
alternating 3 and 2 -> 3,2,3,2 then 2,3,2,3 starting from left (8 batches)
3) Make trump -> trump suit determined through "ordering up"
*/

// Game ADT with constructor that takes in players, points to win, etc, and a public play() function

class Game {
 public:
  Game(/* game details */);
  void play();
  
   private:
  std::vector<Player*> players;
  Pack pack;
  // ...

  void shuffle();
  void deal(/* ... */);
  void make_trump(/* ... */);
  void play_hand(/* ... */);
  // ...
};

int main(int argc, char **argv) {
  // Read command line args and check for errors
  Game game(/* game details */);
  game.play();

    for (size_t i = 0; i < players.size(); ++i) {
    delete players[i];
    }
}