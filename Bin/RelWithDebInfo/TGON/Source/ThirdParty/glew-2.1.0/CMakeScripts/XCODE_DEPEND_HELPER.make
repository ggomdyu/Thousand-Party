# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.glew.Debug:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Debug/libglew.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Debug/libglew.a


PostBuild.glew.Release:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Release/libglew.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/Release/libglew.a


PostBuild.glew.MinSizeRel:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/MinSizeRel/libglew.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/MinSizeRel/libglew.a


PostBuild.glew.RelWithDebInfo:
/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/RelWithDebInfo/libglew.a:
	/bin/rm -f /Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Bin/RelWithDebInfo/TGON/RelWithDebInfo/RelWithDebInfo/libglew.a




# For each target create a dummy ruleso the target does not have to exist
