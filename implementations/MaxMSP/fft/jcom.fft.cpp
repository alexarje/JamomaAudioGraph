/* 
 *	fft≈
 *	External object for Jamoma AudioGraph
 *	Copyright © 2010 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the GNU LGPL
 *	http://www.gnu.org/licenses/lgpl.html 
 */

#include "maxAudioGraph.h"

int TTCLASSWRAPPERMAX_EXPORT main(void)
{
	wrapAsMaxAudioGraph(TT("fft"), "jcom.fft≈", NULL);
	return 0;
}
