/* 
 *	op≈
 *	External object for Max/MSP to perform basic mathematical operations on objects in a Jamoma AudioGraph dsp chain.
 *	Copyright © 2008 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the GNU LGPL
 *	http://www.gnu.org/licenses/lgpl.html 
 */

#include "maxAudioGraph.h"
#include "maxGraph.h"


// Data Structure for this object
struct Op {
   	Object					obj;
	TTAudioGraphObjectPtr	audioGraphObject;
	TTPtr					outlet;
	SymbolPtr				attrOperator;
	TTFloat32				attrOperand;
};
typedef Op* OpPtr;


// Prototypes for methods
OpPtr	OpNew			(SymbolPtr msg, AtomCount argc, AtomPtr argv);
void   	OpFree			(OpPtr self);
void   	OpAssist		(OpPtr self, void* b, long msg, long arg, char* dst);
TTErr  	OpResetAudio	(OpPtr self, long vectorSize);
TTErr  	OpSetupAudio	(OpPtr self);
//TTErr	OpSetup			(OpPtr self);
TTErr  	OpConnectAudio	(OpPtr self, TTAudioGraphObjectPtr audioSourceObject, long sourceOutletNumber);
MaxErr 	OpSetOperator	(OpPtr self, void* attr, AtomCount argc, AtomPtr argv);
MaxErr 	OpSetOperand	(OpPtr self, void* attr, AtomCount argc, AtomPtr argv);


// Globals
static ClassPtr sOpClass;


/************************************************************************************/
// Main() Function

int TTCLASSWRAPPERMAX_EXPORT main(void)
{
	ClassPtr c;
	
	TTAudioGraphInit();	
	common_symbols_init();
	
	c = class_new("jcom.op≈", (method)OpNew, (method)OpFree, sizeof(Op), (method)0L, A_GIMME, 0);
	
	class_addmethod(c, (method)OpResetAudio,		"audio.reset",		A_CANT, 0);
	class_addmethod(c, (method)OpSetupAudio,		"audio.setup",		A_CANT, 0);
	class_addmethod(c, (method)OpConnectAudio,		"audio.connect",	A_OBJ, A_LONG, 0);
	class_addmethod(c, (method)MaxAudioGraphDrop,	"audio.drop",		A_CANT, 0);
	class_addmethod(c, (method)MaxAudioGraphObject,	"audio.object",		A_CANT, 0);
	class_addmethod(c, (method)MaxAudioGraphReset,	"graph.reset",			A_CANT, 0);
	//class_addmethod(c, (method)OpSetup,			"graph.setup",			A_CANT, 0); // no setup -- no graph outlets
	class_addmethod(c, (method)MaxGraphConnect,		"graph.connect",		A_OBJ, A_LONG, 0);
 	class_addmethod(c, (method)MaxGraphDrop,		"graph.drop",			A_CANT, 0);
	class_addmethod(c, (method)MaxGraphObject,		"graph.object",			A_CANT, 0);
	class_addmethod(c, (method)OpAssist,			"assist",				A_CANT, 0); 
    class_addmethod(c, (method)object_obex_dumpout,	"dumpout",				A_CANT, 0);  
	
	CLASS_ATTR_SYM(c,		"operator",	0,		Op,	attrOperator);
	CLASS_ATTR_ACCESSORS(c,	"operator",	NULL,	OpSetOperator);
	
	CLASS_ATTR_FLOAT(c,		"operand",	0,		Op,	attrOperand);
	CLASS_ATTR_ACCESSORS(c,	"operand",	NULL,	OpSetOperand);
	
	class_register(_sym_box, c);
	sOpClass = c;
	return 0;
}


/************************************************************************************/
// Object Creation Method

OpPtr OpNew(SymbolPtr msg, AtomCount argc, AtomPtr argv)
{
    OpPtr	self;
	TTValue	v;
	TTErr	err;
	
    self = OpPtr(object_alloc(sOpClass));
    if (self) {
    	object_obex_store((void*)self, _sym_dumpout, (ObjectPtr)outlet_new(self, NULL));	// dumpout	
		self->outlet = outlet_new(self, "audio.connect");
		
		v.setSize(2);
		v.set(0, TT("operator"));
		v.set(1, TTUInt32(1));
		err = TTObjectInstantiate(TT("audio.object"), (TTObjectPtr*)&self->audioGraphObject, v);

		if (!self->audioGraphObject->getUnitGenerator()) {
			object_error(SELF, "cannot load Jamoma DSP object");
			return NULL;
		}
		
		attr_args_process(self, argc, argv);
	}
	return self;
}


// Memory Deallocation
void OpFree(OpPtr self)
{
	TTObjectRelease((TTObjectPtr*)&self->audioGraphObject);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void OpAssist(OpPtr self, void* b, long msg, long arg, char* dst)
{
	if (msg==1)			// Inlets
		strcpy (dst, "multichannel input and control messages");		
	else if (msg==2){	// Outlets
		if (arg == 0)
			strcpy(dst, "multichannel output");
		else
			strcpy(dst, "dumpout");
	}
}


// METHODS SPECIFIC TO AUDIO GRAPH EXTERNALS

TTErr OpResetAudio(OpPtr self, long vectorSize)
{
	return self->audioGraphObject->resetAudio();
}


TTErr OpSetupAudio(OpPtr self)
{
	Atom a[2];
	
	atom_setobj(a+0, ObjectPtr(self->audioGraphObject));
	atom_setlong(a+1, 0);
	outlet_anything(self->outlet, gensym("audio.connect"), 2, a);
	return kTTErrNone;
}


/*
TTErr OpSetup(OpPtr self)
{
	Atom				a[2];
	TTUInt16			i=0;
	
	atom_setobj(a+0, ObjectPtr(self->graphObject));
	while (self->graphOutlets[i]) {
		atom_setlong(a+1, i);
		outlet_anything(self->graphOutlets[i], gensym("graph.connect"), 2, a);
		i++;
	}
	return kTTErrNone;
}
 */


TTErr OpConnectAudio(OpPtr self, TTAudioGraphObjectPtr audioSourceObject, long sourceOutletNumber)
{
	return self->audioGraphObject->connectAudio(audioSourceObject, sourceOutletNumber);
}


// ATTRIBUTE SETTERS

MaxErr OpSetOperator(OpPtr self, void* attr, AtomCount argc, AtomPtr argv)
{
	if (argc) {
		self->attrOperator = atom_getsym(argv);
		self->audioGraphObject->getUnitGenerator()->setAttributeValue(TT("Operator"), TT(self->attrOperator->s_name));
	}
	return MAX_ERR_NONE;
}


MaxErr OpSetOperand(OpPtr self, void* attr, AtomCount argc, AtomPtr argv)
{
	if (argc) {
		self->attrOperand = atom_getfloat(argv);
		self->audioGraphObject->getUnitGenerator()->setAttributeValue(TT("Operand"), self->attrOperand);
	}
	return MAX_ERR_NONE;
}

