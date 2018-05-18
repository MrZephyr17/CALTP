#ifndef _STRING_MATCHING_H
#define _STRING_MATCHING_H

#include <string>

/** @defgroup stringMatching stringMatching
* @{
*
* Implementations of KMPMatcher and Edit Distance, in order to support exact and approximate search
*/

/**
 * @brief Returns the amount of changes requiered to transform text to pattern
 * 
 * @param pattern In this context, the user input
 * @param text In this context, the street name
 * @return the minimun edit distance
 */
int editDistance(std::string pattern, std::string text);

/**
 * @brief Checks if pattern exists in text using KMP matcher algorithm, with a small nuance.
 * 
 * Since we don't care if the pattern exists more than once, once it is found the function returns
 * immediately.
 * 
 * @param text In this context, the street name
 * @param pattern In this context, the user input
 * @return true if the pattern exists at least once, false otherwise
 */
bool KMPMatcher(std::string text, std::string pattern);

/** @} end of stringMatching */

#endif /*_STRING_MATCHING_H */