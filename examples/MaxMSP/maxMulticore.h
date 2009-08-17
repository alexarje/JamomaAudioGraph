/* 
 *	Maxbaer
 *	A thin wrapper of the Lydbaer audio system for use in the Cycling '74 Max/MSP environment.
 *	Includes an automated class wrapper to make TTBlue object's available as objects for Max/MSP.
 *	Copyright © 2008 by Timothy Place
 * 
 * License: This code is licensed under the terms of the GNU LGPL
 * http://www.gnu.org/licenses/lgpl.html 
 */

#ifndef __MAXBAER_H__
#define __MAXBAER_H__

#include "ext.h"					// Max Header
#include "ext_obex.h"				// Max Object Extensions (attributes) Header
#include "ext_strings.h"			// String Functions
#include "commonsyms.h"				// Common symbols used by the Max 4.5 API
#include "z_dsp.h"					// MSP Header
#include "jpatcher_api.h"			// Required for patcher traversal code

#include "TTDSP.h"					// TTBlue Interfaces
#include "TTMulticore.h"			// Definitions for Lydbaer Wrapping




// TYPE DEFINITIONS

typedef t_class*	ClassPtr;
typedef t_object*	ObjectPtr;
typedef t_symbol*	SymbolPtr;
typedef t_atom*		AtomPtr;
typedef long		AtomCount;
typedef t_max_err	MaxErr;

typedef TTErr (*TTValidityCheckFunction)(const TTPtr data);		///< A type that can be used to store a pointer to a validity checking function.

class WrappedClassOptions;

typedef struct _wrappedClass {
	ClassPtr				maxClass;							///< The Max class pointer.
	SymbolPtr				maxClassName;						///< The name to give the Max class.
	TTSymbolPtr				ttblueClassName;					///< The name of the class as registered with the TTBlue framework.
	TTValidityCheckFunction validityCheck;						///< A function to call to validate the context for an object before it is instantiated.
	TTPtr					validityCheckArgument;				///< An argument to pass to the validityCheck function when it is called.
	WrappedClassOptions*	options;							///< Additional configuration options specified for the class.
} WrappedClass;



class WrappedClassOptions {
protected:
	TTHash*	options;

public:
	WrappedClassOptions()
	{
		options = new TTHash;
	}
	
	virtual ~WrappedClassOptions()
	{
		delete options;
	}
	
	TTErr append(const TTSymbolPtr optionName, const TTValue& optionValue)
	{
		return options->append(optionName, optionValue);
	}
	
	/**	Returns an error if the requested option doesn't exist. */
	TTErr lookup(const TTSymbolPtr optionName, TTValue& optionValue)
	{
		return options->lookup(optionName, optionValue);
	}
	
};
	

typedef WrappedClass* WrappedClassPtr;							///< A pointer to a WrappedClass.
typedef WrappedClassOptions* WrappedClassOptionsPtr;			///< A pointer to WrappedClassOptions.


// FUNCTIONS

// Wrap a TTBlue class as a Max class.
TTErr wrapAsMaxbaer(TTSymbolPtr ttblueClassName, char* maxClassName, WrappedClassPtr* c);

// This version can be passed a method that is called to make sure it is legit to instantiate the class.
TTErr wrapAsMaxbaer(TTSymbolPtr ttblueClassName, char* maxClassName, WrappedClassPtr* c, TTValidityCheckFunction validityCheck);

// This version can be passed a method that is called to make sure it is legit to instantiate the class.
TTErr wrapAsMaxbaer(TTSymbolPtr ttblueClassName, char* maxClassName, WrappedClassPtr* c, TTValidityCheckFunction validityCheck, TTPtr validityCheckArgument);


// These are versions of the above, but for which additional options can be specified.
TTErr wrapAsMaxbaer(TTSymbolPtr ttblueClassName, char* maxClassName, WrappedClassPtr* c, WrappedClassOptionsPtr options);
TTErr wrapAsMaxbaer(TTSymbolPtr ttblueClassName, char* maxClassName, WrappedClassPtr* c, TTValidityCheckFunction validityCheck, WrappedClassOptionsPtr options);
TTErr wrapAsMaxbaer(TTSymbolPtr ttblueClassName, char* maxClassName, WrappedClassPtr* c, TTValidityCheckFunction validityCheck, TTPtr validityCheckArgument, WrappedClassOptionsPtr options);



#endif // __MAXBAER_H__
