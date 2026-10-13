#include "Card.hpp"
#include <array>
#include <string>
#include "Pack.hpp"
#include <cassert>
#include <iostream>

using namespace std;

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack() : next(0) {
    int i = 0;
    
    for (int s = SPADES; s <= DIAMONDS; ++s) {
        Suit suit =static_cast<Suit>(s);
            for (int r = NINE; r <= ACE; ++r) {
                Rank rank = static_cast<Rank>(r);
                    cards[i] = Card(rank, suit);
                    ++i;
        }
    }
}

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(std::istream& pack_input) : next(0) {
    for (int i = 0; i < PACK_SIZE; ++i) {
        pack_input >> cards[i];
    }
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one() {
    assert(!empty());
    Card next_card= cards[next];
    ++next;
    return next_card; 
  }

// EFFECTS: Resets next index to first card in the Pack
  void Pack::reset() {
    next = 0;
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times.
  void Pack::shuffle() {
  for (int n = 0; n < 7; ++n) {
    Card shuffled[PACK_SIZE];
    for (int i = 0; i < PACK_SIZE / 2; ++i) {

      shuffled[2 * i] = cards[PACK_SIZE / 2 + i];
      shuffled[2 * i + 1] = cards[i];
      
    }
    for (int i = 0; i < PACK_SIZE; ++i) {
      cards[i] = shuffled[i];
    }
  }
  next = 0;
}

  bool Pack::empty() const {
  return next == PACK_SIZE;
} 