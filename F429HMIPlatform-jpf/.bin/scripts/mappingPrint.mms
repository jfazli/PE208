# Copyright 2013 IS2T. All rights reserved.

# merge some sections
union $Label_CLDC_B-ON $Label_CLDC_B-ON $Label_LIBFloat
union $Label_MiscRuntime $Label_Strings $Label_JavaRuntime
union $Label_MiscRuntime $Label_MiscRuntime $Label_IceteaRuntime
union $Label_VMCore $Label_VMCore $Label_OS
union $Label_MicroUI $Label_MicroUI $Label_LIBDisplay
union $Label_MicroUI $Label_MicroUI $Label_LIBInput
union $Label_MicroUI $Label_MicroUI $Label_LIBPNG
union $Label_MicroUI $Label_MicroUI $Label_LIBBMPM


# Fix labels 
union VMDebugger $Label_VMDebugger $Label_VMDebugger
union VMConsole $Label_VMConsole $Label_VMConsole
union JavaStrings $Label_JavaStrings $Label_JavaStrings
union Statics $Label_Statics $Label_Statics
union CLDC_B-ON $Label_CLDC_B-ON $Label_CLDC_B-ON
union PAP $Label_PAP $Label_PAP
union ECOM $Label_ECOM $Label_ECOM
union MicroUI $Label_MicroUI $Label_MicroUI
union MWT $Label_MWT $Label_MWT
union Widgets $Label_Widgets $Label_Widgets
union OSGiME $Label_OSGiME $Label_OSGiME
union NLS $Label_NLS $Label_NLS
union SP $Label_SP $Label_SP
union StoryBoard $Label_StoryBoard $Label_StoryBoard
union Internet_Pack $Label_Internet_Pack $Label_Internet_Pack
union Drivers $Label_Drivers $Label_Drivers
union VMAllocator $Label_VMAllocator $Label_VMAllocator
union VMCore $Label_VMCore $Label_VMCore
union ApplicationResources $Label_ApplicationResources $Label_ApplicationResources
union ApplicationImmutables $Label_ApplicationImmutables $Label_ApplicationImmutables
union JavaApplication $Label_JavaApplication $Label_JavaApplication
union MiscRuntime $Label_MiscRuntime $Label_MiscRuntime
union BSP $Remaining $Remaining


# Print results
totalImageSize All

echo
echo APPLICATION:
totalImageSize JavaApplication
totalImageSize ApplicationResources
totalImageSize ApplicationImmutables

echo
echo Java LIBRARIES:
totalImageSize CLDC_B-ON
totalImageSize PAP
totalImageSize ECOM
totalImageSize MicroUI
totalImageSize MWT
totalImageSize OSGiME
totalImageSize NLS
totalImageSize SP
totalImageSize StoryBoard
totalImageSize Widgets
totalImageSize JavaStrings


echo
echo MicroJvm LIBRARIES & RUNTIME:
totalImageSize VMCore
totalImageSize VMAllocator
totalImageSize VMConsole
totalImageSize VMDebugger
totalImageSize Drivers
totalImageSize MiscRuntime

