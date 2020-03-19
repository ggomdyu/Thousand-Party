# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.gainputstatic.Debug:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Debug/libgainputstatic-d.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Debug/libgainputstatic-d.a


PostBuild.gainputstatic.Release:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Release/libgainputstatic.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Release/libgainputstatic.a


PostBuild.gainputstatic.MinSizeRel:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/MinSizeRel/libgainputstatic.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/MinSizeRel/libgainputstatic.a


PostBuild.gainputstatic.RelWithDebInfo:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/RelWithDebInfo/libgainputstatic.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/RelWithDebInfo/libgainputstatic.a




# For each target create a dummy ruleso the target does not have to exist
