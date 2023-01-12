#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//

Transaction::Transaction (std::string ticker_symbol,
                          unsigned int day_date,
                          unsigned int month_date,
                          unsigned int year_date,
                          bool buy_sell_trans,
                          unsigned int number_shares,
                          double trans_amount)
{
//initialize the variables to the private member of transaction class
    this -> symbol = ticker_symbol;
    this -> day = day_date;
    this -> month = month_date;
    this -> year = year_date;
    if (buy_sell_trans){
      this -> trans_type = "Buy";
    }else{
      this -> trans_type = "Sell";
    }
    this -> shares = number_shares;
    this -> amount = trans_amount;
    this -> trans_id = assigned_trans_id;
    ++assigned_trans_id;

    p_next = nullptr;

    acb = 0;
    acb_per_share = 0;
    share_balance = 0;
    cgl = 0;

}

// Destructor
// TASK 1
//

Transaction::~Transaction ()
{
//deallocate all memory from class
//when you call delete operator, you're calling deconstructor

}

// Overloaded < operator.
// TASK 2
//
///if (transaction1 < transaction2){

//}
bool Transaction::operator<( Transaction const &other){
if(this -> year < other.year)
  return true;
else if (this -> year == other.year && this ->month < other.month)
    return true;
else if (this->year == other.year && this->month == other.month && this->day < other.day)
      return true; 

return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next;}

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//

History::History ()
{
this->p_head = nullptr;
}

// Destructor
// TASK 3
//

History::~History()
{
  Transaction *p_temp{p_head};
  if (p_head==nullptr){

  }else{
    while(p_temp != nullptr){
      p_temp = p_temp->get_next();
      delete p_head;
      p_head = p_temp;
    }
  }
  
}


// read_history(...): Read the transaction history from file.
// TASK 4
//

void History::read_history()
{

ece150::open_file();

while(ece150::next_trans_entry() == true)
{
  Transaction *p_new_trans{new Transaction(ece150::get_trans_symbol(), 
                                           ece150::get_trans_day(), 
                                           ece150::get_trans_month(), 
                                           ece150::get_trans_year(),
                                           ece150::get_trans_type(),
                                           ece150::get_trans_shares(),
                                           ece150::get_trans_amount())};
  insert(p_new_trans);
}

ece150::close_file();

}

// insert(...): Insert transaction into linked list.
// TASK 5
//

void History::insert(Transaction * p_new_trans)//attended lab sessiob 3 with professor David Lau
{
  if(p_head == nullptr)
  {
    this -> p_head = p_new_trans;
  } 
  else
  {
    Transaction * p_traverse {p_head};

    while(p_traverse->get_next() != nullptr)
    {
    p_traverse = p_traverse -> get_next();
    }

    p_traverse->set_next(p_new_trans);
  }

}


// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

void History::sort_by_date()
{
Transaction *p_front = nullptr;
Transaction *p_sorted = nullptr;
Transaction *p_temp = nullptr;
p_sorted = p_head;
p_head = p_head->get_next();
p_sorted->set_next(nullptr);

while (p_head != nullptr){
  p_front = p_head;
  p_head = p_head->get_next();
  p_front->set_next(nullptr);

if(p_sorted == nullptr){
  p_sorted = p_front;

}else{
    if (*p_front < *p_sorted){
        p_front->set_next(p_sorted);
        p_sorted = p_front;
    }else{
      p_temp = p_sorted;
      while( (p_temp->get_next() != nullptr) && (*(p_temp->get_next( )) < *p_front)){
        p_temp = p_temp->get_next();
      }
      p_front-> set_next(p_temp->get_next() );
      p_temp->set_next(p_front);
    }
  }
}
p_head = p_sorted;
}


// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl()
{
  Transaction *p_current = p_head;

  double total_amount {0};
  unsigned int total_shares {0};
  double current_acb_per_shares {0};
  

  while( p_current != nullptr){
      p_current = p_current -> get_next();

    if (p_current->get_trans_type()){
      total_amount += p_current->get_amount();
      p_current->set_share_balance(total_shares);
      total_shares += p_current->get_share_balance();
      p_current->set_share_balance(total_shares);
      current_acb_per_shares = p_current->get_acb() / p_current->get_share_balance();
      p_current->set_acb_per_share(current_acb_per_shares);

          }else {
            total_amount -= (total_shares*current_acb_per_shares);
            p_current->set_acb(total_amount);
            total_shares -= p_current->get_amount();
            p_current->set_share_balance(total_shares);
            p_current ->set_cgl(total_amount - (current_acb_per_shares*total_shares));
          }
    p_current = nullptr;
  }


}



// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl( unsigned int year )
{
  Transaction *p_current = p_head;
  double computed_cgl = 0;
  double capital_gains = 0;

//Computes the capital gains or capital losses for every transaction in the history of
//transactions and updates the respective instances in the linked list. In addition, this
//function returns the total capital gains for the specified year.

  p_current = nullptr;


  return 0;
}


// print() Print the transaction history.
//TASK 9
//

void History::print()
{

  if(p_head == nullptr){
    std::cout <<"============ BEGIN TRANSACTION HISTORY ============"<<std::endl;
    std::cout <<"============ END TRANSACTION HISTORY ============"<<std::endl;

  }else{
    Transaction * p_find = p_head;
    std::cout <<"============ BEGIN TRANSACTION HISTORY ============"<<std::endl;

    while(p_find != nullptr){
      p_find->print();
      p_find = p_find ->get_next();
    }
    std::cout <<"============ END TRANSACTION HISTORY ============"<<std::endl;
  
  }

}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
