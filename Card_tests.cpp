#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here

//Test if constructer gives Two of Spades
TEST(test1_two_spade) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

//test if card is trump
TEST(test2_card_is_trump) {
    Card king_hearts(KING, HEARTS);
    ASSERT_TRUE(king_hearts.is_trump(HEARTS));
    ASSERT_FALSE(king_hearts.is_trump(CLUBS));
}

//test if jack is also trump
TEST(test3_jack_also_trump) {
    Card king_hearts(KING, HEARTS);
    ASSERT_TRUE(king_hearts.is_trump(HEARTS));
    ASSERT_FALSE(king_hearts.is_trump(CLUBS)); 

    Card jack_hearts(JACK, HEARTS);
    ASSERT_TRUE(jack_hearts.is_trump(HEARTS));
    ASSERT_FALSE(jack_hearts.is_trump(CLUBS));
}

//Test if the next suit is acurate
TEST(test4_next_suit) {
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

//Test for tie breaker using suit
TEST(test5_tie_breaker_suit) {
    Card king_clubs(KING, CLUBS);
    Card king_spades(KING, SPADES);
    ASSERT_TRUE(king_spades < king_clubs);
    ASSERT_FALSE(king_spades > king_clubs);
}

//test if the left bower keeps its suit when becoming a trump
TEST(test6_left_bower_keep_suit) {
    Card jack_diamonds(JACK, DIAMONDS);
    //trump = heart, then jack_diamonds become left bower
    ASSERT_EQUAL(jack_diamonds.get_suit(HEARTS), HEARTS); 
    //keeps the same suit though
    ASSERT_EQUAL(jack_diamonds.get_suit(), DIAMONDS);
}

//test if a non-bower card stays as is 
TEST(test7_nonbower_suit_stay_same) {
    Card king_clubs (KING, CLUBS);
    ASSERT_EQUAL(king_clubs.get_suit(SPADES), CLUBS);
    ASSERT_EQUAL(king_clubs.get_suit(DIAMONDS), CLUBS);
    ASSERT_EQUAL(king_clubs.get_suit(HEARTS), CLUBS);
}

//test if the card is the right bower
TEST(test8_card_right_bower){
    Card jack_clubs (JACK, CLUBS);
    ASSERT_TRUE(jack_clubs.is_right_bower(CLUBS));
}

//test if the card is not the right bower
TEST(test9_card_not_right_bower){
    Card jack_spades (JACK, SPADES);
    ASSERT_FALSE(jack_spades.is_right_bower(CLUBS));
}

//test if the right bower is not a jack
TEST(test10_right_bower_not_jack){
    Card king_clubs (KING, CLUBS);
    ASSERT_FALSE(king_clubs.is_right_bower(CLUBS));
}

//test if is_ace_or_face is true 
TEST(test11_is_ace_or_face_true) {
    ASSERT_TRUE(Card(JACK, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(QUEEN, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(KING, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(ACE, CLUBS).is_face_or_ace());
}

//test if is_ace_or_face is false
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

TEST_MAIN()
