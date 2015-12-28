//
//  helper.hpp
//  Server
//
//  Created by Trevor Meiss on 10/20/15.
//  Copyright © 2015 Trevor Meiss. All rights reserved.
//

#ifndef helper_hpp
#define helper_hpp

bool stringIsValid(const std::string &str);
std::string systemTime();
std::string trim(const std::string &s);
bool compareStringNoCase(std::string a, int start, int length, std::string b);

#endif /* helper_hpp */
