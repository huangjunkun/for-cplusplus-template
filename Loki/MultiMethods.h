////////////////////////////////////
// Generated header: MultiMethods.h
// Forwards to the appropriate code
// that works on the detected compiler
// Generated on Sun Sep 15 15:31:17 2002
////////////////////////////////////

#ifdef LOKI_USE_REFERENCE
#	include "Reference/MultiMethods.h"
#else
#	if (__INTEL_COMPILER)
#		include "Reference/MultiMethods.h"
#	elif (__MWERKS__)
#		include "Reference/MultiMethods.h"
#	elif (__BORLANDC__ >= 0x560)
#		include "Borland/MultiMethods.h"
#	elif (_MSC_VER >= 1300)
#		include "MSVC/1300/MultiMethods.h"
#	elif (_MSC_VER >= 1200)
#		include "MSVC/1200/MultiMethods.h"
#	else
#		include "Reference/MultiMethods.h"
#	endif
#endif
