#ifndef _Navigator_h_
#define _Navigator_h_

/** 
   This is the class that should be instantiated in case the
   user wants to have a select menu, the elements of which, 
   submit a request when clicked on. 
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"

class Navigator : public WebElement
{
 private:

 public:

  Navigator(std::string the_url ,         /// url of the application
	    std::string top, std::string left) /// position
    : WebElement(the_url, top, left)
    {
    }
  ~Navigator()
    {
    }

  void printHTML(xgi::Output * out);
  void printSelectHTML(xgi::Output * out, std::string name, std::string onchange);
};


#endif
