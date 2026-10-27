#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
  Player * alice = Player_factory("Alice", "Simple");
  ASSERT_EQUAL("Alice", alice->get_name());

  delete alice;
}

// Test 1: test round 1 and check if trump with 2+ cards are face/ace
TEST(test_make_trump_round1_has_2face_ace){
  Player * saw = Player_factory("saw", "Simple");

  // declare 2 face/ace in a hands of 5
  saw->add_card(Card(QUEEN, DIAMONDS));
  saw->add_card(Card(ACE, DIAMONDS));
  saw->add_card(Card(TEN, CLUBS));
  saw->add_card(Card(JACK, HEARTS));
  saw->add_card(Card(TEN, SPADES));

  Card upcard(TEN, DIAMONDS);
  Suit order_up_suit;

  bool round1_result = saw->make_trump(upcard, false, 1, order_up_suit);
  ASSERT_TRUE(round1_result);
  ASSERT_EQUAL(order_up_suit, DIAMONDS);
  delete saw;
}

//Test 2: Test if we have a left-bower for the trump
TEST(test_make_trump_round1_left_bower_face_ace) {
    Player * saw = Player_factory("saw", "Simple");

    
    saw->add_card(Card(JACK, SPADES));
    saw->add_card(Card(KING, CLUBS));
    saw->add_card(Card(TEN, CLUBS));
    saw->add_card(Card(JACK, HEARTS));
    saw->add_card(Card(TEN, SPADES));

    Card upcard(TEN, CLUBS);
    Suit order_up_suit;

    bool left_bower_trump = saw->make_trump(upcard, false, 1, order_up_suit);
    ASSERT_TRUE(left_bower_trump);
    ASSERT_EQUAL(order_up_suit, CLUBS);
    delete saw;
}

//Tet 3: test when I have 2 face/ace, but not the trump suit 
TEST(test_make_trump_suit_not_trump) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(QUEEN, HEARTS));
  saw->add_card(Card(KING, SPADES));
  saw->add_card(Card(TEN, SPADES));
  saw->add_card(Card(NINE, CLUBS));
  saw->add_card(Card(NINE, SPADES));

  Card upcard(TEN, DIAMONDS);
  Suit order_up_suit;

  bool round1_result = saw->make_trump(upcard, false, 1, order_up_suit);
  ASSERT_FALSE(round1_result);
  delete saw;
}

// Test 4: test round 1 and check if there are no trumps of 2+ face/ace
TEST(test_make_trump_round1_has_no_face_ace) {
  Player * saw = Player_factory("saw", "Simple");

  saw->add_card(Card(TEN, DIAMONDS));
  saw->add_card(Card(NINE, DIAMONDS));
  saw->add_card(Card(TEN, CLUBS));
  saw->add_card(Card(JACK, HEARTS));
  saw->add_card(Card(TEN, SPADES));

  Card upcard(TEN, DIAMONDS);
  Suit order_up_suit;

  bool round1_result = saw->make_trump(upcard, false, 1, order_up_suit);
  ASSERT_FALSE(round1_result);
  delete saw;
}

// Test 5: test round 2 when I have 1+ face/ace of the next suit
TEST(test_round2_has1_next_suit) {
  Player *saw = Player_factory("saw", "Simple");

  // 1+ face/ace of the next suit (Hearts)
  saw->add_card(Card(QUEEN, HEARTS));
  saw->add_card(Card(KING, CLUBS));
  saw->add_card(Card(TEN, SPADES));
  saw->add_card(Card(NINE, CLUBS));
  saw->add_card(Card(NINE, SPADES));

  Card upcard(TEN, DIAMONDS);
  Suit order_up_suit;

  bool round2_result = saw->make_trump(upcard, false, 2, order_up_suit);
  ASSERT_TRUE(round2_result);
  ASSERT_EQUAL(order_up_suit, HEARTS);
  delete saw;
}

// Test 6: test round 2 when there are no face/ace of the next suit and pass
TEST(test_round2_has0_next_suit) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(TEN, HEARTS));
  saw->add_card(Card(NINE, SPADES));
  saw->add_card(Card(TEN, SPADES));
  saw->add_card(Card(NINE, CLUBS));
  saw->add_card(Card(NINE, SPADES));

  Card upcard(TEN, DIAMONDS);
  Suit order_up_suit;

  bool round2_result = saw->make_trump(upcard, false, 2, order_up_suit);
  ASSERT_FALSE(round2_result);
  delete saw;
}

//Test 7: In round 2, test when screw the dealear should occur
TEST(test_make_trump_round2_screw_dealer) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(TEN, HEARTS));
  saw->add_card(Card(NINE, SPADES));
  saw->add_card(Card(TEN, SPADES));
  saw->add_card(Card(NINE, CLUBS));
  saw->add_card(Card(TEN, CLUBS));

  Card upcard(TEN, DIAMONDS);
  Suit order_up_suit;

  bool screw_dealer = saw->make_trump(upcard, true, 2, order_up_suit);
  ASSERT_TRUE(screw_dealer);
  ASSERT_EQUAL(order_up_suit, HEARTS);
  delete saw;
}

//Tst 8: test Lead_card to play the highest trump card if we only have trump cards
TEST(test_lead_card_highest_trump_only_trump_cards) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, SPADES));
  saw->add_card(Card(QUEEN, SPADES));
  saw->add_card(Card(KING, SPADES));
  saw->add_card(Card(ACE, SPADES));
  saw->add_card(Card(TEN, SPADES));

  Card Highest_trump = saw -> lead_card(SPADES);
  ASSERT_EQUAL(Highest_trump, Card(JACK, SPADES));
  delete saw;
}

//Test 9: test Lead_card to play the highest non-trump card
TEST(test_lead_card_highest_nontrump) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, CLUBS));
  saw->add_card(Card(QUEEN, DIAMONDS));
  saw->add_card(Card(KING, CLUBS));
  saw->add_card(Card(ACE, SPADES));
  saw->add_card(Card(TEN, CLUBS));

  Card Highest_nontrump = saw -> lead_card(HEARTS);
  ASSERT_EQUAL(Highest_nontrump, Card(ACE, SPADES));
  delete saw;
}

//Test 10: test if lead_card avoids the left bower 
TEST(test_lead_card_avoid_left_bower) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, CLUBS));
  saw->add_card(Card(QUEEN, DIAMONDS));
  saw->add_card(Card(KING, HEARTS));
  saw->add_card(Card(ACE, HEARTS));
  saw->add_card(Card(TEN, DIAMONDS));

  Card led = saw->lead_card(SPADES);
  ASSERT_EQUAL(led, Card(ACE, HEARTS));
  delete saw;
}

//Test 11: test if play_card follows suit and plays the highest 
TEST(test_play_card_play_highest_suit){
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, CLUBS));
  saw->add_card(Card(QUEEN, DIAMONDS));
  saw->add_card(Card(KING, CLUBS));
  saw->add_card(Card(ACE, SPADES));
  saw->add_card(Card(TEN, CLUBS));
  
  Card led_card(JACK, CLUBS);
  Card card_played = saw->play_card (led_card, CLUBS);
  ASSERT_EQUAL(card_played, Card(JACK, CLUBS));
  delete saw;
}

//Test 12: test play_card to play the lowest card if we dont have a led suit
TEST(test_play_card_play_lowest_if_nosuit) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, CLUBS));
  saw->add_card(Card(QUEEN, DIAMONDS));
  saw->add_card(Card(KING, CLUBS));
  saw->add_card(Card(ACE, SPADES));
  saw->add_card(Card(TEN, CLUBS));
  
  Card led_card(KING, HEARTS);
  Card card_played = saw->play_card (led_card, DIAMONDS);
  ASSERT_EQUAL(card_played, Card(TEN, CLUBS));
  delete saw;
}

//Test 13: test if play_card follows a left bower lead with a trump card 
TEST(test_play_card_follow_with_left_bower){
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, CLUBS));
  saw->add_card(Card(QUEEN, CLUBS));
  saw->add_card(Card(KING, CLUBS));
  saw->add_card(Card(ACE, HEARTS));
  saw->add_card(Card(TEN, HEARTS));
  
  Card led_card(JACK, SPADES);
  Card card_played = saw->play_card (led_card, CLUBS);
  ASSERT_EQUAL(card_played, Card(JACK, CLUBS));
  delete saw;
}

//Test 14: test if add_and_upcard gets rid of the lowest card of the same suit 
TEST(test_add_and_discard_lowest_card) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, SPADES));
  saw->add_card(Card(QUEEN, SPADES));
  saw->add_card(Card(KING, SPADES));
  saw->add_card(Card(ACE, SPADES));
  saw->add_card(Card(TEN, SPADES));

  Card upcard(ACE, SPADES);
  saw->add_and_discard(upcard);
  Card lowest_card = saw -> lead_card(SPADES);
  ASSERT_EQUAL(lowest_card, Card(JACK, SPADES));
  delete saw;
}

//Test 15: test if led_card plays right bower if we have trump, left, and right bower
TEST(test_lead_card_left_right_trump){
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(JACK, SPADES));
  saw->add_card(Card(JACK, CLUBS));
  saw->add_card(Card(ACE, SPADES));
  saw->add_card(Card(NINE, HEARTS));
  saw->add_card(Card(TEN, DIAMONDS));

  Card led_play = saw -> lead_card(SPADES);
  ASSERT_EQUAL(led_play, Card(TEN, DIAMONDS));
  delete saw; 
}

//Test 16: test if play_card can follow the suit even if I have higher trumps 
TEST(test_player_card_follow_suit) {
  Player *saw = Player_factory("saw", "Simple");

  saw->add_card(Card(TEN, SPADES));
  saw->add_card(Card(NINE, CLUBS));
  saw->add_card(Card(TEN, CLUBS));
  saw->add_card(Card(NINE, HEARTS));
  saw->add_card(Card(TEN, HEARTS));

  Card led_card(KING, HEARTS);
  Card card_played = saw -> play_card(led_card, SPADES);
  ASSERT_EQUAL(card_played, Card(TEN, HEARTS));
  delete saw; 
}


TEST_MAIN()
