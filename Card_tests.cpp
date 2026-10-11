#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <sstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here

//Test if constructer gives Two of Spades - Saw
TEST(test1_two_spade) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

//test if card is trump - Saw
TEST(test2_card_is_trump) {
    Card king_hearts(KING, HEARTS);
    ASSERT_TRUE(king_hearts.is_trump(HEARTS));
    ASSERT_FALSE(king_hearts.is_trump(CLUBS));
}

//test if jack is also trump. - Saw
TEST(test3_jack_also_trump) {
    Card king_hearts(KING, HEARTS);
    ASSERT_TRUE(king_hearts.is_trump(HEARTS));
    ASSERT_FALSE(king_hearts.is_trump(CLUBS)); 

    Card jack_hearts(JACK, HEARTS);
    ASSERT_TRUE(jack_hearts.is_trump(HEARTS));
    ASSERT_FALSE(jack_hearts.is_trump(CLUBS));
}

//Test if the next suit is acurate - Saw
TEST(test4_next_suit) {
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

//Test for tie breaker using suit - Saw
TEST(test5_tie_breaker_suit) {
    Card king_clubs(KING, CLUBS);
    Card king_spades(KING, SPADES);
    ASSERT_TRUE(king_spades < king_clubs);
    ASSERT_FALSE(king_spades > king_clubs);
}

//test if the left bower keeps its suit when becoming a trump - Saw
TEST(test6_left_bower_keep_suit) {
    Card jack_diamonds(JACK, DIAMONDS);
    //trump = heart, then jack_diamonds become left bower
    ASSERT_EQUAL(jack_diamonds.get_suit(HEARTS), HEARTS); 
    //keeps the same suit though
    ASSERT_EQUAL(jack_diamonds.get_suit(), DIAMONDS);
}

//test if a non-bower card stays as is  - Saw
TEST(test7_nonbower_suit_stay_same) {
    Card king_clubs (KING, CLUBS);
    ASSERT_EQUAL(king_clubs.get_suit(SPADES), CLUBS);
    ASSERT_EQUAL(king_clubs.get_suit(DIAMONDS), CLUBS);
    ASSERT_EQUAL(king_clubs.get_suit(HEARTS), CLUBS);
}

//test if the card is the right bower - Saw
TEST(test8_card_right_bower){
    Card jack_clubs (JACK, CLUBS);
    ASSERT_TRUE(jack_clubs.is_right_bower(CLUBS));
}

//test if the card is not the right bower - Saw
TEST(test9_card_not_right_bower){
    Card jack_spades (JACK, SPADES);
    ASSERT_FALSE(jack_spades.is_right_bower(CLUBS));
}

//test if the right bower is not a jack - Saw
TEST(test10_right_bower_not_jack){
    Card king_clubs (KING, CLUBS);
    ASSERT_FALSE(king_clubs.is_right_bower(CLUBS));
}

//test if is_ace_or_face is true - Saw
TEST(test11_is_ace_or_face_true) {
    ASSERT_TRUE(Card(JACK, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(QUEEN, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(KING, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(ACE, CLUBS).is_face_or_ace());
}

//test if is_ace_or_face is false - Saw
TEST(test12_is_ace_or_face_false) {
    ASSERT_FALSE(Card(TWO, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(THREE, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(FOUR, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(FIVE, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(SIX, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(SEVEN, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(EIGHT, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(NINE, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(TEN, CLUBS).is_face_or_ace());
}

//test to see is_left_bower for all 4 trumps - Sue
TEST(test_left_bower_all_trumps) {
  ASSERT_TRUE(Card(JACK, CLUBS).is_left_bower(SPADES));
  ASSERT_TRUE(Card(JACK, SPADES).is_left_bower(CLUBS));
  ASSERT_TRUE(Card(JACK, DIAMONDS).is_left_bower(HEARTS));
  ASSERT_TRUE(Card(JACK, HEARTS).is_left_bower(DIAMONDS));
  ASSERT_FALSE(Card(JACK, HEARTS).is_left_bower(SPADES));
  ASSERT_FALSE(Card(JACK, SPADES).is_left_bower(SPADES));
  ASSERT_FALSE(Card(KING, CLUBS).is_left_bower(SPADES));
}

//test for is_left_bower to see if left bower qualifies as trump / has trump suit. - Sue
TEST(test_left_bower_is_trump_and_suit) {
  Card lb(JACK, DIAMONDS);
  ASSERT_TRUE(lb.is_trump(HEARTS));
  ASSERT_EQUAL(lb.get_suit(HEARTS), HEARTS);
  Card cross(JACK, CLUBS);            
  ASSERT_FALSE(cross.is_trump(HEARTS));
  ASSERT_EQUAL(cross.get_suit(HEARTS), CLUBS);
}

//tests comparison operators primarily for identical cards - Sue
TEST(test_comparison_operators) {
  Card a(TEN, HEARTS), same(TEN, HEARTS), lo(NINE, CLUBS);
  ASSERT_TRUE(a <= same);   ASSERT_TRUE(a >= same);
  ASSERT_FALSE(a > same);   ASSERT_FALSE(a < same);
  ASSERT_TRUE(lo <= a);     ASSERT_FALSE(lo >= a);
  ASSERT_TRUE(a >= lo);     ASSERT_FALSE(a <= lo);
  ASSERT_TRUE(a == same);   ASSERT_FALSE(a != same);
  ASSERT_TRUE(a != Card(TEN, CLUBS));
  ASSERT_TRUE(a != Card(NINE, HEARTS));
  ASSERT_FALSE(a == Card(TEN, CLUBS));
}

//tests when ranks differ (operator) which rank beats suit - Sue
TEST(test_less_rank_ordering) {
  ASSERT_TRUE(Card(NINE, DIAMONDS) < Card(TEN, SPADES)); 
  ASSERT_FALSE(Card(TEN, SPADES) < Card(NINE, DIAMONDS));
  ASSERT_FALSE(Card(TEN, SPADES) < Card(TEN, SPADES)); 
}

//tests for 3 arguments in card_less
TEST(test_card_less_trump_basics) {
  ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(NINE, SPADES), SPADES));
  ASSERT_FALSE(Card_less(Card(NINE, SPADES), Card(ACE, HEARTS), SPADES));
  ASSERT_TRUE(Card_less(Card(NINE, HEARTS), Card(TEN, HEARTS), SPADES));
  ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(ACE, DIAMONDS), SPADES));
}

//tests for 4 arguments in card_less
TEST(test_card_less_led_suit) {
  Card led(NINE, HEARTS);
  ASSERT_TRUE(Card_less(Card(ACE, CLUBS), Card(TEN, HEARTS), led, SPADES));
  ASSERT_FALSE(Card_less(Card(TEN, HEARTS), Card(ACE, CLUBS), led, SPADES));
  ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(NINE, SPADES), led, SPADES));
  ASSERT_FALSE(Card_less(Card(NINE, SPADES), Card(ACE, HEARTS), led, SPADES));
  ASSERT_TRUE(Card_less(Card(NINE, CLUBS), Card(TEN, CLUBS), led, SPADES));
}

//tests the led card is the left bower
TEST(test_card_less_led_is_left_bower) {
  Card led(JACK, CLUBS);
  ASSERT_TRUE(Card_less(Card(NINE, CLUBS), Card(ACE, HEARTS), led, SPADES));
  ASSERT_FALSE(Card_less(Card(ACE, HEARTS), Card(NINE, CLUBS), led, SPADES));
}

//tests 4 arguments for card_less when led suit is trump
TEST(test_card_less_led_suit_is_trump) {
  Card led(NINE, SPADES);
  ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(NINE, SPADES), led, SPADES));
  ASSERT_TRUE(Card_less(Card(NINE, SPADES), Card(JACK, CLUBS), led, SPADES));
}

//tests 
TEST(test_card_less_bowers) {
  Card right(JACK, SPADES), left(JACK, CLUBS), ace(ACE, SPADES);
  ASSERT_TRUE(Card_less(left, right, SPADES));
  ASSERT_FALSE(Card_less(right, left, SPADES));
  ASSERT_TRUE(Card_less(ace, left, SPADES));
  ASSERT_FALSE(Card_less(left, ace, SPADES));
  ASSERT_TRUE(Card_less(ace, right, SPADES));
  ASSERT_FALSE(Card_less(right, ace, SPADES));
}


TEST_MAIN()
