#include <iostream>
#include "CTree.h"
using namespace std;

int main() {

  CTree T ;

  T.insert(70) ;
  T.inorder() ; cout << endl ;

  T.insert(30) ;
  T.inorder() ; cout << endl ;

  T.insert(110) ;

  T.inorder() ; cout << endl ;

  cout << "\n\n***** Insert 40 *****\n" ;
  T.insert(40) ;
  T.inorder() ; cout << endl ;

  cout << "\n\n***** Insert 20 *****\n" ;
  T.insert(20) ;
  T.inorder() ; cout << endl ;

  cout << "\n\n***** Insert 41 *****\n" ;
  T.insert(41) ;
  T.inorder() ; cout << endl ;

  cout << "\n\n***** Insert 31 *****\n" ;
  T.insert(31) ;
  T.inorder() ; cout << endl ;
  

	return 0;
}
