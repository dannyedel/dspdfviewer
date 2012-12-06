/** hacks_for_lucid.h
 *
 * This file messes around with QT's macros, in the hope that the combination of
 * g++-4.7 from the test-ppa and lucid's Qt 4.6.2 can actually compile.
 *
 * Note: Also define -DHACKS_FOR_LUCID if you want this file to do something.
 */


#ifndef HACKS_FOR_LUCID_INCLUDED
#define HACKS_FOR_LUCID_INCLUDED

#ifdef HACKS_FOR_LUCID
/** Custom Fix for lucid's Qt
 *
 * Found on http://dev.modmancer.com/index.php/2012/02/06/181/
 */
#undef QLOCATION
#define QLOCATION "\0" __FILE__ ":" QTOSTRING(__LINE__)
#endif /* HACKS_FOR_LUCID */

#endif /* HACKS_FOR_LUCID_INCLUDED */
