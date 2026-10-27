#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//default constructor starts with the Nine of Spades
TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

//test helper: builds the text of a default pack, one card per line
static string default_pack_text() {
  ostringstream oss;
  for (int s = SPADES; s <= DIAMONDS; ++s) {
    for (int r = NINE; r <= ACE; ++r) {
      oss << Card(static_cast<Rank>(r), static_cast<Suit>(s)) << "\n";
    }
  }
  return oss.str();
}

//default pack goes suit by suit, lowest rank to highest, all 24 cards
TEST(test_pack_default_full_order) {
  Pack pack;
  for (int s = SPADES; s <= DIAMONDS; ++s) {
    for (int r = NINE; r <= ACE; ++r) {
      ASSERT_EQUAL(pack.deal_one(),
                   Card(static_cast<Rank>(r), static_cast<Suit>(s)));
    }
  }
}

//last card of the default pack is the Ace of Diamonds
TEST(test_pack_default_last_card) {
  Pack pack;
  Card last;
  for (int i = 0; i < 24; ++i) {
    last = pack.deal_one();
  }
  ASSERT_EQUAL(last, Card(ACE, DIAMONDS));
}

//default pack has no duplicates and only ranks Nine through Ace
TEST(test_pack_default_unique_and_valid_ranks) {
  Pack pack;
  Card dealt[24];
  for (int i = 0; i < 24; ++i) {
    dealt[i] = pack.deal_one();
    ASSERT_TRUE(dealt[i].get_rank() >= NINE);
  }
  for (int i = 0; i < 24; ++i) {
    for (int j = i + 1; j < 24; ++j) {
      ASSERT_TRUE(dealt[i] != dealt[j]);
    }
  }
}

//pack is not empty until exactly 24 cards have been dealt
TEST(test_pack_empty_after_24_deals) {
  Pack pack;
  for (int i = 0; i < 24; ++i) {
    ASSERT_FALSE(pack.empty());
    pack.deal_one();
  }
  ASSERT_TRUE(pack.empty());
}

//boundary: one card left means the pack is not empty yet
TEST(test_pack_not_empty_with_one_card_left) {
  Pack pack;
  for (int i = 0; i < 23; ++i) {
    pack.deal_one();
  }
  ASSERT_FALSE(pack.empty());
  ASSERT_EQUAL(pack.deal_one(), Card(ACE, DIAMONDS));
  ASSERT_TRUE(pack.empty());
}

//empty() works on a const Pack
TEST(test_pack_empty_on_const_pack) {
  const Pack pack;
  ASSERT_FALSE(pack.empty());
}

//reset on a fresh pack changes nothing
TEST(test_pack_reset_fresh_pack) {
  Pack pack;
  pack.reset();
  ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

//reset after a few deals puts the first card back on top
TEST(test_pack_reset_partial) {
  Pack pack;
  pack.deal_one();
  pack.deal_one();
  pack.reset();
  ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
  ASSERT_EQUAL(pack.deal_one(), Card(TEN, SPADES));
}

//reset after dealing everything makes the pack full again
TEST(test_pack_reset_after_empty) {
  Pack pack;
  for (int i = 0; i < 24; ++i) {
    pack.deal_one();
  }
  ASSERT_TRUE(pack.empty());
  pack.reset();
  ASSERT_FALSE(pack.empty());
  ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

//resetting twice is the same as resetting once
TEST(test_pack_reset_twice) {
  Pack pack;
  pack.deal_one();
  pack.reset();
  pack.reset();
  ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

//stream constructor on a default-order file matches the default pack
TEST(test_pack_stream_matches_default) {
  istringstream iss(default_pack_text());
  Pack from_stream(iss);
  Pack default_pack;
  for (int i = 0; i < 24; ++i) {
    ASSERT_EQUAL(from_stream.deal_one(), default_pack.deal_one());
  }
}

//stream constructor keeps file order (input is reversed on purpose)
TEST(test_pack_stream_reverse_order) {
  ostringstream oss;
  for (int s = DIAMONDS; s >= SPADES; --s) {
    for (int r = ACE; r >= NINE; --r) {
      oss << Card(static_cast<Rank>(r), static_cast<Suit>(s)) << "\n";
    }
  }
  istringstream iss(oss.str());
  Pack pack(iss);
  ASSERT_EQUAL(pack.deal_one(), Card(ACE, DIAMONDS));
  ASSERT_EQUAL(pack.deal_one(), Card(KING, DIAMONDS));
  for (int i = 2; i < 23; ++i) {
    pack.deal_one();
  }
  ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
  ASSERT_TRUE(pack.empty());
}

//stream constructor reads exactly 24 cards and leaves the rest in the stream
TEST(test_pack_stream_reads_only_24) {
  istringstream iss(default_pack_text() + "Two of Spades\n");
  Pack pack(iss);
  Card extra;
  iss >> extra;
  ASSERT_EQUAL(extra, Card(TWO, SPADES));
}

//a pack read from a stream starts full, with the first card on top
TEST(test_pack_stream_starts_full) {
  istringstream iss(default_pack_text());
  Pack pack(iss);
  ASSERT_FALSE(pack.empty());
  ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

//shuffle keeps all 24 cards and creates no duplicates
TEST(test_pack_shuffle_keeps_all_cards) {
  Pack pack;
  pack.shuffle();
  Card dealt[24];
  for (int i = 0; i < 24; ++i) {
    dealt[i] = pack.deal_one();
  }
  ASSERT_TRUE(pack.empty());
  for (int i = 0; i < 24; ++i) {
    for (int j = i + 1; j < 24; ++j) {
      ASSERT_TRUE(dealt[i] != dealt[j]);
    }
  }
}

//shuffle changes the order (Nine of Spades is no longer first)
TEST(test_pack_shuffle_changes_order) {
  Pack pack;
  pack.shuffle();
  ASSERT_NOT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

//shuffle resets next, even after some cards were dealt
TEST(test_pack_shuffle_resets_next) {
  Pack pack;
  for (int i = 0; i < 10; ++i) {
    pack.deal_one();
  }
  pack.shuffle();
  ASSERT_FALSE(pack.empty());
  for (int i = 0; i < 24; ++i) {
    pack.deal_one();
  }
  ASSERT_TRUE(pack.empty());
}

//shuffling an empty pack makes it full again
TEST(test_pack_shuffle_after_empty) {
  Pack pack;
  for (int i = 0; i < 24; ++i) {
    pack.deal_one();
  }
  pack.shuffle();
  ASSERT_FALSE(pack.empty());
}

//exact order after 7 in shuffles of a default pack (worked out on paper)
TEST(test_pack_shuffle_exact_order) {
  const Card expected[24] = {
    Card(KING, CLUBS),     Card(JACK, HEARTS),    Card(NINE, SPADES),
    Card(ACE, CLUBS),      Card(QUEEN, HEARTS),   Card(TEN, SPADES),
    Card(NINE, DIAMONDS),  Card(KING, HEARTS),    Card(JACK, SPADES),
    Card(TEN, DIAMONDS),   Card(ACE, HEARTS),     Card(QUEEN, SPADES),
    Card(JACK, DIAMONDS),  Card(NINE, CLUBS),     Card(KING, SPADES),
    Card(QUEEN, DIAMONDS), Card(TEN, CLUBS),      Card(ACE, SPADES),
    Card(KING, DIAMONDS),  Card(JACK, CLUBS),     Card(NINE, HEARTS),
    Card(ACE, DIAMONDS),   Card(QUEEN, CLUBS),    Card(TEN, HEARTS)
  };
  Pack pack;
  pack.shuffle();
  for (int i = 0; i < 24; ++i) {
    ASSERT_EQUAL(pack.deal_one(), expected[i]);
  }
}

//shuffling twice (14 in shuffles) gives a different known order
TEST(test_pack_shuffle_twice) {
  Pack pack;
  pack.shuffle();
  pack.shuffle();
  ASSERT_EQUAL(pack.deal_one(), Card(TEN, CLUBS));
  ASSERT_EQUAL(pack.deal_one(), Card(JACK, SPADES));
  ASSERT_EQUAL(pack.deal_one(), Card(KING, CLUBS));
}

//shuffle works on the current order, not a fresh default pack
TEST(test_pack_shuffle_uses_current_order) {
  ostringstream oss;
  for (int s = DIAMONDS; s >= SPADES; --s) {
    for (int r = ACE; r >= NINE; --r) {
      oss << Card(static_cast<Rank>(r), static_cast<Suit>(s)) << "\n";
    }
  }
  istringstream iss(oss.str());
  Pack pack(iss);
  pack.shuffle();
  ASSERT_EQUAL(pack.deal_one(), Card(TEN, HEARTS));
  ASSERT_EQUAL(pack.deal_one(), Card(QUEEN, CLUBS));
  ASSERT_EQUAL(pack.deal_one(), Card(ACE, DIAMONDS));
}

//reset after a shuffle goes back to the top of the shuffled order
TEST(test_pack_reset_after_shuffle) {
  Pack pack;
  pack.shuffle();
  Card first = pack.deal_one();
  pack.deal_one();
  pack.reset();
  ASSERT_EQUAL(pack.deal_one(), first);
}

TEST_MAIN()