# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.freetype.Debug:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Debug/libfreetyped.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Debug/libfreetyped.a


PostBuild.freetype.Release:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Release/libfreetype.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Release/libfreetype.a


PostBuild.freetype.MinSizeRel:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/MinSizeRel/libfreetype.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/MinSizeRel/libfreetype.a


PostBuild.freetype.RelWithDebInfo:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/RelWithDebInfo/libfreetype.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/RelWithDebInfo/libfreetype.a




# For each target create a dummy ruleso the target does not have to exist
