/* 
 *	window≈
 *	Jamoma Multicore external object for Max
 *	Copyright © 2010 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the GNU LGPL
 *	http://www.gnu.org/licenses/lgpl.html 
 */

#include "maxMulticore.h"

int main(void)
{
	TTMulticoreInit();
	return wrapAsMaxMulticore(TT("WindowFunction"), "jcom.window≈", NULL);
}
